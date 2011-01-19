/*-- Schild --*/

#strict 2


/* Intialisierung */

func Initialize()
{
  SetAction("Exist");
}

/* Zerstörung */

func Damage()
{
  if(GetDamage() > 80)
  {
    //Effekte
    CastParticles("WoodSplinter", 8, 50, 0,0, 141);
    CastSmoke("Smoke3",12,10,0,0,100,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
    Sound("WoodCrack*.ogg");

    RemoveObject();
  }
}