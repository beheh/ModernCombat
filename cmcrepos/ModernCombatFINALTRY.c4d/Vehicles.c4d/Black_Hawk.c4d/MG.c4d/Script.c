/* Heli-MG */

#strict 2
#include WEPN

local heli, vis;
local Crosshair;

protected func Initialize()
{
  vis = true;
  //am Anfang sitzt bestimmt noch keiner drin
  SetAction("Empty");
  SetPhase(GetActMapVal("Length",GetAction())/3); // Nach unten Schaun
}

public func Set(object pObj)
{
  heli = pObj;
}

protected func TimerCall()
{
  //ohne Heli fühlen wir uns einsam...
  if (!heli) return RemoveObject();
  //nur sichtbar, wenn nicht drehend
  if (GetAction(heli) == "Turn")
  {
    if (vis)
    {
      SetVisibility(VIS_None);
      vis = false;
    }
  }
  else
    if (!vis)
    {
      SetVisibility(VIS_All);
      vis = true;
    }
  //schauen, ob MG überhaupt besetzt
  if (LocalN("Gunner", heli) && GetAction() != "Occupied")
  {
    SetAction("Occupied");
    SetOwner(GetOwner(LocalN("Gunner", heli)), this());
  }
  if (!LocalN("Gunner", heli) && GetAction() != "Empty")
  {
    SetAction("Empty");
    SetOwner(-1, this());
  }
  //Drehung des Heli abfragen
  var rot = GetR(heli)+(GetDir(heli)*2-1)*90+(GetDir(heli)*2-1)*30;
  //und in die Positionsbestimmung einfließen lassen
  SetPosition(GetX(heli) + Sin(rot, 24),
              GetY(heli) - Cos(rot, 24), this());
  SetXDir(GetXDir(heli));
  SetYDir(GetYDir(heli));
  SetR(GetR(heli));
  SetRDir(GetRDir(heli));
  return(true);
}


/** Controllerzeugs **/

public func Collection2(object Obj)
{
  SetUser(Obj);
	SetOwner(GetController(Obj));
	
	InitAim();
}

public func Ejection(object Obj)
{
	EndAim();
}


/* Zielzeug */

private func InitAim()
{
	if(Crosshair)
		RemoveObject(Crosshair);
	
	Crosshair = CreateObject(HCRH); // Owner wird in Init gesetzt
	Crosshair->Init(this());
	Crosshair->SetAngle(PtoR());
} 

private func EndAim()
{
	PauseReload();
	AimCancel(GetUser());
	SetUser();
	if(Crosshair)
		RemoveObject(Crosshair);
}

public func IsAiming() { return true; }

// Winkel -> Phase
private func RtoP(int angle)
{
	var phases = GetActMapVal("Length",GetAction());

	// Maximale Drehung nach oben
	angle = BoundBy(angle,90,270);
	// 0 bis 180
	angle -= 90;
	
	// umrechnen von 0..2*StartAngle() nach 0..phases-1
	var p = angle/3;
	
	return p;
}

// Phase -> Winkel
private func PtoR()
{
	var phases = GetActMapVal("Length",GetAction());
	
	var a = GetPhase()*3;
	
	a += 90;
	
	return a;
}

/** Zielen **/

public func ControlCommand(string strCommand, object pTarget, int iTx, int iTy, object pTarget2, int iData, object pCmdObj)
{
	if(strCommand == "MoveTo") {
		var angle = Normalize(Angle(GetX(),GetY(),iTx,iTy),-180);
		// max angle
		angle = BoundBy(angle,90,270);
		
		Crosshair->SetAngle(angle);
		SetPhase(RtoP(angle));
		
		// doof: In controlCommand erfahren wir nicht wer das verursacht hat... aber
		var user = GetCursor(GetController());
		ControlThrow(user);
		return 1;
	}
}

public func ContainedLeft()
{
    [$TxtLeft$]
	AimUp(GetUser(), 1, "ControlConf");
	return true;
}

public func ContainedRight()
{
    [$TxtRight$]
	AimDown(GetUser(), 1, "ControlConf");
	return(true);
}  

public func ContainedThrow(object byObj)
{
    [$TxtFire$]
  ControlThrow(byObj);
}

public func ControlUpdate(object clonk, int comdir, bool dig, bool throw)
{
	if(comdir == COMD_Left)
		AimUpdate(GetUser(), COMD_Up, 1, "ControlConf");
	else if(comdir == COMD_Right)
		AimUpdate(GetUser(), COMD_Down, 1, "ControlConf");
	else
		AimUpdate(GetUser(), 0, 1, "ControlConf");
	
	inherited(clonk, comdir, dig, throw);
	
	return(true);
}

