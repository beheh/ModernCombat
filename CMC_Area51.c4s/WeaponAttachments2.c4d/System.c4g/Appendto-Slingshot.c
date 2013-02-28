/*-- Granatwerfer-Erweiterung für Waffenupgrades --*/

#strict 2
#appendto SGST

func PermittedAtts()
{
  return AT_ExtendedMag | AT_Bayonet | AT_Laserpointer;
}

public func GetMCData(int data)
{
  if(data == MC_Damage)		
    return _inherited(data) + (iAttachment == AT_Bayonet)*6;	//Schaden eines Kolbenschlages
  
  return _inherited(data);
}

func FxLaserDotTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  for(var i = 0; i < 10; i++)
  {
  //Nutzer festlegen
  var user = this->~GetUser();
  var x, y, z;
  if(!user || !user->~IsClonk() || !user->WeaponAt(x, y, z) || !user->IsAiming() || Contents(0, user) != this || (iAttachment != AT_Laserpointer))
  {
    RemoveTrajectory(pTarget);
    return;
  }

  var iAngle = EffectVar(1, user, GetEffect("ShowWeapon", user));
  var empty = IsReloading() || !GetCharge();
  AddTrajectory(pTarget, GetX(pTarget), GetY(pTarget), Sin(iAngle,120), -Cos(iAngle,120), 35*3, RGB(255*(empty), 255*(!empty), 0));
  }
}

public func FMData1(int data)
{
  if(data == FM_AmmoLoad)	return _inherited(data) + (iAttachment == AT_ExtendedMag)*2;	//Magazingröße

  return _inherited(data);
}