/*-- Ausgangslicht --*/

#strict 2
#include BLGH

public func LightSize()	{return 50;}


/* (Temporäre) Zerstörung */

func Damage()
{
  if(broken) return;
  broken = true;

  //Licht ausschalten
  if(Light())
    Light()->TurnOff();
  SetAction("Broken");

  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",5,5,0,0,40,100,RGBa(255,255,255,120),RGBa(255,255,255,150));
  CastParticles("SplinterGlass", 1, 35, RandomX(-10,10), -5, 20, 20, RGBa(255,255,255,0), RGBa(255,255,255,0));
  Sparks(7+Random(5), RGBa(255,255,150,100));
  AddLightFlash(50, 0,0, RGBa(255,255,200,100));
  Sound("GlassBreak*.ogg");
}