public func ControlConf(int conf)
{
  // Feineres Zielen mit JnR-Steuerung möglich
  if(GetPlrCoreJumpAndRunControl(GetController()))
    DoAiming(conf * AIM_Step / 5);
  else
    DoAiming(conf * AIM_Step);
}

public func DoAiming(int change)
{
	var angle = Crosshair->GetAngle() + change;

  // Winkel anpassen, wenn keine freie Auswahl (klassische Steuerung)
  if(!GetPlrCoreJumpAndRunControl(GetController()))
    angle = angle-angle%AIM_Step;

  // Winkel wird zu groß?
  if(Abs(angle) > 90)
    return;

  Crosshair->SetAngle(angle);

	//UpdatePhase();
	SetPhase(RtoP(angle));
}


/** Waffenzeugs **/

public func FMData1(int data)
{
	if(data == FM_Name)			return("$Auto$");
	if(data == FM_AmmoID)		return(STAM);

	if(data == FM_AmmoLoad)		return(100);
	if(data == FM_AmmoUsage)	return(1);

	if(data == FM_Reload)		return(80);
	if(data == FM_Recharge)		return(3);

	if(data == FM_Damage)		return(8);
	if(data == FM_Auto)			return(true);

	return(Default(data));
}

public func Fire1()
{
  var user = GetUser();
  var angle = Crosshair -> GetR();
  var x = Sin(angle, 10);
  var y = Sin(angle, 10);
  var ammo = SALaunchBullet(x,y,GetController(user),angle+RandomX(-2,+2),350,1000,GetFMData(FM_Damage));
  ammo->Sound("MNGN_Fire.ogg");

  // Effekte
  SAMuzzleFlash(RandomX(35,50),user,x,y,angle);
  SABulletCasing(x/3,y/3,-1*14*(Random(1)+1),-(13+Random(2)),5);
}

public func NoWeaponChoice() { return(1); }


/* ----- Platzwechsel ----- */

//Platz wechseln
protected func ContainedDigDouble(object ByObj)
{
  [$CtrlDigD$]
  CreateMenu(GetID(),ByObj,this(),0,"$Seats$",0,1);
    //Ausstieg
    //Pilot
    if(LocalN("Pilot",heli))
      AddMenuItem("<c ff8888>Pilot</c>", "SeatOccupied()",GetID(),ByObj,0,ByObj,"$SeatOccupied$");
    else
      AddMenuItem("<c 88ff88>Pilot</c>", "EnterSeat1",GetID(),ByObj,0,ByObj,"$PilotSeat$");
    //MG-Schütze
    if(LocalN("Gunner",heli))
      AddMenuItem("<c ff8888>$Gunner$</c>", "SeatOccupied()",GetID(),ByObj,0,ByObj,"$SeatOccupied$");
    else
      AddMenuItem("<c 88ff88>$Gunner$</c>", "EnterSeat2",GetID(),ByObj,0,ByObj,"$GunnerSeat$");
    //Raketen-Schütze
    if(LocalN("Rocketeer",heli))
      AddMenuItem("<c ff8888>$Rocketeer$</c>", "SeatOccupied()",GetID(),ByObj,0,ByObj,"$SeatOccupied$");
    else
      AddMenuItem("<c 88ff88>$Rocketeer$</c>", "EnterSeat3",GetID(),ByObj,0,ByObj,"$RocketeerSeat$");
    //Passagier 1
    if(LocalN("Passenger1",heli))
      AddMenuItem("<c ff8888>$Passenger1$</c>", "SeatOccupied()",GetID(),ByObj,0,ByObj,"$SeatOccupied$");
    else
      AddMenuItem("<c 88ff88>$Passenger1$</c>", "EnterSeat4",GetID(),ByObj,0,ByObj,"$PassengerSeat$");
    //Passagier 2
    if(LocalN("Passenger2",heli))
      AddMenuItem("<c ff8888>$Passenger2$</c>", "SeatOccupied()",GetID(),ByObj,0,ByObj,"$SeatOccupied$");
    else
      AddMenuItem("<c 88ff88>$Passenger2$</c>", "EnterSeat5",GetID(),ByObj,0,ByObj,"$PassengerSeat$");
      
  return 1;
}

//Sitz belegen
public func EnterSeat1(a, object Obj)
{
  heli -> EnterSeat1(a, Obj);

  return 1;
}

public func EnterSeat2(a, object Obj)
{
  heli -> EnterSeat2(a, Obj);

  return 1;
}

public func EnterSeat3(a, object Obj)
{
  heli -> EnterSeat3(a, Obj);

  return 1;
}

public func EnterSeat4(a, object Obj)
{
  heli -> EnterSeat4(a, Obj);

  return 1;
}

public func EnterSeat5(a, object Obj)
{
  heli -> EnterSeat5(a, Obj);

  return 1;
}
