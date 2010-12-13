/*-- Ausgangslicht --*/

#strict 2

local pLight, bOn, broken;

public func LightSize()	{return 50;}
public func IsLamp()	{return true;}
public func IsMachine()	{return true;}


/* Initialisierung */

protected func Initialize()
{
  CreateLight();
  TurnOn();
  return 1;
}

public func Light() 
{
  return pLight; 
}

protected func CreateLight()
{
  pLight = AddLightAmbience(LightSize());
}

/* Ein- und Ausschalten */

public func TurnOn()
{
  if(EMPShocked()) return;
    if(broken) return ;
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
  if(GetAction() == "On")
    TurnOff();
    else
    TurnOn();
}

/* EMP Effekt */

public func EMPShock()
{
  if(broken) return ;
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

/* Zerstörung */

public func IsBulletTarget(id def)
{
  if(broken) return ;
  if(def->~NoDecoDamage()) return ;
  return Random(2);
}

func Damage()
{
  if(broken) return ;
  broken = true;
  SetAction("Broken");
  if(Light())
    Light()->TurnOff();
  Sound("Glass");
  Sparks(7+Random(5), RGBa(255,255,150,100));
  CastParticles("SplinterGlass", 1, 35, RandomX(-10,10), -5, 20, 20, RGBa(255,255,255,0), RGBa(255,255,255,0));
  CastParticles("Smoke3",5,5,0,0,40,100,RGBa(255,255,255,120),RGBa(255,255,255,150));
  AddLightFlash(50, 0,0, RGBa(255,255,200,100));
}

/* Serialisierung */

public func Serialize(array& extra)
{
  if (GetAction() != "On")
    extra[GetLength(extra)] = "TurnOff()";
}