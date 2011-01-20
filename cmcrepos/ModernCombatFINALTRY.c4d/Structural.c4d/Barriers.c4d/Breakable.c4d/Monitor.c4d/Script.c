/*-- Monitor --*/

#strict 2

local pLight;


/* Initialisierung */

protected func Initialize()
{
  SetAction("Activity1");
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
  CastObjects(SPRK, 7, 50);
  Sparks(15, RGB(210, 210, 0));
  CreateSmokeTrail(RandomX(15,20), Random(360), 0,0, this());
  CreateSmokeTrail(RandomX(15,20), Random(360), 0,0, this());
  Sound("CrystalHit*");
  Sound("Blast1");

  if(pLight) RemoveObject(pLight);

  //Umherfliegen
  SetSpeed(RandomX(-25, 25), RandomX(-45, -35));
  SetRDir(GetXDir()*2);

  //Zerstört
  SetAction("Broken");
}

public func IsBulletTarget(id def)
{
  if(GetAction() != "Broken") return 1;
  if(def->~NoDecoDamage()) return;
}

private func ChooseActivity()
{
  //Zufällige nächste Aktion wählen
  var iAction = Random(3)+1;
  SetAction(Format("Activity%d", iAction));
}

/* Aufschlag */ 

protected func Hit3()
{
  Damage();
}