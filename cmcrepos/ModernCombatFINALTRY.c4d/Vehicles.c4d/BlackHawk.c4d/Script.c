/* Helicopter */

#strict 2


/* ----- Variablen ----- */

local throttle,                 //int    - Schub
      rotation,                 //int    - erwartete Drehung
      idle,                     //int    - Motor-Start/Stop Sequenz
      view_mode;                //bool   - f�r den Piloten

local hud;                      //object - Anzeige f�r Pilot

local Pilot,                    //object - Pilot
      Gunner,                   //object - Sch�tze
      Rocketeer,                //object - Raketensch�tze
      Passenger1, Passenger2;   //object - verschiedene Sitzpl�tze im Heli

local MGStation,                //object - Das MG-Objekt
      RocketStation;            //object - Das Raketenwerfer-Objekt

local s_counter,                //int    - eine kleine Counter-Variable f�r Warnsounds
      d_counter;                //int    - eine kleine Counter-Variable f�r die Zerst�rung

local destroyed;                //bool   - ob der Heli schon zerst�rt ist

local fuel;

static const throttle_speed = 5;//int    - "Feinf�hligkeit"
static const rot_speed = 1;     //int    - Drehgeschwindigkeit / frame
static const control_speed = 3; //int    - "Feinf�hligkeit"
static const max_throttle = 200;//int    - h�chste Schubeinstellung
static const max_rotation = 30; //int    - st�rkste erlaubte Neigung
static const auto_throttle_speed = 1;
static const auto_max_throttle = 150;
static const auto_max_rotation = 10;

/* ----- Callbacks ----- */

public func IsMachine() { return 1; }
public func MaxDamage() { return 200; }

//Ha, einfacher und effektiver
public func IsBulletTarget(id idBullet, object pBullet)
{
  if(idBullet == MISS || idBullet == HMIS || idBullet == ROKT || idBullet == ESHL)
    return ObjectDistance(pBullet) < 40;
  return 1;
}


/* ----- Existenz ----- */

//Initialisierung
protected func Initialize()
{
  //Steuerung initialisieren
  throttle = 0;
  rotation = 0; 
  //Stuff.
  fuel = 999999; //Blar?
  SetAction("Stand");
  
  //Pilot
  view_mode = true;
  
  //Gesch�tze
  MGStation = CreateObject(H_MA,0,0,GetOwner());
  MGStation -> Set(this,10,90,90,270);
  MGStation -> Arm(ACCN);
  RocketStation = CreateObject(H_MA,0,0,GetOwner());
  RocketStation -> Set(this,40,10,210,270);
  RocketStation -> Arm(RLSA);

  //Eingang
  SetEntrance(true);
  //Solidmask
  SetSolidMask();
  //fertig
  return true;
}

//wird genutzt, den Landport davon abzuhalten, den zerst�rten Heli zu versorgen
func IsReady()
{
  return GetDamage() < MaxDamage() && GetContact(this(), -1, CNAT_Bottom) && !destroyed;
}

//alle dazugeh�rigen Objekte l�schen
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

public func GetPilot() {
	return Pilot;
}

public func GetThrottle() {
	return throttle;
}

/* Autopilot - betreten auf eigene Gefahr! - Eltern haften f�r ihre Kinder */

public func GetAutopilot() {
	return GetEffect("BlackhawkAutopilot", this) != false;
}

public func SetAutopilot(object pTarget, int iX, int iY) {
	if(!Pilot) return false;
	ResetAutopilot();
	var xto, yto;
	if(pTarget) {
		xto = AbsX(GetX(pTarget));
		yto = AbsY(GetY(pTarget));
	}
	xto += iX;
	yto += iY;
	AddEffect("BlackhawkAutopilot", this, 10, 1, this, 0, xto, yto);
	return true;
}

public func ResetAutopilot() {
	while(GetEffect("BlackhawkAutopilot", this)) RemoveEffect("BlackhawkAutopilot", this);
	return true;
}

protected func FxBlackhawkAutopilotStart(object pTarget, int iNumber, iTemp, int iX, int iY) {
	if(GBackLiquid(AbsX(iX), AbsY(iY))) return -1;
	EffectVar(0, pTarget, iNumber) = iX;
	EffectVar(1, pTarget, iNumber) = iY;	
}

