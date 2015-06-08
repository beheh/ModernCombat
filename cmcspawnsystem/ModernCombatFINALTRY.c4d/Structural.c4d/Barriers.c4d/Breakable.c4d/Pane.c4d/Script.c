/*-- Fensterscheibe --*/

#strict 2

local damaged;

public func IsMeleeTarget()			{return true;}
public func CanBeHitByShockwaves()		{return true;}
public func BlastObjectsShockwaveCheck()	{return true;}
public func MaxDamage()				{return 10;}
public func IsBulletTarget(id def)
{
  if(def->~NoDecoDamage()) return false;
  return true;
}


/* Schaden */

public func OnHit(int iDamage, int iType, object pFrom)
{
  if(GetDamage()+10>=MaxDamage())
    Shatter(BoundBy(iDamage,10,20),pFrom);
  else
    DoDamage(10);
}

public func MeleeHit(pFrom)
{
  if(GetDamage()+10>=MaxDamage())
    Shatter(BoundBy(20),pFrom);
  else
    DoDamage(10);

  return true;
}

public func OnShockwaveHit(iLevel,iX,iY)
{
  DoDamage(10);
}

/* Zerstörung */

public func Damage()
{
  if(damaged)
    return;
  if(GetDamage() > MaxDamage())
  {
    Shatter();
    return;
  }
  return 1;
}

public func Shatter(int iPower, object pFrom)
{
  if(damaged) return;
  damaged = true;

  if(!iPower) iPower = 10;
  var angle = 0;
  var spread = 360;

  if(pFrom)
  {
    angle = ObjectAngle(pFrom);
    spread = 60;
  }

  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastParticles("Glas", 2+Random(4), 50, 0,0, 60,10, RGBa(200,200,200), RGBa(200,200,200));
  for(var i = 0 ; i < GetDefHeight()/3 ; i++)
    CreateParticle("GlassSplinter", 0, i*3-GetDefHeight()/2, +Sin(angle+(Random(spread)-spread/2),iPower), -Cos(angle+(Random(spread)-spread/2),iPower), 20);
  Sound("GlassBreak*.ogg");

  RemoveObject();
}