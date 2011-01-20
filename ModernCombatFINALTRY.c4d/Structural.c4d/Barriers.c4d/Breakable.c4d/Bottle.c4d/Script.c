/*-- Flasche --*/

#strict 2


/* Schaden */

protected func Damage()
{
  //Effekte
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

/* Aufschlag */ 

protected func Hit3()
{
  Damage();
}