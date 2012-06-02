/*-- Blackhawk --*/

#strict 2
#include CVHC

local throttle,	rotation, hud;
local pMGStation, pRocketStation;
local smokereload, flarereload;
local fShowSpotlight, pSpotlight;
local fRadioPlaying,	iRadioTrack;
local iWarningSound
local iRotorSpeed;

static const BKHK_ThrottleSpeed = 5;
static const BKHK_RotationSpeed = 1;
static const BKHK_ControlSpeed = 3;
static const BKHK_MaxThrottle = 200;
static const BKHK_MaxRotation = 30;
static const BKHK_AutoThrottleSpeed = 1;
static const BKHK_AutoMaxThrottle = 150;
static const BKHK_AutoMaxRotation = 10;

static const BKHK_PilotLayer = 2;
static const BKHK_PassengerLayer = 3;

local aSeats;

static const BKHK_Seat_Pilot = 1;	
static const BKHK_Seat_Gunner = 2;
static const BKHK_Seat_Coordinator = 3;
static const BKHK_Seat_Passenger1 = 4;
static const BKHK_Seat_Passenger2 = 5;
static const RDIO_TrackCount = 6;

/* Callbacks */

protected func IsRadio()		{return true;}
public func IsMachine()			{return true;}
public func MaxDamage()			{return 200;}
public func IsThreat()			{return true;}
public func IsHelicopter()		{return true;}

public func EngineRunning()		{return GetEffect("Engine", this);}
public func GetRotorSpeed()		{return iRotorSpeed;}

public func IsBulletTarget(id idBullet, object pBullet)
{
  //Nicht treffbar wenn neutral, au�er bei FF
  if(ObjectCount(NOFF) && GetOwner() == NO_OWNER)
    return false;

  if (EffectVar(0, pBullet, GetEffect("IntHeliProtection", pBullet)) == this)
    return false;

  if(idBullet == MISS || idBullet == HMIS || idBullet == MISL || idBullet == LRML || idBullet == ESHL)
    return ObjectDistance(pBullet) < 40;
  return true;
}

protected func FxIntHeliProtectionStart(object pTarget, int iEffect, int iTemp, object pObj)
{
  if (!iTemp)
    EffectVar(0, pTarget, iEffect) = pObj;
}

/* Initialisierung */

protected func Initialize()
{
  if (NoBlackHawks())
    return RemoveObject();

  //Steuerung initialisieren
  throttle = 0;
  rotation = 0; 
  SetAction("Stand");

  //Pilot
  aSeats = [];

  //Gesch�tze aufstellen
  pMGStation = CreateObject(WNRK,0,0,GetOwner());
  pMGStation -> Set(this,10,90,90,270);
  pMGStation -> Arm(ACCN);
  pRocketStation = CreateObject(WNRK,0,0,GetOwner());
  pRocketStation -> Set(this,40,10,210,270);
  pRocketStation -> Arm(RLSA);

  //Scheinwerfer einrichten
  pSpotlight = [0];

  //Zuf�lligen Song anw�hlen
  iRadioTrack = Random(RDIO_TrackCount)+1;

  //Rotorgeschwindigkeit
  iRotorSpeed = 0;

  //Vertices richtig drehen
  ScheduleCall(this,"ChangeDir",1,2);

  //Eingang
  SetEntrance(true);

  return _inherited();
}

/* Positionsberechnung von Objekten */

public func GetPosition(int iVertex, int& iX, int& iY)
{
	iX = GetVertex(iVertex, false);
	iY = GetVertex(iVertex, true);
	return true;
}

/* Erfassung */

public func & GetPilot()        {return aSeats[BKHK_Seat_Pilot];}
public func & GetGunner()       {return aSeats[BKHK_Seat_Gunner];}
public func & GetCoordinator()  {return aSeats[BKHK_Seat_Coordinator];}
public func & GetPassenger1()   {return aSeats[BKHK_Seat_Passenger1];}
public func & GetPassenger2()   {return aSeats[BKHK_Seat_Passenger2];}
public func GetThrottle()       {return throttle;}

public func GetPassengerCount()
{
  var i;
  for (var passenger in aSeats)
    if (passenger)
      i++;
  return i;
}

public func GetRocket()
{
  var aRockets = FindObjects(Find_Func("IsRocket"), Find_Distance(800, AbsX(GetX()), AbsY(GetY())), Find_Not(Find_Func("IsDamaged")));
  var fRocket = false;
  for(var pCheck in aRockets)
  {
    if(FindObject(NOFF) && !Hostile(GetOwner(pCheck), GetOwner())) continue;
    if(ObjectDistance(pCheck, this) <= 150) fRocket = true;
    var aObj = FindObjects(Find_ID(GetID()), Find_OnLine(AbsX(GetX(pCheck)), AbsY(GetY(pCheck)), AbsX(GetX(pCheck)+Sin(GetR(pCheck), 800)), AbsX(GetX(pCheck)-Cos(GetR(pCheck), 800))));
    for(var pCheck in aObj)
    {
      if(pCheck == this)
      {
        fRocket = true;
        break;
      }
    }
    if(fRocket) break;
  }
  return fRocket;
}

/* Autopilot */

public func GetAutopilot()	{return GetEffect("BlackhawkAutopilot", this);}

public func SetAutopilot(object pTarget, int iX, int iY)
{
  if(!GetPilot()) return;
  ResetAutopilot();
  var xto, yto;
  if(pTarget)
  {
    xto = AbsX(GetX(pTarget));
    yto = AbsY(GetY(pTarget));
  }
  xto += iX;
  yto += iY;
  AddEffect("BlackhawkAutopilot", this, 10, 1, this, 0, xto, yto);
  return true;
}

public func ResetAutopilot()
{
  while(GetEffect("BlackhawkAutopilot", this)) RemoveEffect("BlackhawkAutopilot", this);
  return true;
}

protected func FxBlackhawkAutopilotStart(object pTarget, int iNumber, iTemp, int iX, int iY)
{
  if(GBackLiquid(AbsX(iX), AbsY(iY))) return -1;
  EffectVar(0, pTarget, iNumber) = iX;
  EffectVar(1, pTarget, iNumber) = iY;	
}

