/*-- Computer --*/

#strict 2

public func IsSpecialGoalItem()	{return true;}
public func IsBorderTarget()	{return true;}

/* Initialisierung */

public func Initialize()
{
  //Computereffekt
  AddEffect("CompEffect", this, 101, 2, this);

  //Statusbalken
  var bar = CreateObject(SBAR, 0, 0, -1);
  bar->Set(this, 0, BAR_BASBombBar, 100, 0, SM25, 0, 11000, true);
  bar->Update(0, true, true);
  bar->SetIcon(0, SM25, 0, 11000, 32);
  bar->PositionToVertex(0, true);

  //Effekte
  SetAction("Screen");
  AddLight(300, RGBa(255, 0, 0, 60));
  Sound("SNSR_Scan.ogg", false, this, 0, 0, +1);
  Sound("RSHL_Deploy.ogg");

  return _inherited(...);
}

/* Aufnahme */

public func RejectEntrance(object pContainer)
{
  //Kann nur von Clonks aufgehoben werden
  if(!(GetOCF(pContainer) & OCF_CrewMember))
    return true;

  return false;
}

public func Entrance(object pClonk)
{
  //Eventnachricht: Computer aufgehoben
  EventInfo4K(0, Format("$ComputerCollected$", GetTaggedPlayerName(GetOwner(pClonk))), COMP, 0, 0, 0, "Info_Event.ogg");

  //Effekt an Clonk �bergeben
  AddComputer(pClonk);
  Sound("RSHL_Deploy.ogg",0,pClonk);
  Sound("SNSR_Scan.ogg", false, pClonk, 0, 0, +1);

  //Verschwinden
  RemoveObject(this);
  return true;
}

public func AddComputer(object pTarget)
{
  AddEffect("DataRunComputer", pTarget, 1, 2, 0, COMP);
  return true;
}

public func FxCompEffectTimer(object pTarget, int iEffect, int iTime)
{
  //Computer in Grenzgebiet, Lava oder S�ure: Entfernen und neuen anfordern
  if(FindObject(GDAR) && !(GBAS->SpawningConditions(pTarget)))
  {
    FindObject(GDAR)->DelayedComputerRespawn(pTarget, 0, 0);
    return -1;
  }

  //Effekte
  if(!(iTime % 100))
    Beep(pTarget);
  
  //Respawn-Timer
  if(EffectVar(0, pTarget, iEffect)) {
  	EffectVar(1, pTarget, iEffect)--;
  	
  	if(EffectVar(1, pTarget, iEffect) <= 0) {
  		FindObject(GDAR)->DelayedComputerRespawn(pTarget, 0, 0);
  	  EventInfo4K(0, "$ComputerUseless$", COMP, 0, 0, 0, "Info_Event.ogg");

			//Effekt
			Sound("SNSR_Scan.ogg", false, pTarget, 0, 0, -1);
		
			//Objekt loeschen
			if(pTarget)
				RemoveObject(pTarget);
    	return -1;
  	}
  }

  return true;
}

/* Trageeffekt f�r Computertr�ger */

public func Beep(object pTarget)
{
  pTarget->AddLightFlash(300, 0, 0, RGB(255), pTarget);
  Sound("BOMB_Beep.ogg", false, pTarget);

  return true;
}

public func FxDataRunComputerStart(object pTarget, int iNr)
{
  //Grafik setzen
  var bar = CreateObject(SBAR, 0, 0, -1);
  bar->Set(pTarget, 0, BAR_BASBombBar, 100, 0, SM25, 0, 11000, true);
  bar->Update(0, true, true);
  bar->SetIcon(0, SM25, 0, 11000, 32);
  bar->PositionToVertex(0, true);

  EffectVar(0, pTarget, iNr) = bar;
  EffectVar(1, pTarget, iNr) = AddLight(300, RGBa(255, 0, 0, 60), pTarget);
  EffectVar(2, pTarget, iNr) = GetTaggedPlayerName(GetOwner(pTarget));

  EffectVar(3, pTarget, iNr) = -1;

  ShowTransmitterRadius(pTarget);
}

static const GDAR_TransmitterRadius = 100;
static const GDAR_NoSignalTimer = 525; //Frameanzahl wird VERDOPPELT
static const GDAR_PCDataInterval = 50; //Alle 100 Frames Daten liefern
static const GDAR_PCDataAmount = 20; //In Intervallen 20 Daten aufs Konto hinzuf�gen

