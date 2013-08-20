/*-- Grundgebäude --*/

#strict 2
#include DOOR	//Clonk-eigenes Türsystem

local iEnergyAmount, aObjectList, aUpgradeList;

public func IsCMCBuilding()		{return true;}

public func BuildingRadius()		{return 0;}				//Bauradius
public func NeedBuildingRadius()	{return true;}				//Nur in Bauradius
public func TechLevel()			{return 0;}				//Benötigte Techstufe

public func RequiredEnergy()		{return 0;}				//Benötigte Energie
public func EnergyProduction()		{return 0;}				//Erzeugt Energie
public func HasEnoughEnergy()		{return !CurrentRequiredEnergy();}
public func CurrentRequiredEnergy()	{return RequiredEnergy()-iEnergyAmount;}

public func PossibleUpgrades() {return [];}

public func MaxDamage()			{return 500;}				//Maximaler Schadenswert bis zur Zerstörung


/* Bauanforderungen */

public func BuildingConditions(object pBy, int iX, int iY) {return true;}

/* Konstruktion */

protected func Construction()
{
}

/* Initialisierung */

public func Initialize()
{
	for(var upgrade in PossibleUpgrades())
		if(upgrade->~IsGroupUpgrade() && GetTeamUpgrade(GetPlayerTeam(GetOwner()), upgrade))
			AddUpgrade(upgrade);
	
	return true;
}

public func AddObject(object pObj)
{
	if(!aObjectList)
		aObjectList = [];
	
  aObjectList[GetLength(aObjectList)] = pObj;
  this->~OnAddObject(pObj);
  return true;
}

public func AddUpgrade(id idUpgrade)
{
	if(!aUpgradeList)
		aUpgradeList = [];
	
	aUpgradeList[GetLength(aUpgradeList)] = idUpgrade;
	this->~OnUpgrade(idUpgrade);
	return true;
}

/* Zerstörung */

public func Destruction()
{
	//Zugehörige Objekte mitlöschen
	for(var obj in aObjectList)
		if(obj)
			RemoveObject(obj);
	
	return true;
}

public func EnergySupply(int iAmount, object pBy)
{
  iEnergyAmount += iAmount;
  return true;
}

public func ResetCurrentEnergy()	{iEnergyAmount = 0;}

public func SendEnergy()
{
  var energy = EnergyProduction();
  if(energy <= 0)
    return;

  var plr = GetOwner();
  var objects = FindObjects(Find_Func("CurrentRequiredEnergy"), Find_Allied(plr), Sort_Distance());
  for(var obj in objects)
  {
    if(energy <= 0)
      break;

    var supply = Min(energy, obj->RequiredEnergy());
    obj->~EnergySupply(supply, this);
    energy -= supply;
  }

  return energy;
}

public func OnUpgrade(id idUpgrade) { }
