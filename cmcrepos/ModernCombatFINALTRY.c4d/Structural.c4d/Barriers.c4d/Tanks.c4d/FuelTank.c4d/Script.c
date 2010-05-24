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
  Sound("StructuralDamage*.ogg");
  CastParticles("MetalCrateSplinter", 15+Random(5), 100, 0,0, 60,100,RGB(250,0,0));
  CastParticles("MetalSplinter", 15+Random(5), 100, 0,0, 35,50,RGB(40,20,20));
  CastParticles("Glas", 10+Random(5), 100, 0,0, 60,100, RGBa(200,200,200), RGBa(200,200,200));
  //Explosion
  Explode(50);
}
