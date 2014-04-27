/*-- Hintergrundlicht --*/

//Lichter können zerschossen werden und reaktivieren sich automatisch.

#strict 2
#appendto BLGH

local pLight, bOn, broken;

public func LightSize()		{return 100;}	//Lichtgröße
public func DownTime()		{return 1500;}	//Zeit bis zur Reaktivierung
public func IsLamp()		{return true;}
public func IsMachine()		{return true;}
public func RejectC4Attach()	{return true;}


/* Initialisierung */

protected func Initialize()
{
  //Licht erstellen
  CreateLight();
  //Einschalten
  TurnOn(1);
  bOn = true;
}

/* Lichtverwaltung */

protected func CreateLight()
{
  pLight = AddLightAmbience(LightSize());
}

public func Light() 
{
  return pLight;
}

/* Ein- und Ausschalten */

public func Switch()
{
  if(broken)		return;
  if(EMPShocked())	return;

  if(GetAction() == "On")
    TurnOff();
  else
    TurnOn();
}

public func TurnOn(bool fSound)
{
  if(broken)		return;
  if(EMPShocked())	return;

  bOn = true;
  SetAction("On");
  if(Light())
    Light()->TurnOn();

  if(!fSound)
    Sound("LightSwitch*.ogg");
}

public func TurnOff(bool fSound)
{
  if(broken)		return;

  bOn = false;
  if(!SetAction("Off"))
    SetAction("Idle");
  if(Light())
    Light()->TurnOff();

  if(!fSound)
    Sound("LightSwitch*.ogg");
}

/* EMP Effekt */

public func EMPShock()
{
  TurnOff(1);
  EMPShockEffect(800+Random(200));
}

public func EMPShockEnd()
{
  if(bOn)
  {
    SetAction("On");
    if(Light()) Light()->TurnOn();
    Sound("LightSwitch*.ogg");
  }
}

/* (Temporäre) Zerstörung */

public func IsBulletTarget(id def)
{
  if(broken)			return;
  if(!bOn)			return;
  if(def->~NoDecoDamage())	return;

  return Random(2);
}

func Damage()
{
  if(broken) return;
  broken = true;

  //Licht ausschalten
  if(Light())
    Light()->TurnOff();
  SetAction("Idle");

  //Reaktivierung planen
  ScheduleCall(0, "Reactivate", DownTime());

  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",5,5,0,0,40,100,RGBa(255,255,255,120),RGBa(255,255,255,150));
  CastParticles("SplinterGlass", 5, 35, RandomX(-10,10), -5, 20, 20, RGBa(255,255,255,0), RGBa(255,255,255,0));
  Sparks(7+Random(5), RGBa(255,255,150,100));
  Sound("GlassBreak*.ogg");
}

func Reactivate()
{
  //Licht einschalten
  if(bOn)
  {
    SetAction("On");
    if(Light()) Light()->TurnOn();
    Sound("LightSwitch*.ogg");
  }

  broken = false;
}