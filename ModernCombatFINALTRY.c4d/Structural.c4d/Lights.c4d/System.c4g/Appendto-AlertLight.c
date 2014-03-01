/*-- Alarmlicht --*/

//Kompatibilität zum neuen Lichtsystem.

#strict 2
#appendto ALGH


/* Ein- und Ausschalten */

public func Switch()
{
  if(broken) return;
  Sound("LightSwitch*.ogg");

  if(GetAction() = "On")
    TurnOff();
  else
    TurnOn();
}

public func TurnOn()
{
  if(broken) return;
  if(EMPShocked()) return;
  bOn = true;
  SetAction("On");
  if(pLight) pLight->TurnOn();
  if(pAlert) pAlert->TurnOn();
}

public func TurnOff()
{
  if(broken) return;
  if(!SetAction("Off"))
    SetAction("Idle");
  bOn = false;
  if(pLight) pLight->TurnOff();
  if(pAlert) pAlert->TurnOff();
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
  }

  broken = false;
}