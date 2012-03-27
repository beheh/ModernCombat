/*-- Hintergrundlicht --*/

#strict 2
#appendto BLGH

local pLight, active, broken;

public func LightSize()	{return 100;}
public func IsLamp()	{return true;}
public func IsMachine()	{return true;}


/* Initialisierung */

protected func Initialize()
{
  //Licht erstellen
  CreateLight();
  //Einschalten
  TurnOn();
  active = true;
}

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
  if(GetAction() = "On")
    TurnOff();
  else
    TurnOn();
}

public func TurnOn()
{
  if(EMPShocked()) return;
  active = true;
  SetAction("On");
  if(Light())
    Light()->TurnOn();
}

public func TurnOff()
{
  if(!SetAction("Off"))
    SetAction("Idle");
  active = false;
  if(Light())
    Light()->TurnOff();
}

/* EMP Effekt */

public func EMPShock()
{
  TurnOff();
  EMPShockEffect(800+Random(200));
}

public func EMPShockEnd()
{
  if(active)
  {
    SetAction("On");
    if(Light()) Light()->TurnOn();
  }
}

/* (Temporäre) Zerstörung */

public func IsBulletTarget(id def)
{
  if(broken) return ;
  if(def->~NoDecoDamage()) return;
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
  ScheduleCall(0, "Reactivate", 500);

  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",5,5,0,0,40,100,RGBa(255,255,255,120),RGBa(255,255,255,150));
  CastParticles("SplinterGlass", 1, 35, RandomX(-10,10), -5, 20, 20, RGBa(255,255,255,0), RGBa(255,255,255,0));
  Sparks(7+Random(5), RGBa(255,255,150,100));
  AddLightFlash(50, 0,0, RGBa(255,255,200,100));
  Sound("Glass");
}

func Reactivate()
{
  //Licht einschalten
  if(active)
  {
    SetAction("On");
    if(Light()) Light()->TurnOn();
  }

  broken = false;
}