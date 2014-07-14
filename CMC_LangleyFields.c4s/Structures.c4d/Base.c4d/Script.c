/*-- St�tzpunkt --*/

#strict 2
#include CCBS

//Technikstufe
public func TechLevel()   {return TECHLEVEL_Start;}
public func ProvideTechLevel() {return TECHLEVEL_1;}

public func IsBase()			{return true;}	//Heimatbasis
public func NeedBuildingRadius()	{return false;}	//Ben�tigt keinen Bauradius
public func BuildingRadius()		{return 400;}	//Bauradius
public func EnergyProduction()		{return 50;}	//Energiehersteller
public func BasementID()		{return BT02;}	//Fundament

/* Initalisierung */

protected func Initialize()
{
  SetTeamTechLevel(GetPlayerTeam(GetOwner()), TECHLEVEL_Start, false);

  //Effekt
  Sound("Building_BaseUp.ogg",true,0,0,GetOwner()+1);

  return _inherited(...);
}

/* Zerst�rung */

public func Destroyed()
{
  //Effekt
  Sound("Building_BaseDown.ogg",true,0,0,GetOwner()+1);

  return _inherited(...);
}
