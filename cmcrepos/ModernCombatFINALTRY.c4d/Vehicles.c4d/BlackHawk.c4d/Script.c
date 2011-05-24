/*-- Blackhawk --*/

#strict 2
#include CVHC

/*----- Variablen -----*/

local throttle,				//int    - Schub
      rotation,				//int    - erwartete Drehung
      idle,				//int    - Motor-Start/Stop Sequenz
      view_mode;			//bool   - für den Piloten

local hud;				//object - Anzeige für Pilot

local aSeats;

local MGStation,			//object - Das MG-Objekt
      RocketStation;			//object - Das Raketenwerfer-Objekt

local s_counter,			//int    - eine kleine Counter-Variable für Warnsounds
      d_counter;			//int    - eine kleine Counter-Variable für die Zerstörung
 
local smokereload;			//int    - Nachladezeit für Rauchwand
local flarereload;			//int    - Nachladezeit für Flareabwurf

static const throttle_speed = 5;	//int    - "Feinfühligkeit"
static const rot_speed = 1;		//int    - Drehgeschwindigkeit / frame
static const control_speed = 3;		//int    - "Feinfühligkeit"
static const max_throttle = 200;	//int    - höchste Schubeinstellung
static const max_rotation = 30;		//int    - stärkste erlaubte Neigung
static const BKHK_AutoThrottleSpeed = 1;
static const BKHK_AutoMaxThrottle = 150;
static const BKHK_AutoMaxRotation = 10;

static const BKHK_PilotLayer = 2;
static const BKHK_PassengerLayer = 3;

static const BKHK_Seat_Pilot = 1;
static const BKHK_Seat_Gunner = 2;
static const BKHK_Seat_Coordinator = 3;
static const BKHK_Seat_Passenger1 = 4;
static const BKHK_Seat_Passenger2 = 5;

/*----- Callbacks -----*/

public func IsMachine()			{return true;}
public func MaxDamage()			{return 200;}
public func IsThreat()			{return true;}
public func IsHelicopter()		{return true;}

public func IsBulletTarget(id idBullet, object pBullet)
{
  //Nicht treffbar wenn neutral, außer bei FF
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

/*----- Initialisierung -----*/

protected func Initialize()
{
  if (NoBlackHawks())
    return RemoveObject();

  //Steuerung initialisieren
  throttle = 0;
  rotation = 0; 
  SetAction("Stand");

  //Pilot
  view_mode = true;
  aSeats = [];

  //Geschütze aufstellen
  MGStation = CreateObject(WNRK,0,0,GetOwner());
  MGStation -> Set(this,10,90,90,270);
  MGStation -> Arm(ACCN);
  RocketStation = CreateObject(WNRK,0,0,GetOwner());
  RocketStation -> Set(this,40,10,210,270);
  RocketStation -> Arm(RLSA);

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

/*----- Erfassung -----*/

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

public func GetRotorSpeed()
{
  var iSpeed = throttle;
  if(GetAction() == "EngineStartUp") iSpeed = GetActTime();
  if(GetAction() == "EngineStartUp2") iSpeed = 95/(45+24)*(45+GetActTime());
  if(GetAction() == "EngineStartUp3") iSpeed = 95/(45+24+16)*(45+24+GetActTime());
  if(GetAction() == "EngineShutDown") iSpeed = 95-GetActTime();
  if(GetAction() == "EngineShutDown2") iSpeed = 95-60/(30+30)*(30+GetActTime());
  if(GetAction() == "EngineShutDown3") iSpeed = 95-90/(30+30+30)*(30+30+GetActTime());
  if(GetAction() == "Fly" || GetAction() == "Turn")
  {
    iSpeed += 95;
  }
  return iSpeed;
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

/*----- Autopilot -----*/

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
      if(GetAction() == "Fly" || GetAction() == "Turn") {
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
	    if(GetAction() == "Fly" || GetAction() == "Turn") {
      	throttle = BoundBy(throttle + BKHK_AutoThrottleSpeed, 0, BKHK_AutoMaxThrottle);
      }
      else if(throttle == 0 && GetAction() == "Stand") {
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
    rotation = BoundBy(rotation - control_speed, -BKHK_AutoMaxRotation, BKHK_AutoMaxRotation);
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
    rotation = BoundBy(rotation + control_speed, -BKHK_AutoMaxRotation, BKHK_AutoMaxRotation);
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
      rotation = BoundBy(rotation + control_speed, 0, BKHK_AutoMaxRotation);
    }
    else if(GetXDir(pTarget) > 1)
    {
      rotation = BoundBy(rotation - control_speed, -BKHK_AutoMaxRotation, 0);
    }
    else
    {
      if(GetXDir(pTarget) < 0)
      {
        rotation = BoundBy(rotation - control_speed, 0, BKHK_AutoMaxRotation);
      }
      else if(GetXDir(pTarget) > 0)
      {
        rotation = BoundBy(rotation + control_speed, -BKHK_AutoMaxRotation, 0);
      }
    }
  }
  return FX_OK;
}

/*----- Eingangssteuerung -----*/

protected func Ejection(object ByObj)
{
  if(!ByObj)
    return;
  if(!(GetOCF(ByObj) & OCF_CrewMember))
    return;
  
  //Erst mal löschen
  DeleteActualSeatPassenger(ByObj);
    
  //Soundschleife übergeben
  Sound("CockpitRadio.ogg", true, 0, 100, GetOwner(ByObj)+1, -1);

  //Nicht bei Schaden
  if(GetDamage() >= MaxDamage()) return;
  
  if(!PathFree(GetX(),GetY(),GetX(),GetY()+100))
    return; //Trozdem Sitz freimachen

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
      //Soundschleife übergeben
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

      //Kein Platz mehr. :/
      return SetCommand(pObj, "Exit");
    }
    else
      return SetCommand(pObj, "Exit");
  }
}
                
