/*-- Phosphortank --*/

#strict 2


/* Entzündung */

func Incineration()
{
  ScheduleCall(this, "BlowUp", 20+Random(100), 0, GetController());
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
    Incinerate(this, iPlr+1);
}

func BlowUp(iPlr)
{
  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",50,20,0,0,100,200,RGBa(100,150,250,130));
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastParticles("MetalCrateSplinter", 15+Random(5), 100, 0,0, 60,100,RGB(50,0,250));
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter", 15+Random(5), 100, 0,0, 35,50,RGB(40,20,20));
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("Glas", 10+Random(5), 100, 0,0, 60,100, RGBa(200,200,200), RGBa(200,200,200));
  Sound("PhosphorExplosion.ogg");
  Sound("StructureHeavyHit*.ogg");
  Sound("StructureDebris*.ogg");

  //Phosphor verschleudern
  for(var i = 0; i <= 20; i++)
  {
    CreateObject(PSPR, AbsX(GetX()), AbsY(GetY())-GetDefHeight(GetID())/2, GetController())->SetSpeed(RandomX(-50,50), RandomX(-50,50));
  }

  //Explosion
  FakeExplode(20, iPlr+1);
  RemoveObject();
}