protected func FxBlackhawkAutopilotTimer(object pTarget, int iNumber, int iTime) {
  if(!Pilot) return pTarget->ResetAutopilot();
	var iX = EffectVar(0, pTarget, iNumber);
	var iY = EffectVar(1, pTarget, iNumber);
	if(GetY(pTarget) < iY-50) {
		if(GetYDir(pTarget) < 3 || GetContact(this(), -1, CNAT_Bottom)) {
			//vom Gas weg
			if(GetAction()=="Fly" || GetAction()=="Turn")
			throttle = BoundBy(throttle - auto_throttle_speed, 0, auto_max_throttle);
			//Motor aus
	    if (throttle == 0 && (GetAction()=="Fly" || GetAction()=="EngineStartUp")) {
	    	SetAction("EngineShutDown");
	   		return(pTarget->ResetAutopilot());
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
			  if (GetAction() == "Turn" || GetContact(this(), -1)) return true;
			  SetAction("Turn");
			}
	}
	else if(GetX(pTarget) < iX-50){
		 if (GetAction()=="Fly" || GetAction()=="Turn") 
      rotation = BoundBy(rotation + control_speed, -auto_max_rotation, auto_max_rotation);
      if (rotation > 0 && !GetDir() && GetAction()=="Fly")
			{
			  if (GetAction() == "Turn" || GetContact(this(), -1)) return true;
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

protected func Ejection(object ByObj) {
	if(ByObj != Pilot && PathFree(GetX(),GetY(),GetX(),GetY()+50))
	{
	  var rope = CreateObject(CK5P,0,0,-1);
	  rope->ConnectObjects(this,ByObj);
	  Local(8,rope) = true;
	  AddEffect("CheckGround",ByObj,30,3,this,GetID(),rope,this);
	}
	DeleteActualSeatPassenger(ByObj);
  Sound("CockpitRadio.ogg", true, 0, 100, GetOwner(ByObj)+1, -1);
  return true;
}

//herausgeworfene Objekte sollen vor der T�r erscheinen
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
  	if(ByObj == Pilot) {
  		SetAutopilot(Target, TargetX, TargetY);
  	}
  	return true;
  }
  return false;
}

protected func Collection2(object pObj)
{
  if(GetOCF(pObj) & OCF_Alive && GetOCF(pObj) & OCF_CrewMember)
  {
    if(!Hostile(GetOwner(this),GetOwner(pObj)))
    {
	    Sound("CockpitRadio.ogg", true, 0, 100, GetOwner(pObj)+1, +1);
      if(!Pilot)
        return EnterSeat1(0,pObj); //Bl�de Platzhalter :/
      if(!Gunner)
        return EnterSeat2(0,pObj);
      if(!Rocketeer)
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
    if (GetAction()=="Fly" || GetAction()=="Turn")
      throttle = BoundBy(throttle + throttle_speed, 0, max_throttle);
  }
  
  //Gunner
  if (ByObj == Gunner)
    MGStation->~ControlUp(ByObj);
  //Rocketeer
  if (ByObj == Rocketeer)
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
  
  //Gunner
  if (ByObj == Gunner)
    MGStation->~ControlDown(ByObj);
  //Rocketeer
  if (ByObj == Rocketeer)
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
  if(ByObj == Passenger1 || ByObj == Passenger2) {
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
      rotation = BoundBy(rotation - control_speed, -max_rotation, max_rotation);
  }
  
  //Gunner
  if (ByObj == Gunner)
    MGStation->~ControlLeft(ByObj);
  //Rocketeer
  if (ByObj == Rocketeer)
    RocketStation->~ControlLeft(ByObj);
    
  return true;
}

protected func ContainedRight(object ByObj)
{
  [$CtrlRight$]
  
  //Pilot
  if (ByObj == Pilot)
  {
   	//Autopilot aus
  	ResetAutopilot();
    if (GetAction()=="Fly" || GetAction()=="Turn")
      rotation = BoundBy(rotation + control_speed, -max_rotation, max_rotation);
  }
  
  //Gunner
  if (ByObj == Gunner)
    MGStation->~ControlRight(ByObj);
  //Rocketeer
  if (ByObj == Rocketeer)
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
      if (GetAction() == "Turn" || GetContact(this(), -1)) return true;
      SetAction("Turn");
    }
  }
  
  //Gunner
  if (ByObj == Gunner)
    MGStation->~ControlLeftDouble(ByObj);
  //Rocketeer
  if (ByObj == Rocketeer)
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
      if (GetAction() == "Turn" || GetContact(this(), -1)) return true;
      SetAction("Turn");
    }
  }
  
  //Gunner
  if (ByObj == Gunner)
    MGStation->~ControlRightDouble(ByObj);
  //Rocketeer
  if (ByObj == Rocketeer)
    RocketStation->~ControlRightDouble(ByObj);
    
  return true;
}

