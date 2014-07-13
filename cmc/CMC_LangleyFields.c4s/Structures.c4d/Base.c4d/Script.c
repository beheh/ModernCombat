/*-- Stützpunkt --*/

#strict 2
#include CCBS

public func IsBase()			{return true;}	//Heimatbasis
public func NeedBuildingRadius()	{return false;}	//Benötigt keinen Bauradius
public func BuildingRadius()		{return 400;}	//Bauradius
public func EnergyProduction()		{return 50;}	//Energiehersteller
public func BasementID()		{return BT02;}	//Fundament


/* Bau */

public func BuildingConditions(object pBy, int iX, int iY)	{return !GetTeamTechLevel(GetPlayerTeam(GetOwner(pBy)));}

public func Construction()
{
  //Techlevel anheben
  SetTeamTechLevel(GetPlayerTeam(GetOwner()), 1);

  return _inherited(...);
}

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