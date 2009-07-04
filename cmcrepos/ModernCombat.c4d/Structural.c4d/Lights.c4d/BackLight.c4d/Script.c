/*-- Ausgangslicht --*/

#strict

local pLight, bOn, broken;

public func LightSize() { return(50); }


public func Light() 
{
  return pLight; 
}

protected func Initialize()
{
  CreateLight();
  TurnOn();
  return(1);
}

protected func CreateLight()
{
  pLight = AddLightAmbience(LightSize());
}

public func TurnOn()
{
  if(EMPShocked()) return;
   if(broken) return();
    bOn = true;
    SetAction("On");
    if(Light())
    Light()->TurnOn();
}

public func TurnOff()
{
  if(!SetAction("Off"))
   SetAction("Idle");
   bOn = false;
   if(Light())
   Light()->TurnOff();
}

public func Switch()
{
  if(GetAction() S= "On")
   TurnOff();
   else
   TurnOn();
}

public func IsLamp() { return(true); }
public func IsMachine() { return(true); }

public func EMPShock()
{
  if(broken) return();
  TurnOff();
  EMPShockEffect(800+Random(200));
}

public func EMPShockEnd()
{
  if(bOn)
  {
   SetAction("On");
   if(Light()) Light()->TurnOn();
  }
}

func Damage()
{
  if(broken) return();
  broken = true;
  SetAction("Broken");
  Sound("Glass");
  Sparks(7+Random(5), RGBa(255,255,150,100));
  CastParticles("SplinterGlass", 1, 35, RandomX(-10,10), -5, 20, 20, RGBa(255,255,255,0), RGBa(255,255,255,0));
  CastParticles("Smoke3",5,5,0,0,40,100,RGBa(255,255,255,120),RGBa(255,255,255,150));
  AddLightFlash(50, 0,0, RGBa(255,255,200,100));
}

public func IsBulletTarget(id def)
{
  if(broken) return();
  if(def->~NoDecoDamage()) return();
  return(Random(2));
}

/* Serialisierung */

public func Serialize(array& extra)
{
  if (GetAction() ne "On")
   extra[GetLength(extra)] = "TurnOff()";
}