/*-- Sonnenschirm --*/

#strict 2

local broken;


/* Initialisierung */

public func Initialize()
{
  //Standardfarbe: Rot
  SetColorDw(HSL(250, 250, 140, 50));
}

/* Zerstörung */

func Damage()
{
  if(broken) return;

  if(GetDamage() > 50)
  {
    broken = true;

    //Umkippen
    SetCategory(C4D_Vehicle);
    if(Random(2))
      var direction = RandomX(-2,-1);
    else
      var direction = RandomX(1,2);
    SetRDir(direction);
    Fling(this, direction, -2);

    //Aktion setzen
    SetAction("Broken");

    //Effekte
    if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",5,5,0,12,80,100,RGBa(255,255,255,120),RGBa(255,255,255,150));
    if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("WoodSplinter", 10, 100, 0, -10, 20, 40, RGBa(255,255,255,0), RGBa(255,255,255,0));
    CastParticles("Paper", RandomX(5,10), 80, -10, -15, 10, 20, RGBa(GetRGBaValue(GetColorDw(this()),1),GetRGBaValue(GetColorDw(this()),2),GetRGBaValue(GetColorDw(this()),3),0));
    CastParticles("Paper", RandomX(5,10), 80, 10, -15, 10, 20, RGBa(GetRGBaValue(GetColorDw(this()),1),GetRGBaValue(GetColorDw(this()),2),GetRGBaValue(GetColorDw(this()),3),0));
    Sound("CrateDestruct*.ogg");

    //Verschwinden
    FadeOut();
  }
}

/* Aufprall */

func Hit()
{
  Sound("MetalPoleHit*.ogg", 0, 0, 40);
}