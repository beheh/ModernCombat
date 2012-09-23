/*-- Apache --*/

#strict 2
#include BKHK

static const APCE_Seat_Pilot = 1;
static const APCE_Seat_Gunner = 2;
local pEntrance;

public func MaxDamage()		{return 190;}


/* Hitbox */

public func HitboxXOffset()	{return 15;}	//X-Abstand vom Offset zum Hitboxmittelpunkt
public func HitboxYOffset()	{return 5;}	//Y-Abstand vom Offset zum Hitboxmittelpunkt
public func HitboxWidth()	{return 160;}	//Breite der Hitbox
public func HitboxHeight()	{return 46;}	//Höhe der Hitbox

/* Initialisierung */

protected func Initialize()
{
  //Keine Blackhawks-Regel aktiv? Verschwinden
  if(NoBlackHawks())
    return RemoveObject();

  //Steuerung initialisieren
  throttle = 0;
  rotation = 0; 
  SetAction("Stand");

  //Hitbox erstellen
  InitializeHitbox();

  //Pilot
  aSeats = [];

  //Geschütze aufstellen
  pMGStation = CreateObject(WNRK,0,0,GetOwner());
  pMGStation -> Set(this,60,10,210,270);
  pMGStation -> Arm(LCAC);
  pRocketStation = CreateObject(WNRK,0,0,GetOwner());
  pRocketStation -> Set(this,10,100,270,270);
  pRocketStation -> Arm(ACRL);

  //Scheinwerfer einrichten
  pSpotlight = [0];

  //Zufälligen Song anwählen
  iRadioTrack = Random(RDIO_TrackCount)+1;

  //Rotorgeschwindigkeit
  iRotorSpeed = 0;

  //Vertices richtig drehen
  ScheduleCall(this,"ChangeDir",1,2);

  //Eingang
  pEntrance = CreateObject(ENTR, 0, 0, GetOwner());
  pEntrance->SetApache(this);
  pEntrance->SetOffset(GetDefCoreVal("Offset", "DefCore", GetID(), 0) * (1 - 2 * GetDir()) / 2, 5);

  //Neutrale Fahrzeuge sind weiß
  if(GetOwner() == NO_OWNER)
  {
    SetColorDw(RGB(255,255,255));
  }
  else
  {
    SetColorDw(GetPlrColorDw(GetOwner()));
  }

  AddEffect("VehicleNoOwner", this, 50, 38, this);

  return 1;
}

/* Erfassung */

public func & GetPilot()	{return aSeats[APCE_Seat_Pilot];}
public func & GetGunner()	{return aSeats[APCE_Seat_Gunner];}

/* Eingangssteuerung */

protected func Collection2(object pObj)
{
  if(GetOCF(pObj) & OCF_Alive && GetOCF(pObj) & OCF_CrewMember)
  {
    //Freund: Rein. Feind: Raus
    if(!Hostile(GetOwner(this),GetOwner(pObj)) || (GetOwner() == -1) || !GetPassengerCount())
    {
      //Soundschleife übergeben
      Sound("CockpitRadio.ogg", true, 0, 100, GetOwner(pObj)+1, +1);

      //Freien Sitz belegen
      if(!GetPilot())
        return EnterSeat(APCE_Seat_Pilot, pObj);
      if(!GetGunner())
        return EnterSeat(APCE_Seat_Gunner, pObj);

      //Kein Platz mehr
      return SetCommand(pObj, "Exit");
    }
    else
      return SetCommand(pObj, "Exit");
  }
}

/* Steuerung */

protected func ContainedUp(object ByObj)
{
  [$CtrlUp$]

  //Pilot
  if (ByObj == GetPilot())
  {
    if(GetY() < 80)
      return;
    //Autopilot aus
    ResetAutopilot();
    //Startup-Sequence
    if (!throttle && (GetAction() == "Stand" || GetAction() == "EngineShutDown"))
      SetAction("EngineStartUp");
    //beim Flug mehr Schub
    if(GetAction() == "Fly" || GetAction() == "Turn")
      if(GetPlrCoreJumpAndRunControl(GetOwner(GetPilot())) && !GetAutopilot())
        AddEffect("BlackhawkChangeThrottle", this, 50, 3, this, GetID(), BKHK_ThrottleSpeed);
      else
        throttle = BoundBy(throttle + BKHK_ThrottleSpeed, 0, BKHK_MaxThrottle);
  }

  //Schütze
  if (ByObj == GetGunner())
    pMGStation->~ControlUp(ByObj);

  return true;
}

