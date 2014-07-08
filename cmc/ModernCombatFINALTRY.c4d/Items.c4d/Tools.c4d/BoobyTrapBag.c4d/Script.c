/*-- Sprengfallentasche --*/

#strict 2
#include PACK

local controller, thrown;

public func IsDrawable()		{return true;}			//Wird sichtbar getragen
public func HandX()			{return 5000;}
public func HandSize()			{return 1000;}
public func BarrelXOffset()		{return -850;}

public func CanAim()			{return Contained();}
public func Color()			{return RGB(200,200,200);}
public func IsEquipment()		{return true;}
public func NoArenaRemove()		{return true;}
public func AttractTracer()		{return false;}
public func LimitationCount()		{return 1;}
public func MaxPoints()			{return 3;}
public func StartPoints()		{return 1;}
public func PackLight()			{return false;}
public func DestroyEmptyPack()		{return true;}


/* Steuerung */

public func ControlThrow(object caller)
{
  if(GetPlrDownDouble(GetOwner(caller)))
    return _inherited(...);

  //Wird nicht getragen: Werfen gesperrt
  if(!Contained() || !GetPackPoints()) return true;

  //Sprengfalle erstellen und legen
  SetOwner(GetOwner(Contained()));
  var trap = CreateContents(BBTP, this, 1);
  trap->ControlThrow(this);

  //Plazierung erfolgreich?
  if(LocalN("bActive", trap))
    DoPackPoints(-1);
  else
    RemoveObject(trap);

  //Verschwinden sobald aufgebraucht
  if(!GetPackPoints())
    RemoveObject(this);

  return true;
}

/* Schaden */

protected func Damage(int iChange) 
{
  return false;
}

/* Allgemein */

protected func Selection()
{
  Sound("BBTP_Charge.ogg");
  return 1;
}

func Hit()
{
  Sound("BBTP_Hit*.ogg");
}