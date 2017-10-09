/*-- Flasche --*/

#strict 2

public func CanBeHitByShockwaves()		{return true;}
public func BlastObjectsShockwaveCheck()	{return true;}


/* Schaden */

protected func Damage()
{
  //Effekte
  CastParticles("Glas",3+Random(3),50,0,0,60,10);
  CastParticles("GlassSplinter",5+Random(5),50,0,0,20,25);
  Sound("GlassBreak*.ogg");

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