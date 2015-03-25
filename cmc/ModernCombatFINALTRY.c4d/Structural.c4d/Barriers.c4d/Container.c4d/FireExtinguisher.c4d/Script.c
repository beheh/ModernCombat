/*-- Feuerlöscher --*/

#strict 2
#include GSBL

local damaged;

public func NotInflammable()		{return true;}
public func IsBulletTarget(idBullet)	{return idBullet == TRDT || !Random(6);}
public func IsMeleeTarget(object obj)
{
  if(damaged) return;
  if(obj) if(GetID(obj) == BWTH) return 1;
  return;
}


/* Zerstörung */

func Damage(int iChange)
{
  if(damaged || !this || GetDamage() < 50) return;
  InstaExplode();
}

func InstaExplode()
{
  if(damaged) return;
  damaged = true;

  //Effekte
  Sound("BarrelImpact*.ogg");
  Sound("Fuse.wav");
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastParticles("MetalSplinter", 3+Random(3), 80, 0,0, 30,80,RGB(250,0,0));
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",4,30,0,0,100,300,RGBa(255,255,255,100),RGBa(255,255,255,130));
  if(GetEffectData(EFSM_ExplosionEffects) > 1) AddEffect("GSBL_Smoke",this,251,1,this);

  //Kategorie wechseln
  SetCategory(C4D_Vehicle);

  //Nach oben schleudern
  SetRDir(RandomX(-50,50));
  SetXDir(Sin(GetR(),60));
  SetYDir(-Cos(GetR(),60));

  //Sound
  Sound("MISL_Thrust.ogg",0,0,0,-1,1);

  //Explosion
  ScheduleCall(this, "BlowUp", 25);
}

func BlowUp()
{
  //Rauch erzeugen
  CastObjects(SM4K,4,20);

  //Sound
  Sound("SGRN_Fused.ogg");

  //Verschwinden lassen
  DecoExplode(10);
}