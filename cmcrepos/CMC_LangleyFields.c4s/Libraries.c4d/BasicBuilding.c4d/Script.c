/*-- Grundgebäude --*/

#strict 2
#include DOOR	//Clonk-eigenes Türsystem

local iEnergyAmount, aObjectList, aUpgradeList;

public func IsCMCBuilding()		{return true;}

public func IsBase()					  {return false;}				//Heimatbasis
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
  OnAddObject(pObj);
  return true;
}

public func AddUpgrade(id idUpgrade)
{
	if(!aUpgradeList)
		aUpgradeList = [];
	
	aUpgradeList[GetLength(aUpgradeList)] = idUpgrade;
	OnUpgrade(idUpgrade);
	return true;
}

public func GetUpgradeList() { return aUpgradeList; }

/* Zerstörung */

public func Destruction()
{
	//Zugehörige Objekte mitlöschen
	for(var obj in aObjectList)
		if(obj)
			RemoveObject(obj);
	
	return true;
}

/* Gebäudemenü */

public func ContainedUp(object pCaller)
{
	OpenBuildingMenu(pCaller);
	return true;
}

public func OpenBuildingMenu(object pMenuObj)
{
	CreateMenu(GetID(), pMenuObj, this, C4MN_Extra_None, GetName(this), 0, C4MN_Style_Dialog);
	//Gebäudespezifische Menüeinträge
	if(AdditionalBuildingMenu())
		AddMenuItem(" ", 0, 0, pMenuObj);
	
	//Upgrademenü
	AddMenuItem("$UpgradeMenu$", "OpenUpgradeMenu", CCUS, pMenuObj, 0, pMenuObj);
	//Kaufmenü
	if(this->~IsBase())
		AddMenuItem("$BuyMenu$", "OpenBuyMenu", GOLD, pMenuObj, 0, pMenuObj);
	
	return true;
}

public func AdditionalBuildingMenu(object pMenuObj) { }

public func OpenUpgradeMenu(id dummy, object pMenuObj)
{
	CloseMenu(pMenuObj);
	CreateMenu(GetID(), pMenuObj, this, C4MN_Extra_None, Format("%s - $UpgradeMenu$", GetName(this)), 0, C4MN_Style_Dialog);
	
	for(var upgrade in PossibleUpgrades())
	{
		if(GetIndexOf(upgrade, aUpgradeList) > -1)
			AddMenuItem(Format("<c 777777>%s</c>", GetName(0, upgrade)), 0, upgrade, pMenuObj);
		else if(!upgrade->~CanBeResearched(this))
			AddMenuItem(Format("<c FF0000>%s</c>", GetName(0, upgrade)), 0, upgrade, pMenuObj);
		else
			AddMenuItem(GetName(0, upgrade), "StartUpgrade", upgrade, pMenuObj, 0, pMenuObj);
	}
	
	return true;
}

public func OpenBuyMenu(id dummy, object pMenuObj)
{
	
}

/* Energieversorgung */

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
public func OnAddObject(object pObj) { } 
