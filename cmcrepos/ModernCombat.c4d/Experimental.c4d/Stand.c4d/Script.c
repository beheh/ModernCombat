/*-- Waffenständer --*/
#strict

local pWeapon, pUser;

/* Allgemeines */
protected func Initialize()
{
  SetController(GetOwner());	
  SetAction("Be");
  AddEffect("ShowWeapon",this(),1,1,this(),GetID());
  SetGraphics(0,this(),GetID(),3,5,0,0,this());
	return(true);
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
  wepn->Enter(this());
  return(wepn);
}

public func WeaponAt(&x, &y, &r)
{
  x = 0;
  y = -5000;//TODO: Werte anpassen.
  if(Crosshair)
    r = Crosshair->GetAngle()+270;
  return(1);
}

public func WeaponBegin(&x, &y)
{
  var number = GetEffect("ShowWeapon",this());
  if(!number)
    return(0);
  x = EffectVar(2, this(), number)/1000;
  y = EffectVar(3, this(), number)/1000;
}

public func WeaponEnd(&x, &y)
{
  var number = GetEffect("ShowWeapon",this());
  if(!number)
    return(0);
  x = EffectVar(4, this(), number)/1000;
  y = EffectVar(5, this(), number)/1000;
}

public func GetWeaponR()
{
  var number = GetEffect("ShowWeapon",this());
  if(!number)
    return(0);
  return(EffectVar(1, this(), number));
}

/* Steuerung */
/*protected func RejectCollection(object pObject)
{
  if(!pObject->~IsWeapon())//Nanu? Keine Waffe? Nee, die nehmen wir nicht.
  {
    Sound("Error", false, this());
    return(true);
  }
  else
    return(false);
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
        pWeapon->~AimStop();
        byObj->Collect(pWeapon);
        ScheduleCall(this(),"SelectWeapon",2,0,byObj);
        EffectCall(this(),GetEffect("ShowWeapon",this()),"Timer");
      }
    }
  	SetUser();
    
    return();
  }

	if(GetUser() && (pUser != byObj))
	{
		return(AddCommand(byObj,"UnGrab"));
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
        wpn->Enter(this());//Collect(wpn,this())
        SetWeapon(wpn);
        EffectCall(byObj,LocalN("wpneffect",byObj),"Timer");
        EffectCall(this(),GetEffect("ShowWeapon",this()),"Timer");
        pWeapon->~AimStart();
        InitAim();
      }
    }
     
    if(!GetWeapon())   
      return(AddCommand(byObj,"UnGrab"));//Scher dich zum Teufel du Lausbub!
  }
}

/** Zielen **/

local Crosshair;

