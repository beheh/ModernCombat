/*-- Gestein --*/

#strict 2


/* Initialisierung */

protected func Initialize() 
{
  SetAction("Standard");
  var phase = Random(6);
  SetPhase(phase);
  SetSolidMask(phase * 32, 0, 32, 32);
}

/* Zerstörung */

func Damage()
{
  if(GetDamage() > 70)
  {
   //Effekte
   if(GetEffectData(EFSM_ExplosionEffects) > 0) CastParticles("Smoke3",12,10,0,0,100,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
   if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",4,150,0,0,20,100);
   Sound("RockBreak*");

   //Verschwinden
   RemoveObject();
  }
}