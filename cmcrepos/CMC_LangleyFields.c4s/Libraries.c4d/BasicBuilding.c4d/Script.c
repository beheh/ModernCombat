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
  
  if(!aUpgradeList)
    aUpgradeList = [];

  if(!aObjectList)
    aObjectList = [];

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

/* Upgrades */

public func CanResearch(id idUpgrade)
{
	//Upgrade nicht erforschbar für dieses Gebäude
	if(GetIndexOf(idUpgrade, PossibleUpgrades()) == -1)
		return false;
	
	//Upgrade ist bereits erforscht
	if(GetIndexOf(idUpgrade, aUpgradeList) != -1)
		return false;
	
	//Ist gerade am erforschen
	if(GetEffect("ResearchingUpgrade", this))
		return false;
	
	//Hat genug Geld?
	if(GetWealth(GetOwner()) < idUpgrade->~ResearchCost())
		return false;
	
	//Erfordeliche Upgrades schon erforscht?
	if(!BaseUpgradesResearched(idUpgrade))
		return false;
	
	//Weitere Bedingungen
	if(!FurtherUpgradeConditions(idUpgrade, idUpgrade->~ResearchDuration(), idUpgrade->~ResearchCost()))
		return false;
	
	//Upgradeseitige Bedingungen
	if(!idUpgrade->~UpgradeConditions(this))
		return false;
	
	return true;
}

public func ResearchUpgrade(id idUpgrade)
{
	//Auf Bedingungen überprüfen
	if(!CanResearch(idUpgrade))
		return false;

	//Geld abziehen
	SetWealth(GetOwner(), GetWealth(GetOwner())-idUpgrade->~ResearchCost());
	
	//Upgrade entwickeln!
	var effect = AddEffect("ResearchingUpgrade", this, 1, idUpgrade->~ResearchDuration(), this, 0, idUpgrade);
	OnResearchingUpgradeStart(effect, idUpgrade, idUpgrade->~ResearchDuration(), idUpgrade->~ResearchCost());
	
	return effect;
}

public func FxResearchingUpgradeStart(object pTarget, int iNr, int iTemp, id idUpgrade)
{
	if(iTemp)
		return;

	EffectVar(0, pTarget, iNr) = idUpgrade;
	
	//Uralte Standard Clonk-Sounds op!!1 :D
	Sound("Research", 0, this, 100, 0, +1);
	return true;
}

public func FxResearchingUpgradeStop(object pTarget, int iNr)
{
	EffectVar(0, pTarget, iNr)->Researched(pTarget);
	Sound("Research", 0, this, 100, 0, -1);
	Sound("ResearchDone", 0, this);

	return true;
}

public func BaseUpgradesResearched(id idUpgrade)
{
	for(var baseupg in idUpgrade->~ResearchBase())
		if(baseupg)
			if(GetIndexOf(baseupg, aUpgradeList) == -1 && !GetTeamUpgrade(GetPlayerTeam(GetOwner()), baseupg))
				return false;
	
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
public func OnResearchingUpgradeStart(int iEffect, id idUpgrade, int iDuration, int iCost) {return;}
public func FurtherUpgradeConditions(id idUpgrade, int iDuration, int iCost) {return true;}

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
	if(GetMenu(pMenuObj))
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
    else if(!CanResearch(upgrade))
      AddMenuItem(Format("<c ff0000>%s</c>", GetName(0, upgrade)), 0, upgrade, pMenuObj);
    else
      AddMenuItem(GetName(0, upgrade), "ResearchUpgrade", upgrade, pMenuObj, 0, pMenuObj);
  }

  return true;
}

public func CountHomebaseMaterial(int iPlr, int iOffset, bool fHide)
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

public func OpenBuyMenu(id dummy, object pMenuObj, int iOffset, int iButton)
{
  MenuHeader(pMenuObj, "$BuyMenu$");
  
  var iMaterialCount = CountHomebaseMaterial(GetOwner(pMenuObj));
  
  //Seitenanzeige
  AddMenuItem(Format("<c 33ccff>$Showing$</c>", iOffset/10+1, (iMaterialCount)/10+1), 0, NONE, pMenuObj);

  var def = 0, i = iOffset, plr = GetOwner(pMenuObj), sel, sel2;
   
  while((def = GetHomebaseMaterial(plr, 0, i++, BuyCategory())) && i <= 10+iOffset)
  {
    var amount = GetHomebaseMaterial(plr, def);

    if(amount)
    {
      if(GetWealth(GetOwner(pMenuObj)) > GetValue(0, def))
      {
        AddMenuItem(GetName(0, def), Format("ProcessBuy(%i, Object(%d), %d)", def, ObjectNumber(pMenuObj), iOffset), def, pMenuObj, amount);
        if(def != dummy)
        {
        	if(!sel2)
          	sel++;
        }
        else
        {
          sel2 = true;
        	sel++;
        }
      }
      else
        AddMenuItem(Format("<c 990000>%s</c>", GetName(0, def)), 0, def, pMenuObj, amount);
    }
    else
      AddMenuItem(Format("<c 777777>%s</c>", GetName(0, def)), 0, def, pMenuObj, amount);
  }
  
  //Leerzeile
  AddMenuItem(" ", 0, NONE, pMenuObj);
	
	if(iOffset+10 <= iMaterialCount)
  	AddMenuItem("$NextPage$", Format("OpenBuyMenu(%i, Object(%d), %d, 1)", GetID(), ObjectNumber(pMenuObj), BoundBy(iOffset+10, 0, iMaterialCount)), NONE, pMenuObj, 0, 0, "", C4MN_Add_ForceNoDesc);

  //Zurück
  if(iOffset > 0)
    AddMenuItem("$LastPage$", Format("OpenBuyMenu(%i, Object(%d), %d, 2)", GetID(), ObjectNumber(pMenuObj), BoundBy(iOffset-10, 0, iMaterialCount)), NONE, pMenuObj, 0, 0, "", C4MN_Add_ForceNoDesc);
  else if(iButton == 2)
  	iButton = 1;

  if(sel2)
    SelectMenuItem(sel, pMenuObj);
  else if(iButton)
  	SelectMenuItem(i-iOffset+iButton, pMenuObj);

  //todo: 
  // - Selection-Verschiebung bei aufgekauften Items etc.
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
