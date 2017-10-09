/*-- Papierstapel --*/

#strict 2

public func CanBeHitByShockwaves()		{return true;}
public func BlastObjectsShockwaveCheck()	{return true;}


/* Schaden */

protected func Damage()
{
  //Effekte
  CastParticles("Paper",RandomX(20, 25),40,0,0,30,60);
  Sound("PaperHit*.ogg");

  //Verschwinden
  RemoveObject();
}

public func IsBulletTarget(id def)
{
  if(def->~NoDecoDamage()) return false;
  return true;
}

public func Incineration()
{
  Extinguish();
  Damage();
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