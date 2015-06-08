/*-- Markierung --*/

#strict 2

local broken;


/* Zerstörung */

func Damage()
{
  if(broken) return;

  if(GetDamage() > 50)
  {
    broken = true;

    SetAction("Destroyed");

    //Umkippen
    SetCategory(C4D_Vehicle);
    if(Random(2))
      var direction = RandomX(-2,-1);
    else
      var direction = RandomX(1,2);
    SetRDir(direction);
    Fling(this, direction, -2);

    //Effekte
    if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",5,5,0,13,80,100,RGBa(255,255,255,120),RGBa(255,255,255,150));
    CastParticles("MetalSplinter", 5, 50, 0, 10, 20, 20);
    Sound("MetalHit*");

    //Verschwinden
    FadeOut();
  }
}

/* Aufprall */

func Hit()
{
  Sound("MetalPoleHit*.ogg",0,0,50);
}