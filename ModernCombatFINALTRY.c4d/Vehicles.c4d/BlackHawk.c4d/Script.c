/*-- Blackhawk --*/

#strict 2
#include CVHC

/* ----- Variablen ----- */

local throttle,				//int    - Schub
      rotation,				//int    - erwartete Drehung
      idle,				//int    - Motor-Start/Stop Sequenz
      view_mode;			//bool   - für den Piloten

local hud;				//object - Anzeige für Pilot

local Pilot,				//object - Pilot
      Gunner,				//object - Schütze
      Coordinator,			//object - Koordinator
      Passenger1, Passenger2;		//object - 2 Passagiere

local MGStation,			//object - Das MG-Objekt
      RocketStation;			//object - Das Raketenwerfer-Objekt

local s_counter,			//int    - eine kleine Counter-Variable für Warnsounds
      d_counter;			//int    - eine kleine Counter-Variable für die Zerstörung

local destroyed;			//bool   - ob der Heli schon zerstört ist

static const throttle_speed = 5;	//int    - "Feinfühligkeit"
static const rot_speed = 1;		//int    - Drehgeschwindigkeit / frame
static const control_speed = 3;		//int    - "Feinfühligkeit"
static const max_throttle = 200;	//int    - höchste Schubeinstellung
static const max_rotation = 30;		//int    - stärkste erlaubte Neigung
static const auto_throttle_speed = 1;
static const auto_max_throttle = 150;
static const auto_max_rotation = 10;

/* ----- Callbacks ----- */

public func IsMachine()		{return 1;}
public func MaxDamage()		{return 200;}
public func IsBulletTarget(id idBullet, object pBullet)
{
  if(idBullet == MISS || idBullet == HMIS || idBullet == ROKT || idBullet == ESHL)
    return ObjectDistance(pBullet) < 40;
  return 1;
}

/* ----- Existenz ----- */

/* Initialisierung */

protected func Initialize()
{
  //Steuerung initialisieren
  throttle = 0;
  rotation = 0; 
  SetAction("Stand");

  //Pilot
  view_mode = true;

  //Geschütze aufstellen
  MGStation = CreateObject(H_MA,0,0,GetOwner());
  MGStation -> Set(this,10,90,90,270);
  MGStation -> Arm(ACCN);
  RocketStation = CreateObject(H_MA,0,0,GetOwner());
  RocketStation -> Set(this,40,10,210,270);
  RocketStation -> Arm(RLSA);
  
  //Vertices richtig drehen
  ScheduleCall(this,"ChangeDir",1,2);

  //Eingang
  SetEntrance(true);
  
  //Solidmask
  SetSolidMask();
  
  //fertig
  return _inherited();
}

//wird genutzt, den Landport davon abzuhalten, den zerstörten Heli zu versorgen
func IsReady()
{
  return GetDamage() < MaxDamage() && GetContact(this, -1, CNAT_Bottom) && !destroyed;
}

/* Zerstörung */

protected func Destruction()
{
  if(hud)
    RemoveObject(hud);
  if(MGStation)
    RemoveObject(MGStation,true);
  if(RocketStation)
    RemoveObject(RocketStation,true);
  return true;
}

public func GetPilot()
{return Pilot;}

public func GetThrottle()
{return throttle;}

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
  var aRockets = FindObjects(Find_ID(ROKT), Find_Distance(800, AbsX(GetX()), AbsY(GetY())), Find_Not(Find_Func("IsDamaged")));
  var fRocket = false;
  for(var pCheck in aRockets)
  {
    if(FindObject(NOFF) && !Hostile(GetOwner(pCheck), GetOwner())) continue;
    if(ObjectDistance(pCheck, this) < 300)
    {
      fRocket = true;
      break;
    }
    else
    {
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
  }
  return fRocket;
}

/* Autopilot */

