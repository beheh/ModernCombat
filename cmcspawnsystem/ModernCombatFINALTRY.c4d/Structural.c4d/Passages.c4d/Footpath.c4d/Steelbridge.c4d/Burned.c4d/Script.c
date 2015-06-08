/*-- Zerstörte Stahlbrücke --*/

#strict 2

public func IsRepairable()	{return false;}


/* Aufschlag */

protected func Hit3()
{
  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",25,15,0,0,50,200, RGBa(255,255,255,0));
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",4,150,0,0,40,100);
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",8,230,0,0,40,15,RGB(40,20,20));
  Sound("StructureHit*.ogg");
}