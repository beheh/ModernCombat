/*-- Sprengfallenpack --*/

#strict 2
#include PACK

local controller, thrown;


public func IsDrawable()		{return true;}
public func CanAim()			{return Contained();}
public func Color()			{return RGB(200,200,200);}
public func HandX()			{return 5000;}
public func HandSize()			{return 1000;}
public func BarrelXOffset()		{return -850;}
public func IsEquipment()		{return true;}
public func NoArenaRemove()		{return true;}
public func AttractTracer()		{return false;}
public func LimitationCount()		{return 1;}
public func MaxPoints()		{return 3;}
public func StartPoints()	{return 2;}
public func PackLight()				{return false;}
public func DestroyEmptyPack()			{return true;}

/* Steuerung */

public func ControlThrow(object caller)
{
	if(GetPlrDownDouble(GetOwner(caller)))
    return _inherited(...);

  //Wird nicht getragen: Werfen gesperrt
  if(!Contained() || !GetPackPoints()) return true;
  
  CreateContents(BBTP, this, 1)->ControlThrow(this);
  DoPackPoints(-1);
  
  if (!GetPackPoints())
  	RemoveObject(this);
  	
  return true;
}

/* Schaden */

protected func Damage(int iChange) 
{
  //Kein Schaden nehmen wenn gehalten und eventuelles Feuer löschen
  if(Contained())
  {
    if(OnFire())
    {
      Extinguish();
      return;
    }
    else
    return;
  }
}

public func OnDmg(int iDmg, int iType)
{
  if(iType == DMG_Fire) return 90;	//Feuer
  if(iType == DMG_Bio)	return 100;	//Säure und biologische Schadstoffe
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
