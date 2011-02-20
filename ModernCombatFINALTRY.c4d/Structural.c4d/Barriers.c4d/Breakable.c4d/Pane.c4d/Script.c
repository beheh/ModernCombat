/*-- Fensterscheibe --*/

#strict 2

public func IsMeleeTarget()	{return true;}


/* Zerstörung */

public func Damage()
{
  if(GetDamage() > 10)
  {
    Shatter();
    return;
  }
  return 1;
}

public func OnHit(int iDamage, int iType, object pFrom)
{
  Shatter(BoundBy(iDamage,10,20),pFrom);
}

public func MeleeHit(pFrom)
{
  Shatter(BoundBy(10),pFrom);
}

public func Shatter(int iPower, object pFrom)
{
  if(!iPower) iPower = 10;
  var angle = 0;
  var spread = 360;

  if(pFrom)
  {
    angle = ObjectAngle(pFrom);
    spread = 60;
  }

  //Zersplittern
  Sound("GlassBreak*.ogg");
  for(var i = 0 ; i < GetDefHeight()/3 ; i++)
    CreateParticle ("GlassSplinter", 0, i*3-GetDefHeight()/2, +Sin(angle+(Random(spread)-spread/2),iPower), -Cos(angle+(Random(spread)-spread/2),iPower), 20);

  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastParticles("Glas", 2+Random(4), 50, 0,0, 60,10, RGBa(200,200,200), RGBa(200,200,200));
  RemoveObject();
}

public func IsBulletTarget(id def)
{
  if(def->~NoDecoDamage()) return false;
  return true;
}