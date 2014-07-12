/*-- Fahrstuhl --*/

#strict 2
#include CCBS

local basement;	//Fundament
local aPUpgrades, iAdditionalEnergy;

public func TechLevel()		{return 1;}	//Techstufe
public func BuildingRadius()	{return 200;}	//Bauradius
public func RequiredEnergy() 	{return 50;}	//Energieverbraucher


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