/*-- Stützpunkt --*/

#strict 2
#include CCBS

local basement;	//Fundament

public func IsBase()		{return true;}	//Heimatbasis
public func BuildingRadius()	{return 400;}	//Bauradius
public func EnergyProduction()	{return 50;}	//Energiehersteller


/* Konstruktion */

protected func Construction()
{
  basement = CreateObject(BT01,0,8,GetOwner());
}

/* Initalisierung */

protected func Initialize()
{
  //Effekt
  Sound("Building_BaseUp.ogg",true,0,0,GetOwner()+1);

  return _inherited(...);
}

public func Destroyed()
{
  //Effekt
  Sound("Building_BaseDown.ogg",true,0,0,GetOwner()+1);

  return _inherited(...);
}