protected func ContainedDown(object ByObj)
{
  [$CtrlDown$]

  //Pilot
  if (ByObj == GetPilot())
  {
    //Autopilot aus
    ResetAutopilot();
    //Motor aus
    if(!throttle && (GetAction() == "Fly" || GetAction() == "EngineStartUp") && GetContact(0, -1, CNAT_Bottom))
      SetAction("EngineShutDown");
    //Vom Gas weg
    if(GetAction() == "Fly" || GetAction() == "Turn")
      if(GetPlrCoreJumpAndRunControl(GetOwner(GetPilot())) && !GetAutopilot())
        AddEffect("BlackhawkChangeThrottle", this, 50, 3, this, GetID(), -BKHK_ThrottleSpeed);
      else
        throttle = BoundBy(throttle - BKHK_ThrottleSpeed, 0, BKHK_MaxThrottle);
  }

  //Schütze
  if(ByObj == GetGunner())
    pMGStation->~ControlDown(ByObj);

  return true;
}

protected func ContainedDownDouble(object ByObj)
{
  [$CtrlDownD$]

  //Pilot
  if (ByObj == GetPilot())
  {
    //Autopilot aus
    ResetAutopilot();
    //Motor aus
    if (throttle == 0 && (GetAction() == "Fly" || GetAction() == "EngineStartUp") && GetContact(0, -1, CNAT_Bottom))
      SetAction("EngineShutDown");
    //Vom Gas weg
    if (GetAction() == "Fly")
      throttle = BoundBy(throttle - BKHK_ThrottleSpeed*2, 0, 170);
  }

  return true;
}

protected func ContainedLeft(object ByObj)
{
  [$CtrlLeft$]

  //Pilot
  if (ByObj == GetPilot())
  {
    //Autopilot aus
    ResetAutopilot();
    if (GetAction() == "Fly" || GetAction() == "Turn")
      if(GetPlrCoreJumpAndRunControl(GetController(ByObj)))
        rotation = -BKHK_MaxRotation;
      else
        rotation = BoundBy(rotation - BKHK_ControlSpeed, -BKHK_MaxRotation, BKHK_MaxRotation);
  }

  //Schütze
  if(ByObj == GetGunner())
    pMGStation->~ControlLeft(ByObj);

  return true;
}

protected func ContainedRight(object ByObj, fRelease)
{
  [$CtrlRight$]
  
  //Pilot
  if (ByObj == GetPilot())
  {
    //Autopilot aus
    ResetAutopilot();
    if(fRelease)
      rotation = GetR();
    else 
      if (GetAction() == "Fly" || GetAction() == "Turn")
        if(GetPlrCoreJumpAndRunControl(GetController(ByObj)))
          rotation = BKHK_MaxRotation;
        else
          rotation = BoundBy(rotation + BKHK_ControlSpeed, -BKHK_MaxRotation, BKHK_MaxRotation);
  }

  //Schütze
  if(ByObj == GetGunner())
    pMGStation->~ControlRight(ByObj);

  return true;
}

protected func ContainedLeftDouble(object ByObj)
{
  [$CtrlLeftD$]
  //Pilot
  if (ByObj == GetPilot())
  {
    //Autopilot aus
    ResetAutopilot();
    if (GetDir() && GetAction() == "Fly")
      if (GetAction() == "Turn" || GetContact(this, -1))
        return true;
      else
        SetAction("Turn");
  }

  //Schütze
  if(ByObj == GetGunner())
    pMGStation->~ControlLeftDouble(ByObj);

  return true;
}

protected func ContainedRightDouble(object ByObj)
{
  [$CtrlRightD$]
  
  //Pilot
  if (ByObj == GetPilot())
  {
    //Autopilot aus
    ResetAutopilot();
    if (!GetDir() && GetAction() == "Fly")
      if (GetAction() == "Turn" || GetContact(this, -1))
        return true;
      else
        SetAction("Turn");
  }

  //Schütze
  if(ByObj == GetGunner())
    pMGStation->~ControlRightDouble(ByObj);

  return true;
}

