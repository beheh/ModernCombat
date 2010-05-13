/*-- Gestein --*/

#strict 2


/* Initalisierung */

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
   CastParticles("ConcreteSplinter",4,150,0,0,20,100);
   CastParticles("Smoke3",12,10,0,0,100,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
   Sound("RockBreak*");
   //Verschwinden
   RemoveObject();
  }
}
