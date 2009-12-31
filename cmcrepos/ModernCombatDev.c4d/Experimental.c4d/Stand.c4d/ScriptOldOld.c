/*-- Waffenständer --*/
#strict 2

local pWeapon, pUser;

/* Allgemeines */
protected func Initialize()
{
  SetController(GetOwner());	
  SetAction("Be");
  AddEffect("ShowWeapon",this,1,1,this,GetID());
  SetGraphics(0,this,GetID(),3,5,0,0,this);
	return true;
}

public func Damage()
{
  if(GetDamage() > 100)//TODO: Anpassen.
    Explode(20);
}

/* Waffenzeugs */
public func Arm(id idType)//Armed and dangerous! >:D
{
  if(!idType->~IsWeapon()) return 0;
  var wepn = CreateObject(idType,0,0,GetController());
  wepn->Enter(this);
  return wepn;
}

public func WeaponAt(&x, &y, &r)
{
  x = 0;
  y = -5000;//TODO: Werte anpassen.
  r = AimAngle()+270;
  return 1;
}

public func WeaponBegin(&x, &y)
{
  var number = GetEffect("ShowWeapon",this);
  if(!number)
    return 0;
  x = EffectVar(2, this, number)/1000;
  y = EffectVar(3, this, number)/1000;
}

public func WeaponEnd(&x, &y)
{
  var number = GetEffect("ShowWeapon",this);
  if(!number)
    return 0;
  x = EffectVar(4, this, number)/1000;
  y = EffectVar(5, this, number)/1000;
}

public func GetWeaponR()
{
  var number = GetEffect("ShowWeapon",this);
  if(!number)
    return 0;
  return EffectVar(1, this, number);
}

/* Steuerung */
/*protected func RejectCollection(object pObject)
{
  if(!pObject->~IsWeapon())//Nanu? Keine Waffe? Nee, die nehmen wir nicht.
  {
    Sound("Error", false, this);
    return true;
  }
  else
    return false;
}*/

public func SelectWeapon(object pClonk)
{
  pClonk->ShiftContents(0,0,GetID(pWeapon));
  EffectCall(pClonk,LocalN("wpneffect",pClonk),"Timer");
  pClonk->~CheckArmed();
}

public func GrabLost()
{
	Grabbed(0,true);
}

public func Grabbed(object byObj, bool grabbed)
{
  if(!grabbed && (pUser == byObj))
  {
    EndAim();
    if(GetWeapon())
    {
      pWeapon->Exit();
      if(byObj)
      {
        byObj->Collect(pWeapon);
        ScheduleCall(this,"SelectWeapon",2,0,byObj);
        EffectCall(this,GetEffect("ShowWeapon",this),"Timer");
      }
    }
  	SetUser();
    
    return ;
  }

	if(GetUser() && (pUser != byObj))
	{
		return AddCommand(byObj,"UnGrab");
	}
	else
	{
		SetUser(byObj);
		SetOwner(GetController(byObj));
	}

  if(byObj && !GetWeapon())
  {
    var wpn = byObj->Contents();//potentielle Waffe
    if(wpn)
    {
      if(wpn->~IsWeapon())
      {
        while(Contents()) Exit(Contents(),0,0);
        wpn->Enter(this);//Collect(wpn,this())
        SetWeapon(wpn);
        EffectCall(byObj,LocalN("wpneffect",byObj),"Timer");
        EffectCall(this,GetEffect("ShowWeapon",this),"Timer");
        InitAim();
      }
    }
     
    if(!GetWeapon())   
      return AddCommand(byObj,"UnGrab");//Scher dich zum Teufel du Lausbub!
  }
}

/** Zielen **/

local Crosshair;

private func InitAim(int angle)
{
	if(Crosshair)
		RemoveObject(Crosshair);
	
	Crosshair = CreateObject(HCRH); // Owner wird in Init gesetzt
	Crosshair->Init(this);
	Crosshair->SetAngle(angle);
} 

private func EndAim()
{
	if(pWeapon)
    pWeapon->PauseReload();
	AimCancel(GetUser());
	SetUser();
	if(Crosshair)
		RemoveObject(Crosshair);
}

private func MinAngle() { return 120; }

public func ControlCommand(string strCommand, object pTarget, int iTx, int iTy, object pTarget2, int iData, object pCmdObj)
{
	if(strCommand == "MoveTo")
  {
		var angle = Normalize(Angle(GetX(),GetY(),iTx,iTy),-180);
    
    if(angle > 0)
			TurnRight();
		else
			TurnLeft();
      
    angle = BoundBy(angle,-MinAngle(),+MinAngle());
		
		Crosshair->SetAngle(angle);
    UpdateAiming();
    EffectCall(this,LocalN("wpneffect",this),"Timer");
		
		// doof: In controlCommand erfahren wir nicht wer das verursacht hat... aber
    if(!GetUser())
		  SetUser(GetCursor(GetController()));
		ControlThrow(GetUser());
		return 1;
	}
}

public func ControlUp()
{
    [$TxtUp$|Image=ROCK]
	AimUp(GetUser(), 1, "ControlConf");
	return true;
}

public func ControlDown()
{
    [$TxtRight$|Image=ROCK]
	AimDown(GetUser(), 1, "ControlConf");
	return true;
}

public func ControlRightDouble()
{
	AddCommand(GetUser(),"UnGrab");
	return true;
}  

