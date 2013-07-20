/*-- Tr�mmer --*/

#strict 2
#include ATRP


/* Initalisierung */

func Initialize()
{
  //Zuf�lliges Aussehen
  SetAction("Be");
  SetPhase(RandomX(0,4));

  AddEffect("DmgObjects", this, 101, 5, this);
}

/* Aufschlag */

public func Hit(int iXDir, int iYDir)
{
  Sound("StructureHit*.ogg");

  //Spielerbildschirme sch�tteln
  ShakeViewPort(30, this);

  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("GunSmoke",4,10,0,0,500,100,RGBa(255,255,255,0));
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter", 8, 80, 0, 0, 20, 50);
  CastSmoke("Smoke",4,10,0,0,500,100,RGBa(255,255,255,0));

  return true;
}