/*-- Automat --*/

#strict 2

local damaged;


/* Initialisierung */

protected func Initialize()
{
  //Soundeffekt
  Sound("FreezerLoop", 0,0,0,0, +1);
  //Aktionsschleife starten
  SetAction("Glow");
}

/* Zerstörung */

func Damage()
{
  if(damaged) return ;
  if(GetDamage() > 80)
  {
    damaged = true;

    //Effekte
    Sound("FreezerLoop", 0,0,0,0, -1);
    Sound("CokeCrash");
    Schedule(Format("CreateParticle(\"Tin\", -5, 12, RandomX(-10, 10), -1, 30, %d);", RGB(255, 25, 25)), 2, 10, this());
    Schedule("CastObjects(SPRK, 1, 15, RandomX(-10, 10), RandomX(-15, 15));", 3, 15, this());
    CastParticles("MetalSplinter", 10, 100, 0,0, 80);
    CastSmoke("Smoke3",12,10,0,0,100,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
    SetAction("Destroyed");
  }
}