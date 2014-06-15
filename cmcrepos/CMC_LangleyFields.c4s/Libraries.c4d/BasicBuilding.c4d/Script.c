/*-- Grundgeb�ude --*/

#strict 2
#include DOOR	//Clonk-eigenes T�rsystem

local iEnergyAmount, aObjectList, aUpgradeList, fDestroyed, iLastAttacker;

public func IsCMCBuilding()		{return true;}

public func IsBase()			{return false;}					//Heimatbasis
public func BuildingRadius()		{return 0;}					//Bauradius
public func NeedBuildingRadius()	{return true;}					//Nur in Bauradius
public func TechLevel()			{return 0;}					//Ben�tigte Techstufe

public func RequiredEnergy()		{return 0;}					//Ben�tigte Energie
public func EnergyProduction()		{return 0;}					//Erzeugt Energie
public func HasEnoughEnergy()		{return !CurrentRequiredEnergy();}		//Energieversorgung
public func CurrentRequiredEnergy()	{return RequiredEnergy()-iEnergyAmount;}	//Ben�tigte Energie
public func PossibleUpgrades()		{return [];}					//M�gliche Upgrades
public func MaxDamage()			{return 500;}					//Maximaler Schadenswert bis zur Zerst�rung
public func IsDestroyed()   {return fDestroyed;}

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
	//Upgrade nicht erforschbar f�r dieses Geb�ude
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
	//Auf Bedingungen �berpr�fen
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

public func GetUpgradeResearchProgress() {return GetEffect("ResearchingUpgrade", this, 0, 6);}
public func GetUpgradeResearchDuration() {return GetEffect("ResearchingUpgrade", this, 0, 3);}
public func IsResearchingUpgrades() {return GetEffect("ResearchingUpgrade", this);}

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

/* Schaden */

public func Damage(int change)
{
  //Struktur zerst�ren, aber mehr Schaden als den Maximalen nicht zulassen
  if(change > 0 && IsDestroyed())
  {
    if(GetDamage() > MaxDamage())
      DoDamage(-(GetDamage()-MaxDamage()));
  }

  //Bei h�herem Schaden als dem Maximalen entsprechend zerst�ren
  if(GetDamage() > MaxDamage() && !IsDestroyed())
  {
    Destroyed();
  }

  //Bei beendetem Reparaturvorgang Sonderfunktionen aufrufen
  /*if(IsDestroyed() && GetDamage() == 0 && !IsRepairing())
  {
    fDestroyed = false;
    ObjectSetAction(this, "RepairStop");
    OnRepair();
  }*/
}

public func Destroyed()
{
  //Status setzen
  //SetAction("Destroyed");
  fDestroyed = true;

  //Schaden auf Maximalwert setzen
  if(GetDamage() > MaxDamage())
    DoDamage(-(GetDamage()-MaxDamage()));

  //Punkte bei Belohnungssystem (Strukturzerst�rung)
  /*if(BonusPointCondition() && iLastAttacker != -1)
    if((GetOwner() != -1 && Hostile(GetOwner(), iLastAttacker)) || (GetOwner() == -1 && !GetTeam(this)) || (GetTeam(this) != GetPlayerTeam(iLastAttacker)))
      DoPlayerPoints(BonusPoints("Destruction"), RWDS_BattlePoints, iLastAttacker, GetCursor(iLastAttacker), IC03);*/

  //Explosion
  FakeExplode(20, iLastAttacker+1);

  //Sound
  Sound("Blast2", false, this);

  //Letzen Angreifer zur�cksetzen
  iLastAttacker = -1;

  //Callback
  OnDestruction();
  
  //L�schen planen (Momentan noch nicht besonders fancy)
  Schedule("RemoveObject()", 35*6, 1, this);
}

public func OnDestruction() {}

public func OnDmg(int iDmg, int iType)
{
  return 50;	//Default
}

public func OnHit(int iDmg, int iType, object pBy)
{
  if(!IsDestroyed())	
    iLastAttacker = GetController(pBy);
}
/* Zerst�rung */

public func Destruction()
{
  //Zugeh�rige Objekte mitl�schen
  for(var obj in aObjectList)
    if(obj)
      RemoveObject(obj);

  return true;
}

/* Geb�udemen� */

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
  //Men� erstellen
  CreateMenu(GetID(), pMenuObj, this, C4MN_Extra_None, GetName(this), 0, C4MN_Style_Dialog);
  
  AddMenuItem(Format("$Hitpoints$", GetDamage(), MaxDamage()), 0, NONE, pMenuObj);
  
  AddMenuItem(" ", 0, NONE, pMenuObj); 

  //Geb�udespezifische Men�eintr�ge
  AdditionalBuildingMenu(pMenuObj);
  //Upgrademen�
  AddMenuItem("$UpgradeMenu$", "OpenUpgradeMenu", CCUS, pMenuObj, 0, pMenuObj);
  //Kaufmen�
  if(this->~IsBase())
    AddMenuItem("$BuyMenu$", "OpenBuyMenu", GOLD, pMenuObj, 0, pMenuObj);

  return true;
}

public func AdditionalBuildingMenu(object pMenuObj) {}

public func OpenUpgradeMenu(id dummy, object pMenuObj)
{
  MenuHeader(pMenuObj, "$UpgradeMenu$");
  
  if(!IsResearchingUpgrades())
  	AddMenuItem("$NoUpgrades$", 0, NONE, pMenuObj);
	else
	{
		var upgradeID = EffectVar(0, this, IsResearchingUpgrades());
		AddMenuItem(Format("$IsUpgrading$", GetName(0, upgradeID), GetUpgradeResearchProgress(), GetUpgradeResearchDuration()), 0, upgradeID, pMenuObj);
	}
	
	//Leerzeile
	AddMenuItem(" ", 0, NONE, pMenuObj);
	
	var aMenuUpgradeList = [[],[]];
	
	aMenuUpgradeList[0] = aUpgradeList;

  for(var upgrade in PossibleUpgrades())
    if(GetIndexOf(upgrade, aUpgradeList) == -1)
			aMenuUpgradeList[1][GetLength(aMenuUpgradeList[1])] = upgrade;

	if(GetLength(aMenuUpgradeList[0]))
	{
		AddMenuItem("$ActiveUpgrades$", 0, NONE, pMenuObj);
		for(var upgrade in aMenuUpgradeList[0])
		  AddMenuItem(Format("%s", GetName(0, upgrade)), 0, upgrade, pMenuObj);
	}
	if(GetLength(aMenuUpgradeList[1]))
	{
		AddMenuItem("$UpgradesToResearch$", 0, NONE, pMenuObj);
		for(var upgrade in aMenuUpgradeList[1])
		{
		  if(!CanResearch(upgrade))
		    AddMenuItem(Format("<c ff0000>%s</c>", GetName(0, upgrade)), 0, upgrade, pMenuObj);
		  else
		    AddMenuItem(GetName(0, upgrade), "ResearchUpgrade", upgrade, pMenuObj, 0, pMenuObj);
		}
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

  //Zur�ck
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
  // - Nicht verf�gbare Items ggf. ausblenden (fHide)

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
