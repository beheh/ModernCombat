/*-- Fusionsreaktor --*/

#strict 2
#include CCBS

local basement;	//Fundament

public func TechLevel()		{return 1;}	//Techstufe
public func BuildingRadius()	{return 200;}	//Bauradius
public func EnergyProduction()	{return 100;}	//Energiehersteller


/* Konstruktion */

protected func Construction()
{
  basement = CreateObject(BT02,0,8,GetOwner());
}

/* Initalisierung */

protected func Initialize()
{
  Sound("CCBS_PowerUp.ogg");
  return _inherited(...);
}

/* Effekte */

private func Smoke()
{
  //Effekte
  CastSmoke("Smoke",3,10,-20,-30,100,500);
  CastSmoke("Smoke",3,10,40,-30,100,500);
}