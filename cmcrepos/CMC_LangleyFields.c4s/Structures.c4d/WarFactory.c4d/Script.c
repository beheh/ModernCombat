/*-- Fahrzeugfabrik --*/

#strict 2
#include CCBS

local basement;	//Fundament

public func TechLevel()		{return 3;}	//Techstufe
public func BuildingRadius()	{return 200;}	//Bauradius
public func RequiredEnergy() 	{return 100;}	//Energieverbraucher


/* Konstruktion */

protected func Construction()
{
  basement = CreateObject(BT04,0,8,GetOwner());
}

/* Initalisierung */

protected func Initialize()
{
  Sound("CCBS_PowerUp.ogg");
  return _inherited(...);
}