/*-- St�tzpunkt --*/

#strict 2
#include CCBS
#include BT02

public func TechLevel()			{return TECHLEVEL_Start;}	//Ben�tigte Techstufe
public func ProvideTechLevel()		{return TECHLEVEL_1;}		//Vorhandene Techstufe
public func IsBase()			{return true;}			//Heimatbasis
public func NeedBuildingRadius()	{return false;}			//Ben�tigt keinen Bauradius
public func BuildingRadius()		{return 400;}			//Bauradius
public func EnergyProduction()		{return 50;}			//Energiehersteller
public func PossibleUpgrades()		{return [U_T2, U_T3];}		//M�gliche Upgrades
public func CanNotBeSold()  {return true;}  //Kann nicht verkauft werden


/* Konstruktion */

protected func Construction()
{
  //Techlevel anpassen
  SetTeamTechLevel(GetPlayerTeam(GetOwner()), TECHLEVEL_Start, false);

  return _inherited(...);
}

/* Initalisierung */

protected func Initialize()
{
  //Effekte
  var deco = CreateObject(OFLG,-27,-38);
  SetOwner(GetOwner(), deco);
  AddObject(deco);
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