/*----- Steuerung -----*/

protected func FxBlackhawkChangeThrottleStart(object pTarget, int iNumber, iTemp, int iChange)
{
  EffectVar(0, pTarget, iNumber) = iChange;
  EffectCall(pTarget, iNumber, "Timer");
}

protected func FxBlackhawkChangeThrottleTimer(object pTarget, int iNumber, int iTime)
{
	var old = throttle;
  throttle = BoundBy(throttle + EffectVar(0, pTarget, iNumber), 0, max_throttle);
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
        AddEffect("BlackhawkChangeThrottle", this, 50, 3, this, GetID(), throttle_speed);
      else
        throttle = BoundBy(throttle + throttle_speed, 0, max_throttle);
  }

  //Schütze
  if (ByObj == GetGunner())
    MGStation->~ControlUp(ByObj);
  //Koordinator
  if (ByObj == GetCoordinator())
    RocketStation->~ControlUp(ByObj);

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
        AddEffect("BlackhawkChangeThrottle", this, 50, 3, this, GetID(), -throttle_speed);
      else
      	throttle = BoundBy(throttle - throttle_speed, 0, max_throttle);
  }

  //Schütze
  if(ByObj == GetGunner())
    MGStation->~ControlDown(ByObj);
  //Koordinator
  if(ByObj == GetCoordinator())
    RocketStation->~ControlDown(ByObj);

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
      throttle = BoundBy(throttle + throttle_speed * 2, 0, max_throttle);
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
      throttle = BoundBy(throttle - throttle_speed*2, 0, 170);
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
        rotation = -max_rotation;
      else
        rotation = BoundBy(rotation - control_speed, -max_rotation, max_rotation);
  }

  //Schütze
  if(ByObj == GetGunner())
    MGStation->~ControlLeft(ByObj);
  //Koordinator
  if(ByObj == GetCoordinator())
    RocketStation->~ControlLeft(ByObj);

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
          rotation = max_rotation;
        else
          rotation = BoundBy(rotation + control_speed, -max_rotation, max_rotation);
  }

  //Schütze
  if(ByObj == GetGunner())
    MGStation->~ControlRight(ByObj);
  //Koordinator
  if(ByObj == GetCoordinator())
    RocketStation->~ControlRight(ByObj);

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
    MGStation->~ControlLeftDouble(ByObj);
  //Koordinator
  if(ByObj == GetCoordinator())
    RocketStation->~ControlLeftDouble(ByObj);

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

  //Schütze
  if(ByObj == GetGunner())
    MGStation->~ControlRightDouble(ByObj);
  //Koordinator
  if(ByObj == GetCoordinator())
    RocketStation->~ControlRightDouble(ByObj);

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

    return Sound("BKHK_Switch.ogg", false, this, 100, GetOwner(ByObj) + 1);
  }

  //Schütze: Feuer eröffnen/einstellen
  if(ByObj == GetGunner())
    if(!GetPlrCoreJumpAndRunControl(GetController(ByObj)))
      MGStation->~ControlThrow(ByObj);
  //Koordinator
  if(ByObj == GetCoordinator())
    RocketStation->~ControlThrow(ByObj);

  return true;
}