public func GetAutopilot()
{return GetEffect("BlackhawkAutopilot", this);}

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
	if(GetY(pTarget) < iY-50) {
		if(GetYDir(pTarget) < 3 || GetContact(this, -1, CNAT_Bottom)) {
			//vom Gas weg
			if(GetAction()=="Fly" || GetAction()=="Turn")
			throttle = BoundBy(throttle - auto_throttle_speed, 0, auto_max_throttle);
			//Motor aus
	    if (throttle == 0 && (GetAction()=="Fly" || GetAction()=="EngineStartUp")) {
	    	SetAction("EngineShutDown");
	   		return pTarget->ResetAutopilot();
	   	}
	  }
	}
	else if(GetY(pTarget) > iY+50) {
		if(GetYDir(pTarget) > -3) {
			if (throttle == 0 && (GetAction()=="Stand" || GetAction()=="EngineShutDown"))
			SetAction("EngineStartUp");
			//beim Flug mehr Schub
			if (GetAction()=="Fly" || GetAction()=="Turn")
			throttle = BoundBy(throttle + auto_throttle_speed, 0, auto_max_throttle);
	  }
	}
	else {
		if(!(iTime % BoundBy(5-GetYDir(pTarget), 5, 0)) && GetYDir(pTarget) > 0) {
			//beim Flug mehr Schub
			if (GetAction()=="Fly" || GetAction()=="Turn")
			throttle = BoundBy(throttle + auto_throttle_speed, 0, auto_max_throttle);
		}
		else if(!(iTime % BoundBy(GetYDir(pTarget), 5, 0)) && GetYDir(pTarget) < 0) {
			//vom Gas weg
			if(GetAction()=="Fly" || GetAction()=="Turn")
			throttle = BoundBy(throttle - auto_throttle_speed, 0, auto_max_throttle);
		}
	}
	if(GetX(pTarget) > iX+50) {
		 if (GetAction()=="Fly" || GetAction()=="Turn") 
      rotation = BoundBy(rotation - control_speed, -auto_max_rotation, auto_max_rotation);
     if (rotation < 0 && GetDir() && GetAction()=="Fly")
			{
			  if (GetAction() == "Turn" || GetContact(this, -1)) return true;
			  SetAction("Turn");
			}
	}
	else if(GetX(pTarget) < iX-50){
		 if (GetAction()=="Fly" || GetAction()=="Turn") 
      rotation = BoundBy(rotation + control_speed, -auto_max_rotation, auto_max_rotation);
      if (rotation > 0 && !GetDir() && GetAction()=="Fly")
			{
			  if (GetAction() == "Turn" || GetContact(this, -1)) return true;
			  SetAction("Turn");
			}
	}
	else if(GetXDir() != 0) {
		if(GetXDir(pTarget) < -1) {
    	rotation = BoundBy(rotation + control_speed, 0, auto_max_rotation);
		}
		else if(GetXDir(pTarget) > 1) {
    	rotation = BoundBy(rotation - control_speed, -auto_max_rotation, 0);
		}
		else {
			if(GetXDir(pTarget) < 0) {
		  	rotation = BoundBy(rotation - control_speed, 0, auto_max_rotation);
			}
			else if(GetXDir(pTarget) > 0) {
		  	rotation = BoundBy(rotation + control_speed, -auto_max_rotation, 0);
			}
		}
	}
  return FX_OK;
}

/* ----- Eingangssteuerung ----- */

protected func Ejection(object ByObj)
{
  Sound("CockpitRadio.ogg", true, 0, 100, GetOwner(ByObj)+1, -1);
  DeleteActualSeatPassenger(ByObj);
  if(GetDamage() >= MaxDamage()) return;
  if(ByObj != GetPilot() && PathFree(GetX(),GetY(),GetX(),GetY()+50))
  {
    AddEffect("CheckGround",ByObj,30,3,this,GetID(),this);
  }
  return true;
}

//herausgeworfene Objekte sollen vor der Tür erscheinen
protected func ControlCommand(string Command, object Target, int TargetX, int TargetY, object target2, int Data, object ByObj)
{
  if (Command == "Exit")
  {
    var rot = GetDir()*180-90 + GetR() + GetDir()*120-60;
    Exit(ByObj, Sin(rot,25), -Cos(rot,25), GetR(), GetXDir(0,1), GetYDir(0,1), GetRDir());
    return true;
  }
  if (Command == "MoveTo")
  {
    if(ByObj == Pilot)
    SetAutopilot(Target, TargetX, TargetY);
    return true;
  }
  return;
}

protected func Collection2(object pObj)
{
  if(GetOCF(pObj) & OCF_Alive && GetOCF(pObj) & OCF_CrewMember)
  {
    if(!Hostile(GetOwner(this),GetOwner(pObj)))
    {
      //Soundschleife übergeben
      Sound("CockpitRadio.ogg", true, 0, 100, GetOwner(pObj)+1, +1);
      if(!Pilot)
        return EnterSeat1(0,pObj); //Blöde Platzhalter :/
      if(!Gunner)
        return EnterSeat2(0,pObj);
      if(!Coordinator)
        return EnterSeat3(0,pObj);        
      if(!Passenger1)
        return EnterSeat4(0,pObj);        
      if(!Passenger2)
        return EnterSeat5(0,pObj);
    }
    else //Feindliche Clonks kommen nicht rein.
      SetCommand(pObj, "Exit");
  }
}
                
/* ----- Steuerung ----- */

