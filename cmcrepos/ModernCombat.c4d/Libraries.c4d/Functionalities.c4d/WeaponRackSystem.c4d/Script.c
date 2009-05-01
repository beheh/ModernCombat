/*-- WeaponRack-System --*/
/*
  Hiermit können von Clonks Waffen an Fahrzeugen etc. angebracht werden.
  Funktionen die überladen werden sollten sind gekennzeichnet. (Später sollte das mal hier stehen.)
*/

#strict 2

public func Initialize()
{
  AddEffect("ShowWeapon",this,1,1,this,GetID());
}

public func UpdateWpnEffect()
{
  EffectCall(this,GetEffect("ShowWeapon",this),"Timer");//Effekt updaten.
}

/* Callbacks */
public func ReadyToFire() { return true; }
public func ReadyToAim() { return true; }
public func AimOverride() { return true; }

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

//-> Bitte überladen !
public func WeaponAt(&x, &y, &r)
{
  x = 0;
  y = 0;
  r = 0;
}


/* Benutzer */
local user;

public func GetUser()
{
  return user;
}

public func SetUser(object pUser)
{
  user = pUser;
  SetOwner(GetOwner(pUser));
}


/* Waffe */
local weapon;

public func GetWeapon()
{
  return weapon;
}

public func Arm(object pWeapon)
{
  if(!pWeapon || (pWeapon == weapon)) return false;

  Disarm();
  pWeapon->Enter(this);
  weapon = pWeapon;
  UpdateWpnEffect();
  
  //...
  return true;
}

public func Disarm()
{
  if(!weapon) return false;
  
  StopAiming();
  weapon->Exit();
  weapon = 0;
  UpdateWpnEffect();

  return true;
}

public func Fire()
{
  if(!weapon) return false;

  if(weapon->IsReloading()) return false;
  
  if(!weapon->GetCharge())//Waffe leer? :o
    return weapon->ControlThrow(GetUser());
  else
    return weapon->ControlThrow(this);
}

public func WeaponMenu()
{
  var wp = GetWeapon();
  if(!weapon || !GetUser()) return false;
  
  weapon->SetUser(user);
  return weapon->ControlDigDouble(user);
}


/* Zielen */
local crosshair;

public func GetCrosshair()
{
  return crosshair;
}

public func StartAiming()
{
  if(!weapon) return false;
  weapon->~AimStart();
  
  if(!crosshair)
  {
    crosshair = CreateObject(HCRH,0,0,GetOwner(user));
    if(!crosshair)
      return false;
  }
  crosshair->Init(this);
  crosshair->SetAngle(90);//TODO: ?

  return true;
}

public func StopAiming()
{
  if(weapon)
    weapon->~AimStop();

  if(crosshair)
  {
    //crosshair->UnSet();
    RemoveObject(crosshair);
  }
  
  return true;
}

public func IsAiming() { return crosshair; }

public func AimAngle(int iAim, int iRange)
{
	var angle;
	var x,y,r;
	this->~WeaponAt(x,y,r);
  
  if(!IsAiming())
		angle = (90+r)*(Direction()*2-1);
	else
		angle = crosshair->GetAngle();
    
  angle += GetR();
  
  if(iAim)
  {
		var target = this->~GetTarget(angle,iAim,iRange);
		if(target)
			angle = Angle(GetX(),GetY(),GetX(target),GetY(target));
	}

	return(angle);
}

public func ControlConf(int conf)
{
  if(GetPlrCoreJumpAndRunControl(GetController(user)))
    DoAiming(conf * AIM_Step / 5);
  else
    DoAiming(conf * AIM_Step);
}

public func SetAiming(int iAngle)
{
  if(!IsAiming())
    return;
    
  // Winkel anpassen, wenn keine freie Auswahl (klassische Steuerung)
  if(!GetPlrCoreJumpAndRunControl(GetController(user)))
    iAngle = iAngle-iAngle%10;
    
  iAngle = CorrectAimAngle(iAngle);
  crosshair->SetAngle(iAngle);
  UpdateAiming();
}

public func DoMouseAiming(int iTx, int iTy)
{
  SetAiming(Normalize(Angle(GetX(),GetY(),iTx,iTy),-180));
  return Fire();
}

public func DoAiming(int iChange)
{
  return SetAiming(Abs(crosshair->GetAngle()) + iChange);
}

public func UpdateAiming()
{
  if(!IsAiming())
    return;

  var a = crosshair->GetAngle();

  if((Direction() == DIR_Left && a > 0) ||
     (Direction() == DIR_Right && a < 0))
    crosshair->ChangeDir();
  
  UpdateWpnEffect();
}

//-> Bitte überladen !
public func CorrectAimAngle(int iAngle)
{
  return iAngle;
}

public func TurnLeft()
{
  OnTurnLeft();
  UpdateAiming();
}

//-> Bitte überladen !
public func OnTurnLeft()
{
  SetDir(DIR_Left);
}

public func TurnRight()
{
  OnTurnRight();
  UpdateAiming();
}

//-> Bitte überladen !
public func OnTurnRight()
{
  SetDir(DIR_Right);
}

//-> Bitte überladen !
public func Direction()
{
  return GetDir();
}