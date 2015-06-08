/*-- Grundgebäude --*/

#strict 2
#include DOOR	//Clonk-eigenes Türsystem

local fHasEnergy, aObjectList, aUpgradeList, fDestroyed, iLastAttacker, aScaffolds;

public func IsCMCBuilding()			{return true;}			//Ist CMC-Gebäude
public func ScaffoldID()			{return SFFD;}			//Gerüst-ID 

public func ProvideTechLevel()			{return TECHLEVEL_None;}	//Vorhandener Techlevel
public func TechLevel()				{return 0;}			//Benötigte Techstufe
public func IsBase()				{return false;}			//Heimatbasis
public func BuildingRadius()			{return 0;}			//Bauradius
public func NeedBuildingRadius()		{return true;}			//Nur in Bauradius

public func RequiredEnergy()			{return 0;}			//Benötigte Energie
public func AdditionalRequiredEnergy()		{return 0;}			//Zusätzlich benötigte Energie
public func EnergyProduction()			{return 0;}			//Erzeugt Energie
public func AdditionalEnergyProduction()	{return 0;}			//Zusätzlich erzeugte Energie
public func PossibleUpgrades()			{return [];}			//Mögliche Upgrades
public func MaxDamage()				{return 500;}			//Maximaler Schadenswert bis zur Zerstörung
public func IsDestroyed()			{return fDestroyed;}
public func IsRepairable()			{return true;}			//Ist reparierbar
public func BuyCategory()			{return C4D_All;}

public func BombDamage()			{return 502;}			//Schaden, den die Gebäudebombe macht
public func BombPlaceTime()			{return 36;}			//Platzierungszeit in Frames
public func BombDefuseTime()			{return 36*5;}			//Entschärfungszeit in Frames
public func BombExplodeTime()			{return 36*15;}			//Explosionszeit in Frames
public func BombDefuseRadius()			{return GetDefWidth();}		//Entschärfungsradius von der Gebäudemitte aus

/* Bauanforderungen */

public func BuildingConditions(object pBy, int iX, int iY, bool fReturnError) {return true;}


/* Initialisierung */

public func Initialize()
{
  //potentielle Baugerüste entfernen
  if(aScaffolds)
    RemoveBuildScaffolds(aScaffolds);

  var team = GetPlayerTeam(GetOwner());
  if(ProvideTechLevel())
  {
    var def, i = ProvideTechLevel(), lvl = -1;
    while(i)
    {
      lvl++;
      i /= 2;
    }

    //Techlevel erforschbar?
    if(GetName(0, def = C4Id(Format("U_T%d", lvl))))
    {
      //Nur wenn Upgrade erforscht wurde
      if(GetTeamUpgrade(GetPlayerTeam(GetOwner()), def))
        SetTeamTechLevel(team, ProvideTechLevel(), true);
    }
    else
      SetTeamTechLevel(team, ProvideTechLevel(), true);
  }

  for(var upgrade in PossibleUpgrades())
    if(upgrade->~IsGroupUpgrade() && GetTeamUpgrade(GetPlayerTeam(GetOwner()), upgrade))
      AddUpgrade(upgrade);
  
  if(!aUpgradeList)
    aUpgradeList = [];

  if(!aObjectList)
    aObjectList = [];
  
  //Effekt zum Anzeigen der Gebäudebalken
  AddEffect("ShowHealthBar", this, 100, 3, this);

  return true;
}

/* Baugerüst */

public func Construction()
{
  aScaffolds = CreateBuildScaffolds(this);
}

public func AddObject(object pObj)
{
  if(!aObjectList)
    aObjectList = [];

  aObjectList[GetLength(aObjectList)] = pObj;
  OnAddObject(pObj);
  return true;
}

/* Eingangskontrolle */

protected func ActivateEntrance(object pObj)
{
  //Objekt kann nicht eintreten wenn am hangeln, klettern oder in der Luft
  if(GetProcedure(pObj) == "FLIGHT" || GetProcedure(pObj) == "HANGLE" || GetProcedure(pObj) == "SCALE" || GetProcedure(pObj) == "KNEEL")
    return false;

  //kann nicht eintreten, wenn Feind
  if(Hostile(GetOwner(), GetOwner(pObj)) || HostileTeam(GetPlayerTeam(GetOwner()), GetPlayerTeam(GetOwner(pObj))))
    return false;
	
  return _inherited(pObj, ...);
}

protected func RejectCollect(id objID, object pObj)
{ 
  //kann nicht eintreten, wenn Feind
  if(Hostile(GetOwner(), GetOwner(pObj)) || HostileTeam(GetPlayerTeam(GetOwner()), GetPlayerTeam(GetOwner(pObj))))
    return true;
	
  return false;
}

/* Türsteuerung */

private func SoundOpenDoor()
{
  return(Sound("DOOR_Open.ogg"));
}

private func SoundCloseDoor()
{
  return(Sound("DOOR_Close.ogg"));
}

/* Techlevels */

public func CheckProvideTechLevel(int iLevel)	{return ProvideTechLevel() == iLevel;}
public func ResetTechLevel()
{
  if(ProvideTechLevel())
    if(!FindObject2(Find_Category(C4D_Structure), Find_Func("CheckProvideTechLevel", ProvideTechLevel()), Find_Not(Find_Func("IsDestroyed")), Find_Allied(GetOwner()), Find_Exclude(this)))
      SetTeamTechLevel(GetPlayerTeam(GetOwner()), ProvideTechLevel(), false);

  return true;
}