protected func ContainedDig(object pBy)
{
  if (pBy == GetCoordinator())
    RocketStation->~ControlDig(pBy);
}

public func ContainedUpdate(object ByObj, int comdir, bool dig, bool throw)
{
  if(ByObj == GetGunner())
    if(throw)
      return MGStation->ControlThrow(ByObj);
    else
      return MGStation->StopAutoFire();
}

//eine Funktion, welche einfach nur die Richtung eines Objektes ändert
protected func ChangeDir()
{
  for (var i = 0; i < GetDefCoreVal("Vertices", "DefCore", GetID()); i++)
    SetVertex(i, 0, (GetDir() * 2 - 1) * GetDefCoreVal("VertexX", "DefCore", GetID(), i), this, 2);
  SetDir(!GetDir());
  return true;
}

/*----- Pilotenfähigkeiten -----*/

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

/*----- Sitzsteuerung -----*/

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

    //Koordinator
    if(GetCoordinator())
      AddMenuItem("<c 777777>$Coordinator$</c>", "SeatOccupied", GetID(), ByObj, 0, ByObj, "$SeatOccupied$");
    else
      AddMenuItem("<c ffffff>$Coordinator$</c>", Format("EnterSeat(%d, Object(%d))", BKHK_Seat_Coordinator, ObjectNumber(ByObj)), GetID(), ByObj, 0, ByObj, "$CoordinatorDesc$");

    //Passagier 1
    if(GetPassenger1())
      AddMenuItem("<c 777777>$Passenger$</c>", "SeatOccupied", GetID(), ByObj, 0, ByObj, "$SeatOccupied$");
    else
      AddMenuItem("<c ffffff>$Passenger$</c>", Format("EnterSeat(%d, Object(%d))", BKHK_Seat_Passenger1, ObjectNumber(ByObj)), GetID(), ByObj, 0, ByObj, "$PassengerDesc$");

    //Passagier 2
    if(GetPassenger2())
      AddMenuItem("<c 777777>$Passenger$</c>", "SeatOccupied", GetID(), ByObj, 0, ByObj, "$SeatOccupied$");
    else
      AddMenuItem("<c ffffff>$Passenger$</c>", Format("EnterSeat(%d, Object(%d))", BKHK_Seat_Passenger2, ObjectNumber(ByObj)), GetID(), ByObj, 0, ByObj, "$PassengerDesc$");
    
    //Ausstieg per Seil
    AddMenuItem("<c ffff33>$ExitRope$</c>", "ExitClonkByRope", CK5P, ByObj, 0, ByObj, "$ExitRopeDesc$");

  return 1;
}

