/*-- Maststück --*/

#strict 2


/* Initalisierung */

protected func Initialize()
{
  SetAction("Be");
}

protected func Hit()
{
  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",4,150,0,0,80,100);
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",8,230,0,0,40,15,RGB(40,20,20));
  CastSmoke("Smoke3",80,25,0,0,200,250, RGBa(255,255,255,0));
  Sound("TowerPartHit*.ogg");

  //Kategorie ändern
  SetCategory(C4D_StaticBack);
}