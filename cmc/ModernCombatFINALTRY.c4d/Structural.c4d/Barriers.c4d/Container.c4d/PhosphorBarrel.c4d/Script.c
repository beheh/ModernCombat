/*-- Phosphorfass --*/

#strict 2
#include XBRL

public func NotInflammable()		{return true;}

/* Zerstörung */

func Damage(int iChange, int iPlr)
{
  if(GetDamage() < 70) return;
  InstaExplode(iPlr);
}

func BlowUp(int iPlr)
{
  //Achievement-Fortschritt (Barrel Roll)
  DoAchievementProgress(1, AC47, iPlr);

  //Zu Wrack wechseln
  SetAction("Wreck");
  SetController(iPlr);
  SetRDir(RandomX(-40,+40));
  Extinguish();
  SetObjectLayer(this());

  //Effekte
  Sound("PhosphorExplosion.ogg");
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",50,20,0,0,100,200,RGBa(100,150,250,130));
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter",4,100,0,0,20,70,RGB(50,0,250));
  FadeOut();

  //Phosphor verschleudern
  for(var i = 0; i <= 8; i++)
  {
    CreateObject(PSPR, AbsX(GetX()), AbsY(GetY())-GetDefHeight(GetID())/2, GetController())->SetSpeed(RandomX(-50,50), RandomX(-50,50));
  }
}

/* Aufprall */

protected func Hit3()
{
  if(!damaged)
  {
    Sound("BarrelDamaged*.ogg");
    DoDamage(50);
  }
}