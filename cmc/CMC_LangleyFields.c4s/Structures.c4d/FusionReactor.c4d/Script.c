/*-- Fusionsreaktor --*/

#strict 2
#include CCBS

<<<<<<< HEAD
local basement;
=======
>>>>>>> * Geb√§udesystem: Fundamente gefixt, Geb√§udefundamente werden nun vom BasicBuilding erstellt
local aPUpgrades, iAdditionalEnergy;

public func TechLevel()		{return 1;}			//Techstufe
public func BuildingRadius()	{return 200;}			//Bauradius
public func EnergyProduction()	{return 100+iAdditionalEnergy;}	//Energiehersteller
<<<<<<< HEAD
public func PossibleUpgrades()	{return aPUpgrades;}

=======
public func PossibleUpgrades()   {return aPUpgrades;}
public func BasementID()			{return BT04;}
>>>>>>> * Geb√§udesystem: Fundamente gefixt, Geb√§udefundamente werden nun vom BasicBuilding erstellt

/* Konstruktion */

protected func Construction()
{
  aPUpgrades = [U_FR, U_SP, U_WA, U_WT];
  return _inherited(...);
}

/* Initalisierung */

protected func Initialize()
{
  //Techlevel anheben
  if(GetTeamTechLevel(GetPlayerTeam(GetOwner())) < 2)
    SetTeamTechLevel(GetPlayerTeam(GetOwner()), 2);

  //Effekte
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

/* Upgrades */

public func OnResearchingUpgradeStart(int iEffect, id idUpgrade, int iDuration, int iCost)
{
  //Alle anderen Erweiterungen rausnehmen
  aPUpgrades = [idUpgrade];

  return true;
}

public func OnUpgrade(id idUpgrade)
{
  //[Allgemeine Effekte/Sounds]

  if(idUpgrade == U_FR)
    iAdditionalEnergy = 40;
  else if(idUpgrade == U_SP)
    SetupSolarPanel();
  else if(idUpgrade == U_WA)
    SetupWaterTurbine();
  else if(idUpgrade == U_WT)
    SetupWindTurbine();
}

public func UpgradeOffset(id idUpgrade, int &iX, int &iY)
{
  if(idUpgrade == U_SP)		//Solarpanele
  {
     iX = -6;
     iY = -10;
  }
  else if(idUpgrade == U_WA)	//Wasserturbine
  {
    iX = -10;
    iY = 8;
  }
  else if(idUpgrade == U_WT)	//Windanlage
  {
    iX = 16;
    iY = -10;
  }
}

public func SetAdditionalEnergy(int iValue)	{return iAdditionalEnergy = iValue;}

/* Upgrade: Solarpanele */

public func SetupSolarPanel()
{
  //Eventuelle Starteffekte/-sounds
  AddEffect("SolarPanel", this, 100, 35, 0, GetID());
  return true;
}

public func FxSolarPanelTimer(object pTarget, int iNr)
{
  //Offsets f¸r Upgrade rausfinden
  var x,y,active;
  pTarget->~UpgradeOffset(U_SP, x, y);
  x += GetX(pTarget);
  y += GetY(pTarget);

  var time;

  //Nur tags¸ber benutzbar
  if((time = FindObject2(Find_ID(TIME))) && time->IsDay())
    active = true;
  //Hintergrund ist Himmel?
  else if(GetMaterial(x, y) == -1)
    active = true;
  else
  {
    //Alternativ: Nach oben hin frei?
    var simx = x, simy, simxdir, simydir = 1;
    SimFlight(simx, simy, simxdir, simydir);
    
    if(simy >= y)
      active = true;
  }

  if(active)
    pTarget->~SetAdditionalEnergy(50);
  else
    pTarget->~SetAdditionalEnergy(0);
}

/* Upgrade: Wasserturbine */

public func SetupWaterTurbine()
{
  //Eventuelle Starteffekte/-sounds  
  AddEffect("WaterTurbine", this, 100, 35, 0, GetID());
  return true;
}

public func FxWaterTurbineTimer(object pTarget, int iNr)
{
  //...todo
}

/* Upgrade: Windanlge */

public func SetupWindTurbine()
{
  //Eventuelle Starteffekte/-sounds  
  AddEffect("WindTurbine", this, 100, 35, 0, GetID());
  return true;
}

public func FxWindTurbineTimer(object pTarget, int iNr)
{
  var x,y;
  pTarget->~UpgradeOffset(U_WT, x, y);
  x += GetX(pTarget);
  y += GetY(pTarget);
  
  //Zus‰tzliche Energie abh‰ngig von Windst‰rke (0 bis 50 Energie)
  pTarget->~SetAdditionalEnergy(Abs(GetWind(x, y))/2);

  return true;
}