protected func ContainedUp(object ByObj)
{
  [$CtrlUp$]

  //Pilot
  if (ByObj == Pilot)
  {
    //Autopilot aus
    ResetAutopilot();
    //Startup-Sequence
    if (throttle == 0 && (GetAction()=="Stand" || GetAction()=="EngineShutDown"))
      SetAction("EngineStartUp");
    //beim Flug mehr Schub
    if(GetAction()=="Fly" || GetAction()=="Turn")
      throttle = BoundBy(throttle + throttle_speed, 0, max_throttle);
  }
  
  //Schütze
  if (ByObj == Gunner)
    MGStation->~ControlUp(ByObj);
  //Coordinator
  if (ByObj == Coordinator)
    RocketStation->~ControlUp(ByObj);
    
  return true;
}

protected func ContainedDown(object ByObj)
{
  [$CtrlDown$]

  //Pilot
  if (ByObj == Pilot)
  {
    //Autopilot aus
    ResetAutopilot();
    //Motor aus
    if(throttle == 0 && (GetAction()=="Fly" || GetAction()=="EngineStartUp"))
      SetAction("EngineShutDown");
    //vom Gas weg
    if(GetAction()=="Fly" || GetAction()=="Turn")
      throttle = BoundBy(throttle - throttle_speed, 0, max_throttle);
  }
  
  //Schütze
  if(ByObj == Gunner)
    MGStation->~ControlDown(ByObj);
  //Coordinator
  if(ByObj == Coordinator)
    RocketStation->~ControlDown(ByObj);

  return true;
}

protected func ContainedUpDouble(object ByObj)
{
  [$CtrlUpD$]
  
  //Pilot
  if (ByObj == Pilot)
  {
    //Autopilot aus
    ResetAutopilot();
    if(throttle == 0 && (GetAction()=="Stand" || GetAction()=="EngineShutDown"))
      SetAction("EngineStartUp");  
    if(GetAction()=="Fly")
      throttle = BoundBy(throttle + throttle_speed*2, 0, max_throttle);
    return true;
  }
}

protected func ContainedDownDouble(object ByObj)
{
  [$CtrlDownD$]

  //Pilot
  if (ByObj == Pilot)
  {
    //Autopilot aus
    ResetAutopilot();
    //Motor aus
    if (throttle == 0 && (GetAction()=="Fly" || GetAction()=="EngineStartUp")) SetAction("EngineShutDown");
    //vom Gas weg
    if (GetAction()=="Fly") throttle = BoundBy(throttle - throttle_speed*2, 0, 170);
  }
  if(ByObj == Passenger1 || ByObj == Passenger2)
  {
    SetCommand(ByObj,"Exit");
  }
  return true;
}

protected func ContainedLeft(object ByObj)
{
  [$CtrlLeft$]

  //Pilot
  if (ByObj == Pilot)
  {
    //Autopilot aus
    ResetAutopilot();
    if (GetAction()=="Fly" || GetAction()=="Turn")
    {
      /*if(GetPlrCoreJumpAndRunControl(GetController(ByObj)))
      {
        rotation = -max_rotation;
      }
      else
      {*/
        rotation = BoundBy(rotation - control_speed, -max_rotation, max_rotation);
      //}
    }
  }

  //Schütze
  if(ByObj == Gunner)
    MGStation->~ControlLeft(ByObj);
  //Coordinator
  if(ByObj == Coordinator)
    RocketStation->~ControlLeft(ByObj);

  return true;
}

protected func ContainedRight(object ByObj, fRelease)
{
  [$CtrlRight$]
  
  //Pilot
  if (ByObj == Pilot)
  {
    //Autopilot aus
    ResetAutopilot();
    if(fRelease)
    {
      rotation = GetR();
    }
    else if (GetAction()=="Fly" || GetAction()=="Turn")
    {
      /*if(GetPlrCoreJumpAndRunControl(GetController(ByObj)))
      {
        rotation = max_rotation;
      }
      else
      {*/
        rotation = BoundBy(rotation + control_speed, -max_rotation, max_rotation);
      //}
    }
  }

  //Schütze
  if(ByObj == Gunner)
    MGStation->~ControlRight(ByObj);
  //Coordinator
  if(ByObj == Coordinator)
    RocketStation->~ControlRight(ByObj);

  return true;
}

protected func ContainedLeftDouble(object ByObj)
{
  [$CtrlLeftD$]
  //Pilot
  if (ByObj == Pilot)
  {
    //Autopilot aus
    ResetAutopilot();
    if (GetDir() && GetAction()=="Fly")
    {
      if (GetAction() == "Turn" || GetContact(this, -1)) return true;
      SetAction("Turn");
    }
  }

  //Schütze
  if(ByObj == Gunner)
    MGStation->~ControlLeftDouble(ByObj);
  //Coordinator
  if(ByObj == Coordinator)
    RocketStation->~ControlLeftDouble(ByObj);

  return true;
}