protected func ContainedThrow(object ByObj)
{
  [Image=KOKR|$CtrlThrow$]
  
  //nicht wenn kaputt
  if (GetDamage() > MaxDamage())
    return true;
  //Piloten-Speedmenü
  if (ByObj == GetPilot())
  {
    var ring = CreateSpeedMenu(this, ByObj);

    var overlay;

    //Flareabwurf
    //Nur wenn geladen
    if(CanDeployFlares())
    {
      overlay = ring->AddThrowItem("$Flares$", "DeployFlares", ByObj, SMIN);
      SetGraphics("6", ring, SMIN, overlay, GFXOV_MODE_IngamePicture);
    }

    //Raketenwerfer
    if(RocketPodsReady())
    {
      overlay = ring->AddLeftItem("$RocketLauncher$", "FireRockets", ByObj, SMIN);
      SetGraphics("0", ring, SMIN, overlay, GFXOV_MODE_IngamePicture);
    }

    //HUD ein- oder ausblenden
    overlay = ring->AddRightItem("$HUD$", "SwitchHUD", ByObj, SMIN);
    SetGraphics("8", ring, SMIN, overlay, GFXOV_MODE_IngamePicture);

    //Scheinwerfer ein- oder ausschalten
    overlay = ring->AddDownItem("$Spotlight$", "SwitchSpotlights", ByObj, SMIN);
    SetGraphics("9", ring, SMIN, overlay, GFXOV_MODE_IngamePicture);

    //Radio ein- oder ausschalten
    overlay = ring->AddUpItem("$Radio$", "SwitchRadio", ByObj, SMIN);
    SetGraphics("10", ring, SMIN, overlay, GFXOV_MODE_IngamePicture);

    return Sound("BKHK_Switch.ogg", false, this, 100, GetOwner(ByObj) + 1);
  }

  //Schütze: Feuer eröffnen/einstellen
  if(ByObj == GetGunner())
    if(!GetPlrCoreJumpAndRunControl(GetController(ByObj)))
      pMGStation->~ControlThrow(ByObj);

  return true;
}

/* Sitzsteuerung */

protected func ContainedDigDouble(object ByObj)
{
  [$CtrlDigD$]
  CreateMenu(GetID(), ByObj, this, 0, "$Seats$", 0, 1);

    //Ausstieg
    AddMenuItem("<c ffff33>$Exit$</c>", "ExitClonk", STDR, ByObj, 0, ByObj, "$ExitDesc$");

    //Pilot
    if(GetPilot())
      AddMenuItem("<c 777777>$Pilot$</c>", "SeatOccupied", GetID(), ByObj, 0, ByObj, "$SeatOccupied$");
    else
      AddMenuItem("<c ffffff>$Pilot$</c>", Format("EnterSeat(%d, Object(%d))", BKHK_Seat_Pilot, ObjectNumber(ByObj)), GetID(), ByObj, 0, ByObj, "$PilotDesc$");

    //Schütze
    if(GetGunner())
      AddMenuItem("<c 777777>$Gunner$</c>", "SeatOccupied", GetID(), ByObj, 0, ByObj, "$SeatOccupied$");
    else
      AddMenuItem("<c ffffff>$Gunner$</c>", Format("EnterSeat(%d, Object(%d))", BKHK_Seat_Gunner, ObjectNumber(ByObj)), GetID(), ByObj, 0, ByObj, "$GunnerDesc$");

  return 1;
}

private func DeleteActualSeatPassenger(object Obj)
{
  if(!Obj) return;
  if(GetPilot() == Obj)
  {
    GetPilot() = 0;
    if(hud)
      RemoveObject(hud);
    SetGraphics(0, this, EFMN,BKHK_PilotLayer, GFXOV_MODE_Object, 0, GFX_BLIT_Additive, this);
  }
  if(GetGunner() == Obj)
  {
    GetGunner() = 0;
    if(pMGStation) pMGStation->SetGunner(0);
  }

  //Falls keine Passagiere außer Pilot mehr da
  if((!GetPassengerCount() && !GetPilot()) || (GetPassengerCount() == 1 && GetPilot()))
    SetGraphics(0, this, EFMN, BKHK_PassengerLayer, GFXOV_MODE_Object, 0, GFX_BLIT_Additive, this);
  return 1;
}

