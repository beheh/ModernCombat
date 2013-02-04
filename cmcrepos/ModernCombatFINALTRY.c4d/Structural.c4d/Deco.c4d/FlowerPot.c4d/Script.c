/*-- Blumentopf --*/

#strict 2

local broken;

private func PlantCount()	{return 5;}	//Anzahl Pflanzen (abzüglich 1)


/* Initialisierung */

protected func Initialize()
{
  //Zufällige Pflanze wählen
  var random = Random(PlantCount());
  SetAction(Format("Plant%d", random+1));
}

/* Schaden */

protected func Damage()
{
  if(broken) return;
  if(GetDamage() < 50) return;
  broken = true;

  //Aussehen ändern
  SetGraphics("Broken");

  //Schleudern
  SetRDir(RandomX(-10,10));
  SetSpeed(RandomX(-10,10),-20);

  //Effekte
  Sound("PotBreak.ogg");
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("GrassBlade",RandomX(6,8),10,0,-30,100,150,RGB(255,255,255),RGBa(255,255,255,130));
  CreateParticle("HalfPot", -2, 0, -10-Random(5), -2, 65, RGBa(255,255,255,0));
  CreateParticle("HalfPot",  4, 0,  10+Random(5), -2, 65, RGBa(255,255,255,0));

  //Verschwinden
  FadeOut();
}

/* Aufprall */

protected func Hit3()
{
  DoDamage(50);
}