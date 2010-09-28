/*-- Busch --*/

#strict 2

#include TREE


/* Initialisierung */

protected func Initialize() 
{
  //Zufällige Richtung
  SetDir(Random(2));

  SetAction("Breeze");
}

/* Zerstörung */

func Damage()
{
  if(GetDamage() > 100)
  {
   //Effekte
   if(GetEffectData(EFSM_ExplosionEffects) > 0) CastParticles("Smoke3",12,10,0,0,100,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
   CastParticles("GrassBlade",10,80,0,0,200,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
   CastParticles("GrassBlade",20,100,0,0,100,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
   Sound("TreeDown*");

   //Verschwinden
   RemoveObject();
  }
}