protected func FxBlackhawkAutopilotTimer(object pTarget, int iNumber, int iTime)
{
  if(!pTarget->GetPilot()) return -1;
  var iX = EffectVar(0, pTarget, iNumber);
  var iY = EffectVar(1, pTarget, iNumber);
  if(GetY(pTarget) < iY-50)
  {
    if(GetYDir(pTarget) < 3 || GetContact(this, -1, CNAT_Bottom))
    {
      //vom Gas weg
      if(GetAction() == "Fly" || GetAction() == "Turn")
      {
      	throttle = BoundBy(throttle - BKHK_AutoThrottleSpeed, 0, BKHK_AutoMaxThrottle);
      }
      else if(throttle != 0 && GetAction() == "Fly")
      {
        SetAction("EngineShutDown");
        return pTarget->ResetAutopilot();
      }
    }
  }
  else if(GetY(pTarget) > iY+50)
  {
    if(GetYDir(pTarget) > -3)
    {
      if(GetAction() == "Fly" || GetAction() == "Turn")
      {
      	throttle = BoundBy(throttle + BKHK_AutoThrottleSpeed, 0, BKHK_AutoMaxThrottle);
      }
      else if(throttle == 0 && GetAction() == "Stand")
      {
      	SetAction("EngineStartUp");
      	return FX_OK;
      }
    }
  }
  else
  {
    if(!(iTime % BoundBy(5-GetYDir(pTarget), 5, 0)) && GetYDir(pTarget) > 0)
    {
      //beim Flug mehr Schub
      if(GetAction()=="Fly" || GetAction()=="Turn")
      throttle = BoundBy(throttle + BKHK_AutoThrottleSpeed, 0, BKHK_AutoMaxThrottle);
    }
    else
    if(!(iTime % BoundBy(GetYDir(pTarget), 5, 0)) && GetYDir(pTarget) < 0)
    {
      //vom Gas weg
      if(GetAction()=="Fly" || GetAction()=="Turn")
      throttle = BoundBy(throttle - BKHK_AutoThrottleSpeed, 0, BKHK_AutoMaxThrottle);
    }
  }
  if(GetX(pTarget) > iX+50)
  {
    if (GetAction()=="Fly" || GetAction()=="Turn")
    rotation = BoundBy(rotation - BKHK_ControlSpeed, -BKHK_AutoMaxRotation, BKHK_AutoMaxRotation);
    if (rotation < 0 && GetDir() && GetAction()=="Fly")
    {
      if (GetAction() == "Turn" || GetContact(this, -1)) return true;
      SetAction("Turn");
    }
  }
  else
  if(GetX(pTarget) < iX-50)
  {
    if (GetAction()=="Fly" || GetAction()=="Turn") 
    rotation = BoundBy(rotation + BKHK_ControlSpeed, -BKHK_AutoMaxRotation, BKHK_AutoMaxRotation);
    if (rotation > 0 && !GetDir() && GetAction()=="Fly")
    {
      if(GetAction() == "Turn" || GetContact(this, -1)) return true;
      SetAction("Turn");
    }
  }
  else
  if(GetXDir() != 0)
  {
    if(GetXDir(pTarget) < -1)
    {
      rotation = BoundBy(rotation + BKHK_ControlSpeed, 0, BKHK_AutoMaxRotation);
    }
    else if(GetXDir(pTarget) > 1)
    {
      rotation = BoundBy(rotation - BKHK_ControlSpeed, -BKHK_AutoMaxRotation, 0);
    }
    else
    {
      if(GetXDir(pTarget) < 0)
      {
        rotation = BoundBy(rotation - BKHK_ControlSpeed, 0, BKHK_AutoMaxRotation);
      }
      else if(GetXDir(pTarget) > 0)
      {
        rotation = BoundBy(rotation + BKHK_ControlSpeed, -BKHK_AutoMaxRotation, 0);
      }
    }
  }
  return FX_OK;
}

/* Eingangssteuerung */

protected func Ejection(object ByObj)
{
  if(!ByObj)
    return;
  if(!(GetOCF(ByObj) & OCF_CrewMember))
    return;
  
  //Erst mal l�schen
  DeleteActualSeatPassenger(ByObj);
    
  //Soundschleife �bergeben
  Sound("CockpitRadio.ogg", true, 0, 100, GetOwner(ByObj)+1, -1);

  //Nicht bei Schaden
  if(GetDamage() >= MaxDamage()) return;
  
  if(!PathFree(GetX(),GetY(),GetX(),GetY()+100))
    return;

  if(!GetEffect("CheckGround",ByObj))
    CreateObject(PARA,GetX(ByObj),GetY(ByObj),GetOwner(ByObj))->Set(ByObj);
  
  return true;
}

protected func ControlCommand(string szCommand, object Target, int TargetX, int TargetY, object target2, int Data, object ByObj)
{
  if (szCommand == "Exit")
  {
    var rot = GetDir()*180-90 + GetR() + GetDir()*120-60;
    Exit(ByObj, Sin(rot,25), -Cos(rot,25), GetR(), GetXDir(0,1), GetYDir(0,1), GetRDir());
    return true;
  }
  if (szCommand == "MoveTo" || szCommand == "Attack")
  {
    if(ByObj == GetPilot())
      SetAutopilot(Target, TargetX, TargetY);
    else if(ByObj == GetGunner())
    	pMGStation->~ControlCommand(szCommand, Target, TargetX, TargetY, target2, Data, ByObj);
    else if(ByObj == GetCoordinator())
    	pRocketStation->~ControlCommand(szCommand, Target, TargetX, TargetY, target2, Data, ByObj);
    return true;
  }
}

protected func Collection2(object pObj)
{
  if(GetOCF(pObj) & OCF_Alive && GetOCF(pObj) & OCF_CrewMember)
  {
    //Freund: Rein. Feind: Raus
    if(!Hostile(GetOwner(this),GetOwner(pObj)) || (GetOwner() == -1) || !GetPassengerCount())
    {
      //Soundschleife �bergeben
      Sound("CockpitRadio.ogg", true, 0, 100, GetOwner(pObj)+1, +1);

      //Freien Sitz belegen
      if(!GetPilot())
        return EnterSeat(BKHK_Seat_Pilot, pObj);
      if(!GetGunner())
        return EnterSeat(BKHK_Seat_Gunner, pObj);
      if(!GetCoordinator())
        return EnterSeat(BKHK_Seat_Coordinator, pObj);        
      if(!GetPassenger1())
        return EnterSeat(BKHK_Seat_Passenger1, pObj);        
      if(!GetPassenger2())
        return EnterSeat(BKHK_Seat_Passenger2, pObj);

      //Kein Platz mehr
      return SetCommand(pObj, "Exit");
    }
    else
      return SetCommand(pObj, "Exit");
  }
}

