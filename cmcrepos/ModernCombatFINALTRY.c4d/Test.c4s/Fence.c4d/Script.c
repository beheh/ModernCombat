/*-- Zaun --*/

#strict 2


/* Zerstörung */

func Damage()
{
  if(GetDamage() > 30)
  {
   //Effekte
   CastParticles("Smoke3",12,25,0,0,100,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
   CastParticles("FenceShred", 4, 50, -15,-15, 150);
   CastParticles("FenceShred", 4, 50, 15,-15, 150);
   //CastParticles("FenceShred", 8, 50, 0,0, 150);
   Sound("CrateCrack");

   //Verschwinden
   RemoveObject();
  }
}