/*-- Gestein --*/

#strict 2


/* Initialisierung */

protected func Initialize() 
{
  //Aktion und Aussehen setzen
  SetAction("Standard");
  var phase = Random(8);
  SetPhase(phase);
  SetSolidMask(phase * 32, 0, 32, 32);
}

/* Zerstörung */

func Damage()
{
  if(GetDamage() > 70)
  {
    //Effekte
    CastParticles("ConcreteSplinter",10,100,0,0,20,100);
    CastSmoke("Smoke3",12,10,0,0,100,200,RGBa(255,255,255,50),RGBa(255,255,255,130));
    Sound("RockBreak*");
    Sound("Blast2");

    //Verschwinden
    RemoveObject();
  }
}

/* Aussehen festlegen */

public func Set(int i)
{
  var phase = i;
  SetPhase(phase);
  SetSolidMask(phase * 32, 0, 32, 32);
}

/* Aufschlag */

protected func Hit()
{
  Sound("RockHit*");
}