/* Steuerung */

protected func FxBlackhawkChangeThrottleStart(object pTarget, int iNumber, iTemp, int iChange)
{
  EffectVar(0, pTarget, iNumber) = iChange;
  EffectCall(pTarget, iNumber, "Timer");
}

protected func FxBlackhawkChangeThrottleTimer(object pTarget, int iNumber, int iTime)
{
	var old = throttle;
  throttle = BoundBy(throttle + EffectVar(0, pTarget, iNumber), 0, BKHK_MaxThrottle);
  if(old == throttle) return -1;
  return FX_OK;
}

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

  //Sch�tze
  if (ByObj == GetGunner())
    pMGStation->~ControlUp(ByObj);
  //Koordinator
  if (ByObj == GetCoordinator())
    pRocketStation->~ControlUp(ByObj);

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

  //Sch�tze
  if(ByObj == GetGunner())
    pMGStation->~ControlDown(ByObj);
  //Koordinator
  if(ByObj == GetCoordinator())
    pRocketStation->~ControlDown(ByObj);

  return true;
}

protected func ContainedUpReleased(object ByObj)
{
  if(ByObj == GetPilot())
    RemoveEffect("BlackhawkChangeThrottle", this);

  return true;
}

protected func ContainedDownReleased(object ByObj)
{
  if(ByObj == GetPilot())
    RemoveEffect("BlackhawkChangeThrottle", this);

  return true;
}

protected func ContainedUpDouble(object ByObj)
{
  [$CtrlUpD$]
  
  //Pilot
  if (ByObj == GetPilot())
  {
    if(GetY() < 80)
      return;
    //Autopilot aus
    ResetAutopilot();
    if(throttle == 0 && (GetAction() == "Stand" || GetAction() == "EngineShutDown"))
      SetAction("EngineStartUp");  
    if(GetAction() == "Fly")
      throttle = BoundBy(throttle + BKHK_ThrottleSpeed * 2, 0, BKHK_MaxThrottle);
    return true;
  }
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

  if(ByObj == GetPassenger1() || ByObj == GetPassenger2())
    SetCommand(ByObj,"Exit");
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

  //Sch�tze
  if(ByObj == GetGunner())
    pMGStation->~ControlLeft(ByObj);
  //Koordinator
  if(ByObj == GetCoordinator())
    pRocketStation->~ControlLeft(ByObj);

  return true;
}

protected func ContainedLeftReleased(object ByObj)
{
  if(ByObj == GetPilot())
    rotation = GetR();
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

  //Sch�tze
  if(ByObj == GetGunner())
    pMGStation->~ControlRight(ByObj);
  //Koordinator
  if(ByObj == GetCoordinator())
    pRocketStation->~ControlRight(ByObj);

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

  //Sch�tze
  if(ByObj == GetGunner())
    pMGStation->~ControlLeftDouble(ByObj);
  //Koordinator
  if(ByObj == GetCoordinator())
    pRocketStation->~ControlLeftDouble(ByObj);

  return true;
}

protected func ContainedRightReleased(object ByObj)
{
  if(ByObj == GetPilot())
    rotation = GetR();
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

  //Sch�tze
  if(ByObj == GetGunner())
    pMGStation->~ControlRightDouble(ByObj);
  //Koordinator
  if(ByObj == GetCoordinator())
    pRocketStation->~ControlRightDouble(ByObj);

  return true;
}

public func CanDeployFlares()
{
  return !flarereload;
}

public func CanDeploySmokeWall()
{
  return !smokereload;
}

