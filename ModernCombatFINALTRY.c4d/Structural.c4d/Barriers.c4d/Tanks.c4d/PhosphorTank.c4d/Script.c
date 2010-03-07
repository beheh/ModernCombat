/*-- Phosphortank --*/

#strict 2


/* Entzündung */

func Incineration(int iPlr)
{
  ScheduleCall(this, "BlowUp", 20+Random(100),0,iPlr);
}

func IncinerationEx(int iPlr)
{
  ClearScheduleCall(this, "BlowUp");
}

/* Zerstörung */

func Damage(int iChange, int iPlr)
{
  SetController(iPlr);
  if(GetDamage() > 200)
   Incinerate();
}

func BlowUp(int iPlr)
{
  //Effekte
  Sound("Inflame");
  Sound("StructuralDamage*.ogg");
  CastParticles("MetalCrateSplinter", 15+Random(5), 100, 0,0, 60,100,RGB(50,0,250));
  CastParticles("MetalSplinter", 15+Random(5), 100, 0,0, 35,50,RGB(40,20,20));
  CastParticles("Smoke3",50,20,0,0,100,200,RGBa(255,255,255,100),RGBa(255,0,255,130));
  //Phosphor verschleudern
  CastObjects(PSPR, 20, 50);
  //Explosion
  Explode(20);
}
