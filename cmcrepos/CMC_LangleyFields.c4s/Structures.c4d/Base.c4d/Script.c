/*-- Stützpunkt --*/

#strict 2
#include CCBS

local basement;	//Fundament

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
  Sound("CCBS_PowerUp.ogg");
  return _inherited(...);
}
