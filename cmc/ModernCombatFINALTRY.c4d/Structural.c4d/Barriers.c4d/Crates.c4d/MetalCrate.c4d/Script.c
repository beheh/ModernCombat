/*-- Metallkiste --*/

#strict 2
#include WCR2

public func IsMeleeTarget()	{}
public func IsBulletTarget()	{}


/* Zerst�rung */

func Damage()
{
  if(GetDamage() > 90)
  {
    //Effekte
    if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter", 4, 40, 0,0, 50, 75, RGBa(255,255,255,0), RGBa(255,255,255,0));
    CastParticles("MetalCrateSplinter", 4, 50, 0,0, 141);
    Sound("CrateDestruct*.ogg");

    //Verschwinden
    RemoveObject(0, 1);
  }
}