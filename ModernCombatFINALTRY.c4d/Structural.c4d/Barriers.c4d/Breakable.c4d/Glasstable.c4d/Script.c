/*-- Glastisch --*/

#strict 2

local damaged;

public func IsMeleeTarget()		{if(!damaged) return true;}
public func IsCraneGrabable()		{return 1;}
public func MaxDamage()			{return 40;}
public func IsBulletTarget(id def)
{
  if(def->~NoDecoDamage() || damaged) return false;
  return true;
}


/* Zerstörung */

public func MeleeHit(pFrom)
{
  DoDamage(20);

  //Effekte
  CastSmoke("Smoke3",5,10,-20,0,20,150);
  CastSmoke("Smoke3",5,10,20,0,20,150);
  Sound("DoorKick.ogg",0,0,50);

  return true;
}

public func Damage()
{
  if(GetDamage() > 20 && !damaged)
    Shatter();
  if(GetDamage() > MaxDamage())
  {
    Destruct();
    return;
  }
  return 1;
}

public func Shatter()
{
  //Effekte
  CastParticles("Glas",4+Random(8),50,0,0,60,10);
  for(var i = 0 ; i < 10 ; i++)
    CastParticles("GlassSplinter",1,35,RandomX(-20,20),-5,20,20);
  Sound("GlassBreak*.ogg");
  SetAction("Broken");

  //SolidMask entfernen
  SetSolidMask();
  damaged = true;
}

public func Destruct()
{
  //Effekte
  CastSmoke("Smoke3",8,15,0,-5,250,200);
  CastParticles("MetalSplinter",3,50,-20,-5,140,70);
  CastParticles("MetalSplinter",3,50,20,-5,140,70);
  Sound("CrateDestruct*.ogg",0,0,50);

  RemoveObject();
}