/*-- Straßenlicht --*/

#strict 2
#include BLGH


func CreateLight()
{
  pLight = AddLightCone(3500,RGBa(255,255,255,70),this());
  pLight -> ChangeR(180);
  pLight -> TurnOn();
}

/* Zerstörung */

public func IsBulletTarget(id def)
{
  return false;
}

func Damage()
{
  if(broken) return;

  if(GetDamage() > 50)
  {
    broken = true;

    //Licht entfernen
    if(Light())
      Light()->RemoveObject();
    SetAction("Idle");

    //Umkippen
    SetCategory(C4D_Vehicle);
    if(Random(2))
      var direction = RandomX(-2,-1);
    else
      var direction = RandomX(1,2);
    SetRDir(direction);
    Fling(this, direction, -2);

    //Effekte
    if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",5,5,0,30,80,100,RGBa(255,255,255,120),RGBa(255,255,255,150));
    CastParticles("SplinterGlass", 5, 35, 0, -20, 20, 20, RGBa(255,255,255,0), RGBa(255,255,255,0));
    Sparks(7+Random(5), RGBa(255,255,150,100));
    Sound("GlassBreak*.ogg");

    //Verschwinden
    FadeOut();
  }
}

/* Aufprall */

func Hit()
{
  Sound("MetalPoleHit*.ogg");
}