/*-- Warnschild --*/

#strict 2

public func IsBulletTarget()	{return(1);}


/* Zerstörung */

func Damage()
{
  if(GetDamage() > 80)
  {
   CastParticles("WoodSplinter", 8, 50, 0,0, 141);
   CastParticles("Smoke3",12,10,0,0,100,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
   Sound("CrateCrack");
   RemoveObject(0, 1);
  }
}

/* Schaden */

public func OnHit()
{
   CastParticles("WoodSplinter", 3, 50, 0,0, 141);
}