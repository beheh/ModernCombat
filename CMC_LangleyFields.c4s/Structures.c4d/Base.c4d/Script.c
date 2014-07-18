/*-- Stützpunkt --*/

#strict 2
#include CCBS
#include BT02

local flag;

public func TechLevel()			{return TECHLEVEL_Start;}	//Benötigte Techstufe
public func ProvideTechLevel()		{return TECHLEVEL_1;}		//Vorhandene Techstufe
public func IsBase()			{return true;}			//Heimatbasis
public func NeedBuildingRadius()	{return false;}			//Benötigt keinen Bauradius
public func BuildingRadius()		{return 400;}			//Bauradius
public func EnergyProduction()		{return 50;}			//Energiehersteller

public func PossibleUpgrades()    {return [U_T2, U_T3];}


/* Initalisierung */

protected func Construction()
{
  SetTeamTechLevel(GetPlayerTeam(GetOwner()), TECHLEVEL_Start, false);

  return _inherited(...);
}

protected func Initialize()
{
  //Flagge
  flag = CreateObject(OFLG,-27,-38);
  //notwendig, da die Flagge den Besitzer in Initialize löscht
  SetOwner(GetOwner(), flag);
  
  //Effekt
  Sound("Building_BaseUp.ogg",true,0,0,GetOwner()+1);

  return _inherited(...);
}

/* Zerstörung */

public func Destroyed()
{
  //Effekt
  Sound("Building_BaseDown.ogg",true,0,0,GetOwner()+1);
  
  //Flagge wegmachen
  RemoveObject(flag);
  
  return _inherited(...);
}
