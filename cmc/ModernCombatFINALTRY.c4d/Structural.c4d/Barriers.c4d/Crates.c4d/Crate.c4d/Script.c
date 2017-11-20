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
    //Effekte
    if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",3,20,0,0,200,200);
    CastParticles("WoodSplinter",5,50,0,0,75,100);
    CastParticles("WoodCrateSplinter",4,70,0,0,100,100);
    Sound("CrateDestruct*.ogg");
    Sound("WoodCrack*.ogg");

    //Inhalte verschleudern
    while(Contents())
      Exit(Contents(),0,0,0,RandomX(-5,5),RandomX(-5,5),RandomX(-5,5));

    //Verschwinden
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