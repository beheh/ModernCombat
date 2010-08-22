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
	AimUp(GetUser(), 1, "ControlConf");
	return true;
}

public func ContainedRight()
{
	AimDown(GetUser(), 1, "ControlConf");
	return(true);
}  

public func ContainedThrow(object byObj)
{
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
