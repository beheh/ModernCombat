/*-- Rauchgranate --*/

#strict 2
#include NADE

local time;

public func FuseTime()		{return 2*35;}
public func Color()		{return RGB(128,255,0);}
public func ContainedDamage()	{return 20;}


/* Aktivierung */

public func Fuse()
{
  Sound("SGRN_Activate.ogg");
  SetGraphics("Active");
  return AddEffect ("IntFuse",this,200,1,this);
}

public func Fused()
{
  //Rauchen (gefährdet die Gesundheit)
  ScheduleCall(this, "Smoke", 3, 10);

  //Effekte
  Sound("SGRN_Fused.ogg");
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",8, 50, 0, 0, 120, 140, RGBa(255,255,255,100), RGBa(255,255,255,130));

  //Verschwinden
  FadeOut();
}

/* Rauchen */

func Smoke()
{
  CastSmoke("Smoke3",2, 5, 0, 0, 40, 90, RGBa(255,255,255,150), RGBa(255,255,255,200));

  var smoke = CreateObject(SM4K, 0, 0, GetOwner(GetUser()));
  SetXDir((GetXDir() / 3 * 2) + (time * RandomX(-30, +30)),smoke,100);
  SetYDir((GetYDir() / 3 * 2) - (time * (10 + Random(20))),smoke,100);

  time++;
}

/* Schaden */

protected func Damage(int iChange) 
{
  //Zündung durch Schaden
  if(GetDamage() < 10 || activated) return ;

  //Effekte
  Sparks(2,RGB(250,100));
  Sparks(2,RGB(0,200));
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastParticles("BlastDirt",2,10,0,0,400,100);
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastParticles("BlastFlame",2,10,0,0,150,100);
  Sound("SGRN_Fused.ogg");

  Smoke();
  RemoveObject();
}

/* Aufprall */

func HitSound()
{
  Sound("SGRN_Hit*.ogg");
}