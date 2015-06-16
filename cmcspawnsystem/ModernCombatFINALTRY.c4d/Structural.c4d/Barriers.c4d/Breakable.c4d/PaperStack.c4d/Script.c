/*-- Papierstapel --*/

#strict 2

public func CanBeHitByShockwaves()		{return true;}
public func BlastObjectsShockwaveCheck()	{return true;}


/* Schaden */

protected func Damage()
{
  //Effekte
  CastParticles("Paper", RandomX(20, 25), 40, 0, 0, 30, 60);
  CastParticles("Paper", 5, 60, 0, 0, 40, 60);
  Sound("PaperHit*.ogg");

  //Verschwinden
  RemoveObject();
}

public func IsBulletTarget(id def)
{
  if(def->~NoDecoDamage()) return false;
  return true;
}

public func OnShockwaveHit(iLevel,iX,iY)
{
  Damage();
}

/* Aufschlag */

protected func Hit3()
{
  Damage();
}