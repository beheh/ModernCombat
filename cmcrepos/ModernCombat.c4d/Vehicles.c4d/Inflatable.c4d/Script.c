/*-- Schlauchboot (inaktiv) --*/

#strict 2

local damaged;


/* Initalisierung */

protected func Initialize()
{
  SetDir(Random(2));
  SetAction("OnLand");
}

/* Zerstörung */

func Incineration()
{
  if(damaged) return;
  damaged = true;

  //Brennen
  Incinerate();

  //Effekte
  Sound("OutOfAir");
  Sound("StructuralDamage*.ogg");
  CastParticles("MetalSplinter",4,100,0,0,20,170,RGB(50,250,50));
  CastParticles("MetalSplinter",2,100,0,0,30,100,RGB(0,0,0));
  CreateParticle("Blast",0,-10,-20,0,5*50,RGB(255,255,128));
  CreateParticle("Blast",0,-10,20,0,5*50,RGB(255,255,128));
  CastParticles("Smoke3",15,15,0,-10,100,200,RGBa(0,0,0,100));

  //Verschwinden
  ChangeDef(INFB);
  FadeOut();
}

/* Aktivierung */

private func Floating()
{
  // Aufrichten
  SetRDir(BoundBy(GetR(),-20,+20)/-5);
  // Nicht mehr im Wasser
  if (!(GetOCF()&OCF_InLiquid)) 
    return SetAction("OnLand");
  // Segeln, wenn aufgerichtet
  if (Inside(GetR(),-5,+5))
  {
    ChangeDef(INFS);
    SetAction("LowerSail");
    this->~SetUp();
  }
}