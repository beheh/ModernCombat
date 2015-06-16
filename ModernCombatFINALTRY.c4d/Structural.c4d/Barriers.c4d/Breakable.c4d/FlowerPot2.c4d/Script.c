/*-- Blumentopf --*/

#strict 2
#include PLT3


private func PlantCount()	{return 4;}	//Anzahl Pflanzen


/* Schaden */

protected func Damage()
{
  if(broken) return;
  if(GetDamage() < 10) return;
  broken = true;

  //Aussehen ändern
  SetGraphics("Broken");

  //Schleudern
  SetRDir(RandomX(-10,10));
  SetSpeed(RandomX(-10,10),-20);

  //Effekte
  Sound("CeramicBreak*.ogg");
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("GrassBlade",RandomX(6,8),10,0,-10,100,150,RGB(255,255,255),RGBa(255,255,255,130));
  CreateParticle("HalfPot", -2, 0, -10-Random(5), -2, 30, RGBa(255,255,255,0));
  CreateParticle("HalfPot",  4, 0,  10+Random(5), -2, 30, RGBa(255,255,255,0));

  //Verschwinden
  FadeOut();
}

public func IsBulletTarget(id def)
{
  if(broken) return;
  if(def->~NoDecoDamage()) return false;
  return true;
}

/* Aufprall */

protected func Hit3()
{
  DoDamage(10);
}