public func EnterSeat(int iSeat, object pObj)
{
  //Besetzt
  if (aSeats[iSeat])
  {
    SeatOccupied(0, pObj);
    return false;
  }
  
  //Alten Sitz räumen
  DeleteActualSeatPassenger(pObj);

  Sound("RSHL_Deploy.ogg", true, this, 100, GetOwner(pObj) + 1);

  //Pilot
  if (iSeat == BKHK_Seat_Pilot)
  {
    if(GetOwner() == -1 || GetPlayerTeam(GetOwner()) != GetPlayerTeam(GetOwner(pObj))) {
      //Besitz ergreifen
      SetOwnerFade(GetOwner(pObj));
    }
    SetGraphics("Pilot", this, GetID(), BKHK_PilotLayer, GFXOV_MODE_ExtraGraphics, 0, GFX_BLIT_Custom, this);
    GetPilot() = pObj;
    pObj->~SetHUDTarget(pRocketStation->GetAttWeapon());
    
    hud = CreateObject(AHUD, 0, 0, GetOwner(pObj));
    hud->~SetHelicopter(this);
    return true;
  }

  //Schütze
  if (iSeat == BKHK_Seat_Gunner)
  {
    SetGraphics("Passenger", this, GetID(), BKHK_PassengerLayer, GFXOV_MODE_ExtraGraphics, 0, GFX_BLIT_Custom, this);
    GetGunner() = pObj;
    pMGStation->~SetGunner(GetGunner());
    return true;
  }
}

protected func Ejection(object ByObj)
{
  if(!ByObj)
    return;
  if(!(GetOCF(ByObj) & OCF_CrewMember))
    return;
  
  if(GetPilot() == ByObj)
    ByObj->~SetHUDTarget(0);

  //Erst mal löschen
  DeleteActualSeatPassenger(ByObj);

  //Soundschleife übergeben
  Sound("CockpitRadio.ogg", true, 0, 100, GetOwner(ByObj)+1, -1);

  //Nicht bei Schaden
  if(GetDamage() >= MaxDamage()) return;

  SetPosition(GetX(pEntrance), GetY(pEntrance)+5, ByObj);

  if(!PathFree(GetX(pEntrance),GetY(pEntrance),GetX(pEntrance),GetY(pEntrance)+100))
    return;

  if(!GetEffect("CheckGround",ByObj))
    CreateObject(PARA,GetX(ByObj),GetY(ByObj),GetOwner(ByObj))->Set(ByObj);

  return true;
}

/* Pilotenfähigkeiten */

/* Raketenwerfer */

public func FireRockets()
{
  //Feuerbefehl geben
  pRocketStation->~ControlThrow(GetPilot());
}

public func RocketPodsReady()
{
  var weapon;
  if(!pRocketStation || !(weapon = pRocketStation->~GetAttWeapon()))
    return false;

  return (GetAmmo(0, weapon) == weapon->GetFMData(FM_AmmoLoad));
}

/* Schaden */

public func OnDestruction()
{
  //Inhalt auswerfen und töten bzw. zerstören
  for(var obj in FindObjects(Find_Container(this), Find_Not(Find_ID(FKDT))))
  {
    DeleteActualSeatPassenger(obj);
    if(GetOCF(obj) & OCF_Alive && GetID(Contained(obj)) != FKDT)
      DoDmg(200, DMG_Explosion, obj, 0, GetLastAttacker()+1);
    else
      DoDamage(200, obj);
    if(Contained(obj) == this)
      Exit(obj, 0, 0, Random(360), RandomX(-5, 5), RandomX(-4, 8), Random(10));
  }

  //Eingang entfernen
  RemoveObject(pEntrance, true);

  //Explosion
  FakeExplode(70, GetLastAttacker() + 1);
  FakeExplode(50, GetLastAttacker() + 1);
  RemoveObject();
  Sound("BigExplosion.ogg", false, this);
  Sound("StructuralDamage*.ogg", false, this);

  //Wrack erstellen
  var obj;
  obj = CreateObject(AEWK, 0, 20, GetLastAttacker());
  Incinerate(obj);
  SetDir(GetDir(), obj);
  SetR(GetR(), obj);
  SetXDir(GetXDir(), obj);
  SetYDir(GetYDir(), obj);
  SetRDir(GetRDir(), obj);
  return true;
}

protected func ChangeDir()
{
  var fBool = _inherited();
  pEntrance->SetOffset(GetDefCoreVal("Offset", "DefCore", GetID(), 0) * (1 - 2 * GetDir()) / 2, 5);
  return fBool;
}