public func OnTechLevelChange(int iLevel, bool fActive)	{}

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

  //Upgrade entwickeln
  var effect = AddEffect("ResearchingUpgrade", this, 1, 1, this, 0, idUpgrade, idUpgrade->~ResearchDuration());
  OnResearchingUpgradeStart(effect, idUpgrade, idUpgrade->~ResearchDuration(), idUpgrade->~ResearchCost());

  return effect;
}

public func FxResearchingUpgradeStart(object pTarget, int iNr, int iTemp, id idUpgrade, int iDuration)
{
  if(iTemp)
    return;

  EffectVar(0, pTarget, iNr) = idUpgrade;
  EffectVar(1, pTarget, iNr) = iDuration;

  //Effekte
  Sound("Research", 0, this, 100, 0, +1);

  return true;
}

public func FxResearchingUpgradeTimer(object pTarget, int iNr, int iTime)
{
  var bar, plr;
  var duration = EffectVar(1, pTarget, iNr);
  var percent = iTime*100/duration;

  for(var bar in FindObjects(Find_ID(SBAR), Find_ActionTarget(this), Find_Func("HasBarType", BAR_Ammobar)))
  {
    if(!bar)
      continue;
  
    var plr = GetOwner(bar);
    if(!FindObject2(Find_OCF(OCF_CrewMember), Find_Or(Find_Container(this), Find_ActionTarget(this)), Find_Owner(plr)))
      RemoveObject(bar);
  }

  plr = [];

  for(var obj in FindObjects(Find_OCF(OCF_CrewMember), Find_Or(Find_Container(pTarget), Find_ActionTarget(this)), Find_Allied(GetOwner(pTarget))))
  {
    if(GetIndexOf(GetOwner(obj), plr) == -1)
      plr[GetLength(plr)] = GetOwner(obj);
    else
      continue;

    bar = FindObject2(Find_ID(SBAR), Find_ActionTarget(this), Find_Owner(GetOwner(obj)), Find_Func("HasBarType", BAR_Ammobar));
    if(!bar)
    {
      bar = CreateObject(SBAR, 0, 0, GetOwner(obj));
      bar->Set(this, RGB(30, 100, 255), BAR_Ammobar, GetObjWidth(pTarget)*1000/GetDefWidth(SBAR)/10, 0, SM12);
    }

    bar->Update(percent);
  }

  if(iTime >= duration)
    return -1;

  return true;
}

public func FxResearchingUpgradeStop(object pTarget, int iNr)
{
  for(var bar in FindObjects(Find_ID(SBAR), Find_ActionTarget(this), Find_Func("HasBarType", BAR_Ammobar)))
    if(bar)
      RemoveObject(bar);

  EffectVar(0, pTarget, iNr)->Researched(pTarget);

  //Effekte
  Sound("Building_UpgradeComplete.ogg", 0, this, 100, GetOwner()+1);
  Sound("Research", 0, this, 100, 0, -1);

  return true;
}

public func GetUpgradeResearchProgress()	{return GetEffect("ResearchingUpgrade", this, 0, 6);}
public func GetUpgradeResearchDuration()	{return EffectVar(1, this, GetEffect("ResearchingUpgrade", this));}
public func IsResearchingUpgrades()		{return GetEffect("ResearchingUpgrade", this);}

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

public func RemoveUpgrade(id idUpgrade)
{
  if(!aUpgradeList)
    return true;

  var list = [];
  for(var data in aUpgradeList)
    if(data != idUpgrade)
      list[GetLength(list)] = data;

  aUpgradeList = list;
  OnUpgradeRemoved(idUpgrade);

  return true;
}

public func GetUpgrade(id idUpgrade)
{
  if(!aUpgradeList)
    return;

  for(var upgrade in aUpgradeList)
    if(upgrade == idUpgrade)
      return true;

  return false;
}

public func OnUpgrade(id idUpgrade)								{}
public func OnUpgradeRemoved(id idUpgrade)							{}
public func GetUpgradeList()									{return aUpgradeList;}
public func OnResearchingUpgradeStart(int iEffect, id idUpgrade, int iDuration, int iCost)	{return;}
public func FurtherUpgradeConditions(id idUpgrade, int iDuration, int iCost)			{return true;}

/* Energiebalken */

public func FxShowHealthBarTimer(object pTarget, int iNr)
{
  var bar, plr;
  var percent, color;
  if(!this->~ChangeHealthBars(percent, color))
  {
    percent = 100-(GetDamage()*100/MaxDamage());
    color = RGB(235, 60, 60);
  }

  //Wenn Clonk nicht Gebäude anfässt/im Gebäude ist, Balken löschen
  for(var bar in FindObjects(Find_ID(SBAR), Find_Or(Find_Container(this), Find_ActionTarget(this)), Find_Func("HasBarType", BAR_Energybar)))
  {
    if(!bar)
      continue;

    var plr = GetOwner(bar);
    if(!FindObject2(Find_OCF(OCF_CrewMember), Find_Or(Find_Container(this), Find_And(Find_ActionTarget(this), Find_Action("Push"))), Find_Owner(plr)))
      RemoveObject(bar);
  }

  plr = [];

  //Falls Clonk im Gebäude ist/es anfässt
  for(var obj in FindObjects(Find_OCF(OCF_CrewMember), Find_Or(Find_Container(this), Find_ActionTarget(this)), Find_Allied(GetOwner(pTarget))))
  {
    if(GetActionTarget(0, obj) == this && GetProcedure(obj) != "PUSH" && Contained(obj) != this)
      continue;

    //Balken für den Spieler wurde schon aktualisiert
    if(GetIndexOf(GetOwner(obj), plr) == -1)
      plr[GetLength(plr)] = GetOwner(obj);
    else
      continue;

    //Zugehörigen Balken suchen
    bar = FindObject2(Find_ID(SBAR), Find_Or(Find_Container(this), Find_ActionTarget(this)), Find_Owner(GetOwner(obj)), Find_Func("HasBarType", BAR_Energybar));

    //Ansonsten neu erstellen
    if(!bar)
    {
      bar = CreateObject(SBAR, 0, 0, GetOwner(obj));
      bar->Set(this, color, BAR_Energybar, GetObjWidth(pTarget)*1000/GetDefWidth(SBAR)/10, 0, SM13);
    }

    //Schadensanzeige updaten
    bar->Update(percent);
    bar->SetBarColor(color);
  }

  return true;
}

