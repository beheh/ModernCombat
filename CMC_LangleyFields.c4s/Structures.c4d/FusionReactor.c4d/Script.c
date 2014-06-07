/*-- Fusionsreaktor --*/

#strict 2
#include CCBS

local basement;	//Fundament
local aPUpgrades, iAdditionalEnergy;

public func TechLevel()		{return 1;}	//Techstufe
public func BuildingRadius()	{return 200;}	//Bauradius
public func EnergyProduction()	{return 100+iAdditionalEnergy;}	//Energiehersteller
public func PossibleUpgrades()  {return aPUpgrades;}


/* Konstruktion */

protected func Construction()
{
  basement = CreateObject(BT02,0,8,GetOwner());
  aPUpgrades = [U_FR, U_SP, U_WA, U_WT];
}

/* Initalisierung */

protected func Initialize()
{
  Sound("CCBS_PowerUp.ogg");
  Sound("CFRT_Fuse.ogg",50,0,0,0,+1);
  return _inherited(...);
}

/* Effekte */

private func Smoke()
{
  //Effekte
  CastSmoke("Smoke",3,10,-20,-30,100,500);
  CastSmoke("Smoke",3,10,40,-30,100,500);
}

public func OnResearchingUpgradeStart(int iEffect, id idUpgrade, int iDuration, int iCost)
{
	//Alle anderen Erweiterungen rausnehmen
	aPUpgrades = [idUpgrade];

	return true;
}

public func OnUpgrade(id idUpgrade)
{
	//Vorab keine wirkliche Funktion in Upgrades;

	//ToDo: Ortsabhängiges Arbeiten der Upgrades anhand von Effekten

	if(idUpgrade == U_FR)
		iAdditionalEnergy = 40;
	else
		iAdditionalEnergy = 50;
}
