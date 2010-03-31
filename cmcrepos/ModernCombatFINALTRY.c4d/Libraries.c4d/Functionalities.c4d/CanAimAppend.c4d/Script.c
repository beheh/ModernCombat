/*-- Zielen --*/

#strict 2
#appendto L_CA

//Callback: CanAim
//Callback: ControlAim

/*
protected func ControlAim(string command)
{
  if(Contained())
  {
    if(Contained()->~CanAim())
      return Contained()->ControlAim(command,this,...);
  }
  
  if(GetProcedure() == "PUSH")
  {
    if(GetActionTarget())
      if(GetActionTarget()->~CanAim())
        return GetActionTarget()->~ControlAim(command,this,...);
  }
  
  return _inherited(command,...);
}

public func IsAiming()
{ 
  if(GetProcedure() == "PUSH")
  {
    if(GetActionTarget())
      //if(GetActionTarget()->~CanAim())
        return GetActionTarget()->~IsAiming();
  }

	return _inherited();
}*/

local crosshair;

public func DoMouseAiming(int iTx, int iTy)
{
	if(!Contained())
  {
		var iAngle = Normalize(Angle(GetX(),GetY(),iTx,iTy),-180);

		if(iAngle > 0)
			SetDir(DIR_Right);
		else
			SetDir(DIR_Left);

		//Winkel wird zu groß?
		iAngle = BoundBy(iAngle,-this->~AimMax(),+this->~AimMax());
    
    //Henry empfiehlt das nicht, aber gewisse Leute wollen das ja unbedingt. -,-
    //Brauchen wir das wirklich? Macht ja FireAimWeapon schon den Check
    //So funktioniert jetzt auch das Dauerfeuer-Abbrechen
    //if(!(Contents()->~IsRecharging()) && !(Contents()->~IsShooting()))
    //{
      var old = crosshair->GetAngle();
      if((Abs(AngleOffset4K(old,iAngle)) < 5)/* || GetEffect("IntMouseAiming",this)*/)
        this->~FireAimWeapon();
    //}
    
    AddEffect("IntMouseAiming",this,10,1,this,0,iAngle);
    
		return true;
	}
	return false;
}

public func FxIntMouseAimingStart(object pTarget, int iEffectNumber, int iTemp, iAngle)
{
  EffectVar(0,pTarget,iEffectNumber) = iAngle;
  return 0;
}

public func FxIntMouseAimingEffect(string szNewEffect, object pTarget, int iEffectNumber)
{
  if(szNewEffect == "IntMouseAiming") return -3;
  return 0;
}

public func FxIntMouseAimingAdd(object pTarget, int iEffectNumber, string szNewEffectName, int iNewEffectTimer, iAngle)
{
  EffectVar(0,pTarget,iEffectNumber) = iAngle;
  return 1;
}

public func FxIntMouseAimingTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  if(!pTarget->IsAiming()) return -1;

  var end = EffectVar(0,pTarget,iEffectNumber);
  var cur = crosshair->GetAngle();
  
  var dir = +1;
  if(end < cur)
    dir = -1;
  
  var change = dir * Min(1 * this->~AimStep() / 5, Abs(end - cur));

  crosshair->SetAngle(cur+change);
  this->~UpdateAiming();
  EffectCall(this,LocalN("wpneffect",this),"Timer");

  if(cur+change == end)
    return -1;
}
