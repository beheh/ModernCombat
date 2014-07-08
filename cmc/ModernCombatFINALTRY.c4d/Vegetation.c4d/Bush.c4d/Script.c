/*-- Busch --*/

#strict 2

#include TREE

private func TreeStrength()	{return 40;}


/* Zerstörung */

func Damage()
{
  if(GetDamage() > 100)
  {
    //Effekte
    if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",12,10,0,0,100,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
    CastParticles("GrassBlade",20,70,0,0,50,100,RGBa(255,255,255,100),RGBa(255,255,255,130));
    CastParticles("GrassBlade",10,100,0,0,100,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
    CastParticles("WoodSplinter",3,30,0,0,50,80,RGBa(255,255,255,100),RGBa(255,255,255,130));
    Sound("TreeDown*");

    //Verschwinden
    RemoveObject();
  }
}