/*-- Alarmlicht --*/

//Kompatibilität zum Lichtsystem.

#strict 2
#appendto ALGH


/* Ein- und Ausschalten */

public func TurnOn(bool fSound)
{
  if(broken)		return;
  if(EMPShocked())	return;

  bOn = true;
  SetAction("On");

  if(pLight) pLight->TurnOn();
  if(pAlert) pAlert->TurnOn();

  if(!fSound)
    Sound("LightSwitch*.ogg");
}

public func TurnOff(bool fSound)
{
  if(broken)		return;

  bOn = false;
  if(!SetAction("Off"))
    SetAction("Idle");

  if(pLight) pLight->TurnOff();
  if(pAlert) pAlert->TurnOff();

  if(!fSound)
    Sound("LightSwitch*.ogg");
}

/* (Temporäre) Zerstörung */

func Damage()
{
  if(broken) return;
  broken = true;

  //Licht ausschalten
  if(pLight) pLight->TurnOff();
  if(pAlert) pAlert->TurnOff();
  SetAction("Idle");

  //Reaktivierung planen
  ScheduleCall(0, "Reactivate", DownTime());

  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",5,5,0,0,40,100,RGBa(255,255,255,120),RGBa(255,255,255,150));
  CastParticles("SplinterGlass", 1, 35, RandomX(-10,10), -5, 20, 20, RGBa(255,255,255,0), RGBa(255,255,255,0));
  Sparks(7+Random(5), RGBa(255,255,150,100));
  AddLightFlash(50, 0,0, RGBa(255,255,200,100));
  Sound("GlassBreak*.ogg");
}

func Reactivate()
{
  //Licht einschalten
  if(bOn)
  {
    SetAction("On");
    if(pLight) pLight->TurnOn();
    if(pAlert) pAlert->TurnOn();
    Sound("LightSwitch*.ogg");
  }

  broken = false;
}