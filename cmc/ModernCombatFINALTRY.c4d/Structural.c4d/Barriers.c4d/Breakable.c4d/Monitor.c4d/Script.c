/*-- Monitor --*/

#strict 2

local pLight;


/* Initialisierung */

protected func Initialize()
{
  SetAction("Activity1");

  //Licht erstellen
  pLight = AddLightAmbience(15);
  return 1;
}

public func On()
{
  if(pLight)
    pLight->TurnOn();
  SetAction("Activity1");
}

public func Off()
{
  //Licht entfernen
  if(pLight)
    pLight->TurnOff();

  SetGraphics(0,0,0,1);

  return(SetAction("Off"));
}

/* Schaden */

protected func Damage()
{
  if(GetAction() == "Broken") return;

  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastParticles("Glas", 2+Random(4), 80, 0,0, 20,80, RGBa(200,200,200), RGBa(200,200,200));
  CreateSmokeTrail(RandomX(15,20), Random(360), 0,0, this());
  CreateSmokeTrail(RandomX(15,20), Random(360), 0,0, this());
  Sound("GlassBreak*.ogg");
  Sound("Blast1");

  //Licht entfernen
  if(pLight) RemoveObject(pLight);

  //Umherfliegen
  SetSpeed(RandomX(-25, 25), RandomX(-45, -35));
  SetRDir(GetXDir()*2);

  //Zerstört
  SetAction("Broken");

  //Verschwinden
  FadeOut();
}

public func IsBulletTarget(id def)
{
  if(GetAction() != "Broken")	return 1;
  if(def->~NoDecoDamage())	return;
}

private func ChooseActivity()
{
  //Zufällige nächste Aktion wählen
  var iAction = Random(5)+1;
  SetAction(Format("Activity%d", iAction));
}

/* Aufschlag */ 

protected func Hit()
{
  Sound("MetalHit*");
  return 1;
}

protected func Hit3()
{
  Damage();
}