/*-- Benzintank --*/

#strict 2


/* Entzündung */

func Incineration()
{
  ScheduleCall(this, "BlowUp", 20+Random(80), 0, GetController());
}

func IncinerationEx()
{
  ClearScheduleCall(this, "BlowUp");
}

/* Zerstörung */

func Damage(int iChange, int iPlr)
{
  SetController(iPlr);
  if(GetDamage() >= 100)
    Incinerate(this, iPlr+1);
}

func BlowUp(iPlr)
{
  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastParticles("MetalCrateSplinter", 15+Random(5), 100, 0,0, 60,100,RGB(250,0,0));
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter", 15+Random(5), 100, 0,0, 35,50,RGB(40,20,20));
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("Glas", 10+Random(5), 100, 0,0, 60,100, RGBa(200,200,200), RGBa(200,200,200));
  Sound("StructureHeavyHit*.ogg");
  Sound("StructureDebris*.ogg");

  //Explosion
  FakeExplode(50, iPlr+1);
  RemoveObject();
}