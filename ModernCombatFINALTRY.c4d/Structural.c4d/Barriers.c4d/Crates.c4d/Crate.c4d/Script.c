/*-- Holzkiste --*/

#strict 2

public func IsMeleeTarget()	{return true;}
public func IsBulletTarget()	{return true;}
public func IsCraneGrabable()	{return 1;}
public func IsSpawnBlocker()	{return true;}


/* Zerstörung */

func Damage()
{
  if(GetDamage() > 50)
  {
    if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("WoodSplinter", 4, 40, 0,0, 50, 75, RGBa(255,255,255,0), RGBa(255,255,255,0));
    CastParticles("WoodCrateSplinter", 4, 50, 0,0, 141);
    Sound("CrateDestruct*.ogg");
    Sound("WoodCrack*.ogg");
    RemoveObject(0, 1);
  }
}

public func MeleeHit(object pByObj)
{
  if(GetID(pByObj) == RSLH)
    return false;

  //Effekte
  CastSmoke("Smoke3",5,10,-5,0,20,150);
  CastSmoke("Smoke3",5,10,5,0,20,150);
  Sound("DoorKick.ogg");

  DoDmg(17);

  return true;
}

/* Aufschlag */ 

protected func Hit3()
{
  Sound("WoodCrack3.ogg");
  DoDamage(20);
}

protected func Hit()
{
  Sound("CrateImpact*.ogg");
  return 1;
}