public func FxDataRunComputerTimer(object pTarget, int iNr, int iTime)
{
  //Blinken
  if(!(iTime % 100))
  {
    //Effekte
    COMP->Beep(pTarget);
  }

  if(!(Random(5)) && GetProcedure(pTarget) == "WALK" && (Abs(GetXDir(pTarget)) > 5 || Abs(GetYDir(pTarget)) > 5) && !Contained(pTarget))
    Sound("BOMB_Rustle*.ogg", 0, pTarget, RandomX(25,50));

  //Tr�ger verstorben: Computer fallenlassen
  if(pTarget->~IsFakeDeath())
    return -1;
  
  var bar = EffectVar(0, pTarget, iNr);
  
  //Suche nach Sendeanlagen
  var transmitter = pTarget->FindObject2(pTarget->Find_Distance(GDAR_TransmitterRadius), Find_Func("IsDataTransmitter"));
  if(!transmitter)
  {
    if(EffectVar(3, pTarget, iNr) == -1)
      EffectVar(3, pTarget, iNr) = GDAR_NoSignalTimer;

    bar->SetIcon(0, SM26, 0, 11000, 32);

    //L�nger keine Signale empfangen? Abbrechen
    if(!EffectVar(3, pTarget, iNr))
    {
      //Kuhle "Wegwerfanimtion" w�re kuhl.
      CreateObject(CMP2, GetX(pTarget), GetY(pTarget), NO_OWNER)->ThrowAway();
      Schedule("PlaceComputerSpawnpoint()", GDAR_ComputerRespawnDelay);

      EventInfo4K(0, "$ComputerUseless$", COMP, 0, 0, 0, "Info_Event.ogg");
      return -1;
    }

    EffectVar(3, pTarget, iNr)--;
    EffectVar(5, pTarget, iNr) = 0;
  }
  else
  {
  	if(!transmitter->GetPoints())
  	{
  		EffectVar(5, pTarget, iNr) = 0;
  		return;
  	}
  	
  	EffectVar(5, pTarget, iNr) = transmitter;
  
  	var e;
  	if(!(e = GetEffect("ConnectionInUse", transmitter)))
  		AddEffect("ConnectionInUse", transmitter, 1, 4, transmitter);
		else //Timerrefresh
			EffectVar(0, transmitter, e) = 0;
	  
	  transmitter->DoPoints(-1);

  	bar->SetIcon(0, SM25, 0, 11000, 32);
  	EffectVar(3, pTarget, iNr) = -1;
  	EffectVar(4, pTarget, iNr)++;

    Sound("AHBS_Progress*.ogg", false, pTarget);

  	if(EffectVar(4, pTarget, iNr) >= GDAR_PCDataInterval && FindObject2(Find_ID(GDAR)))
  	{
  		FindObject2(Find_ID(GDAR))->GiveData(GetOwner(pTarget), GDAR_PCDataAmount);
  		EffectVar(4, pTarget, iNr) = 0;
  	}
  }
  
  return true;
}

public func FxDataRunComputerStop(object pTarget, int iNr, int iReason)
{
  if(iReason >= 3 || (pTarget && pTarget->~IsFakeDeath()))
  {
    //Eventnachricht: Computer fallengelassen
    EventInfo4K(0, Format("$ComputerDropped$", EffectVar(2, pTarget, iNr)), COMP, 0, 0, 0, "Info_Event.ogg");

    //Computer erstellen und platzieren
    PlaceComputerSpawnpoint(GetX(pTarget), GetY(pTarget), false, true, EffectVar(3, pTarget, iNr), GetOwner(pTarget));
  }

  //Grafik l�schen
  if(EffectVar(0, pTarget, iNr))
    RemoveObject(EffectVar(0, pTarget, iNr));

  if(EffectVar(1, pTarget, iNr))
    RemoveObject(EffectVar(1, pTarget, iNr));

  //Effekt
  Sound("SNSR_Scan.ogg", false, pTarget, 0, 0, -1);
}


protected func ShowTransmitterRadius(object pTarget)
{
  //Kreis-Symbol erstellen
  var obj = CreateObject(SM09, 0, 0, GetOwner(pTarget));
  obj->Set(pTarget);

  //Symbolgr��e anpassen
  var wdt = GDAR_TransmitterRadius * 2000 / GetDefWidth(SM09);

  //Symbol konfigurieren
  obj->SetObjDrawTransform(wdt, 0, 0, 0, wdt, 0);
  obj->SetGraphics("Big");
  obj->SetColorDw(RGB(255,0,0));
  obj->SetVisibility(VIS_Owner);

  return obj;
}

/* Allgemein */

protected func Hit()
{
  Sound("AmmoBoxHit*.ogg");
  Sound("CrateImpact*.ogg");
}
