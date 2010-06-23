/*-- Benzintank --*/

#strict 2


/* Entzündung */

func Incineration(int iPlr)
{
  ScheduleCall(this, "BlowUp", 20+Random(80),0,iPlr);
}

func IncinerationEx(int iPlr)
{
  ClearScheduleCall(this, "BlowUp");
}

/* Zerstörung */

func Damage(int iChange, int iPlr)
{
  SetController(iPlr);
  if(GetDamage() >= 100)
   Incinerate();
}

func BlowUp(int iPlr)
{
  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastParticles("MetalCrateSplinter", 15+Random(5), 100, 0,0, 60,100,RGB(250,0,0));
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter", 15+Random(5), 100, 0,0, 35,50,RGB(40,20,20));
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("Glas", 10+Random(5), 100, 0,0, 60,100, RGBa(200,200,200), RGBa(200,200,200));
  Sound("StructuralDamage*.ogg");

  //Explosion
  Explode(50);
}