protected func ContainedRightDouble(object ByObj)
{
  [$CtrlRightD$]
  
  //Pilot
  if (ByObj == Pilot)
  {
    //Autopilot aus
    ResetAutopilot();
    if (!GetDir() && GetAction()=="Fly")
    {
      if (GetAction() == "Turn" || GetContact(this, -1)) return true;
      SetAction("Turn");
    }
  }

  //Schütze
  if(ByObj == Gunner)
    MGStation->~ControlRightDouble(ByObj);
  //Coordinator
  if(ByObj == Coordinator)
    RocketStation->~ControlRightDouble(ByObj);

  return true;
}

protected func ContainedThrow(object ByObj)
{
  [Image=KOKR|$CtrlThrow$]
  
  //nicht wenn kaputt
  if (GetDamage() > MaxDamage()) return true;
  //Piloten-HUD
  if (ByObj == Pilot)
  {
    if (!hud)
    {
      hud = CreateObject(BHUD, 0, 0, GetOwner(ByObj));
      hud->SetHelicopter(this);
      SetOwner(GetOwner(), hud);
    }
    else
      if(GetVisibility(hud) & VIS_Owner)
      {
        SetVisibility(VIS_None, hud);
      }
      else
        SetVisibility(VIS_Owner, hud);
    return Sound("SwitchHUD", false, this, 100, GetOwner(ByObj)+1);
  }

  //Schütze: Feuer eröffnen/einstellen
  if(ByObj == Gunner)
    if(!GetPlrCoreJumpAndRunControl(GetController(ByObj)))
      MGStation->~ControlThrow(ByObj);
  //Koordinator
  if(ByObj == Coordinator)
    RocketStation->~ControlThrow(ByObj);

  return true;
}

public func ContainedUpdate(object ByObj, int comdir, bool dig, bool throw)
{
  if(ByObj == Gunner)
    if(throw)
      return MGStation->ControlThrow(ByObj);
    else
      return MGStation->StopAutoFire();
}

//eine Funktion, welche einfach nur die Richtung eines Objektes ändert
protected func ChangeDir()
{
  for (var i = 0; i < GetDefCoreVal("Vertices", "DefCore", GetID()); i++)
    SetVertex(i, 0, (GetDir()*2-1)*GetDefCoreVal("VertexX", "DefCore", GetID(), i), this, 2);
  SetDir(!GetDir());
  return true;
}

/* ----- Platzwechsel ----- */

//Platz wechseln
protected func ContainedDigDouble(object ByObj)
{
  [$CtrlDigD$]
  CreateMenu(GetID(),ByObj,this,0,"$Seats$",0,1);
    //Ausstieg
    AddMenuItem("$Exit$", "ExitClonk",STDR,ByObj,0,ByObj,"$ExitDesc$");

    //Pilot
    if(Pilot)
      AddMenuItem("<c ff8888>$Pilot$</c>", "SeatOccupied",GetID(),ByObj,0,ByObj,"$SeatOccupied$");
    else
      AddMenuItem("<c 88ff88>$Pilot$</c>", "EnterSeat1",GetID(),ByObj,0,ByObj,"$PilotDesc$");

    //Schütze
    if(Gunner)
      AddMenuItem("<c ff8888>$Gunner$</c>", "SeatOccupied",GetID(),ByObj,0,ByObj,"$SeatOccupied$");
    else
      AddMenuItem("<c 88ff88>$Gunner$</c>", "EnterSeat2",GetID(),ByObj,0,ByObj,"$GunnerDesc$");

    //Raketen-Schütze
    if(Coordinator)
      AddMenuItem("<c ff8888>$Coordinator$</c>", "SeatOccupied",GetID(),ByObj,0,ByObj,"$SeatOccupied$");
    else
      AddMenuItem("<c 88ff88>$Coordinator$</c>", "EnterSeat3",GetID(),ByObj,0,ByObj,"$CoordinatorDesc$");

    //Passagier 1
    if(Passenger1)
      AddMenuItem("<c ff8888>$Passenger$</c>", "SeatOccupied",GetID(),ByObj,0,ByObj,"$SeatOccupied$");
    else
      AddMenuItem("<c 88ff88>$Passenger$</c>", "EnterSeat4",GetID(),ByObj,0,ByObj,"$PassengerDesc$");

    //Passagier 2
    if(Passenger2)
      AddMenuItem("<c ff8888>$Passenger$</c>", "SeatOccupied",GetID(),ByObj,0,ByObj,"$SeatOccupied$");
    else
      AddMenuItem("<c 88ff88>$Passenger$</c>", "EnterSeat5",GetID(),ByObj,0,ByObj,"$PassengerDesc$");
      
  return 1;
}