/* Schaden */

public func Damage(int change)
{
  //Struktur zerstören, aber mehr Schaden als den Maximalen nicht zulassen
  if(change > 0 && IsDestroyed())
  {
    if(GetDamage() > MaxDamage())
      DoDamage(-(GetDamage()-MaxDamage()));
  }

  //Bei höherem Schaden als dem Maximalen entsprechend zerstören
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

  //Punkte bei Belohnungssystem (Strukturzerstörung)
  /*if(BonusPointCondition() && iLastAttacker != -1)
    if((GetOwner() != -1 && Hostile(GetOwner(), iLastAttacker)) || (GetOwner() == -1 && !GetTeam(this)) || (GetTeam(this) != GetPlayerTeam(iLastAttacker)))
      DoPlayerPoints(BonusPoints("Destruction"), RWDS_BattlePoints, iLastAttacker, GetCursor(iLastAttacker), IC03);*/

  //Explosion
  FakeExplode(50, iLastAttacker+1);

  //Sound
  Sound("Blast2", false, this);

  //Letzen Angreifer zurücksetzen
  iLastAttacker = -1;

  //Callback
  OnDestruction();

  //Löschen planen (Momentan noch nicht besonders fancy)
  Schedule("RemoveObject()", 35*6, 1, this);

  //ggf. Technikstufe zurücksetzen
  ResetTechLevel();

  return true;
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
/* Zerstörung */

public func Destruction()
{
  //Verzögerter Gamecall
  ScheduleCall(FindObject(MELE), "OnBuildingDestroyed", 1, 0, GetID());

  //ggf. Technikstufe zurücksetzen
  ResetTechLevel();

  //Zugehörige Objekte mitlöschen
  for(var obj in aObjectList)
    if(obj)
      RemoveObject(obj);

  return true;
}

/* Gebäudemenü */

public func MenuHeader(object pMenuObj, string szName, int iExtra)
{
  if(GetMenu(pMenuObj))
    CloseMenu(pMenuObj);

  CreateMenu(GetID(), pMenuObj, this, iExtra, Format("%s - %s", GetName(this), szName), 0, C4MN_Style_Dialog);
}

//Für anfassbare Gebäude
public func ControlUp(object pCaller)
{
  OpenBuildingMenu(0, pCaller);
  return true;
}

public func ContainedUp(object pCaller)
{
  OpenBuildingMenu(0, pCaller);
  return true;
}

public func OpenBuildingMenu(dummy, object pMenuObj)
{
  //Menü erstellen
  CreateMenu(GetID(), pMenuObj, this, C4MN_Extra_None, GetName(this), 0, C4MN_Style_Dialog);
  AddMenuItem(Format("$Hitpoints$", GetDamage(), MaxDamage()), 0, NONE, pMenuObj);

  //Gebäudespezifische Statusanzeigen
  AdditionalStatusMenu(pMenuObj);
  AddMenuItem(" ", 0, NONE, pMenuObj);

  //Techstufe prüfen
  if(!GetTeamTechLevel(GetPlayerTeam(GetOwner()), TechLevel()) && TechLevel() != TECHLEVEL_Start)
  {
    //"Notfallmenü"
    EmergencyBuildingMenu(pMenuObj);
    return true;
  }

  //Gebäudespezifische Menüeinträge
  AdditionalBuildingMenu(pMenuObj);
  //Kaufmenü
  if(this->~IsBase())
    AddMenuItem("$BuyMenu$", "OpenBaseBuyMenu", GOLD, pMenuObj, 0, pMenuObj);
  //Upgrademenü
  AddMenuItem("$UpgradeMenu$", "OpenUpgradeMenu", CCUS, pMenuObj, 0, pMenuObj);

  return true;
}

public func OpenBaseBuyMenu(dummy, object pMenuObj)
{
  var aBuy = [], i, def, plr = GetOwner(pMenuObj);

  //Kaufliste zusammenstellen
  while(def = GetHomebaseMaterial(plr, 0, i++, BuyCategory()))
    aBuy[GetLength(aBuy)] = def;

  return OpenBuyMenu(dummy, pMenuObj, 0, 0, aBuy);
}

public func AdditionalStatusMenu(object pMenuObj)	{}
public func AdditionalBuildingMenu(object pMenuObj)	{}
public func EmergencyBuildingMenu(object pMenuObj)	{}

public func OpenUpgradeMenu(id dummy, object pMenuObj)
{
  MenuHeader(pMenuObj, "$UpgradeMenu$");

  //Erforscht Upgrades?
  if(!IsResearchingUpgrades())
    AddMenuItem("$NoUpgrades$", 0, NONE, pMenuObj);
  else
  {
    var upgradeID = EffectVar(0, this, IsResearchingUpgrades());
    AddMenuItem(Format("$IsUpgrading$", GetName(0, upgradeID), GetUpgradeResearchProgress()*100/GetUpgradeResearchDuration()), 0, upgradeID, pMenuObj);
  }

  //Leerzeile
  AddMenuItem(" ", 0, NONE, pMenuObj);

  var aMenuUpgradeList = [[],[]];

  aMenuUpgradeList[0] = aUpgradeList;

  for(var upgrade in PossibleUpgrades())
    if(GetIndexOf(upgrade, aUpgradeList) == -1)
      aMenuUpgradeList[1][GetLength(aMenuUpgradeList[1])] = upgrade;

  //Aktive Upgrades auflisten
  if(GetLength(aMenuUpgradeList[0]))
  {
    AddMenuItem("$ActiveUpgrades$", 0, NONE, pMenuObj);
    for(var upgrade in aMenuUpgradeList[0])
      AddMenuItem(Format("%s", GetName(0, upgrade)), 0, upgrade, pMenuObj);
  }

  //Erforschbare Upgrades auflisten
  if(GetLength(aMenuUpgradeList[1]))
  {
    AddMenuItem("$UpgradesToResearch$", 0, NONE, pMenuObj);
    for(var upgrade in aMenuUpgradeList[1])
    {
      if(!CanResearch(upgrade))
        AddMenuItem(Format("<c ff0000>%s</c>", GetName(0, upgrade)), 0, upgrade, pMenuObj, upgrade->ResearchCost());
      else
        AddMenuItem(GetName(0, upgrade), "ResearchUpgrade", upgrade, pMenuObj, upgrade->ResearchCost(), pMenuObj);
    }
  }

  return true;
}

/* Einkaufsmenüsystem */

public func GetBuyMenuEffect(int iPlr)
{
  var e, i, found, b = FindObject(BGRL);
  while(e = GetEffect("BuyMenu", b, i))
  {
    if(EffectVar(1, b, e) == iPlr && EffectVar(2, b, e) == GetID()) //B,E!
    {
      found = true;
      break;
    }
    i++;
  }

  if(!found)
    e = AddEffect("BuyMenu", b, 1, 5, 0, CCBS, iPlr, GetID());

  return e;
}

public func DeliveryCapacity()		{return 10;}		//Maximale Anzahl an Items
public func DeliveryMaxItemCount()	{return 5;}
public func DeliveryCooldownTime()	{return 35*15; }	//Nur alle 15 Sekunden liefern

static const CCBS_BUYMENU_Next = 1;
static const CCBS_BUYMENU_Last = 2;
static const CCBS_BUYMENU_Cart = 3;
static const CCBS_BUYMENU_Back = 4;

public func OpenBuyMenu(id dummy, object pMenuObj, int iOffset, int iButton, array aIDObjects)
{
  var e = GetBuyMenuEffect(GetOwner(pMenuObj));

  //Kauf-ID-Liste ermitteln
  if(!aIDObjects)
  {
    if(!e)
      return;
    else
      aIDObjects = EffectVar(3, FindObject(BGRL), e);

    if(!aIDObjects)
      return;
  }
  else
    EffectVar(3, FindObject(BGRL), e) = aIDObjects;

  //Kopfzeile
  MenuHeader(pMenuObj, "$BuyMenu$", C4MN_Extra_Value);

  var count = GetLength(aIDObjects);

  //Seitenanzeige
  AddMenuItem(Format("<c 33ccff>$Showing$</c>", iOffset/10+1, count/10+1), 0, NONE, pMenuObj);

  var def = 0, i = iOffset, plr = GetOwner(pMenuObj), sel, sel2;

  while((def = aIDObjects[i++]) && i <= 10+iOffset)
  {
    //Einkaufswagen voll?
    if(CartNotFull(def, plr))
    {
      //Hat genug Geld?
      if(GetWealth(plr) > GetCartValue(plr)+GetValue(0, def))
        AddMenuItem(GetName(0, def), Format("AddToCart(%i, Object(%d), %d)", def, ObjectNumber(pMenuObj), iOffset), def, pMenuObj, GetCartItemAmount(def, plr));
      else
        AddMenuItem(Format("<c 990000>%s</c>", GetName(0, def)), Format("BuyError(%i, Object(%d), %d)", def, ObjectNumber(pMenuObj), iOffset), def, pMenuObj, GetCartItemAmount(def, plr));
    }
    else
      AddMenuItem(Format("<c 777777>%s</c>", GetName(0, def)), Format("BuyError(%i, Object(%d), %d)", def, ObjectNumber(pMenuObj), iOffset), def, pMenuObj, GetCartItemAmount(def, plr));

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

  //Leerzeile
  AddMenuItem(" ", 0, NONE, pMenuObj);

  var btn = 0;

  //Nächste Seite
  if(iOffset+10 <= count)
  {
    AddMenuItem("$NextPage$", Format("OpenBuyMenu(%i, Object(%d), %d, 1)", GetID(), ObjectNumber(pMenuObj), BoundBy(iOffset+10, 0, count)), NONE, pMenuObj, 0, 0, "", C4MN_Add_ForceNoDesc);
    btn++;
  }

  //Zurück
  if(iOffset > 0)
  {
    AddMenuItem("$LastPage$", Format("OpenBuyMenu(%i, Object(%d), %d, 2)", GetID(), ObjectNumber(pMenuObj), BoundBy(iOffset-10, 0, count)), NONE, pMenuObj, 0, 0, "", C4MN_Add_ForceNoDesc);
    if(btn && iButton != CCBS_BUYMENU_Next)
      btn++;
  }

  //Einkaufswagen
  AddMenuItem("$GoToCart$", Format("OpenCartMenu(Object(%d), %d)", ObjectNumber(pMenuObj), iOffset), NONE, pMenuObj, GetCartValue(plr), 0, "");
  if(iButton == CCBS_BUYMENU_Cart)
    btn++;

  //Zurück zum Hauptmenü
  AddMenuItem("$BackToMainMenu$", "OpenBuildingMenu", NONE, pMenuObj, 0, pMenuObj, "");
  if(iButton == CCBS_BUYMENU_Back)
    btn++;

  //Menüeintrag auswählen
  if(sel2)
    SelectMenuItem(sel, pMenuObj);
  else if(iButton)
    SelectMenuItem(i-iOffset+btn, pMenuObj);

  return true;
}

public func BuyError(id def, object pMenuObj, int iOffset)
{
  Sound("Error", 0, pMenuObj, 0, GetOwner(pMenuObj));
  return OpenBuyMenu(def, pMenuObj, iOffset); 
}

public func DeliveryInQueue(int iPlr)
{
  var e = GetBuyMenuEffect(iPlr);

  //Lieferung in Warteschlange
  if(EffectVar(4, FindObject(BGRL), e))
    return true;

  return false;
}

/* Einkaufswagen */

public func GetCartValue(int iPlr)
{
  //Einkaufswagensumme zusammenrechnen
  var cart = GetCart(iPlr), value;
  for(var data in cart)
    if(data)
      value += GetValue(0, data[0])*data[1];

  return value;
}

public func GetCartItemAmount(id idDef, int iPlr)
{
  //Anzahl von speziellem Item ermitteln
  var cart = GetCart(iPlr);
  for(var data in cart)
    if(data)
      if(data[0] == idDef)
        return data[1];

  return 0;
}

public func CartNotFull(id idDef, int iPlr)
{
  if(idDef)
  {
    //Anzahl überprüfen
    var amount = GetCartItemAmount(idDef, iPlr);
    if(amount)
      return amount < DeliveryMaxItemCount();
  }

  //Anzahl von verschiedenen Items überprüfen
  if(GetLength(GetCart(iPlr)) >= DeliveryCapacity())
    return false;

  //Hat gerade Bestellung in Warteschlange
  if(DeliveryInQueue(iPlr))
    return false;

  return true; 
}

public func GetCart(int iPlr)
{
  var e = GetBuyMenuEffect(iPlr);

  return EffectVar(0, FindObject(BGRL), e);
}

public func AddToCart(id idItem, object pMenuObj, int iOffset, bool fNoMenuCall)
{
  if(!idItem)
    return;

  var plr = GetOwner(pMenuObj);

  //Zu wenig Geld?
  if(GetWealth(plr) < GetCartValue(plr)+GetValue(0, idItem))
    return OpenBuyMenu(idItem, pMenuObj, iOffset);

  //Einkaufswagen voll?
  if(!CartNotFull(idItem, plr))
    return OpenBuyMenu(idItem, pMenuObj, iOffset);

  //Ansonsten hinzufügen
  EffectCall(FindObject(BGRL), GetBuyMenuEffect(plr), "AddCart", idItem);

  //Zum Kaufmenü
  if(!fNoMenuCall)
    OpenBuyMenu(idItem, pMenuObj, iOffset);

  return true;
}

public func SubFromCart(id idItem, object pMenuObj, int iBuyOffset, int iSel, bool fNoMenuCall, string szBuyMenu, int iExtraBuyInfo)
{
  EffectCall(FindObject(BGRL), GetBuyMenuEffect(GetOwner(pMenuObj)), "SubCart", idItem);

  //Zum Einkaufswagen
  if(!fNoMenuCall)
    OpenCartMenu(pMenuObj, iBuyOffset, iSel, 0, szBuyMenu, iExtraBuyInfo);

  return true;
}

/* Einkaufswagenmenü */

public func NoSubFromCart(int iExtraBuyInfo, array aData, int iPlr) {}

public func OpenCartMenu(object pMenuObj, int iBuyOffset, int iSel, bool fButton, string szBuyMenu, int iExtraBuyInfo)
{
  var plr = GetOwner(pMenuObj);
  var cart = GetCart(plr), entry, fBuy, fCancelOrder;

  //Verweis auf Kaufmenü
  if(!szBuyMenu)
    szBuyMenu = "OpenBuyMenu";

  //Kopfzeile
  MenuHeader(pMenuObj, "$CartMenu$", 0);

  //Einkaufswagen leer?
  if(!GetLength(cart))
    AddMenuItem("$CartIsEmpty$", 0, SM12, pMenuObj);
  else
  {
    if(DeliveryInQueue(plr))
    {
      //Bestellung in Warteschlange
      var pos;
      if((pos = FindObject(BGRL)->GetOrderQueuePosition(plr, GetID())) > 0)
      {
        AddMenuItem("$OrderInQueue$", 0, NONE, pMenuObj);
        AddMenuItem(Format("$OrderQueuePosition$", pos+1), 0, NONE, pMenuObj);
      }
      else
        AddMenuItem("$DeliveryOnTheWay$", 0, NONE, pMenuObj);

      fCancelOrder = true;
    }
    else
    {
      //Einkaufswageninhalt auflisten
      for(var data in cart)
      {
        var cmd = Format("SubFromCart(%i, Object(%d), %d, %d, 0, \"%s\", %d)", data[0], ObjectNumber(pMenuObj), iBuyOffset, entry, szBuyMenu, iExtraBuyInfo);
        if(NoSubFromCart(iExtraBuyInfo, data, plr))
          cmd = 0; 
        AddMenuItem(GetName(0, data[0]), cmd, data[0], pMenuObj, GetCartItemAmount(data[0], plr));
        entry++; 
      }

      //Summe anzeigen
      AddMenuItem("$Total$", 0, MNYS, pMenuObj, GetCartValue(plr));
      fBuy = true;
    }
  }

  //Leerzeile
  AddMenuItem(" ", 0, NONE, pMenuObj);

  //Zur Kasse
  if(fBuy && GetWealth(plr) >= GetCartValue(plr))
    AddMenuItem("$ProceedToCheckout$", Format("Checkout(Object(%d), %d, true, \"%s\", %d)", ObjectNumber(pMenuObj), iBuyOffset, szBuyMenu, iExtraBuyInfo), NONE, pMenuObj);
  else
    AddMenuItem("$ProceedToCheckoutInactive$", 0, NONE, pMenuObj);

  //Bestellung stornieren
  if(fCancelOrder)
    AddMenuItem("$CancelOrder$", Format("CancelOrder(Object(%d), %d, true, \"%s\", %d)", ObjectNumber(pMenuObj), iBuyOffset, szBuyMenu, iExtraBuyInfo), NONE, pMenuObj);

  //Zurück zum Kaufmenü
  AddMenuItem("$BackToBuyMenu$", Format("%s(0, Object(%d), %d, 3, %d)", szBuyMenu, ObjectNumber(pMenuObj), iBuyOffset, iExtraBuyInfo), NONE, pMenuObj);

  //Zurück zum Hauptmenü
  AddMenuItem("$BackToMainMenu$", "OpenBuildingMenu", NONE, pMenuObj, 0, pMenuObj, "");

  //Menüeintrag auswählen
  if(!fButton)
    SelectMenuItem(iSel, pMenuObj);
  else
    SelectMenuItem(entry+fButton, pMenuObj);

  return true;
}

/* Einkauf */

public func Checkout(object pMenuObj, int iBuyOffset, bool fButton, string szBuyMenu, int iExtraBuyInfo)
{
  var plr = GetOwner(pMenuObj);

  //Genug Geld?
  if(GetWealth(plr) < GetCartValue(plr))
  {
    Sound("Error", 0, pMenuObj, 0, plr+1);
    OpenCartMenu(pMenuObj, iBuyOffset, 0, fButton, szBuyMenu, iExtraBuyInfo);
    return true;
  }

  //Geld abziehen
  SetWealth(plr, GetWealth(plr)-GetCartValue(plr));

  //Sound
  Sound("Cash", 0, pMenuObj, 0, plr);

  //In Warteschlange setzen
  AddOrderToQueue(plr, iExtraBuyInfo);

  //Zurück zum Einkaufswagen
  OpenCartMenu(pMenuObj, iBuyOffset, 0, fButton, szBuyMenu, iExtraBuyInfo);

  return true;
}

/* Stornieren */

public func CancelOrder(object pMenuObj, int iBuyOffset, bool fButton, string szBuyMenu, int iExtraBuyInfo)
{
  var plr = GetOwner(pMenuObj);

  //Ist nicht mehr in Warteschlange?
  if(FindObject(BGRL)->GetOrderQueuePosition(plr, GetID()) == -1)
    return OpenCartMenu(pMenuObj, iBuyOffset, 0, fButton, szBuyMenu, iExtraBuyInfo);

  //Geld zurückzahlen
  SetWealth(plr, GetWealth(plr)+GetCartValue(plr));

  //Sound
  Sound("Cash", 0, pMenuObj, 0, plr);

  //Aus Warteschlange nehmen
  RemoveOrderFromQueue(plr);

  //Zurück zum Einkaufswagen
  OpenCartMenu(pMenuObj, iBuyOffset, 0, fButton, szBuyMenu, iExtraBuyInfo);

  return true;
}

/* Lieferwarteschlange */

public func AddOrderToQueue(int iPlr, int iExtraBuyInfo)
{
  //Keine Warteschlange und Lieferung möglich? Sofort liefern!
  if(!FindObject(BGRL)->GetOrderQueueLength(iPlr, GetID()) && !FindObject(BGRL)->GetDeliveryCooldown(iPlr, GetID()))
    return EffectCall(FindObject(BGRL), GetBuyMenuEffect(iPlr), "Deliver", this, iExtraBuyInfo);

  FindObject(BGRL)->AddOrderToQueue(iPlr, GetID());
  EffectCall(FindObject(BGRL), GetBuyMenuEffect(iPlr), "AddToQueue", this, iExtraBuyInfo);

  return true;
}

public func RemoveOrderFromQueue(int iPlr)
{
  FindObject(BGRL)->RemoveOrderFromQueue(iPlr, GetID());
  EffectCall(FindObject(BGRL), GetBuyMenuEffect(iPlr), "RemoveFromQueue");

  return true;
}

/* Kaufsteuerungseffekt */

public func FxBuyMenuStart(object pTarget, int iNr, int iTemp, int iPlr, id idBuilding)
{
  if(iTemp)
    return;

  EffectVar(0, pTarget, iNr) = [];		//Einkaufswagen
  EffectVar(1, pTarget, iNr) = iPlr;		//Spieler
  EffectVar(2, pTarget, iNr) = idBuilding;	//Gebäudetyp
  EffectVar(3, pTarget, iNr) = [];		//aIDObjects
  EffectVar(4, pTarget, iNr) = false;		//In Warteschlange
  EffectVar(5, pTarget, iNr) = 0;		//Liefergebäude
  EffectVar(6, pTarget, iNr) = 0;		//iExtraBuyInfo

  return true;
}

public func FxBuyMenuTimer(object pTarget, int iNr)
{
  //Nicht in Warteschlange?
  if(!EffectVar(4, pTarget, iNr))
    return;

  var plr = EffectVar(1, pTarget, iNr), id = EffectVar(2, pTarget, iNr);

  //Kann Gebäude liefern?
  if(FindObject(BGRL)->GetDeliveryCooldown(plr, id))
    return;

  //Befindet sich an erster Warteschlangenposition?
  if(FindObject(BGRL)->GetOrderQueuePosition(plr, id) > 0)
    return;

  //Dann liefern
  EffectCall(pTarget, iNr, "Deliver");

  //Aus Warteschlange entfernen
  FindObject(BGRL)->RemoveOrderFromQueue(plr, id);

  return true;
}

public func FxBuyMenuAddCart(object pTarget, int iNr, id idItem)
{
  //Nicht während in Warteschlange
  if(EffectVar(4, pTarget, iNr))
    return;

  //Dem Einkaufswagen hinzufügen
  var cart = EffectVar(0, pTarget, iNr), found;
  for(var i = 0; i < GetLength(cart); i++)
  {
    if(cart[i])
      if(cart[i][0] == idItem)
      {
        found = true;
        cart[i][1]++;
      }
  }

  if(!found)
    cart[GetLength(cart)] = [idItem, 1];

  EffectVar(0, pTarget, iNr) = cart;

  return true;
}

public func FxBuyMenuSubCart(object pTarget, int iNr, id idItem)
{
  //Nicht während in Warteschlange
  if(EffectVar(4, pTarget, iNr))
    return;

  //Aus Enkaufswagen löschen
  var cart = EffectVar(0, pTarget, iNr), cart2 = [];
  for(var data in cart)
    if(data)
    {
      if(data[0] == idItem)
        data[1]--;

      if(data[1] < 1)
        continue;

      cart2[GetLength(cart2)] = data;
    }

  EffectVar(0, pTarget, iNr) = cart2;

  return true;
}

public func FxBuyMenuClear(object pTarget, int iNr)
{
  //Nicht während in Warteschlange
  if(EffectVar(4, pTarget, iNr))
    return;

  EffectVar(0, pTarget, iNr) = [];

  return true;
}

public func FxBuyMenuAddToQueue(object pTarget, int iNr, object pTargetBuilding, int iExtraBuyInfo)
{
  EffectVar(4, pTarget, iNr) = true;
  EffectVar(5, pTarget, iNr) = pTargetBuilding;
  EffectVar(6, pTarget, iNr) = iExtraBuyInfo;
  return true;
}

public func FxBuyMenuRemoveFromQueue(object pTarget, int iNr)
{
  EffectVar(4, pTarget, iNr) = false;
  EffectVar(5, pTarget, iNr) = 0;
  EffectVar(6, pTarget, iNr) = 0;
  return true;
}

public func FxBuyMenuDeliver(object pTarget, int iNr, object pTargetBuilding, int iExtraBuyInfo)
{
  //Zielgebäude existiert?
  var target = pTargetBuilding, plr = EffectVar(1, pTarget, iNr);
  if(!target)
    target = EffectVar(5, pTarget, iNr);
  if(!target)
  {
    //Ausweichgebäude suchen (Bei Zerstörung)
    target = FindObject2(Find_ID(EffectVar(2, pTarget, iNr)), Find_Allied(plr));
    if(!target)
      return false;
  }

  var crate, cart = EffectVar(0, pTarget, iNr), i;
  if(!iExtraBuyInfo)
    iExtraBuyInfo = EffectVar(6, pTarget, iNr);
  //Übernimmt Lieferungsprozess selbst?
  if(target->~SelfDeliver(cart, iExtraBuyInfo, plr, target))
    target->~ProcessSelfDeliver(cart, iExtraBuyInfo, plr, target);
  else
  {
    //Ansonsten Einkaufswagen abarbeiten 
    for(var item in cart)
    {
      if(!item)
        continue;

      for(var j = 0; j < item[1]; j++)
      {
        //Neue Kiste erstellen
        if(!crate || crate->ContentsCount() >= crate->ItemLimit())
        {
          crate = CreateObject(SPCT, BoundBy(GetX(target)+Random(60)-30, 15, LandscapeWidth()-15), 60, plr);
          CreateObject(PARA, GetX(crate), GetY(crate), plr)->Set(crate);
          crate->Set(target, true);
        }

        if(!item[0]->~IsClonk())
        {
          var wpn = CreateContents(item[0], crate);
          if(wpn->~IsWeapon())
          {
            //Waffen auffüllen
            while(wpn->~CycleFM(+1))
            {
              var ammo = wpn->GetFMData(FM_AmmoID);
              var load = wpn->GetFMData(FM_AmmoLoad);
              if(wpn->GetAmmo(ammo) == load) break;
              //erst entladen
              DoAmmo(ammo,-load, wpn);
              //dann neu reinladen
              DoAmmo(ammo, load, wpn);
            }
            //noch ein letztes Mal
            wpn->~CycleFM(+1);
          }
        }
        else
        {
          //Clonk erstellen und an Fallschirm hängen
          var clonk = CreateObject(item[0], BoundBy(GetX(target)+Random(60)-30, 15, LandscapeWidth()-15), 60, plr);
          MakeCrewMember(clonk, plr);
          clonk->SetAction("Jump");
          CreateObject(PARA, GetX(clonk), GetY(clonk), plr)->Set(clonk);
        }
      }
    }

    //Sound
    if(crate)
      Sound("JetFlyBy*.ogg", false, crate);
    else if(clonk)
      Sound("JetFlyBy*.ogg", false, clonk);
  }
  
  //Liefercooldown einstellen
  FindObject(BGRL)->DeliveryCooldown(plr, EffectVar(2, pTarget, iNr));

  //Variablen zurücksetzen
  EffectVar(0, pTarget, iNr) = [];
  EffectVar(4, pTarget, iNr) = false;
  EffectVar(5, pTarget, iNr) = 0;
  EffectVar(6, pTarget, iNr) = 0;
  return true;
}

public func SelfDeliver(array aCart, int iExtraBuyInfo, int iPlr)		{}
public func ProcessSelfDeliver(array aCart, int iExtraBuyInfo, int iPlr)	{}

/* Bombenplatzierung */

protected func FxBuildingBombStart(object pTarget, int iEffectNumber, int iTemp, var1)
{  
  EffectVar(0, pTarget, iEffectNumber) = var1;			//Platzierer (Spieler) der Bombe
  EffectVar(1, pTarget, iEffectNumber) = BombExplodeTime();	//Explosionszeit
  EffectVar(2, pTarget, iEffectNumber) = 0;			//Entschärfungszeit
  EffectVar(3, pTarget, iEffectNumber) = 0;			//Prozentanzeige
  EffectVar(4, pTarget, iEffectNumber) = 0;			//Prozentanzeige Defuse
  EffectVar(5, pTarget, iEffectNumber) = 0;			//Bombenradius

  return 1;  
}

protected func FxBuildingBombTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  var defusing = 0;
  var enemyinrange = 0;
  var bombteam = GetPlayerTeam(EffectVar(0, pTarget, iEffectNumber));
  for(var clonk in FindObjects(Find_Distance(BombDefuseRadius()), Find_OCF(OCF_CrewMember | OCF_Alive), Find_NoContainer()))
    if(GetPlayerTeam(GetOwner()) == GetPlayerTeam(GetOwner(clonk)))
      defusing = 1;
    else
    if(HostileTeam(GetPlayerTeam(GetOwner()), GetPlayerTeam(GetOwner(clonk))))
      enemyinrange = 1;

  var explodetime = EffectVar(1, pTarget, iEffectNumber);
  var defusetime = EffectVar(2, pTarget, iEffectNumber);
  var bar = EffectVar(3, pTarget, iEffectNumber);
  var bar2 = EffectVar(4, pTarget, iEffectNumber);
  var radius = EffectVar(5, pTarget, iEffectNumber);

  //wenn kein Entschärfer in der Nähe war, Zeit zurücksetzen und weiterrunterzählen
  if(!defusing)
  {
    defusetime = 0;
    explodetime--;
    if(radius)
      RemoveObject(radius);
    if(bar2)
      RemoveObject(bar2);
  }
  else
  {
    //erhöhen, wenn keine Feinde in der Nähe
    if(!enemyinrange)
      defusetime++;
    //und Kreis anzeigen
    //if(!radius)
    //  radius = ShowBombRadius(this);
    //Defusebar  
    if(!bar2)
    {
      bar2 = CreateObject(SBAR, 0, GetDefHeight(SBAR), -1);
      bar2->Set(this, RGB(30, 100, 255), BAR_AssaultBar, GetDefWidth()*1000/GetDefWidth(SBAR)/10);
    }
    bar2->Update(((BombDefuseTime() - defusetime)*100)/BombDefuseTime());
  }

  //schon explodiert? 
  if(explodetime <= 0)
  {
    DecoExplode(50,CreateObject(ROCK));
    DoDmg(BombDamage());
    return -1;
  }
  //schon entschärft?
  if(defusetime >= BombDefuseTime())
    return -1;

  if(!bar)
  {
    bar = CreateObject(SBAR, 0, 0, -1);
    bar->Set(this, RGB(255, 100, 30), BAR_AssaultBar, GetDefWidth()*1000/GetDefWidth(SBAR)/10, 0, SM19);
  }
  bar->Update((explodetime*100)/BombExplodeTime());
  
  EffectVar(1, pTarget, iEffectNumber) = explodetime;
  EffectVar(2, pTarget, iEffectNumber) = defusetime;
  EffectVar(3, pTarget, iEffectNumber) = bar;
  EffectVar(4, pTarget, iEffectNumber) = bar2;
  EffectVar(5, pTarget, iEffectNumber) = radius;
  return 1;
}

protected func FxBuildingBombStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{  
  var bar = EffectVar(3, pTarget, iEffectNumber);
  var bar2 = EffectVar(4, pTarget, iEffectNumber);
  var radius = EffectVar(5, pTarget, iEffectNumber);
  
  if(bar) RemoveObject(bar);
  if(bar2) RemoveObject(bar2);
  if(radius) RemoveObject(radius);
  
  return 1;
}

/* Umkreis-Effekt */

protected func ShowBombRadius(object pTarget)
{
  //Kreis-Symbol erstellen
  var obj = CreateObject(SM09, 0, 0, -1);
  obj->Set(pTarget);

  //Symbolgröße anpassen
  var wdt = BombDefuseRadius() * 2000 / GetDefWidth(SM09);

  //Symbol konfigurieren
  obj->SetObjDrawTransform(wdt, 0, 0, 0, wdt, 0);
  obj->SetGraphics("Big");
  obj->SetColorDw(RGB(255,0,0));

  return obj;
}

/* Energieversorgung */

static const CCBS_EnergyProd = 1;
static const CCBS_EnergyNeed = 2;

public func GetEnergyData(int iData)
{
  if(GetCon() >= 100)
  {
    if(iData == CCBS_EnergyProd)
      return EnergyProduction()+AdditionalEnergyProduction();
    if(iData == CCBS_EnergyNeed)
      return RequiredEnergy();
  }

  return 0;
}

public func EnergySupply(bool fEnergy, int iEnergySupply, int iEnergyNeed)
{
  fHasEnergy = fEnergy;
  return true;
}

public func HasEnergy()			{return fHasEnergy;}
public func OnAddObject(object pObj)	{}