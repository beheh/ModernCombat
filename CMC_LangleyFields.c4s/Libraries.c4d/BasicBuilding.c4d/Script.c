/*-- Grundgebäude --*/

#strict 2
#include DOOR	//Clonk-eigenes Türsystem

local iEnergyAmount, aObjectList, aUpgradeList;

public func IsCMCBuilding()		{return true;}

public func IsBase()			{return false;}					//Heimatbasis
public func BuildingRadius()		{return 0;}					//Bauradius
public func NeedBuildingRadius()	{return true;}					//Nur in Bauradius
public func TechLevel()			{return 0;}					//Benötigte Techstufe

public func RequiredEnergy()		{return 0;}					//Benötigte Energie
public func EnergyProduction()		{return 0;}					//Erzeugt Energie
public func HasEnoughEnergy()		{return !CurrentRequiredEnergy();}		//Energieversorgung
public func CurrentRequiredEnergy()	{return RequiredEnergy()-iEnergyAmount;}	//Benötigte Energie
public func PossibleUpgrades()		{return [];}					//Mögliche Upgrades
public func MaxDamage()			{return 500;}					//Maximaler Schadenswert bis zur Zerstörung

public func BuyCategory() {return C4D_All;}

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

public func GetUpgradeList()	{return aUpgradeList;}

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

public func MenuHeader(object pMenuObj, string szName)
{
	CloseMenu(pMenuObj);
  CreateMenu(GetID(), pMenuObj, this, C4MN_Extra_None, Format("%s - %s", GetName(this), szName), 0, C4MN_Style_Dialog);
}

public func ContainedUp(object pCaller)
{
  OpenBuildingMenu(pCaller);
  return true;
}

public func OpenBuildingMenu(object pMenuObj)
{
  //Menü erstellen
  CreateMenu(GetID(), pMenuObj, this, C4MN_Extra_None, GetName(this), 0, C4MN_Style_Dialog);

  //Gebäudespezifische Menüeinträge
  AdditionalBuildingMenu(pMenuObj);
  //Upgrademenü
  AddMenuItem("$UpgradeMenu$", "OpenUpgradeMenu", CCUS, pMenuObj, 0, pMenuObj);
  //Kaufmenü
  if(this->~IsBase())
    AddMenuItem("$BuyMenu$", "OpenBuyMenu", GOLD, pMenuObj, 0, pMenuObj);

  return true;
}

public func AdditionalBuildingMenu(object pMenuObj)
{
}

public func OpenUpgradeMenu(id dummy, object pMenuObj)
{
	MenuHeader(pMenuObj, "$UpgradeMenu$");

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

public func CountHombeaseMaterial(int iPlr, int iOffset, bool fHide)
{
	var i = iOffset,j,def;
	while(def = GetHomebaseMaterial(iPlr, 0, i++, BuyCategory()))
	{
		if(fHide)
		{
			if(GetHomebaseMaterial(iPlr, def))
				j++;
		}
		else
			j++;
	}

	return j;
}

public func OpenBuyMenu(id dummy, object pMenuObj, int iOffset)
{
	MenuHeader(pMenuObj, "$BuyMenu$");

	var def = 0, i = iOffset, plr = GetOwner(pMenuObj), sel, sel2;
	while(def = GetHomebaseMaterial(plr, 0, i++, BuyCategory()))
	{
		var amount = GetHomebaseMaterial(plr, def);
		
		if(amount)
		{
			if(GetWealth(GetOwner(pMenuObj)) > GetValue(0, def))
			{
				AddMenuItem(GetName(0, def), "ProcessBuy", def, pMenuObj, amount, iOffset);
				if(def != dummy)
					sel++;
				else
					sel2 = true;
			}
			else
				AddMenuItem(Format("<c 990000>%s</c>", GetName(0, def)), 0, def, pMenuObj, amount, iOffset);
		} 
		else
			AddMenuItem(Format("<c 777777>%s</c>", GetName(0, def)), 0, def, pMenuObj, amount, iOffset);
	}
	
	if(sel2)
		SelectMenuItem(sel, pMenuObj);

	//todo: 
	// - Kaufmenü auf Seitensystem umbasteln (ähnlich wie Achievements)
	// - Nicht verfügbare Items ggf. ausblenden (fHide)
	
	return true;
}

public func ProcessBuy(id idItem, object pMenuObj, int iOffset)
{
	if(GetWealth(GetOwner(pMenuObj)) > GetValue(0, idItem))
		Buy(idItem, GetOwner(pMenuObj), GetOwner(pMenuObj), this);

	return OpenBuyMenu(idItem, pMenuObj, iOffset);
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

public func OnUpgrade(id idUpgrade)	{}
public func OnAddObject(object pObj)	{}
