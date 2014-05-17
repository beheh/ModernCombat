/*-- Slingshot-Erweiterung für Waffenupgrades --*/

#strict 2
#appendto SGST


func PermittedAtts()
{
  return AT_ExtendedMag | AT_Bayonet | AT_Laserpointer | AT_Flashlight;
}

public func GetMCData(int data)
{
  if(data == MC_Damage)
    return _inherited(data) + (iAttachment == AT_Bayonet)*8;	//Schaden eines Kolbenschlages
  if(data == MC_Recharge)
    return _inherited(data) + (iAttachment == AT_Bayonet)*7;	//Zeit nach Kolbenschlag bis erneut geschlagen oder gefeuert werden kann

  return _inherited(data);
}

public func FMData1(int data)
{
  if(data == FM_AmmoLoad)	return _inherited(data) + (iAttachment == AT_ExtendedMag)*2;	//Magazingröße
  if(data == FM_Reload)	return 210 + (iAttachment == AT_ExtendedMag) * 52;

  if(data == FM_SpreadAdd)	return 200 - (iAttachment == AT_Laserpointer)*10;	//Bei jedem Schuss hinzuzuaddierende Streuung
  if(data == FM_StartSpread)	return 80 - (iAttachment == AT_Laserpointer)*20;	//Bei Auswahl der Waffe gesetzte Streuung
  if(data == FM_MaxSpread)	return 400 - (iAttachment == AT_Laserpointer)*150;	//Maximaler Streuungswert

  return _inherited(data);
}

func FxLaserDotTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  //Nutzer festlegen
  var user = this->~GetUser();
  var x, y, z;
  if(!user || !user->~IsClonk() || !user->WeaponAt(x, y, z) || !user->IsAiming() || Contents(0, user) != this || iAttachment != AT_Laserpointer)
  {
    RemoveTrajectory(pTarget);
    return;
  }

  var iAngle = EffectVar(1, user, GetEffect("ShowWeapon", user));
  var empty = IsReloading() || !GetCharge();
  AddTrajectory(pTarget, GetX(pTarget), GetY(pTarget), Sin(iAngle, 120), -Cos(iAngle, 120), 35*3, RGB(255*empty, 255*(!empty), 0));
}
