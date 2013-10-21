/*-- Laterne --*/

#strict
#include BULB


func IsBulletTarget(id ID)
{
  if(broken) return;
  var shot = FindObject(ID,0,0,-1,-1);
  if(!shot)
    return(0);

  speed += shot->GetXDir();
  speed = BoundBy(speed,-55,55);
  dir = BoundBy(-speed,-1,1);
  return(1);
}

/* Zerstörung */

func Damage()
{
  if(broken) return;
  broken = true;

  //Licht ausschalten
  if(Light())
    Light()->TurnOff();
  SetAction("Off");

  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",5,15,0,0,100,150,RGBa(255,50,50,120));
  CastParticles("SplinterGlass", 5, 35, 0, 0, 20, 20, RGBa(255,255,255,0), RGBa(255,255,255,0));
  Sparks(7+Random(5), RGBa(255,255,150,100));
  Sound("GlassBreak*.ogg");

  //Verschwinden
  FadeOut();
}