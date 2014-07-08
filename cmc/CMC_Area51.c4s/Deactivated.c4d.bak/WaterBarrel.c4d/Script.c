/*-- Wasserfass --*/

#strict 2
#include XBRL


/* Zerstörung */

func Damage(int iChange)
{
  if(!this)
    return;
  if(damaged)
    return;

  if(GetDamage() < 20) return;
  InstaExplode();
}

func InstaExplode()
{
  //Effekte
  CastParticles("MetalSplinter",6,80,0,0,20,70,0,0));
  CastPXS("Rain", 200, 50);

  //Sound
  Sound("Splash1");

  SetAction("Wreck");
  Extinguish();
  SetRDir(RandomX(-40,+40));

  //Langsam verschwinden
  FadeOut();
}