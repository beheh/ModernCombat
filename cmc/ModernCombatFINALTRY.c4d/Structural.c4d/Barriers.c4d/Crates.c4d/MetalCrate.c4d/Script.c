/*-- Metallkiste --*/

#strict 2
#include WCR2

public func IsMeleeTarget()	{}
public func IsBulletTarget()	{}


/* Zerstörung */

func Damage()
{
  if(GetDamage() > 90)
  {
    //Effekte
    if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",3,20,0,0,200,200);
    CastParticles("MetalSplinter",10,150,0,0,50,75);
    CastParticles("MetalCrateSplinter",4,70,0,0,100,100);
    Sound("CrateDestruct*.ogg");

    //Verschwinden
    RemoveObject(0, 1);
  }
}