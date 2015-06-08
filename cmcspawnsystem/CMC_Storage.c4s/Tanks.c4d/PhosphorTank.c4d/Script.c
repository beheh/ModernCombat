/*-- Phosphortank --*/

#strict 2


/* Entzündung */

func Incineration()
{
  ScheduleCall(this, "BlowUp", 20+Random(100));
}

func IncinerationEx()
{
  ClearScheduleCall(this, "BlowUp");
}

/* Zerstörung */

func Damage(int iChange, int iPlr)
{
  SetController(iPlr);
  if(GetDamage() >= 150)
   Incinerate();
}

func BlowUp()
{
  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastParticles("Smoke3",50,20,0,0,100,200,RGBa(255,255,255,100),RGBa(255,0,255,130));
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastParticles("MetalCrateSplinter", 15+Random(5), 100, 0,0, 60,100,RGB(50,0,250));
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter", 15+Random(5), 100, 0,0, 35,50,RGB(40,20,20));
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("Glas", 10+Random(5), 100, 0,0, 60,100, RGBa(200,200,200), RGBa(200,200,200));
  Sound("Inflame");
  Sound("StructuralDamage*.ogg");

  //Phosphor verschleudern
  CastObjects(PSPR, 20, 50);

  //Explosion
  Explode(20);
}