private func SeatOccupied(a, object ByObj)
{
  //Hinweis
  PlayerMessage(GetOwner(ByObj), "$SeatOccupied$", ByObj);

  //Sound
  Sound("BKHK_SwitchFail.ogg", 0, 0, 0, GetOwner(ByObj) + 1);

  //Menü wiedereröffnen
  ContainedDigDouble(ByObj);

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
    if(MGStation) MGStation->SetGunner(0);
  }
  if(GetCoordinator() == Obj)
  {
    GetCoordinator() = 0;
    if(RocketStation) RocketStation->SetGunner(0);
  }
  if(GetPassenger1() == Obj)
    GetPassenger1() = 0;

  if(GetPassenger2() == Obj)
    GetPassenger2() = 0;

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
    hud = CreateObject(BHUD, 0, 0, GetOwner(pObj));
    hud->~SetHelicopter(this);
    return true;
  }

  //Schütze
  if (iSeat == BKHK_Seat_Gunner)
  {
    SetGraphics("Passenger", this, GetID(), BKHK_PassengerLayer, GFXOV_MODE_ExtraGraphics, 0, GFX_BLIT_Custom, this);
    GetGunner() = pObj;
    MGStation->~SetGunner(GetGunner());
    return true;
  }

  //Koordinator
  if (iSeat == BKHK_Seat_Coordinator)
  {
    SetGraphics("Passenger", this, GetID(), BKHK_PassengerLayer, GFXOV_MODE_ExtraGraphics, 0, GFX_BLIT_Custom, this);
    GetCoordinator() = pObj;
    RocketStation->~SetGunner(GetCoordinator());
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

/*----- Ausstieg per Seil -----*/

public func GetRopeAttach()
{
  return MGStation;
}

private func ExitClonk(a, ByObj)
{
  SetCommand(ByObj, "Exit");
}

private func ExitClonkByRope(a, ByObj)
{
  AddEffect("CheckGround", ByObj, 30, 3, this, GetID(), this);
  SetCommand(ByObj, "Exit");
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
  //Knapp über dem Boden, Seil zu lang, zu lang die Dauer oder falsche Aktion?
  if(!PathFree(GetX(pTarget), GetY(pTarget), GetX(pTarget), GetY(pTarget) + 30)
     || pRope->GetRopeLength() > 300
     || !WildcardMatch(GetAction(pTarget), "*Jump*"))
  {
    //Abspringen, Seil zurück zum Heli schicken.
    RemoveObject(pRope);
    SetYDir(20,pTarget);
    return -1;
  }
  else
    pRope->SetRopeLength(iTime * 4 + 10);
}

/*----- Zerstörung -----*/

protected func Destruction()
{
  if(hud)
    RemoveObject(hud);
  if(MGStation)
    RemoveObject(MGStation);
  if(RocketStation)
    RemoveObject(RocketStation);
  return true;
}

/*----- Schaden -----*/

public func OnDmg(int iDmg, int iType)
{
  if(iType == DMG_Energy)	return -20;	//Energie
  if(iType == DMG_Bio)		return 100;	//Säure und biologische Schadstoffe
  if(iType == DMG_Melee)	return 80;	//Nahkampf
  if(iType == DMG_Fire)		return 50;	//Feuer
  if(iType == DMG_Explosion)	return -70;	//Explosionen
  if(iType == DMG_Projectile)	return 80;	//Projektile

  return 50;
}

public func OnDamage()
{
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

  if(Hostile(GetLastAttacker(),GetOwner(this)))
  {
    //Achievement-Fortschritt (Junkdealer)
    DoAchievementProgress(1,AC23,GetLastAttacker());
  }

  //Explosion
  FakeExplode(70, GetLastAttacker() + 1);
  FakeExplode(50, GetLastAttacker() + 1);
  RemoveObject();
  Sound("BigExplosion.ogg", false, this);
  Sound("StructuralDamage*.ogg", false, this);

  //Wrack erstellen
  var obj;
  obj = CreateObject(BHWK, 0, 20, -1);
  Incinerate(obj);
  SetDir(GetDir(), obj);
  SetR(GetR(), obj);
  SetXDir(GetXDir(), obj);
  SetYDir(GetYDir(), obj);
  SetRDir(GetRDir(), obj);
  return;
}

/*----- Kollisionsverhalten -----*/

protected func ContactTop()
{
  if (GetCon() != 100) return;
  DoDmg(10, 0, this, 1,  GetOwner() + 1);
  for (var i; i < GetVertexNum(); i++)
    if(GetContact(0, i))
      CreateParticle("Blast", GetVertex(i), GetVertex(i, true), 0, 0, RandomX(50, 100), RGB(255, 255, 255));

  //Sound
  Sound("HeavyHit*.ogg", false, MGStation);
  SetYDir(Min(GetYDir(), -40) / -2);
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
    Sound("HeavyHit*.ogg", false, MGStation);
    SetYDir(GetYDir() * -2 / 3);
  }
  if(GetContact(0, -1, CNAT_Left | CNAT_Right) && throttle)
    SetYDir(Max(GetYDir(), 20) * -2 / 3);
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
    Sound("HeavyHit*.ogg", false, MGStation);
  }
  //Abprallen
  SetXDir(Max(GetXDir(),40) / -2, this);
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
    Sound("HeavyHit*.ogg", false, MGStation);
  }
  SetXDir(Max(GetXDir(), 40) / -2,this);
}

