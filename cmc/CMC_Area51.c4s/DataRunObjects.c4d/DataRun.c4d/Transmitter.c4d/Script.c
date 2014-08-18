/*-- Sendeanlage --*/

#strict 2

local fDestroyed, bar, iPoints;

public func IsDataTransmitter(bool fGoal)	{return true;}
public func IsDestroyed()			{return fDestroyed;}
public func MaxPoints()				{return 100;}
public func PointsGain()			{return 1;}

/* Initialisierung */

protected func Initialize()
{
  //Statusbalken setzen
  bar = CreateObject(SBAR, 0, 0, -1);
  bar->Set(this, RGB(30, 140, 240), BAR_BASBombBar, 100, 0, SM26, 0, 11000, true);
  bar->Update(0, true, true);
  bar->SetIcon(0, SM26, 0, 11000, 32);
  bar->PositionToVertex(0, true);

  //Dateneffekt
  AddEffect("GeneratePoints", this, 1, 3, this);
  SetPoints(MaxPoints());

  return true;
}

/* Statusbalken */

public func GetBar()	{return bar;}

public func GetIcon()
{
  if(!bar || !bar->HasIcon())
    return SM06;

  return bar->GetIconData()[1];
}

public func UpdateBar()
{
  var fShowBar = false;
  if(GetPoints() > 0 && GetPoints() < MaxPoints())
    fShowBar = true;

  bar->Update(GetPoints()*100/MaxPoints(), !fShowBar, !fShowBar);
  
  //Am Downloaden
  if(fShowBar)
    bar->SetIcon(0, SM27, 0, 0, 32);

  //Wiederverbindung
  if(!GetPoints() && !IsDestroyed())
    bar->SetIcon(0, SM28, 0, 11000, 32);

  FindObject(GDAR)->UpdateScoreboard();

  return true;
}

/* Punkteverwaltung */

public func Set(string szName, int iPoints)
{
  SetName(szName, this);
  if(iPoints)
    SetPoints(iPoints);

  return true;
}

public func GetPoints()	{return iPoints;}

public func DoPoints(int iChange)
{
  return SetPoints(GetPoints()+iChange);
}

public func SetPoints(int iAmount)
{
  iPoints = BoundBy(iAmount, 0, MaxPoints());

  UpdateBar();
  return true;
}

/* Effekte */

local isTarget;

//Punkte generieren
public func FxGeneratePointsTimer(object pTarget, int iNr)
{
  if(GetEffect("ConnectionInUse", pTarget))
  {
  	isTarget = true;
    return false;
  }
  
  if(FindObject(TRMR))
  {
  	for(var transmitter in FindObjects(Find_ID(TRMR)))
  		if(transmitter != this)
  			if(GetEffect("ConnectionInUse", transmitter))
  			{
  				isTarget = false;
  				break;
  			}
  }
  
  if(isTarget)
  	return false;

  pTarget->SetPoints(BoundBy(pTarget->GetPoints()+pTarget->PointsGain(), 0, pTarget->MaxPoints()));
  pTarget->UpdateBar();
  return true;
}

//Verbindung wird von Computer belastet
public func FxConnectionInUseTimer(object pTarget, int iNr)
{
  //Downloadicon setzen
  pTarget->GetBar()->SetIcon(0, SM27, 0, 0, 32);

  EffectVar(0, pTarget, iNr)++;
  if(EffectVar(0, pTarget, iNr) < 2)
    return -1;
}

public func FxConnectionInUseStop(object pTarget, int iNr)
{
  //Icon resetten
  pTarget->UpdateBar();

  return;
}

/* Zustand */

public func IsDownloading()	{return GetEffect("ConnectionInUse", this) && GetPoints();}

/* Schaden */

public func OnDmg()
{
  if(GetDamage() > 150 && !fDestroyed)
  {
    fDestroyed = true;
    //DecoExplode();
    bar->SetIcon(0, SM02, 0, 11000, 32);
    ScheduleCall(this, "Repair", 35*30);
  }

  return true;
}

public func Repair()
{
  DoDamage(-GetDamage(), this);
  bar->SetIcon(0, SM26, 0, 11000, 32);
  fDestroyed = false;

  return true;
}
