/*-- Glastisch --*/

#strict 2

local damaged;

public func IsCraneGrabable()		{return 1;}
public func MaxDamage()			{return 40;}
public func IsBulletTarget(id def)
{
  if(def->~NoDecoDamage() || damaged) return false;
  return true;
}


/* Zerstörung */

public func Damage()
{
  if(GetDamage() > 30 && !damaged)
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
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastParticles("Glas", 4+Random(8), 50, 0,0, 60,10, RGBa(200,200,200), RGBa(200,200,200));
  for(var i = 0 ; i < 10 ; i++)
    CastParticles("GlassSplinter", 1, 35, RandomX(-20,20), -5, 20, 20, RGBa(255,255,255,0), RGBa(255,255,255,0));
  SetAction("Broken");
  Sound("GlassBreak*.ogg");

  //SolidMask entfernen
  SetSolidMask();
  damaged = true;
}

public func Destruct()
{
  //Effekte
  CastSmoke("Smoke3",8,15,0,-5,250,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
  CastParticles("MetalSplinter",3,50,-20,-5,140,70);
  CastParticles("MetalSplinter",3,50,20,-5,140,70);
  Sound("CrateDestruct*.ogg",0,0,50);

  RemoveObject();
}