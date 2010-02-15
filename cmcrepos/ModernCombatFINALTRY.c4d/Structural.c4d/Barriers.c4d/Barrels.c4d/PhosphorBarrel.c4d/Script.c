/*-- Phosphorfass --*/

#strict 2
#include XBRL


/* Zerstörung */

func Damage(int iChange, int iPlr)
{
  if(GetDamage() < 70) return ;
  InstaExplode(iPlr);
}

func BlowUp(int iPlr)
{
  //Effekt
  Sound("Inflame");
  CastParticles("MetalSplinter",4,100,0,0,20,70,RGB(50,0,250));
  CastParticles("Smoke3",50,20,0,0,100,200,RGBa(255,255,255,100),RGBa(255,0,255,130));

  SetController(iPlr);
  SetAction("Wreck");
  SetController(iPlr);
  Extinguish();
  SetRDir(RandomX(-40,+40));

  //Phosphor verschleudern
  CastObjects(PSPR, 8, 50);
  //Langsam verschwinden
  FadeOut();
}

/* Aufprall */

protected func Hit3()
{
  DoDamage(50);
}