public func ControlLeftDouble()
{
	AddCommand(GetUser(),"UnGrab");
	return true;
}  

public func ControlLeft()
{
    [$TxtLeft$|Image=FLNT]
	TurnLeft();
	return true;
}

public func ControlRight()
{
    [$TxtRight$|Image=FLNT]
	TurnRight();
	return true;
}

public func TurnLeft()
{
  var user = GetUser();
  if(!user) return ;
  
  if((user->GetDir() != DIR_Left) || (GetDir() != DIR_Left))
  {
    SetDir(DIR_Left);
    user->SetDir(DIR_Left);
    var xoff = user->GetX() - GetX();
    user->SetPosition(user->GetX() - xoff, user->GetY());
    
    UpdateAiming();
  }
}

public func TurnRight()
{
  var user = GetUser();
  if(!user) return ;
  
  if((user->GetDir() != DIR_Right) || (GetDir() != DIR_Right))
  {
    SetDir(DIR_Right);
    user->SetDir(DIR_Right);
    var xoff = user->GetX() - GetX();
    user->SetPosition(GetX() - xoff, user->GetY());
    
    UpdateAiming();
  }
}

public func ControlUpdate(object clonk, int comdir, bool dig, bool throw)
{
	if(comdir == COMD_Left)
		AimUpdate(GetUser(), COMD_Up, 1, "ControlConf");
	else if(comdir == COMD_Right)
		AimUpdate(GetUser(), COMD_Down, 1, "ControlConf");
	else
		AimUpdate(GetUser(), 0, 1, "ControlConf");
    
  if(pWeapon)
    pWeapon->ControlUpdate(clonk,comdir,dig,throw);
	
	return true;
}

public func ControlConf(int conf)
{
  if(GetPlrCoreJumpAndRunControl(GetController()))
    DoAiming(conf * AIM_Step / 5);
  else
    DoAiming(conf * AIM_Step);
}

public func DoAiming(int change)
{
  if(!IsAiming())
    return;

	var angle = Crosshair->GetAngle() + change;

  // Winkel anpassen, wenn keine freie Auswahl (klassische Steuerung)
  if(!GetPlrCoreJumpAndRunControl(GetController()))
    angle = angle-angle%AIM_Step;
    
  if(angle > MinAngle() || angle < 0)
    return;

  if(GetDir() == DIR_Left)
    angle = 360-angle;
    
  Crosshair->SetAngle(angle);
  UpdateAiming();
  
  if(GetWeapon())
    EffectCall(this,GetEffect("ShowWeapon",this),"Timer");
}

public func UpdateAiming()
{
  if(!IsAiming())
    return;

  var a = Crosshair->GetAngle();

  if((GetDir() == DIR_Left && a > 0) ||
     (GetDir() == DIR_Right && a < 0))
    Crosshair->ChangeDir();
}

public func ControlThrow(object pCaller)
{
  if(GetWeapon())
  {
    SetUser(pCaller);
    pWeapon->ControlThrow(GetUser());
  }
}





/** Kontrollfuntkionen **/

protected func SetWeapon(object pNewWeapon)
{
	//AddEffect("InstallWeapon", this, 123, 300, this);
	pWeapon = pNewWeapon;
  if(pWeapon)
    pWeapon->SetUser(GetUser());
	return true;
}

public func GetWeapon()
{
  if(pWeapon)//Updaten.
    if(pWeapon->Contained() != this)
      pWeapon = 0;
  
  return pWeapon;
}

public func FxInstallWeaponTimer()
{
  SetGraphics(0, this, GetID(pWeapon), 1, GFXOV_MODE_Object);
  pWeapon->SetUser(this);
 
  pWeapon->~OnSelect();//HAHA!
  
  return -1;
}

public func GetUser()
{
  if(pUser)//Updaten.
    if((GetProcedure(pUser) != "PUSH") || (pUser->GetActionTarget() != this))
      pUser = 0;
  
  return pUser;
}

public func SetUser(object pNewUser)
{
  pUser = pNewUser;
}

/* Tags */
private func IsMachine()     {return true;}
public func AimAngle()
{
  if(!Crosshair) return 0;
  return Crosshair->GetAngle();
}
public func ReadyToFire()    {return GetUser() && GetWeapon();}
public func IsBulletTarget() {return true;}
public func IsAiming()       {return ReadyToFire();}
public func ReadyToAim()     {return ReadyToFire();}



/** BOING **/

public func CanAim()     {return true;}

protected func ControlAim(string command)
{
	if(!IsAiming()) return false;
	
	else if(command == "ControlUpdate")
		AimUpdate(this, Par(1), 1, "ControlConf");

	else if(command == "ControlUp" || command == "ControlUpDouble")
		AimUp(this, 1, "ControlConf");

	else if(command == "ControlDown")
		AimDown(this, 1, "ControlConf");
		
	else if(command == "ControlLeft")
  {
	  TurnLeft();
	}
	
	else if(command == "ControlRight")
  {
		TurnRight();
	}
	
	else if(command == "ControlLeftDouble" || command == "ControlRightDouble")
  {
		AddCommand(GetUser(),"UnGrab");
	}
	
	else if(command == "ControlCommand")
  {
		return DoMouseAiming(Par(2),Par(3));
	}

	
	return true;
}
