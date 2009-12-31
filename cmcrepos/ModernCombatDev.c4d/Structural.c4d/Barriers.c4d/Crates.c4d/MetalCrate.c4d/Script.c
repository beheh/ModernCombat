/*-- Metallkiste --*/

#strict 2
#include WCR2

public func IsBulletTarget() { return 0; }
public func IsCraneGrabable() { return 1; }
public func IsSpawnBlocker() { return true; }


/* Zerstörung */

func Damage()
{
  if(GetDamage() > 90)
  {
   CastParticles("MetalCrateSplinter", 4, 50, 0,0, 141);
   CastParticles("MetalSplinter", 4, 40, 0,0, 50, 75, RGBa(255,255,255,0), RGBa(255,255,255,0));
   Sound("CrateCrack");
   RemoveObject(0, 1);
  }
}
