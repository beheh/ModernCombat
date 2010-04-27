/*-- Rauchgranate --*/

#strict 2
#include NADE

local time;

public func FuseTime() { return 2*35; }
public func Color() { return RGB(128,255,0); } // grün
public func ContainedDamage() { return 20; }


/* Explosion */

public func Fused()
{
  //Rauchen (gefährdet die Gesundheit)
  ScheduleCall(this, "Smoke", 3, 10);

  //Effekte
  Sound("SGRN_Fused.ogg");
  CastParticles("Smoke3",8,50,0,0,120,150,RGBa(255,255,255,120),RGBa(255,255,255,150));

  //Verschwinden
  FadeOut();
}

/* Rauchen */

func Smoke()
{
  CastParticles("Smoke3", 2, 5, 0, 0, 30, 100, RGBa(255, 255, 255, 130), RGBa(255, 255, 255, 200));

  var smoke = CreateObject(SM4K, 0, 0, GetController());
  SetXDir((GetXDir() / 3 * 2) + (time * RandomX(-30, +30)),smoke,100);
  SetYDir((GetYDir() / 3 * 2) - (time * (10 + Random(20))),smoke,100);
  
  time++;
}

/* Aufprall */

func HitSound()
{
  Sound("SGRN_Hit*.ogg");
}
