/*-- Trümmer --*/

#strict 2
#include ATRP


/* Initalisierung */

func Initialize()
{
  //Zufälliges Aussehen
  SetAction("Be");
  SetPhase(RandomX(0,4));

  //Effekte
  AddFireEffect(this,150,RGB(50,50,50),true,30);
  AddEffect("DmgObjects", this, 101, 5, this);
}

/* Aufschlag */

public func Hit(int iXDir, int iYDir)
{
  //Spielerbildschirme schütteln
  ShakeViewPort(30, this);

  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("GunSmoke",4,10,0,0,500,100,RGBa(255,255,255,0));
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter", 8, 80, 0, 0, 20, 50);
  CastSmoke("Smoke",4,10,0,0,500,100,RGBa(255,255,255,0));
  Sound("StructureHit*.ogg");

  return true;
}