protected func ContainedThrow(object ByObj)
{
  [Image=KOKR|$CtrlThrow$]
  
  //nicht wenn kaputt
  if (GetDamage() > MaxDamage()) return(true);
  //der Pilot kriegt eins...
  if (ByObj == Pilot)
  {
    if (!hud)
    {
      hud = CreateObject(BHUD, 0, 0, GetOwner(ByObj));
      hud->SetHelicopter(this);
      hud->SetOwner(GetOwner());
    }
    else
      RemoveObject(hud);
    return(Sound("SwitchHUD", false, this(), 100, GetOwner(ByObj)+1));
  }
  
  //Gunner
  if (ByObj == Gunner)
    if (!GetPlrCoreJumpAndRunControl(GetController(ByObj)))
      MGStation->~ControlThrow(ByObj);
  //Rocketeer
  if (ByObj == Rocketeer)
    RocketStation->~ControlThrow(ByObj);

  return true;
}

public func ContainedUpdate(object ByObj, int comdir, bool dig, bool throw)
{
  if (ByObj == Gunner)
    if (throw)
	  return MGStation->ControlThrow(ByObj);
	else
	  return MGStation->StopAutoFire();
}

//eine Funktion, welche einfach nur die Richtung eines Objektes �ndert
protected func ChangeDir()
{
  SetDir(!GetDir());
  SetVertex(0, false, 15*(GetDir()*2-1), this(), 2);
  return(true);
}


/* ----- Platzwechsel ----- */