//Sitz besetzt?
private func SeatOccupied(a,ByObj)
{
  PlayerMessage(GetOwner(ByObj),"$SeatOccupied$",ByObj);
  Sound("Error",0,0,0,GetOwner(ByObj)+1);
  ContainedDigDouble(ByObj);
  return 1;
}

//Sitz reinigen *hrhr*
private func DeleteActualSeatPassenger(object Obj)
{
  if(Pilot == Obj)
  {
    Pilot = 0;
    if(hud)
      RemoveObject(hud);
    SetGraphics(0);
  }
  if(Gunner == Obj)
  {
    Gunner = 0;
    MGStation->SetGunner(0);
  }
  if(Coordinator == Obj)
  {
    Coordinator = 0;
    RocketStation->SetGunner(0);
  }
  if(Passenger1 == Obj)
    Passenger1 = 0;
  if(Passenger2 == Obj)
    Passenger2 = 0;
  return 1;
}

//Sitz belegen
public func EnterSeat1(a, object Obj)
{
  SetOwner(GetOwner(Obj));
  DeleteActualSeatPassenger(Obj);
  SetGraphics(0,this,GetID(),1,GFXOV_MODE_Object,0,GFX_BLIT_Additive,this);
  Pilot = Obj;
  SetGraphics("2");
  hud = CreateObject(BHUD, 0, 0, GetOwner(Obj));
	hud->SetHelicopter(this);
  Sound("RSHL_Deploy.ogg", true, this, 100, GetOwner(Obj)+1);

  return 1;
}

public func EnterSeat2(a, object Obj)
{
  DeleteActualSeatPassenger(Obj);
  Gunner = Obj;
  MGStation->SetGunner(Obj);
  Sound("RSHL_Deploy.ogg", false, this, 100, GetOwner(Obj)+1);

  return 1;
}

public func EnterSeat3(a, object Obj)
{
  DeleteActualSeatPassenger(Obj);
  Coordinator = Obj;
  RocketStation->SetGunner(Obj);
  Sound("RSHL_Deploy.ogg", false, this, 100, GetOwner(Obj)+1);

  return 1;
}

public func EnterSeat4(a, object Obj)
{
  DeleteActualSeatPassenger(Obj);
  Passenger1 = Obj;
  Sound("RSHL_Deploy.ogg", false, this, 100, GetOwner(Obj)+1);

  return 1;
}

public func EnterSeat5(a, object Obj)
{
  DeleteActualSeatPassenger(Obj);
  Passenger2 = Obj;
  Sound("RSHL_Deploy.ogg", false, this, 100, GetOwner(Obj)+1);

  return 1;
}

/* Ausstieg per Seil */

public func GetRopeAttach()
{
  return MGStation;
}

private func ExitClonk(a,ByObj)
{
  SetCommand(ByObj, "Exit");
}

protected func FxCheckGroundStart(pTarget, iNo, iTemp, pHeli)
{
  if(iTemp)
    return -1;
  if(!pHeli) return;
    var pRope = CreateObject(CK5P,0,0,GetOwner(pTarget));
    pRope->ConnectObjects(pHeli ,pTarget);
    pRope->SetRopeLength(10);
  EffectVar(0, pTarget, iNo) = pRope; //Das Seil
  EffectVar(1, pTarget, iNo) = pHeli; //Der Helikopter
}

protected func FxCheckGroundTimer(pTarget, iNo, iTime)
{
  var pRope = EffectVar(0, pTarget, iNo);
  //Knapp über dem Boden, Seil zu lang, zu lang die Dauer oder falsche Aktion?
  if(!PathFree(GetX(pTarget),GetY(pTarget),GetX(pTarget),GetY(pTarget)+30)
     || pRope->GetRopeLength() > 300
     || !WildcardMatch(GetAction(pTarget),"*Jump*"))
  { //Abspringen, Seil zurück zum Heli schicken.
    //var pulley = CreateObject(ROCK,AbsX(GetX(pTarget)),AbsY(GetY(pTarget)),-1);
    //SetCategory(C4D_Vehicle,pulley);
    RemoveObject(pRope);
    SetYDir(20,pTarget);
    //EffectVar(0, pTarget, iNo)=CreateObject(CK5P,0,0,-1);
    //EffectVar(0, pTarget, iNo)->ConnectObjects(EffectVar(1, pTarget, iNo),pulley);
    //AddEffect("CheckEnd", pulley, 30, 3, EffectVar(1, pTarget, iNo), 0, EffectVar(0, pTarget, iNo), EffectVar(1, pTarget, iNo));
    return -1;
  }
  else
    pRope->SetRopeLength(iTime*2+10);
  //Sounds fürs Abseilen?
}

