/*-- Stützpunkt --*/

#strict 2
#include CCBS
#include BT02

public func TechLevel()			{return TECHLEVEL_Start;}		//Benötigte Techstufe
public func ProvideTechLevel()		{return TECHLEVEL_1;}			//Vorhandene Techstufe
public func IsBase()			{return true;}				//Heimatbasis
public func NeedBuildingRadius()	{return false;}				//Benötigt keinen Bauradius
public func BuildingRadius()		{return 400;}				//Bauradius
public func EnergyProduction()		{return 50;}				//Energiehersteller
public func PossibleUpgrades()		{return [U_SB, U_SU, U_T2, U_T3];}	//Mögliche Upgrades
public func CanNotBeSold()		{return true;}				//Nicht verkaufbar


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

/* Zerstörung */

public func Destroyed()
{
  //Effekt
  Sound("Building_BaseDown.ogg",true,0,0,GetOwner()+1);

  return _inherited(...);
}

/* Upgrades */

public func OnUpgrade(id idUpgrade)
{
  //Sandsackbarriere
  if(idUpgrade == U_SB)
  {
    AddEffect("SandbagBarrier", this, 100, 0, 0, U_SB);

    //Grafikänderungen/Effekte/Sounds
  }

  return true;
}

public func OnUpgradeRemoved(id idUpgrade)
{
  //Sandsackbarriere
  if(idUpgrade == U_SB)
  {
    //Effekte
  }

  return true;
}

/* Sandsackbarriere */

public func OnDmg(int iDmg, int iType)
{
  if(iType == DMG_Explosion) return 50+25*GetUpgrade(U_SB);
  return _inherited(iDmg, iType, ...);
}