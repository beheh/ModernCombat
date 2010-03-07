/*-- Rauchgranate --*/

#strict 2
#include NADE

local time;

public func FuseTime() { return 2*35; }
public func Color() { return RGB(128,255,0); } // grün
public func ContainedDamage() { return 20; }


public func Fused()
{
  Sound("SGRN_Fused.ogg");
  ScheduleCall(this, "Smoke", 14, 10); // 4 Sekunden lang. :)
  FadeOut();
}

func Smoke()
{
  CastParticles("Smoke3", 2, 5, 0, 0, 30, 100, RGBa(255, 255, 255, 130), RGBa(255, 255, 255, 200));

  var smoke = CreateObject(SM4K, 0, 0, GetController());
  smoke->SetXDir((GetXDir() / 3 * 2) + (time * RandomX(-30, +30) / 10));
  smoke->SetYDir((GetYDir() / 3 * 2) - (time * (10 + Random(20)) / 10));	
  
  time++;
}

func HitSound()
{
  Sound("SGRN_Hit*.ogg");
}