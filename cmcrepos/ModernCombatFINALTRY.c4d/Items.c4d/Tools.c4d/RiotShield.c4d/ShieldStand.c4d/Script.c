/*-- Schildgestell --*/

#strict 2
#include CSTR

local aim_angle;
local pShield;
local fRotRight;

public func MaxDamage()	{return 100;}


/* Initialisierung */

func Initialize()
{
  //Animation und Effekt setzen
  SetAction("Con");
  AddEffect("IntCon", this(), 1, 1, this() );
}

/* Rotationsbegrenzungen */

public func MaxRotLeft()
{
  if(fRotRight)
    return 10;
  else
  return -150;
}

public func MaxRotRight()
{
  if(fRotRight)
    return 150;
  else
   return -10;
}

public func AimAngle()
{
  return aim_angle;
}

public func WeaponAt(&x, &y, &r)
{
  return true;
}

public func DoHit(int iHit)
{
  return true;
}

/* Schild */

public func GetShield()
{
  //Schild entfernen sofern vorhanden
  if(pShield)
    RemoveObject(pShield);

  //Schild erstellen
  pShield = CreateObject(RSLH, 0, 0, GetOwner(this));
  pShield->Set(this, this);
}

public func SetAim(int angle)
{
  aim_angle = angle;

  if (angle > 0)
    fRotRight = true;
  else
    fRotRight = false;
}

/* Aufbau */

protected func FxIntConTimer(object target, int number, iEffectTime)
{
  if(iEffectTime == 1)
    SetDir(!fRotRight);

  //Keine Arbeiter: wieder zusammenfallen
  if(!FindObject2(Find_ActionTarget(this),Find_Action("Push")) && GetActTime() > 5)
  {
    OnDestruction();
    return(-1);
  }

  //Aufbaugeräusche
  if(!(iEffectTime%30))
  {
    Sound("WPN2_Hit*.ogg",0,0,50);
  }
}

private func ConComplete()
{
  if(GetEffect("IntCon", this))
    RemoveEffect("IntCon", this);

  var pTemp;
  while(pTemp = FindObject2(Find_ActionTarget(this), Find_Action("Push")))
    ObjectSetAction(pTemp, "Walk");

  SetAction("Standing");
  GetShield();

  Sound("Connect");
}

/* Steuerung */

public func ControlDigDouble(pClonk)
{
  var pTempShield = CreateContents(RSHL);

  Collect(pTempShield,pClonk);
  return RemoveObject(this,true);
}

public func ControlUpSingle(pByObj)
{
  if(fRotRight)
    TurnLeft();
  else
    TurnRight();

  return true;
}

public func ControlDownSingle(pByObj) 
{
  if(fRotRight)
    TurnRight();
  else
    TurnLeft();

  return true;
}

/* Ausrichtung */

private func TurnLeft()
{
  if(GetEffect("IntCon"))
    return;

  if(aim_angle > MaxRotLeft())
    aim_angle -= 10;
}

private func TurnRight()
{
  if(GetEffect("IntCon"))
    return;

  if(aim_angle < MaxRotRight())
    aim_angle += 10;
}

public func ControlLeft(pByObj)
{
  TurnLeft();

  return true;
}

public func ControlRight(pByObj)
{
  TurnRight();

  return true;
}

/* Schaden */

public func Destroyed()
{
  //Schild erstellen
  CreateObject(RSHL,0,0,this()->GetOwner());

  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter",3,110,0,0,40,100);
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter",1,80,0,0,90,90);
  CastSmoke("Smoke3",8,10,0,5,100,150,RGBa(255,255,255,100),RGBa(255,255,255,130));
  Sound("MISL_ShotDown.ogg");

  //Verschwinden
  RemoveObject();
}

public func OnDmg(int iDmg, int iType)
{
  if(iType == DMG_Fire)		return 100;	//Feuer
  if(iType == DMG_Explosion)	return 20;	//Explosionen und Druckwellen
  if(iType == DMG_Bio)		return 100;	//Säure und biologische Schadstoffe
  return 80;
}