//Platz wechseln
protected func ContainedDigDouble(object ByObj)
{
  [$CtrlDigD$]
  CreateMenu(GetID(),ByObj,this(),0,"$Seats$",0,1);
    //Ausstieg
    AddMenuItem("$Exit$", "ExitClonk",STDR,ByObj,0,ByObj,"Bringt den Clonk dazu auszusteigen.");
    //Pilot
    if(Pilot)
      AddMenuItem("<c ff8888>Pilot</c>", "SeatOccupied",GetID(),ByObj,0,ByObj,"$SeatOccupied$");
    else
      AddMenuItem("<c 88ff88>Pilot</c>", "EnterSeat1",GetID(),ByObj,0,ByObj,"$PilotSeat$");
    //MG-Sch�tze
    if(Gunner)
      AddMenuItem("<c ff8888>$Gunner$</c>", "SeatOccupied",GetID(),ByObj,0,ByObj,"$SeatOccupied$");
    else
      AddMenuItem("<c 88ff88>$Gunner$</c>", "EnterSeat2",GetID(),ByObj,0,ByObj,"$GunnerSeat$");
    //Raketen-Sch�tze
    if(Rocketeer)
      AddMenuItem("<c ff8888>$Rocketeer$</c>", "SeatOccupied",GetID(),ByObj,0,ByObj,"$SeatOccupied$");
    else
      AddMenuItem("<c 88ff88>$Rocketeer$</c>", "EnterSeat3",GetID(),ByObj,0,ByObj,"$RocketeerSeat$");
    //Passagier 1
    if(Passenger1)
      AddMenuItem("<c ff8888>$Passenger1$</c>", "SeatOccupied",GetID(),ByObj,0,ByObj,"$SeatOccupied$");
    else
      AddMenuItem("<c 88ff88>$Passenger1$</c>", "EnterSeat4",GetID(),ByObj,0,ByObj,"$PassengerSeat$");
    //Passagier 2
    if(Passenger2)
      AddMenuItem("<c ff8888>$Passenger2$</c>", "SeatOccupied",GetID(),ByObj,0,ByObj,"$SeatOccupied$");
    else
      AddMenuItem("<c 88ff88>$Passenger2$</c>", "EnterSeat5",GetID(),ByObj,0,ByObj,"$PassengerSeat$");
      
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
  if(Rocketeer == Obj)
  {
    Rocketeer = 0;
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
  Sound("SwitchHUD", false, this(), 100, GetOwner(Obj)+1);

  return 1;
}

public func EnterSeat2(a, object Obj)
{
  DeleteActualSeatPassenger(Obj);
  Gunner = Obj;
  MGStation->SetGunner(Obj);
  Sound("SwitchHUD", false, this(), 100, GetOwner(Obj)+1);

  return 1;
}

public func EnterSeat3(a, object Obj)
{
  DeleteActualSeatPassenger(Obj);
  Rocketeer = Obj;
  RocketStation->SetGunner(Obj);
  Sound("SwitchHUD", false, this(), 100, GetOwner(Obj)+1);

  return 1;
}

public func EnterSeat4(a, object Obj)
{
  DeleteActualSeatPassenger(Obj);
  Passenger1 = Obj;
  Sound("SwitchHUD", false, this(), 100, GetOwner(Obj)+1);

  return 1;
}

public func EnterSeat5(a, object Obj)
{
  DeleteActualSeatPassenger(Obj);
  Passenger2 = Obj;
  Sound("SwitchHUD", false, this(), 100, GetOwner(Obj)+1);

  return 1;
}

/* Ausstieg per Seil */

private func ExitClonk(a,ByObj)
{
  SetCommand(ByObj, "Exit");
}

protected func FxCheckGroundStart(pTarget, iNo, iTemp, pRope, pHeli)
{
  if(iTemp)
    return -1;
  EffectVar(0, pTarget, iNo) = pRope; //Das Seil
  EffectVar(1, pTarget, iNo) = pHeli; //Der Heli
}

protected func FxCheckGroundTimer(pTarget, iNo, iTime)
{
  //Knapp �ber dem Boden, Seil zu lang, zu lang die Dauer oder falsche Aktion?
  if(!PathFree(GetX(pTarget),GetY(pTarget),GetX(pTarget),GetY(pTarget)+30)
     || EffectVar(0, pTarget, iNo)->GetRopeLength() > 300
     || iTime > 300
     || !WildcardMatch(GetAction(pTarget),"*Jump*"))
  {  //Abspringen, Seil zur�ck zum Heli schicken.
    //var pulley = CreateObject(ROCK,AbsX(GetX(pTarget)),AbsY(GetY(pTarget)),-1);
    //SetCategory(C4D_Vehicle,pulley);
    RemoveObject(EffectVar(0, pTarget, iNo));
    SetYDir(20,pTarget);
    //EffectVar(0, pTarget, iNo)=CreateObject(CK5P,0,0,-1);
    //EffectVar(0, pTarget, iNo)->ConnectObjects(EffectVar(1, pTarget, iNo),pulley);
    //AddEffect("CheckEnd", pulley, 30, 3, EffectVar(1, pTarget, iNo), 0, EffectVar(0, pTarget, iNo), EffectVar(1, pTarget, iNo));
    return -1;
  }
  else
    LocalN("iLength",EffectVar(0, pTarget, iNo)) = iTime/3;
  //Sounds f�rs Abseilen?
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

public func OnDmg(int iDmg, int iType) {
	
	if(iType == DMG_Energy)		return(-20);
	if(iType == DMG_Bio)		return(100);
	if(iType == DMG_Melee)		return(80);
	if(iType == DMG_Fire)		return(50);
	if(iType == DMG_Explosion)	return(-50);
	if(iType == DMG_Projectile)	return(80);
	
	return(50);
}

public func Damage()
{
	if(hud)	hud->DamageReceived();
	if(GetContact(this, -1)) ResetAutopilot();
	if(GetDamage() < MaxDamage()) return;

	DestroyHeli();
}

//hier wird der Heli effektreich zerst�rt
func DestroyHeli()
{
  //alles raus und tot/kaputt
  for(var obj in FindObjects(Find_Container(this)))
  {
    DeleteActualSeatPassenger(obj);
    DoDamage(200,obj);
    Sound("CockpitRadio.ogg", true, 0, 100, GetOwner(obj)+1, -1);
    Exit(obj, 0, 0, Random(360), RandomX(-5,5), RandomX(-4,8), Random(10));
  }
  
  Explode(60);
  Sound("MainHelicopterExplosion", false, this);

  //zum Wrack machen
  var obj;
  obj = CreateObject(H_HW, 0, 20, -1);
  obj -> Incinerate();
  obj -> SetR(GetR());
  obj -> SetXDir(GetXDir());
  obj -> SetYDir(GetYDir());
  obj -> SetRDir(GetRDir());
  for (var i; i < 7; i++)
  {
    ScheduleCall(obj, "BlastPar", i*2, 0, Sin(Random(360), 10*i), -Cos(Random(360), 10*i), 200, RGB(255,255,255));
    ScheduleCall(obj, "BlastPar", i*2, 0, Sin(Random(360), 10*i), -Cos(Random(360), 10*i), 200, RGB(255,255,255));
    ScheduleCall(obj, "BlastPar", i*2, 0, Sin(Random(360), 10*i), -Cos(Random(360), 10*i), 200, RGB(255,255,255));
    ScheduleCall(obj, "BlastPar", i*2, 0, Sin(Random(360), 10*i), -Cos(Random(360), 10*i), 200, RGB(255,255,255));
    ScheduleCall(obj, "BlastPar", i*2, 0, Sin(Random(360), 10*i), -Cos(Random(360), 10*i), 200, RGB(255,255,255));
    ScheduleCall(obj, "BlastPar", i*2, 0, Sin(Random(360), 10*i), -Cos(Random(360), 10*i), 200, RGB(255,255,255)); 
  }
  return(RemoveObject());
}

//Kontakt am Rotor: Schaden!
protected func ContactTop()
{
  if (!(GetCon() == 100)) return(false);
  //sehr viel schaden
  DoDamage(25, this());
  for (var i; i < GetVertexNum(); i++)
  {
    if (GetContact(0, i))
      CreateParticle("Blast", GetVertex(i), GetVertex(i, true),
                     0, 0, 50, RGB(255,255,255));
  }
  Sound("HeavyHit*.ogg", false, MGStation);
  SetYDir(GetYDir()*-1/2);
}

//die R�der halten mehr aus
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
  //man soll schon Objekte im Heli ablegen k�nnen
  if (Contained(ByObj))
    return(false);
  //von au�en hei�t es: Schaden!!!
  if (GetAction()!="Stand")
  {
    var dir = (!GetDir())*2-1;
    DoDamage(GetMass(ByObj));
    ProtectedCall(ByObj, "Hit");
    SetXDir((Random(30)+30)*dir, ByObj);
    SetYDir(   RandomX(-20,-20), ByObj);
    return(true);
  }
  else if (GetOCF(ByObj) & OCF_HitSpeed2)
  {
    ProtectedCall(ByObj, "Hit");
    return(true);
  }
  return(true);
}
                           
//f�r Warnsounds und Grafik zust�ndig
protected func TimerCall()
{
  //Absinken, falls kein Pilot da.
  if(!Pilot)
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

	DrawGroundParticles();

  //bis 50% nichts
  if (GetDamage() < MaxDamage()*1/2) return(false);

  // ab 50% rauchen
  Smoking();

  if (GetDamage() < MaxDamage()*3/4) return(false);

  // ab 25% Feuer
  DrawFire();

  if (!GetEffect("Engine", this())) return(false);
  
  //Warnsound
  WarningSound();
}

private func DrawPilot()
{
  if (view_mode && !Pilot)
  {
    view_mode = false;
    SetColorDw(RGBa(200,200,200,255));
  }
  else if (!view_mode && Pilot)
  {
    view_mode = true;
    SetColorDw(RGBa(200,200,200,0));
  }
}

private func WarningSound()
{
    if (GetDamage() < MaxDamage()*3/4) 
  {
    //Sound("DamageCritical.ogg", false, this());
    if (!(s_counter%36))
    {
      var obj;
      for (var i; i < ContentsCount(0, this()); i++)
      {
        if (obj = Contents(i, this()))
          if (GetOCF(obj) & OCF_CrewMember)
           Sound("DamageWarning", false, MGStation, 100, GetOwner(obj)+1);
      }
    }
    s_counter++;
    if (s_counter >= 100) s_counter = 0;
    
    return(false);
  }
  else
  {
    if (!(s_counter%20))
    {
      var obj;
      Local(2) = 0;
      for (var i; i < ContentsCount(0, this()); i++)
      {
        if (obj = Contents(i, this()))
          if (GetOCF(obj) & OCF_CrewMember)
            Sound("DamageCritical.ogg", false, MGStation, 100, GetOwner(obj)+1);

      }
    }
    s_counter++;
    if (s_counter >= 100) s_counter = 0;

    return(false);
  }
}

private func DrawGroundParticles() {
	/*var iY = 0;
	while(!GBackSolid(0, iY) && !GBackLiquid(0, iY)) {
		iY += 5;
	}
	while(GBackSolid(0, iY) && GBackLiquid(0, iY)) {
		iY -= 1;
	}
	Log("%d %d", AbsX(0), AbsY(iY));
  CreateParticle("SlimeGra1v",30,-iY,(1+Random(4)),(7+Random(4)) ,100/30 ,RGBa(100,150,255,100+Random(100)));
  CreateParticle("SlimeGrav",-30,-iY,-(1+Random(4)),-(7+Random(4)) ,-100/30 ,RGBa(100,150,255,100+Random(100)));*/
}

private func DrawFire()
{
  var dir = GetDir()*2-1;
  if (!GBackLiquid(-Sin(GetR()+dir*80, 25), +Cos(GetR()+dir*80, 25)))
  {
    CreateParticle("Blast",-Sin(GetR()+dir*80, 25),
                           +Cos(GetR()+dir*80, 25),
  			             0, -10, 100+Random(20), RGB(255,255,255), this());
  }
  if (!GBackLiquid(-Sin(GetR()+dir*80, 25), +Cos(GetR()+dir*80, 25)))
  {
    CreateParticle("Blast",-Sin(GetR()-dir*60, 25),
                           +Cos(GetR()+dir*100, 25),
  			             0, -10, 100+Random(20), RGB(255,255,255), this());
  }
}

private func Water()
{
  if (GBackLiquid(0,10) )
  {
    DoDamage(5);
  }  
}

private func Smoking()
{
  var dir = GetDir()*2-1;

  for (var a = 0; a < 3; a++)
  { 
    if (!GBackLiquid(-Sin(GetR()+dir*80, 25), +Cos(GetR()+dir*80, 25)))
      Smoke(-Sin(GetR()+dir*80, 25), +Cos(GetR()+dir*80, 25), Random(10));
  }
}

/* ----- Physik ----- */

//Motor anlassen
protected func StartEngine()
{
  Sound("StartSystem.ogg", false, this());
}

//Motor l�uft
protected func EngineStarted()
{
  AddEffect("Engine",this,300,1,this,0);
  throttle = 10;
  rotation =  0;
}

//Motor abstellen/ abgestorben
protected func StopEngine()
{
  if (!fuel) PlayerMessage(GetOwner(), "<c ff0000>$MsgNoFuel$</c>", this());

  Sound("StopSystem.ogg", false, this()); 
  RemoveEffect("Engine",this());
}

//Motor aus
protected func EngineStopped()
{
  throttle = 0;
  rotation = 0;
}


/* ----- Effekt: Engine ----- */

protected func FxEngineStart(object Target, int EffectNumber, int Temp, Var1, Var2, Var3, Var4)
{
  if (Temp) return(true);
}

protected func FxEngineStop(object Target, int EffectNumber, int Reason, bool Temp)
{
  if (Temp) return(true);
}

//hier wird das Flugverhalten gesteuert
protected func FxEngineTimer(object Target, int EffectNumber, int EffectTime)
{
  //Variablen abspeichern
  var rot = LocalN("rotation", Target);
  var thr = LocalN("throttle", Target);
  var Fg, Fv, Fh, Fw, Fs, dFv, dFh, m, mh, g, av, ah;  //physikalische Kr�fte
  
  //�berpr�fung, ob �berhaupt noch ein Pilot drin...
  var has_pilot = (LocalN("Pilot", Target));

  //Rotation anpassen
  if (has_pilot)
  {
    var speed;
    speed = BoundBy(rot-GetR(Target), -rot_speed, rot_speed);
    SetRDir(speed, Target);
  }
  else
  {
    LocalN("rotation", Target) = GetR(Target);
  }
  //Treibstoff verbrauchen
  if (!(EffectTime % 10) && !ObjectCount(H_NP))
  {
    LocalN("fuel", Target) -= thr;
    if (LocalN("fuel", Target) <= 0)
      Target -> SetAction("EngineShutDown");
  }

  //Gewichtskraft berechnen
  m = GetMass(Target);
  g = GetGravity();
  Fg = (m + mh) * g;
  Fw = GetWind(0, -20, false) * 200;
 	if(GetContact(this(), -1, CNAT_Bottom)) Fw = 0;
 
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

  return(true);
}

//eine neue Funktion: Tangens = Sinus / Kosins
global func Tan(int angle, int radius)
{
  return(Sin(angle, radius, 10) / Cos(angle, radius, 10));
}
