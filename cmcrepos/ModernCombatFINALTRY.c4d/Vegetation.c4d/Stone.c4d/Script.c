/*-- Gestein --*/

#strict 2


/* Initialisierung */

protected func Initialize() 
{
  SetAction("Standard");
  var phase = Random(6);
  SetPhase(phase);
  SetSolidMask(phase * 32, 0, 32, 32);
}

/* Zerst�rung */

func Damage()
{
  if(GetDamage() > 70)
  {
    //Effekte
    if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",10,100,0,0,20,100);
    CastSmoke("Smoke3",12,10,0,0,100,200,RGBa(255,255,255,50),RGBa(255,255,255,130));
    Sound("RockBreak*");
    Sound("Blast2");
    Sound("Debris*.ogg");

    //Verschwinden
    RemoveObject();
  }
}

/* Aufschlag */

protected func Hit()
{
  Sound("RockHit*");
}