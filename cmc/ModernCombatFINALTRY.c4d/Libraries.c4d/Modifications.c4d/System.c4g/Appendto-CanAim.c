/*-- Zielen --*/

#strict 2
#appendto L_CA

local crosshair;


public func DoMouseAiming(int iTx, int iTy, int iSpeed)
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

    var eMouseAiming = GetEffect("IntMouseAiming",this);

    //Winkel gleich? Feuer frei und Effekt löschen, weil unnötig

    if(crosshair->GetAngle() == iAngle) 
    {
      this->~FireAimWeapon();
      if(eMouseAiming)
        RemoveEffect(0,this,eMouseAiming);
    }
    //Ansonsten Winkel updaten, bzw. dorthin zielen, falls Effekt nicht existiert
    else
      if(eMouseAiming)
        EffectVar(0,this,eMouseAiming) = iAngle;
      else
        AddEffect("IntMouseAiming", this, 10, 1, this, 0, iAngle, iSpeed); 
    return true;
  }
  return false;
}

public func FxIntMouseAimingStart(object pTarget, int iEffectNumber, int iTemp, iAngle, int iSpeed)
{
  if(iTemp)
    return;

  EffectVar(0,pTarget,iEffectNumber) = iAngle;
  EffectVar(1,pTarget,iEffectNumber) = GetDir(pTarget);
  EffectVar(2, pTarget, iEffectNumber) = iSpeed;
  return 0;
}

public func FxIntMouseAimingEffect(string szNewEffect, object pTarget, int iEffectNumber)
{
  if(szNewEffect == "IntMouseAiming")
    return -3;
  return 0;
}

public func FxIntMouseAimingAdd(object pTarget, int iEffectNumber, string szNewEffectName, int iNewEffectTimer, iAngle)
{
  EffectVar(0,pTarget,iEffectNumber) = iAngle;
  return 1;
}

public func FxIntMouseAimingTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  if(!pTarget->IsAiming() || EffectVar(1,pTarget,iEffectNumber) != GetDir(pTarget))
    return -1;

  var end = EffectVar(0,pTarget,iEffectNumber);
  var cur = crosshair->GetAngle();

  var dir = +1;
  if(end < cur)
    dir = -1;

  var speed = EffectVar(2, pTarget, iEffectNumber);
  if(!speed)
    speed = this->~AimStep() / 5;

  var change = dir * Min(speed, Abs(end - cur));

  crosshair->SetAngle(cur+change);
  this->~UpdateAiming();
  EffectCall(this,LocalN("wpneffect",this),"Timer");

  if(cur+change == end)
    return -1;
}

/* Zielen-Bereitschaft */

public func ReadyToAim()
{
  if(GetProcedure(this) == "WALK") return true;
  return _inherited();
}