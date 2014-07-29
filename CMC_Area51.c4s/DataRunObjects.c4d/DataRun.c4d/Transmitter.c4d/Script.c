/*-- Sendeanlage --*/

#strict 2

local fDestroyed, bar, iPoints;

public func IsDataTransmitter(bool fGoal)	{return true;}
public func IsDestroyed()			{return fDestroyed;}
public func MaxPoints()				{return 100;}
public func PointsGain()			{return 1;}
public func RegainSignalTime()			{return 35*5;}	//Dauer bis zur Wiederverbindung
public func RegainSignalBonus()			{return 10;}	//Startbonus nach Wiederverbindung


/* Initialisierung */

protected func Initialize()
{
  //Dateneffekt
  AddEffect("GeneratePoints", this, 1, 3, this);
  SetPoints(MaxPoints());

  //Statusbalken setzen
  bar = CreateObject(SBAR, 0, 0, -1);
  bar->Set(this, RGB(30, 140, 240), BAR_BASBombBar, 100, 0, SM16, 0, 11000, true);
  bar->Update(0, true, true);
  bar->SetIcon(0, SM16, 0, 11000, 32);
  bar->PositionToVertex(0, true);

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
  if(GetPoints() > 0 && GetPoints() < MaxPoints() && !GetEffect("RegainSignal", this))
    fShowBar = true;

  bar->Update(GetPoints()*100/MaxPoints(), !fShowBar, !fShowBar);
  if(fShowBar)
    bar->SetIcon(0, SM19, 0, 0, 32);

  if(GetEffect("RegainSignal", this) && !IsDestroyed())
    bar->SetIcon(0, SM18, 0, 11000, 32);

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
  if(!iPoints)
  {
    var e;
    if(!(e = GetEffect("RegainSignal", this))) //Wiederverbinden
      AddEffect("RegainSignal", this, 1, 1, this);
    else
      EffectVar(0, this, e) = 0;
  }

  UpdateBar();
  return true;
}

/* Effekte */

//Punkte generieren
public func FxGeneratePointsTimer(object pTarget, int iNr)
{
  if(GetEffect("RegainSignal", pTarget) || GetEffect("ConnectionInUse", pTarget))
    return true;

  pTarget->SetPoints(BoundBy(pTarget->GetPoints()+pTarget->PointsGain(), 0, pTarget->MaxPoints()));
  pTarget->UpdateBar();
  return true;
}

//Verbindung wird von Computer belastet
public func FxConnectionInUseTimer(object pTarget, int iNr)
{
  //Downloadicon setzen
  pTarget->GetBar()->SetIcon(0, SM19, 0, 0, 32);

  EffectVar(0, pTarget, iNr)++;
  if(EffectVar(0, pTarget, iNr) < 2)
    return -1;
}

public func FxConnectionInUseStop(object pTarget, int iNr)
{
  //Icon resetten
  if(!GetEffect("RegainSignal", pTarget))
    pTarget->UpdateBar();
  return;
}

//Neuer Verbindungsaufbau
public func FxRegainSignalTimer(object pTarget, int iNr)
{
  pTarget->UpdateBar();
  EffectVar(0, pTarget, iNr)++;

  if(EffectVar(0, pTarget, iNr) > pTarget->~RegainSignalTime())
    return -1;

  return true;
}

public func FxRegainSignalStop(object pTarget)
{
  pTarget->UpdateBar();
  pTarget->SetPoints(10);
  return true;
}

/* Zustand */

public func IsDownloading()	{return GetEffect("ConnectionInUse", this) && !GetEffect("RegainSignal", this);}
public func IsRegainingSignal()	{return GetEffect("RegainSignal", this);}

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
  bar->SetIcon(0, SM16, 0, 11000, 32);
  fDestroyed = false;

  return true;
}