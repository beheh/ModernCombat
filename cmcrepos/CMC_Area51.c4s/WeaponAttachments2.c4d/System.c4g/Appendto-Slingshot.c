/*-- Slingshot-Erweiterung für Waffenupgrades --*/

#strict 2
#appendto SGST

func Initialize()
{
   iPermittedAtts = AT_ExtendedMag | AT_Bayonet | AT_Laserpointer;
   return _inherited(...);
}

func FxLaserDotTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  //Nutzer festlegen
  var user = this->~GetUser();
  var x, y, z;
  if(!user || !user->~IsClonk() || !user->WeaponAt(x, y, z) || Contents(0, user) != this || (iAttachment != AT_Laserpointer))
  {
    RemoveTrajectory(pTarget);
    return;
  }

  var iAngle = EffectVar(1, user, GetEffect("ShowWeapon", user));
  AddTrajectory(pTarget, GetX(pTarget), GetY(pTarget), Sin(iAngle,120), -Cos(iAngle,120), 35*3, RGB(0, 255, 0));
}