protected func ContainedThrow(object ByObj)
{
  [Image=KOKR|$CtrlThrow$]
  
  //nicht wenn kaputt
  if (GetDamage() > MaxDamage())
    return true;
  //Piloten-Speedmen�
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

    //Rauchwand
    //Nur wenn geladen
    if(CanDeploySmokeWall())
    {
      overlay = ring->AddLeftItem("$Smoke$", "DeploySmoke", ByObj, SMIN);
      SetGraphics("7", ring, SMIN, overlay, GFXOV_MODE_IngamePicture);
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

  //Sch�tze: Feuer er�ffnen/einstellen
  if(ByObj == GetGunner())
    if(!GetPlrCoreJumpAndRunControl(GetController(ByObj)))
      pMGStation->~ControlThrow(ByObj);
  //Koordinator
  if(ByObj == GetCoordinator())
    pRocketStation->~ControlThrow(ByObj);

  return true;
}

protected func ContainedDig(object pBy)
{
  if (pBy == GetCoordinator())
    pRocketStation->~ControlDig(pBy);
}

public func ContainedUpdate(object ByObj, int comdir, bool dig, bool throw)
{
  if(ByObj == GetGunner())
    if(throw)
      return pMGStation->ControlThrow(ByObj);
    else
      return pMGStation->StopAutoFire();
}

//eine Funktion, welche einfach nur die Richtung eines Objektes �ndert
protected func ChangeDir()
{
  for (var i = 0; i < GetDefCoreVal("Vertices", "DefCore", GetID()); i++)
    SetVertex(i, 0, (GetDir() * 2 - 1) * GetDefCoreVal("VertexX", "DefCore", GetID(), i), this, 2);
  SetDir(!GetDir());

  //Eventuelle Statusbalken informieren
  for(var bar in FindObjects(Find_Func("IsBar"), Find_Func("BarActive"), Find_ActionTarget(this)))
    bar->~PositionToVertex(true);

  return true;
}

/* Pilotenf�higkeiten */

/* Flareabwurf */

public func DeployFlares()
{
  //Flares erstellen
  CastParticles("Smoke3", GetEffectData(EFSM_ExplosionEffects) * 4 + 1, 50, 0, 0, 120, 150, RGBa(255, 255, 255, 120), RGBa(255, 255, 255, 150));
  for(var i = -3; i < 3; i++)
  {
    var flare = CreateObject(FLRE, i * 10, 20, GetOwner(GetPilot()));
    SetXDir((GetXDir() * 2 / 3) + (i * RandomX(18, 25)), flare, 10);
    SetYDir((GetYDir() * 2 / 3) + (i * RandomX(-5, 0)), flare, 10);
  }

  //Sound
  Sound("BKHK_Switch.ogg", false, this, 100, GetOwner(GetPilot()) + 1);
  Sound("BKHK_DeployFlares.ogg");

  //Flareladezeit setzen
  flarereload = 35 * 10;
}

/* Rauchwand */

public func DeploySmoke()
{
  //Rauch erstellen
  CastParticles("Smoke3", GetEffectData(EFSM_ExplosionEffects) * 6 + 1, 80, 0, 0, 120, 150, RGBa(255, 255, 255, 120), RGBa(255, 255, 255, 150));
  for(var i = -5; i < 5; i++)
  {
    var smoke = CreateObject(SM4K, 0, 0, GetOwner(GetPilot()));
    SetXDir((GetXDir() * 2 / 3) + (i * RandomX(2, 4)), smoke, 10);
    SetYDir((GetYDir() * 2 / 3) + RandomX(10, 20), smoke, 10);
  }

  //Sound
  Sound("BKHK_Switch.ogg", false, this, 100, GetOwner(GetPilot()) + 1);
  Sound("BKHK_DeploySmoke.ogg");

  //Rauchwandladezeit setzen
  smokereload = 35 * 30;

  return true;
}

/* HUD (de)aktivieren */

public func SwitchHUD()
{
  //Kein HUD? Erstellen
  if(!hud)
  {
    hud = CreateObject(BHUD, 0, 0, GetOwner(GetPilot()));
    hud->SetHelicopter(this);
    SetOwner(GetOwner(), hud);
  }
  else
  {
    //Sichtbar? Dann ausblenden
    if(GetVisibility(hud) & VIS_Owner)
      SetVisibility(VIS_None, hud);
    //Ansonsten einblenden
    else
      SetVisibility(VIS_Owner, hud);
  }

  //Sound
  Sound("BKHK_Switch.ogg", false, this, 100, GetOwner(GetPilot()) + 1);
  return true;
}

/* Scheinwerfer (de)aktivieren */

public func SwitchSpotlights()
{
  fShowSpotlight = !fShowSpotlight;

  //Sound
  Sound("BKHK_Switch.ogg", false, this, 100, GetOwner(GetPilot()) + 1);
}
                           
public func UpdateSpotlights()
{
  pSpotlight[0]->ChangeR(pMGStation->AimAngle());
  return true;
}

public func CreateSpotlights()
{
  pSpotlight[0] = CreateLight(LHC2, 5000, RGBa(255,255,220,70), pMGStation);
  pSpotlight[0]->ChangeR(pMGStation->AimAngle());
  pSpotlight[0]->ChangeOffset(0, 3);
  return true;
}

public func RemoveSpotlights()
{
  if(pSpotlight[0])
    RemoveObject(pSpotlight[0]);

  return true;
}

/* Radio (de)aktivieren */

protected func StopSong(int iPlr)
{
  Sound("RadioMusicTitle_O0*.ogg", false, this, 0, iPlr, -1);
}

protected func StartSong(int iPlrPlusOne)
{
  if(IsPlaying())
    Sound(Format("RadioMusicTitle_O0%d.ogg",iRadioTrack), false, this, 0, iPlrPlusOne, 1);
}

public func SwitchRadio()
{
  if(IsPlaying())
  {
    Sound("RadioMusicTitle_O0*.ogg", false, this, 0, 0, -1);
    iRadioTrack = iRadioTrack % RDIO_TrackCount + 1;

    fRadioPlaying = false;
  }
  else
  {
    for(var i = 0; i < GetPlayerCount(); i++)
    {
      if(!GetPlrExtraData(i, "CMC_RadioMusicMode"))
        Sound(Format("RadioMusicTitle_O0%d.ogg",iRadioTrack), false, this, 0, i+1, 1);
    }
    fRadioPlaying = true;
  }

  //Sound
  Sound("BKHK_Switch.ogg", false, this, 100, GetOwner(GetPilot()) + 1);
}

public func IsPlaying()
{
  return fRadioPlaying;
}

/* Sitzsteuerung */

protected func ContainedDigDouble(object pClonk)
{
  [$CtrlDigD$]
  return OpenSeatMenu(pClonk);
}

protected func OpenSeatMenu(object pClonk, int iSelection) {
  CreateMenu(GetID(), pClonk, this, 0, "$Seats$", 0, C4MN_Style_Context, false);

  //Ausstieg
  AddMenuItem("<c ffff33>$Exit$</c>", "ExitClonk", STDR, pClonk, 0, pClonk, "$ExitDesc$");

  //Pilot
  if(GetPilot())
    AddMenuItem("<c 777777>$Pilot$</c>", "SeatOccupied", GetID(), pClonk, 0, pClonk, "$SeatOccupied$");
  else
    AddMenuItem("<c ffffff>$Pilot$</c>", Format("EnterSeat(%d, Object(%d))", BKHK_Seat_Pilot, ObjectNumber(pClonk)), GetID(), pClonk, 0, pClonk, "$PilotDesc$");

  //Sch�tze
  if(GetGunner())
    AddMenuItem("<c 777777>$Gunner$</c>", "SeatOccupied", GetID(), pClonk, 0, pClonk, "$SeatOccupied$");
  else
    AddMenuItem("<c ffffff>$Gunner$</c>", Format("EnterSeat(%d, Object(%d))", BKHK_Seat_Gunner, ObjectNumber(pClonk)), GetID(), pClonk, 0, pClonk, "$GunnerDesc$");

  //Koordinator
  if(GetCoordinator())
    AddMenuItem("<c 777777>$Coordinator$</c>", "SeatOccupied", GetID(), pClonk, 0, pClonk, "$SeatOccupied$");
  else
    AddMenuItem("<c ffffff>$Coordinator$</c>", Format("EnterSeat(%d, Object(%d))", BKHK_Seat_Coordinator, ObjectNumber(pClonk)), GetID(), pClonk, 0, pClonk, "$CoordinatorDesc$");

  //Passagier 1
  if(GetPassenger1())
    AddMenuItem("<c 777777>$Passenger$</c>", "SeatOccupied", GetID(), pClonk, 0, pClonk, "$SeatOccupied$");
  else
    AddMenuItem("<c ffffff>$Passenger$</c>", Format("EnterSeat(%d, Object(%d))", BKHK_Seat_Passenger1, ObjectNumber(pClonk)), GetID(), pClonk, 0, pClonk, "$PassengerDesc$");

  //Passagier 2
  if(GetPassenger2())
    AddMenuItem("<c 777777>$Passenger$</c>", "SeatOccupied", GetID(), pClonk, 0, pClonk, "$SeatOccupied$");
  else
    AddMenuItem("<c ffffff>$Passenger$</c>", Format("EnterSeat(%d, Object(%d))", BKHK_Seat_Passenger2, ObjectNumber(pClonk)), GetID(), pClonk, 0, pClonk, "$PassengerDesc$");
  
  //Ausstieg per Seil
  AddMenuItem("<c ffff33>$ExitRope$</c>", "ExitClonkByRope", CK5P, pClonk, 0, pClonk, "$ExitRopeDesc$");
  
  SelectMenuItem(iSelection, pClonk);
}

private func SeatOccupied(a, object pClonk, int iSelection)
{
  //Hinweis
  PlayerMessage(GetOwner(pClonk), "$SeatOccupied$", pClonk);

  //Sound
  Sound("BKHK_SwitchFail.ogg", 0, 0, 0, GetOwner(pClonk) + 1);

  //Men� wiederer�ffnen
  ContainedDigDouble(pClonk);

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
  if(GetCoordinator() == Obj)
  {
    GetCoordinator() = 0;
    if(pRocketStation) pRocketStation->SetGunner(0);
  }
  if(GetPassenger1() == Obj)
    GetPassenger1() = 0;

  if(GetPassenger2() == Obj)
    GetPassenger2() = 0;

  //Falls keine Passagiere au�er Pilot mehr da
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
  
  //Alten Sitz r�umen
  DeleteActualSeatPassenger(pObj);

  Sound("RSHL_Deploy.ogg", true, this, 100, GetOwner(pObj) + 1);

  //Pilot
  if (iSeat == BKHK_Seat_Pilot)
  {
    if(GetOwner() == -1 || GetPlayerTeam(GetOwner()) != GetPlayerTeam(GetOwner(pObj)))
    {
      //Besitz ergreifen
      SetOwnerFade(GetOwner(pObj));
    }
    SetGraphics("Pilot", this, GetID(), BKHK_PilotLayer, GFXOV_MODE_ExtraGraphics, 0, GFX_BLIT_Custom, this);
    GetPilot() = pObj;
    hud = CreateObject(BHUD, 0, 0, GetOwner(pObj));
    hud->~SetHelicopter(this);
    Sound("BKHK_Switch.ogg", false, this, 100, GetOwner(GetPilot()) + 1);
    return true;
  }

  //Sch�tze
  if (iSeat == BKHK_Seat_Gunner)
  {
    SetGraphics("Passenger", this, GetID(), BKHK_PassengerLayer, GFXOV_MODE_ExtraGraphics, 0, GFX_BLIT_Custom, this);
    GetGunner() = pObj;
    pMGStation->~SetGunner(GetGunner());
    return true;
  }

  //Koordinator
  if (iSeat == BKHK_Seat_Coordinator)
  {
    SetGraphics("Passenger", this, GetID(), BKHK_PassengerLayer, GFXOV_MODE_ExtraGraphics, 0, GFX_BLIT_Custom, this);
    GetCoordinator() = pObj;
    pRocketStation->~SetGunner(GetCoordinator());
    return true;
  }

  //Passagier 1
  if (iSeat == BKHK_Seat_Passenger1)
  {
    SetGraphics("Passenger", this, GetID(), BKHK_PassengerLayer, GFXOV_MODE_ExtraGraphics, 0, GFX_BLIT_Custom, this);
    GetPassenger1() = pObj;
    return true;
  }

  //Passagier 2
  if (iSeat == BKHK_Seat_Passenger2)
  {
    SetGraphics("Passenger", this, GetID(), BKHK_PassengerLayer, GFXOV_MODE_ExtraGraphics, 0, GFX_BLIT_Custom, this);
    GetPassenger2() = pObj;
    return true;
  }
}

/* Ausstieg per Seil */

public func GetRopeAttach()
{
  return pMGStation;
}

private func ExitClonk(a, pClonk)
{
  SetCommand(pClonk, "Exit");
}

private func ExitClonkByRope(a, pClonk)
{
  AddEffect("CheckGround", pClonk, 30, 3, this, GetID(), this);
  SetCommand(pClonk, "Exit");
}

protected func FxCheckGroundStart(pTarget, iNo, iTemp, pHeli)
{
  if(iTemp)
    return;
  if(!pHeli)
    return;
  var pRope = CreateObject(CK5P, 0, 0, GetOwner(pTarget));
  pRope->ConnectObjects(pHeli ,pTarget);
  pRope->SetRopeLength(10);
  EffectVar(0, pTarget, iNo) = pRope; //Das Seil
  EffectVar(1, pTarget, iNo) = pHeli; //Der Helikopter
}

protected func FxCheckGroundTimer(pTarget, iNo, iTime)
{
  var pRope = EffectVar(0, pTarget, iNo);
  //Knapp �ber dem Boden, Seil zu lang, zu lang die Dauer oder falsche Aktion?
  if(!PathFree(GetX(pTarget), GetY(pTarget), GetX(pTarget), GetY(pTarget) + 30)
     || pRope->GetRopeLength() > 300
     || !WildcardMatch(GetAction(pTarget), "*Jump*"))
  {
    //Abspringen, Seil zur�ck zum Heli schicken.
    RemoveObject(pRope);
    SetYDir(20,pTarget);
    return -1;
  }
  else
    pRope->SetRopeLength(iTime * 4 + 10);
}

/* Zerst�rung */

protected func Destruction()
{
  if(hud)
    RemoveObject(hud);
  if(pMGStation)
    RemoveObject(pMGStation);
  if(pRocketStation)
    RemoveObject(pRocketStation);
  RemoveSpotlights();

  return true;
}

/* Schaden */

public func OnDmg(int iDmg, int iType)
{
  if(iType == DMG_Energy)	return -20;	//Energie
  if(iType == DMG_Bio)		return 100;	//S�ure und biologische Schadstoffe
  if(iType == DMG_Melee)	return 80;	//Nahkampf
  if(iType == DMG_Fire)		return 50;	//Feuer
  if(iType == DMG_Explosion)	return -70;	//Explosionen
  if(iType == DMG_Projectile)	return 80;	//Projektile

  return 50;
}

public func OnDamage(int iDamage)
{
  //Reperaturen haben keine Auswirkung
  if(iDamage < 1) return;
  //Schaden dem HUD melden
  if(hud)
    hud->DamageReceived();
  if(GetContact(this, -1))
    ResetAutopilot();
  //Sound
  Sound("MetalHit*.ogg");

  return true;
}

public func OnDestruction()
{
  //Inhalt auswerfen und t�ten bzw. zerst�ren
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

  //Explosion
  FakeExplode(70, GetLastAttacker() + 1);
  FakeExplode(50, GetLastAttacker() + 1);
  RemoveObject();
  Sound("BigExplosion.ogg", false, this);
  Sound("StructuralDamage*.ogg", false, this);

  //Wrack erstellen
  var obj;
  obj = CreateObject(BHWK, 0, 20, GetLastAttacker());
  Incinerate(obj);
  SetDir(GetDir(), obj);
  SetR(GetR(), obj);
  SetXDir(GetXDir(), obj);
  SetYDir(GetYDir(), obj);
  SetRDir(GetRDir(), obj);
  return true;
}

/* Kollisionsverhalten */

protected func ContactTop()
{
  if (GetCon() != 100) return;
  DoDmg(10, 0, this, 1,  GetOwner() + 1);
  for (var i; i < GetVertexNum(); i++)
    if(GetContact(0, i))
      CreateParticle("Blast", GetVertex(i), GetVertex(i, true), 0, 0, RandomX(50, 100), RGB(255, 255, 255));

  //Sound
  Sound("HeavyHit*.ogg", false, pMGStation);
  SetYDir(Min(GetYDir(), -40) / -2);
  return true;
}

protected func ContactBottom()
{
  if (GetCon() != 100) return;
  if (GetYDir() > 25)
  {
    DoDmg(GetYDir(0, 500) / 2, 0, this, 1,  GetOwner() + 1);
    for (var i; i < GetVertexNum(); i++)
      if (GetContact(0, i))
        CreateParticle("Blast", GetVertex(i), GetVertex(i, true), 0, 0, RandomX(50,100), RGB(255,255,255));
    Sound("HeavyHit*.ogg", false, pMGStation);
    SetYDir(GetYDir() * -2 / 3);
  }
  if(GetContact(0, -1, CNAT_Left | CNAT_Right) && throttle)
    SetYDir(Max(GetYDir(), 20) * -2 / 3);
  return true;
}

protected func ContactLeft()
{
  if (GetCon() != 100) return;
  if (Abs(GetXDir()) > 20 || Abs(GetYDir()) > 20)
  {
    DoDmg(Sqrt(GetXDir(0, 500)**2 + GetYDir(0, 500)**2), 0, this, 1,  GetOwner() + 1);
    for (var i; i < GetVertexNum(); i++)
      if (GetContact(0, i))
        CreateParticle("Blast", GetVertex(i), GetVertex(i, true), 0, 0, RandomX(50, 100), RGB(255, 255, 255));
    Sound("HeavyHit*.ogg", false, pMGStation);
  }
  //Abprallen
  SetXDir(Max(GetXDir(),40) / -2, this);
  return true;
}

protected func ContactRight()
{
  if (GetCon() != 100) return;
  if (Abs(GetXDir()) > 20 || Abs(GetYDir()) > 20)
  {
    DoDmg(Sqrt(GetXDir(0, 500)**2 + GetYDir(0, 500)**2), 0, this, 1,  GetOwner() + 1);
    for (var i; i < GetVertexNum(); i++)
      if (GetContact(0, i))
        CreateParticle("Blast", GetVertex(i), GetVertex(i, true), 0, 0, RandomX(50, 100), RGB(255, 255, 255));
    Sound("HeavyHit*.ogg", false, pMGStation);
  }
  SetXDir(Max(GetXDir(), 40) / -2,this);
  return true;
}

//Objekt, die in den Rotor geraten, verursachen Schaden
protected func RejectCollect(id ID, object ByObj)
{
  //Objekte im Helikopter ablegbar
  if (Contained(ByObj))
    return;
  //Von au�en kommend: Schaden
  if (GetRotorSpeed() > 0)
  {
    var dir = (!GetDir())*2-1;
    DoDamage(GetMass(ByObj));
    ProtectedCall(ByObj, "Hit");
    SetXDir((Random(30) + 30) * dir, ByObj);
    SetYDir(RandomX(-25, -15), ByObj);
    return true;
  }
  else if (GetOCF(ByObj) & OCF_HitSpeed2)
  {
    ProtectedCall(ByObj, "Hit");
    return true;
  }
  return true;
}

//f�r Warnsounds und Grafik zust�ndig
protected func TimerCall()
{
  //Zerst�rt?
  if(IsDestroyed())
    return;	

  //Richtung ermitteln
  var iDir = GetDir() * 2 - 1;

  //Rotorgeschwindigkeit aktualisieren
  iRotorSpeed = throttle;
  if(EngineRunning())
  {
    if(GetAction() == "Fly" || GetAction() == "Turn") iRotorSpeed += 95;
  }
  else
    if(GetAction() != "Fly" && GetAction() != "Turn")
    {
      if(GetAction() == "EngineStartUp") iRotorSpeed += GetActTime();
      else if(GetAction() == "EngineStartUp2") iRotorSpeed += 95/(45+24)*(45+GetActTime());
      else if(GetAction() == "EngineStartUp3") iRotorSpeed += 95/(45+24+16)*(45+24+GetActTime());
      else if(GetAction() == "EngineShutDown") iRotorSpeed += 95-GetActTime();
      else if(GetAction() == "EngineShutDown2") iRotorSpeed += 95-60/(30+30)*(30+GetActTime());
      else if(GetAction() == "EngineShutDown3") iRotorSpeed += 95-90/(30+30+30)*(30+30+GetActTime());
    }
	
  //Absinken, wenn kein Pilot
  if(((!GetPilot() && !GetAutopilot() || GetY() < 100) && throttle) || (GetAction() == "Fly" && throttle && !GetPilot() && !GetAutopilot()))
    if(!Random(3))
    {
      if(throttle > 100)
        throttle -= 3;
      else if(throttle > 75)
        throttle--;
      else if(throttle > 50)
        throttle -= 5;
      else if(throttle > 0)
        throttle -= 10;
      if(throttle <= 0 && !GetPilot() && !GetAutopilot())
      {
        throttle = 0;
        SetAction("EngineShutDown");
      }
    }

  //Namensanzeige f�r Verb�ndete
  for(var first = true, iFlags, i = 0; i < GetPlayerCount(); i++)
  {
    var iPlr = GetPlayerByIndex(i);
    if(GetPilot() && !Hostile(GetOwner(GetPilot()), iPlr) && (!GetCursor(iPlr) || (GetCursor(iPlr) != GetPilot()) && GetCursor(iPlr)->~GetRealCursor() != GetPilot()))
    {
      if(first)
        first = false;
      else
        iFlags = MSG_Multiple;
      var szStr = Format("@%s (%s)", GetName(GetPilot()), GetPlayerName(GetOwner(GetPilot())));
      CustomMessage(szStr, this, iPlr, 0, 15, SetRGBaValue(GetPlrColorDw(GetOwner(GetPilot())), 128), 0, 0, iFlags);
    }
    if(!GetPilot())
      CustomMessage("@", this, iPlr);
  }

  //Scheinwerfer aktualisieren
  if(fShowSpotlight)
    if(pSpotlight[0])
      UpdateSpotlights();
    else
      CreateSpotlights();
  else if(pSpotlight[0])
    RemoveSpotlights();

  /*Nach gestorbenen Clonks suchen
  if(Contents())
  {
    var fUpdate = false;
    for(var pDead in FindObjects(Find_Container(this), Find_Or(Find_And(Find_Not(Find_OCF(OCF_Alive)), Find_Func("IsClonk")), Find_Func("IsFakeDeath"))))
    {
      pDead->Exit();
    }
    //if(fUpdate) UpdateSeats();
  }*/

  if(!(GetActTime() % 50))
  {
    if(!GetAlive(GetPilot()))
      DeleteActualSeatPassenger(GetPilot());
    if(!GetAlive(GetGunner()))
      DeleteActualSeatPassenger(GetGunner());
    if(!GetAlive(GetCoordinator()))
      DeleteActualSeatPassenger(GetCoordinator());
    if(!GetAlive(GetPassenger1()))
      DeleteActualSeatPassenger(GetPassenger1());
    if(!GetAlive(GetPassenger2()))
      DeleteActualSeatPassenger(GetPassenger2());
  }

  //Bodenpartikel zeichnen
  if(GetEffectData(EFSM_Fog) > 0)
  {
    if(GetRotorSpeed() != 0 && !GBackSemiSolid())
    {
      var iX = GetX();
      var iY = GetY();
      var iXDir = 0;
      var iYDir = 0;
      if(SimFlight(iX, iY, iXDir, iYDir, 1, 100, -1))
      {
        iY = iY-GetY();
        if(iY < 150 && GBackSemiSolid(0, iY))
        {
          var iMaterial = GetMaterial(0, iY);
          if(iMaterial == Material("Vehicle")) iMaterial = Material("Wall");
          var iLightness = 0;
          if(GBackLiquid(0, iY)) iLightness += 175;
          var iClrOffset = Random(3)*3;
          var iColor = RGB(Min(GetMaterialVal("Color", "Material", iMaterial, 0 + iClrOffset) + iLightness, 255), Min(GetMaterialVal("Color", "Material", iMaterial, 1 + iClrOffset) + iLightness, 255), Min(GetMaterialVal("Color", "Material", iMaterial, 2 + iClrOffset) + iLightness, 255));
          var iPower = Min(GetRotorSpeed(), 130);
          CreateParticle("GroundSmoke", iDir*21-3, iY, -(70 - iY / 3), RandomX(-5, 5),  RandomX(30, 15 + (14- iY / 10) * iPower / 5), iColor);
          CreateParticle("GroundSmoke", iDir*21+3, iY, (70 - iY / 3), RandomX(-5, 5), RandomX(30, 15 + (14 - iY / 10) * iPower / 5), iColor);
          CreateParticle("GroundSmoke", iDir*21-3, iY - 3, RandomX(-30, -(70 - iY)), -2, RandomX(30, 15 + (14 - iY / 10) * iPower / 5), iColor);
          CreateParticle("GroundSmoke", iDir*21+3, iY - 3, RandomX(30, (70 - iY)), -2, RandomX(30, 15 + (14 - iY / 10) * iPower / 5), iColor);
        }
      }
    }
  }

  //Lebewesen schrappneln
  if(GetRotorSpeed() != 0)
  {
    for(var pClonk in FindObjects(Find_OnLine(GetVertex(7), GetVertex(7, true), GetVertex(11), GetVertex(11, true)) , Find_NoContainer(), Find_OCF(OCF_Alive), Find_Not(Find_ID(FKDT))))
    {
      if(GetOwner(pClonk) != NO_OWNER && GetOwner() != NO_OWNER && !Hostile(GetOwner(), GetOwner(pClonk)))
        continue;
      if(GetEffect("NoRotorHit", pClonk))
        continue;
      Fling(pClonk, GetXDir(pClonk, 1) * 3 / 2 + RandomX(-1, 1), RandomX(-3, -2) - Pow(GetRotorSpeed(), 2) / 15000);
      DoDmg(GetRotorSpeed() / 4, DMG_Projectile, pClonk, 0, GetOwner() + 1);
      Sound("BKHK_RotorHit*.ogg", false, pClonk);
      AddEffect("NoRotorHit", pClonk, 1, 20, pClonk);
      //Achievement-Fortschritt (Meat Grinder)
      if(GetPilot() && (!GetAlive(pClonk) || IsFakeDeath(pClonk)))
        DoAchievementProgress(1, AC29, GetController(GetPilot()));
    }

    //Festes Material im Rotor tut weh, wird von Contact-Calls bei Stillstand nicht erfasst
    if(!PathFree(GetX() + GetVertex(7), GetY() + GetVertex(7, true), GetX() + GetVertex(11), GetY() + GetVertex(11, true)))
      DoDamage(1, this, FX_Call_DmgScript, GetController(GetPilot()) + 1);
  }
  
  //Nachladezeit reduzieren
  if(smokereload) 
    smokereload--;
  if(flarereload)
    flarereload--;

  //Bei Wasserkontakt Schaden nehmen
  if(GBackLiquid(0, 10))
    DoDamage(5);

  //Schadensverhalten
  if(GetDamage() >= MaxDamage() / 2)
  {
    for (var a = 0; a < 3; a++)
      if (!GBackLiquid(-Sin(GetR() + iDir * 80, 25), +Cos(GetR() + iDir * 80, 25)))
        Smoke(-Sin(GetR() + iDir * 80, 25), +Cos(GetR() + iDir * 80, 25), Random(10));
  }
  if(GetDamage() >= MaxDamage() * 3 / 4)
  {
    if (!GBackLiquid(-Sin(GetR() + iDir * 80, 25), +Cos(GetR() + iDir * 80, 25)))
      CreateParticle("Blast", -Sin(GetR() + iDir * 80, 25) + RandomX(-10, 10), +Cos(GetR() + iDir * 80, 25) + RandomX(-10, 10),0, -10, 100 + Random(30), RGB(255, 255, 255), this);
    if (!GBackLiquid(-Sin(GetR() + iDir * 80, 25), +Cos(GetR() + iDir * 80, 25)))
      CreateParticle("Blast", -Sin(GetR() - iDir * 60, 25) + RandomX(-10, 10), +Cos(GetR() + iDir * 100, 25) + RandomX(-10, 10),0, -10, 100 + Random(30), RGB(255, 255, 255), this);

    if(EngineRunning())
    {
      if(GetDamage() < MaxDamage() * 3 / 4) 
      {
        if (!(iWarningSound % 36))
        for (var obj in FindObjects(Find_OCF(OCF_CrewMember), Find_Container(this)))
          Sound("WarningDamage.ogg", false, pMGStation, 100, GetOwner(obj) + 1);
        iWarningSound++;
        if(iWarningSound >= 100)
          iWarningSound = 0;
      }
      else
      {
        if (!(iWarningSound % 20))
        {
          Local(2) = 0;
          for (var obj in FindObjects(Find_OCF(OCF_CrewMember), Find_Container(this)))
            if(obj != GetPilot())
              Sound("WarningDamageCritical.ogg", false, pMGStation, 100, GetOwner(obj) + 1);
        }
        iWarningSound++;
        if (iWarningSound >= 100)
          iWarningSound = 0;
      }
    }
  }
}

/* Physik */

protected func StartEngine()
{
  Sound("BKHK_StartSystem.ogg", false, this);
  Sound("BKHK_RotorSpin*.ogg", false, 0, 0, 0, 1);
}

protected func EngineStarted()
{
  Sound("BKHK_RotorSpin*.ogg", false, 0, 0, 0, -1);
  if(!EngineRunning())
  {  
    AddEffect("Engine", this, 300, 1, this);
    throttle = 0;
    rotation = 0;
  }
}

protected func StopEngine()
{
  Sound("BKHK_StopSystem.ogg", false, this); 
  Sound("BKHK_RotorSpin*.ogg", false, 0, 0, 0, 1);
  RemoveEffect("Engine", this);
}

protected func EngineStopped()
{
  throttle = 0;
  rotation = 0;
  Sound("BKHK_RotorSpin*.ogg", false, 0, 0, 0, -1);
}

/* Effekt: Engine */

//hier wird das Flugverhalten gesteuert
protected func FxEngineTimer(object Target, int EffectNumber, int EffectTime)
{
  //Variablen abspeichern
  var rot = LocalN("rotation", Target);
  var thr = LocalN("throttle", Target);
  var Fg, Fv, Fh, Fw, Fs, dFv, dFh, m, mh, g, av, ah;  //physikalische Kr�fte
  
  //�berpr�fung, ob �berhaupt noch ein Pilot vorhanden ist...
  if (Target->GetPilot() || Target->GetAutopilot())
  {
    //Rotation anpassen
    var speed;
    speed = BoundBy(rot-GetR(Target), -BKHK_RotationSpeed, BKHK_RotationSpeed);
    SetRDir(speed, Target);
  }
  else
  {
    LocalN("rotation", Target) = GetR(Target);
  }

  //Gewichtskraft berechnen
  m = GetMass(Target);
  g = GetGravity();
  Fg = (m + mh) * g;
  Fw = GetWind(0, -20, false) * 200;
  if(GetContact(this, -1, CNAT_Bottom)) Fw = 0;
 
  //Hubkraft und vertikale Beschleunigung berechnen
  Fv  = - Cos(GetR(Target), 1500*thr);
  dFv = Fg + Fv;
  av  = dFv / m; //BoundBy(dFh / m, -80, 80);

  //Kraft nach links oder rechts sowie deren Beschleunigung berechnen
  Fh  = - Sin(-GetR(Target), 1500*thr);
  dFh = Fh + Fw + Fs;
  ah  = dFh / m; //BoundBy(dFf / m, -80, 80);

  //Geschwindigkeit anpassen
  SetYDir(GetYDir(Target, 95) + av, Target, 100);
  SetXDir(GetXDir(Target, 95) + ah, Target, 100);

  return true;
}

//eine neue Funktion: Tangens = Sinus / Kosins
global func Tan(int angle, int radius)
{
  return Sin(angle, radius, 10) / Cos(angle, radius, 10);
}