/*protected func FxCheckEndStart(pTarget, iNo, iTemp, pHeli, pRope)
{
  if(iTemp)
    return -1;
  EffectVar(0, pTarget, iNo) = pRope; //Das Seil
  EffectVar(1, pTarget, iNo) = pHeli; //Der Heli
}

protected func FxCheckEndTimer(pTarget, iNo, iTime)
{
  if(ObjectDistance(EffectVar(1, pTarget, iNo),pTarget) <= 10 || iTime > 300)
  {
    RemoveObject(EffectVar(0, pTarget, iNo));
    RemoveObject(pTarget);
    return -1;
  }
}
*/

/* ----- Schaden ----- */

public func OnDmg(int iDmg, int iType)
{
  if(iType == DMG_Energy)	return -20;
  if(iType == DMG_Bio)		return 100;
  if(iType == DMG_Melee)	return 80;
  if(iType == DMG_Fire)		return 50;
  if(iType == DMG_Explosion)	return -50;
  if(iType == DMG_Projectile)	return 80;

  return 50;
}

public func OnDamage()
{
  if(hud)			hud->DamageReceived();
  if(GetContact(this, -1))	ResetAutopilot();
	return true;
}

public func OnDestruction()
{
  //Inhalt auswerfen und töten bzw. zerstören
  for(var obj in FindObjects(Find_Container(this), Find_Not(Find_ID(FKDT))))
   {
    DeleteActualSeatPassenger(obj);
    if(GetOCF(obj) & OCF_Alive && GetID(Contained(obj)) != FKDT)
    {
      DoDmg(200, DMG_Explosion, obj, 0, GetLastAttacker()+1);
    }
    else
    {
      DoDamage(200,obj);
    }
    Sound("CockpitRadio.ogg", true, 0, 100, GetOwner(obj)+1, -1);
    if(Contained(obj) == this)
      Exit(obj, 0, 0, Random(360), RandomX(-5,5), RandomX(-4,8), Random(10));
  }

  //Explosion
	FakeExplode(60, GetLastAttacker());
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
  for (var i; i < 7; i++)
  {
    ScheduleCall(obj, "BlastPar", i*2, 0, Sin(Random(360), 10*i), -Cos(Random(360), 10*i), 200, RGB(255,255,255));
    ScheduleCall(obj, "BlastPar", i*2, 0, Sin(Random(360), 10*i), -Cos(Random(360), 10*i), 200, RGB(255,255,255));
    ScheduleCall(obj, "BlastPar", i*2, 0, Sin(Random(360), 10*i), -Cos(Random(360), 10*i), 200, RGB(255,255,255));
    ScheduleCall(obj, "BlastPar", i*2, 0, Sin(Random(360), 10*i), -Cos(Random(360), 10*i), 200, RGB(255,255,255));
    ScheduleCall(obj, "BlastPar", i*2, 0, Sin(Random(360), 10*i), -Cos(Random(360), 10*i), 200, RGB(255,255,255));
    ScheduleCall(obj, "BlastPar", i*2, 0, Sin(Random(360), 10*i), -Cos(Random(360), 10*i), 200, RGB(255,255,255)); 
  }
  return;
}

//Kontakt am Rotor: Schaden!
protected func ContactTop()
{
  if (GetCon() != 100) return;
  //sehr viel schaden
  DoDamage(25, this);
  for (var i; i < GetVertexNum(); i++)
  {
    if (GetContact(0, i))
      CreateParticle("Blast", GetVertex(i), GetVertex(i, true),
                     0, 0, 50, RGB(255,255,255));
  }
  Sound("HeavyHit*.ogg", false, MGStation);
  SetYDir(GetYDir()*-1/2);
}

//die Räder halten mehr aus
protected func ContactBottom()
{
  if (GetYDir() > 25)
  {
    for (var i; i < GetVertexNum(); i++)
    {
      if (GetContact(0, i))
        CreateParticle("Blast", GetVertex(i), GetVertex(i, true),
                       0, 0, 50, RGB(255,255,255));
    }
    DoDamage(GetYDir()/2);
    Sound("HeavyHit*.ogg", false, MGStation);
    SetYDir(GetYDir()*-1/3);
  }
}

