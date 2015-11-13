/*-- Labortisch --*/

#strict 2
#include GTBL

public func IsMeleeTarget()		{return false;}
public func MaxDamage()			{return 60;}
public func IsBulletTarget(id def)	{return false;}


/* Zerstörung */

public func Damage()
{
  if(GetDamage() > MaxDamage())
  {
    Destruct();
    return;
  }
  return 1;
}

public func Destruct()
{
  //Effekte
  CastSmoke("Smoke3",8,15,0,-5,250,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
  CastParticles("MetalSplinter",3,50,-20,-5,140,70);
  CastParticles("MetalSplinter",3,70,0,-5,140,70);
  CastParticles("MetalSplinter",3,50,20,-5,140,70);
  Sound("CrateDestruct*.ogg",0,0,50);

  RemoveObject();
}