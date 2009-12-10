/*-- Holziste --*/

#strict

public func IsBulletTarget() { return(1); }
public func IsCraneGrabable() { return(1); }
public func CanBeLaserRecycled() { return(2); }
public func IsSpawnBlocker() { return(true); }


/* Zerstörung */

func Damage()
{
  if(GetDamage() > 50)
  {
   CastParticles("WoodCrateSplinter", 4, 50, 0,0, 141);
   CastParticles("WoodSplinter", 4, 40, 0,0, 50, 75, RGBa(255,255,255,0), RGBa(255,255,255,0));
   Sound("CrateCrack");
   RemoveObject(0, 1);
  }
}

/* Aufschlag */ 
  
protected func Hit()
{
  Sound("CrateImpact*.ogg");
  return(1);
}