protected func ContactLeft()
{
  if (Abs(GetXDir()) > 20 || Abs(GetYDir()) > 20)
  {
    for (var i; i < GetVertexNum(); i++)
    {
      if (GetContact(0, i))
        CreateParticle("Blast", GetVertex(i), GetVertex(i, true),
                       0, 0, 50, RGB(255,255,255));
    }
    DoDamage(Abs(GetXDir())+Abs(GetYDir()));
    Sound("HeavyHit*.ogg", false, MGStation);
  }
  SetXDir(GetXDir()*-1/2); //Abprallen
}

protected func ContactRight()
{
  if (Abs(GetXDir()) > 20 || Abs(GetYDir()) > 20)
  {
    for (var i; i < GetVertexNum(); i++)
    {
      if (GetContact(0, i))
        CreateParticle("Blast", GetVertex(i), GetVertex(i, true),
                       0, 0, 50, RGB(255,255,255));
    }
    DoDamage(Abs(GetXDir())+Abs(GetYDir()));
    Sound("HeavyHit*.ogg", false, MGStation);
  }
  SetXDir(GetXDir()*-1/2); //Abprallen
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
    SetXDir((Random(30)+30)*dir, ByObj);
    SetYDir(RandomX(-25,-15), ByObj);
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
	if(IsDestroyed()) return;	
	
  //Absinken, falls kein Pilot da.
  if(!GetPilot() && !GetAutopilot())
  {
    if(!Random(3))
    {
      if(throttle > 100)
        throttle--;
      else if(throttle > 75)
        throttle-=3;
      else if(throttle > 50)
        throttle-=5;
    }
  }

  //unter Wasser stirbt der Motor ab
  Water();

  //Piloten anpassen
  DrawPilot();

	//Blinken
	/*if(!(GetActTime()%45) && GetPilot() || GetAutopilot())
  {
    if(GetTeam())
      var rgb = GetTeamColor(GetTeam());
    else if(GetOwner() != NO_OWNER)
      var rgb = GetPlrColorDw(GetOwner());
    else
      var rgb = RGB(255,255,255);
    CreateParticle("FlashLight",-104*(GetDir()*2-1),-13,0,0,3*15,rgb,this);
  }*/
	
	//Bodenpartikel zeichnen
	DrawGroundParticles();

	//Lebewesen schrappneln
	if(GetRotorSpeed() > 0) {
		for(var pClonk in FindObjects(Find_InRect(-100,-24,200,16), Find_NoContainer(), Find_OCF(OCF_Alive), Find_Not(Find_ID(FKDT)))) {
			if(GetOwner(pClonk) != NO_OWNER && GetOwner() != NO_OWNER && !Hostile(GetOwner(), GetOwner(pClonk))) continue;
			Fling(pClonk, RandomX(2,3)+GetRotorSpeed()/100*((GetR() > 0 && GetR() <= 180)*2-1), RandomX(-2, -1)-GetRotorSpeed()/100);
			DoDmg(GetRotorSpeed()/3, DMG_Projectile, pClonk, 0, GetOwner()+1);
		}
	}

	//Stuck?
	if(Stuck()) DoDamage(5);

  //bis 50% nichts
  if (GetDamage() < MaxDamage()*1/2) return;

  // ab 50% rauchen
  Smoking();

  if (GetDamage() < MaxDamage()*3/4) return;

  // ab 25% Feuer
  DrawFire();

  if (!GetEffect("Engine", this)) return;
  
  //Warnsound
  WarningSound();
}

private func DrawPilot()
{
  if (view_mode && !GetPilot())
  {
    view_mode = false;
    SetColorDw(RGBa(200,200,200,255));
  }
  else if (!view_mode && GetPilot())
  {
    view_mode = true;
    SetColorDw(RGBa(200,200,200,0));
  }
}

private func WarningSound()
{
    if (GetDamage() < MaxDamage()*3/4) 
  {
    //Sound("WarningDamageCritical.ogg", false, this);
    if (!(s_counter%36))
	  for (var obj in FindObjects(Find_OCF(OCF_CrewMember), Find_Container(this)))
        Sound("WarningDamage.ogg", false, MGStation, 100, GetOwner(obj)+1);
    s_counter++;
    if (s_counter >= 100) s_counter = 0;
    
    return;
  }
  else
  {
    if (!(s_counter%20))
    {
      Local(2) = 0;
      for (var obj in FindObjects(Find_OCF(OCF_CrewMember), Find_Container(this)))
        if(obj != GetPilot())
          Sound("WarningDamageCritical.ogg", false, MGStation, 100, GetOwner(obj)+1);
    }
    s_counter++;
    if (s_counter >= 100) s_counter = 0;

    return;
  }
}