private func InitAim(int angle)
{
	if(Crosshair)
		RemoveObject(Crosshair);
	
	Crosshair = CreateObject(HCRH); // Owner wird in Init gesetzt
	Crosshair->Init(this());
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

private func AimMax() { return(90); }

public func DoMouseAiming(int iTx, int iTy)
{
	if(!Contained())
  {
		var iAngle = Normalize(Angle(GetX(),GetY(),iTx,iTy),-180);

		if(iAngle > 0)
			TurnRight();
		else
			TurnLeft();

		//Winkel wird zu groß?
		iAngle = BoundBy(iAngle,-AimMax(),+AimMax());

		Crosshair->SetAngle(iAngle);
		UpdateAiming();
		//Wichtig: Waffe updaten
		EffectCall(this,LocalN("wpneffect",this),"Timer");

		// Fertig, Feuern!
		ControlThrow(GetUser());
		return true;
	}
	return false;
}

public func TurnLeft()
{
  var user = GetUser();
  if(!user) return();
  
  if((user->GetDir() != DIR_Left) || (GetDir() != DIR_Left))
  {
    SetDir(DIR_Left);
    user->SetDir(DIR_Left);
    var xoff = user->GetX() - GetX();
    user->SetPosition(GetX() - xoff, user->GetY());
    
    UpdateAiming();
  }
}

public func TurnRight()
{
  var user = GetUser();
  if(!user) return();
  
  if((user->GetDir() != DIR_Right) || (GetDir() != DIR_Right))
  {
    SetDir(DIR_Right);
    user->SetDir(DIR_Right);
    var xoff = user->GetX() - GetX();
    user->SetPosition(GetX() - xoff, user->GetY());
    
    UpdateAiming();
  }
}

public func ControlConf(int conf)
{
  if(GetPlrCoreJumpAndRunControl(GetController()))
    DoAiming(conf * AIM_Step / 5);
  else
    DoAiming(conf * AIM_Step);
}

public func DoAiming(int iChange)
{
  //zielen wir überhaupt?
  if(!IsAiming())
    return;

  var angle = Abs(Crosshair->GetAngle()) + iChange;

  // Winkel anpassen, wenn keine freie Auswahl (klassische Steuerung)
  if(!GetPlrCoreJumpAndRunControl(GetController()))
    angle = angle-angle%10;

  // Winkel wird zu groß?
  if(angle > AimMax() || angle < 0)
    return;

  if(GetDir() == DIR_Left)
    angle = 360-angle;
  Crosshair->SetAngle(angle);

  UpdateAiming();
}

public func UpdateAiming()
{
  if(!IsAiming())
    return;

  var a = Crosshair->GetAngle();

  if((GetDir() == DIR_Left && a > 0) ||
     (GetDir() == DIR_Right && a < 0))
    Crosshair->ChangeDir();
    
  if(GetWeapon())
    EffectCall(this(),GetEffect("ShowWeapon",this()),"Timer");
}

public func ControlThrow(object pCaller)
{
  if(GetWeapon())
  {
    SetUser(pCaller);
    GetWeapon()->ControlThrow(GetUser());
  }
}





/** Kontrollfuntkionen **/

protected func SetWeapon(object pNewWeapon)
{
	//AddEffect("InstallWeapon", this, 123, 300, this);
	pWeapon = pNewWeapon;
  if(pWeapon)
    pWeapon->SetUser(GetUser());
	return(true);
}

public func GetWeapon()
{
  if(pWeapon)//Updaten.
    if(pWeapon->Contained() != this())
      pWeapon = 0;
  
  return(pWeapon);
}

public func FxInstallWeaponTimer()
{
  SetGraphics(0, this, GetID(pWeapon), 1, GFXOV_MODE_Object);
  pWeapon->SetUser(this());
 
  pWeapon->~OnSelect();//HAHA!
  
  return(-1);
}

public func GetUser()
{
  if(pUser)//Updaten.
    if((GetProcedure(pUser) ne "PUSH") || (pUser->GetActionTarget() != this()))
      pUser = 0;
  
  return(pUser);
}

public func SetUser(object pNewUser)
{
  pUser = pNewUser;
}

public func AimAngle(int iMaxAngle, int iRange)
{
  if(!IsAiming()) return;

	var angle = Crosshair->GetAngle();
	var x,y,r;
	this->~WeaponAt(x,y,r);
  
  if(iMaxAngle)
  {
		var target = this->~GetTarget(angle,iMaxAngle,iRange);
		if(target)
			angle = Angle(GetX(),GetY(),GetX(target),GetY(target));
	}

	return(angle);
}

/* Tags */
private func IsMachine()     {return true;}
public func ReadyToFire()    {return(GetUser() && GetWeapon());}
public func IsBulletTarget() {return(true);}
public func IsAiming()       {return(ReadyToFire() && Crosshair);}
public func ReadyToAim()     {return(ReadyToFire());}



/** BOING **/

public func CanAim()     {return(true);}

private func ChangeWeapon(object pTarget)
{
	if(!pTarget->~IsWeapon())
  {
    GrabLost();
		AddCommand(GetUser(),"UnGrab");
	}
}

protected func ControlAim(string command, object clonk)
{
	if(!IsAiming()) return false;
	
	else if(command == "ControlUpdate")
		AimUpdate(this(), Par(2), 1, "ControlConf");

	else if(command == "ControlUp" || command == "ControlUpDouble")
		AimUp(this(), 1, "ControlConf");

	else if(command == "ControlDown")
		AimDown(this(), 1, "ControlConf");
		
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
		return DoMouseAiming(Par(3),Par(4));
	}
  
	else if(command == "ControlContents")
  {

		var pTarget = clonk->FindContents(Par(2));
		ChangeWeapon(pTarget);
		return false;

	}

	
	return true;
}