/*-- Stützpunkt --*/

#strict 2
#include CCBS

public func IsBase()		{return true;}	//Heimatbasis
public func BuildingRadius()	{return 400;}	//Bauradius
public func EnergyProduction()	{return 50;}	//Energiehersteller
public func BasementID()	{return BT02;}	//Fundament


/* Initalisierung */

protected func Initialize()
{
  //Effekt
  Sound("Building_BaseUp.ogg",true,0,0,GetOwner()+1);

  return _inherited(...);
}

/* Zerstörung */

public func Destroyed()
{
  //Effekt
  Sound("Building_BaseDown.ogg",true,0,0,GetOwner()+1);

  return _inherited(...);
}