private func DrawGroundParticles()
{
  if(!GetRotorSpeed()) return;
  if(GBackLiquid() || GBackSolid()) return;
  var rot = GetDir()*180-90 + GetR();
  for (var i; i < 30; i++)
  {
    if (GetMaterial(0, i*5) == Material("Earth"))
      return CreateDust(i*5, GetRotorSpeed(), RGB(150, 86, 22));
    if ((Material("Wall") != -1 && GetMaterial(0, i*5) == Material("Wall")) || GetMaterial(0, i*5) == Material("Vehicle"))                    
      return CreateDust(i*5, GetRotorSpeed(), RGB(150,150,150));
    if (GetMaterial(0, i*5) == Material("Snow"))                    
      return CreateDust(i*5, GetRotorSpeed(), RGB(255,255,255));
    if (GetMaterial(0, i*5) == Material("Sand"))                    
      return CreateDust(i*5, GetRotorSpeed(), RGB(247,236,157));
    if (GetMaterial(0, i*5) == Material("Ashes"))                    
      return CreateDust(i*5, GetRotorSpeed(), RGB( 64, 55, 36));
    if (GBackSolid(0, i*5))
      return true;

    if (GetMaterial(0, i*5) == Material("Water")) {
      return CreateDust(i*5, GetRotorSpeed(), RGB(176,194,208));
     }
  }
  return;
}

private func CreateDust(int Y, int Power, int Color)
{
  Power = Min(Power, 130);
  CreateParticle("GroundSmoke", -3, Y, -(70-Y/3), RandomX(-5,5),
                 RandomX(30,15+(14-Y/10)*Power/5), Color);//nach links
  CreateParticle("GroundSmoke", +3, Y, (70-Y/3), RandomX(-5,5),
                 RandomX(30,15+(14-Y/10)*Power/5), Color);//nach rechts
  CreateParticle("GroundSmoke", -3, Y-3, RandomX(-30,-(70-Y)), -2,
                 RandomX(30,15+(14-Y/10)*Power/5), Color);//nach links oben
  CreateParticle("GroundSmoke", +3, Y-3, RandomX(30,(70-Y)), -2,
                 RandomX(30,15+(14-Y/10)*Power/5), Color);//nach rechts oben
  return true;
}

private func DrawFire()
{
  var dir = GetDir()*2-1;
  if (!GBackLiquid(-Sin(GetR()+dir*80, 25), +Cos(GetR()+dir*80, 25)))
  {
    CreateParticle("Blast",-Sin(GetR()+dir*80, 25),
                           +Cos(GetR()+dir*80, 25),
  			             0, -10, 100+Random(20), RGB(255,255,255), this);
  }
  if (!GBackLiquid(-Sin(GetR()+dir*80, 25), +Cos(GetR()+dir*80, 25)))
  {
    CreateParticle("Blast",-Sin(GetR()-dir*60, 25),
                           +Cos(GetR()+dir*100, 25),
  			             0, -10, 100+Random(20), RGB(255,255,255), this);
  }
}

private func Water()
{
  if (GBackLiquid(0,10))
    DoDamage(5);
}

private func Smoking()
{
  var dir = GetDir()*2-1;

  for (var a = 0; a < 3; a++)
    if (!GBackLiquid(-Sin(GetR()+dir*80, 25), +Cos(GetR()+dir*80, 25)))
      Smoke(-Sin(GetR()+dir*80, 25), +Cos(GetR()+dir*80, 25), Random(10));
}

/* ----- Physik ----- */

protected func StartEngine()
{
  Sound("StartSystem.ogg", false, this);
}

protected func EngineStarted()
{
  AddEffect("Engine",this,300,1,this,0);
  throttle = 0;
  rotation =  0;
}

protected func StopEngine()
{
  Sound("StopSystem.ogg", false, this); 
  RemoveEffect("Engine",this);
}

protected func EngineStopped()
{
  throttle = 0;
  rotation = 0;
}

/* ----- Effekt: Engine ----- */

protected func FxEngineStart(object Target, int EffectNumber, int Temp, Var1, Var2, Var3, Var4)
{
  if (Temp) return true;
}

protected func FxEngineStop(object Target, int EffectNumber, int Reason, bool Temp)
{
  if (Temp) return true;
}

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
//Message("%dN %dN|%dN|a=%d", Target, Fg, Fh, dFh, av);
  SetYDir(GetYDir(Target, 95) + av, Target, 100);
  SetXDir(GetXDir(Target, 95) + ah, Target, 100);

  return true;
}

//eine neue Funktion: Tangens = Sinus / Kosins
global func Tan(int angle, int radius)
{
  return Sin(angle, radius, 10) / Cos(angle, radius, 10);
}