//Objekt, die in den Rotor geraten, verursachen Schaden
protected func RejectCollect(id ID, object ByObj)
{
  //man soll schon Objekte im Heli ablegen können
  if (Contained(ByObj))
    return;
  //von außen heißt es: Schaden!!!
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
                           
//für Warnsounds und Grafik zuständig
protected func TimerCall()
{
  //Zerstört?
  if(IsDestroyed())
    return;	

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

  //Namensanzeige für Verbündete
  for(var first = true, iFlags, i = 0; i < GetPlayerCount(); i++)
  {
    var iPlr = GetPlayerByIndex(i);
    if(GetPilot() && !Hostile(GetOwner(GetPilot()), iPlr) && (!GetCursor(iPlr) || (Contained(GetCursor(iPlr)) != this) && Contained(GetCursor(iPlr)->~GetRealCursor()) != this))
    {
      if (first)
        first = false;
      else
        iFlags = MSG_Multiple;
      var szStr = Format("@%s (%s)", GetName(GetPilot()), GetPlayerName(GetOwner(GetPilot())));
      CustomMessage(szStr, this, iPlr, 0, 15, SetRGBaValue(GetPlrColorDw(GetOwner(GetPilot())), 128), 0, 0, iFlags);
    }
    else
      CustomMessage("@", this, iPlr);
  }
  
  //Alle 50 Frames nach Leichen im Heli suchen und löschen.
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

  //Bei Wasser abschalten
  Water();

  //Piloten anpassen
  DrawPilot();

  //Bodenpartikel zeichnen
  DrawGroundParticles();

  //Lebewesen schrappneln
  if(GetRotorSpeed() > 0)
  {
    for(var pClonk in FindObjects(Find_OnLine(GetVertex(7), GetVertex(7, true), GetVertex(11), GetVertex(11, true)) , Find_NoContainer(), Find_OCF(OCF_Alive), Find_Not(Find_ID(FKDT))))
    {
      if(GetOwner(pClonk) != NO_OWNER && GetOwner() != NO_OWNER && !Hostile(GetOwner(), GetOwner(pClonk)))
        continue;
      if(GetEffect("NoRotorHit", pClonk))
        continue;
      Fling(pClonk, GetXDir(pClonk, 1) * 3 / 2 + RandomX(-1, 1), RandomX(-3, -2) - GetRotorSpeed() / 100);
      DoDmg(GetRotorSpeed() / 4, DMG_Projectile, pClonk, 0, GetOwner() + 1);
      Sound("BKHK_RotorHit*.ogg", false, pClonk);
      AddEffect("NoRotorHit", pClonk, 1, 20, pClonk);
    }

    //Festes Material im Rotor tut weh, wird von Contact-Calls bei Stillstand nicht erfasst
    if (!PathFree(GetX() + GetVertex(7), GetY() + GetVertex(7, true), GetX() + GetVertex(11), GetY() + GetVertex(11, true)))
      DoDamage(1, this, FX_Call_DmgScript, GetController(GetPilot()) + 1);
  }
  
  //Nachladezeit reduzieren
  if(smokereload) 
    smokereload--;
  if(flarereload)
    flarereload--;

  //Schadensverhalten
  //bis 50% nichts
  if (GetDamage() < MaxDamage() / 2)
    return;

  //ab 50% rauchen
  Smoking();

  if (GetDamage() < MaxDamage() * 3 / 4)
    return;

  //ab 75% Feuer
  DrawFire();

  if (!GetEffect("Engine", this))
    return;
  
  //Warnsound
  WarningSound();
}

private func DrawPilot()
{
  if (view_mode && !GetPilot())
    view_mode = false;
  else if (!view_mode && GetPilot())
    view_mode = true;
}

private func WarningSound()
{
  if(GetDamage() < MaxDamage() * 3 / 4) 
  {
    if (!(s_counter % 36))
	  for (var obj in FindObjects(Find_OCF(OCF_CrewMember), Find_Container(this)))
        Sound("WarningDamage.ogg", false, MGStation, 100, GetOwner(obj) + 1);
    s_counter++;
    if (s_counter >= 100)
      s_counter = 0;

    return;
  }
  else
  {
    if (!(s_counter % 20))
    {
      Local(2) = 0;
      for (var obj in FindObjects(Find_OCF(OCF_CrewMember), Find_Container(this)))
        if(obj != GetPilot())
          Sound("WarningDamageCritical.ogg", false, MGStation, 100, GetOwner(obj) + 1);
    }
    s_counter++;
    if (s_counter >= 100)
      s_counter = 0;

    return;
  }
}

private func DrawGroundParticles()
{
  if(!GetRotorSpeed())
    return;
  if(GBackLiquid() || GBackSolid())
    return;
  for (var i; i < 30; i++)
  {
    //Omg, wie wärs mit GetMaterialVal...?
    if (GetMaterial(0, i * 5) == Material("Earth"))
      return CreateDust(i * 5, GetRotorSpeed(), RGB(150, 86, 22));
    if ((Material("Wall") != -1 && GetMaterial(0, i * 5) == Material("Wall")) || GetMaterial(0, i * 5) == Material("Vehicle"))                    
      return CreateDust(i * 5, GetRotorSpeed(), RGB(150, 150, 150));
    if (GetMaterial(0, i * 5) == Material("Snow"))                    
      return CreateDust(i * 5, GetRotorSpeed(), RGB(255, 255, 255));
    if (GetMaterial(0, i *5 ) == Material("Sand"))                    
      return CreateDust(i * 5, GetRotorSpeed(), RGB(247, 236, 157));
    if (GetMaterial(0, i * 5) == Material("Ashes"))                    
      return CreateDust(i * 5, GetRotorSpeed(), RGB(64, 55, 36));
    if (GBackSolid(0, i * 5))
      return true;

    if (GetMaterial(0, i * 5) == Material("Water"))
      return CreateDust(i * 5, GetRotorSpeed(), RGB(176, 194, 208));
  }
}

private func CreateDust(int Y, int Power, int Color)
{
  Power = Min(Power, 130);
  CreateParticle("GroundSmoke", -3, Y, -(70 - Y / 3), RandomX(-5, 5),
                 RandomX(30, 15 + (14- Y / 10) * Power / 5), Color);		//nach links
  CreateParticle("GroundSmoke", +3, Y, (70 - Y / 3), RandomX(-5, 5),
                 RandomX(30, 15 + (14 - Y / 10) * Power / 5), Color);		//nach rechts
  CreateParticle("GroundSmoke", -3, Y - 3, RandomX(-30, -(70 - Y)), -2,
                 RandomX(30, 15 + (14 - Y / 10) * Power / 5), Color);		//nach links oben
  CreateParticle("GroundSmoke", +3, Y - 3, RandomX(30, (70 - Y)), -2,
                 RandomX(30, 15 + (14 - Y / 10) * Power / 5), Color);		//nach rechts oben
  return true;
}

private func DrawFire()
{
  var dir = GetDir() * 2 - 1;
  if (!GBackLiquid(-Sin(GetR() + dir * 80, 25), +Cos(GetR() + dir * 80, 25)))
    CreateParticle("Blast",-Sin(GetR() + dir * 80, 25) + RandomX(-10, 10),
                           +Cos(GetR() + dir * 80, 25) + RandomX(-10, 10),
  			             0, -10, 100 + Random(30), RGB(255, 255, 255), this);

  if (!GBackLiquid(-Sin(GetR() + dir * 80, 25), +Cos(GetR() + dir * 80, 25)))
    CreateParticle("Blast",-Sin(GetR() - dir * 60, 25) + RandomX(-10, 10),
                           +Cos(GetR() + dir * 100, 25) + RandomX(-10, 10),
  			             0, -10, 100 + Random(30), RGB(255, 255, 255), this);
}

private func Water()
{
  if(GBackLiquid(0, 10))
    DoDamage(5);
}

private func Smoking()
{
  var dir = GetDir() * 2 - 1;

  for (var a = 0; a < 3; a++)
    if (!GBackLiquid(-Sin(GetR() + dir * 80, 25), +Cos(GetR() + dir * 80, 25)))
      Smoke(-Sin(GetR() + dir * 80, 25), +Cos(GetR() + dir * 80, 25), Random(10));
}

/*----- Physik -----*/

protected func StartEngine()
{
  Sound("BKHK_StartSystem.ogg", false, this);
  Sound("BKHK_RotorSpin*.ogg", false, 0, 0, 0, 1);
}

protected func EngineStarted()
{
  Sound("BKHK_RotorSpin*.ogg", false, 0, 0, 0, -1);
  if(!GetEffect("Engine", this)) {  
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

/*----- Effekt: Engine -----*/

//hier wird das Flugverhalten gesteuert
protected func FxEngineTimer(object Target, int EffectNumber, int EffectTime)
{
  //Variablen abspeichern
  var rot = LocalN("rotation", Target);
  var thr = LocalN("throttle", Target);
  var Fg, Fv, Fh, Fw, Fs, dFv, dFh, m, mh, g, av, ah;  //physikalische Kräfte
  
  //Überprüfung, ob überhaupt noch ein Pilot drin...
  if (Target->GetPilot() || Target->GetAutopilot())
  {
	  //Rotation anpassen
    var speed;
    speed = BoundBy(rot-GetR(Target), -rot_speed, rot_speed);
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
