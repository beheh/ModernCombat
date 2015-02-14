/*-- Fusionsreaktor --*/

#strict 2
#include CCBS
#include BT04

local aPUpgrades, iAdditionalEnergy;

public func TechLevel()				{return TECHLEVEL_1;}		//Benötigte Techstufe
public func ProvideTechLevel()			{return TECHLEVEL_2;}		//Vorhandene Techstufe
public func BuildingRadius()			{return 200;}			//Bauradius
public func EnergyProduction()			{return 200;}			//Energiehersteller
public func AdditionalEnergyProduction()	{return iAdditionalEnergy;}	//Zusätzliche Energie
public func PossibleUpgrades()			{return aPUpgrades;}		//Mögliche Upgrades
public func MaxDamage()				{return 150;}			//Maximaler Schadenswert bis zur Zerstörung


/* Konstruktion */

protected func Construction()
{
  aPUpgrades = [U_FR, U_SP, U_WT];
  return _inherited(...);
}

/* Initalisierung */

protected func Initialize()
{
  //Effekte
  var deco = CreateObject(CGLW,0,8,GetOwner());
  AddObject(deco);
  AddEffect("SmokeEffects", this, 100, 26, this);
  Sound("CFRT_PowerOn.ogg");
  Sound("CFRT_Fuse.ogg",false,0,75,0,+1);

  return _inherited(...);
}

/* Zerstörung */

public func Destroyed()
{
  //Effekt
  Sound("CFRT_Fuse.ogg",false,0,0,0,-1);

  return _inherited(...);
}

/* Effekte */

public func FxSmokeEffectsTimer()
{
  Smoke();

  return true;
}

private func Smoke()
{
  //Effekte
  CastSmoke("Smoke",3,10,-20,-35,100,500);
  CastSmoke("Smoke",3,10,40,-35,100,500);
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
    var x, y;
    FindWaterSource(x, y);
    iX = -10*Sgn(-x);
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
  //Offsets für Upgrade rausfinden
  var x,y,active;
  pTarget->~UpgradeOffset(U_SP, x, y);
  x += GetX(pTarget);
  y += GetY(pTarget);

  var time;

  //Nur tagsüber benutzbar
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

public func FindWaterSource(int &iX, int &iY)
{
  //10px zu beiden Seiten hin nach Wasser absuchen

  //Es fehlen noch erweiterte Checks ob es sich um ein "größeres Gewösser" handelt
  for(var i = -64;i > -75; i--)
  {
    var x = GetX()+i, y = GetY()+40,xdir,ydir;
    SimFlight(x,y,xdir,ydir,25);
    
    if(Abs(y-GetY()-40) < 15 && GetMaterial(AbsX(x), AbsY(y)+11) == Material("Water"))
    {
      iX = AbsX(x);
      iY = AbsY(y)+11;
      return true;
    }
  }
  for(i = 64;i < 75; i++)
  {
    var x = GetX()+i, y = GetY()+40,xdir,ydir;
    SimFlight(x,y,xdir,ydir,25);
    
    if(Abs(y-GetY()-40) < 15 && GetMaterial(AbsX(x), AbsY(y)+11) == Material("Water"))
    {
      iX = AbsX(x);
      iY = AbsY(y)+11;
      return true;
    }
  }

  return false;
}

public func SetupWaterTurbine()
{
  //Eventuelle Starteffekte/-sounds  
  AddEffect("WaterTurbine", this, 100, 35, this);
  return true;
}

public func FxWaterTurbineStart(object pTarget, int iNr)
{
  FindWaterSource(EffectVar(0, pTarget, iNr), EffectVar(1, pTarget, iNr));

  //Starteffekte
  Sound("MotorLoop.ogg", 0, this, 40, 0, +1);
  return true;
}

public func FxWaterTurbineTimer(object pTarget, int iNr)
{
  var x = EffectVar(0, pTarget, iNr), y = EffectVar(1, pTarget, iNr);
  if(GetMaterial(x, y) == Material("Water"))
    SetAdditionalEnergy(50);

  //Effekte


  return true;
}

public func FxWaterTurbineStop(object pTarget, int iNr)
{
  //Stopeffekte (Gebäude zerstört)
  Sound("MotorLoop.ogg", 0, this, 40, 0, -1);

  return true;
}

/* Upgrade: Windanlage */

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

  //Zusätzliche Energie abhängig von Windstärke (0 bis 50 Energie)
  pTarget->~SetAdditionalEnergy(Abs(GetWind(x, y))/2);

  return true;
}