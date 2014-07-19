/*-- Kaserne --*/

#strict 2
#include CCBS
#include BT04

local aPUpgrades, iAdditionalEnergy;

public func TechLevel()				{return TECHLEVEL_1;}		//Benötigte Techstufe
public func BuildingRadius()			{return 200;}			//Bauradius
public func RequiredEnergy()			{return 100;}			//Energiehersteller
public func AdditionalRequiredEnergy()	{return iAdditionalEnergy;}	//Zusätzliche Energie
public func PossibleUpgrades()			{return aPUpgrades;}		//Mögliche Upgrades
public func MaxDamage()				{return 200;}			//Maximaler Schadenswert bis zur Zerstörung


/* Konstruktion */

protected func Construction()
{
  aPUpgrades = [U_AS, U_XA];
  return _inherited(...);
}

/* Initalisierung */

protected func Initialize()
{
  //Effekte

  return _inherited(...);
}

/* Menüführung */

public func AdditionalStatusMenu(object pMenuObj)
{
  if(!GetEffect("AmmoStorage", this))
    return;

  AddMenuItem(Format("$StorageContent$", GetStorage(), MaxStorage()), 0, NONE, pMenuObj);

  return true;
}

public func AdditionalBuildingMenu(object pMenuObj)
{
  //Speicher 
  if(GetUpgrade(U_AS))
  {
    var storage = GetEffect("AmmoStorage", this);
    if(!EffectVar(0, this, storage))
      AddMenuItem("$CloseStorageSupply$", "SwitchStorageSupply", NONE, pMenuObj);
    else
      AddMenuItem("$OpenStorageSupply$", "SwitchStorageSupply", NONE, pMenuObj);
  } 
  //Clonks kaufen
  AddMenuItem("$BuyClonkMenu$", "OpenClonkBuyMenu", PCMK, pMenuObj, 0, pMenuObj);
  //Ausrüstung kaufen
  AddMenuItem("$BuyEquipmentMenu$", "OpenEquipmentBuyMenu", C4EX, pMenuObj, 0, pMenuObj);
  //Presets kaufen
  AddMenuItem("$BuyPresets$", "OpenPresetMenu", MCLS, pMenuObj, 0, pMenuObj);
  
  return true;
}

public func OpenPresetMenu(id dummy, object pMenuObj)
{
  //Kopfzeile
  MenuHeader(pMenuObj, "$ChoosePresetMenu$");

  //Preset Menüeinträge
  AddMenuItem("$Preset1$", Format("OpenSpecPresetMenu(0, Object(%d), 0, 0, 1)", ObjectNumber(pMenuObj)), BuildingSystem()->GetPresetData(0, 0, CPRESET_Clonk), pMenuObj, 0, 0, "");
  AddMenuItem("$Preset2$", Format("OpenSpecPresetMenu(0, Object(%d), 0, 0, 2)", ObjectNumber(pMenuObj)), BuildingSystem()->GetPresetData(0, 1, CPRESET_Clonk), pMenuObj, 0, 0, "");
  AddMenuItem("$Preset3$", Format("OpenSpecPresetMenu(0, Object(%d), 0, 0, 3)", ObjectNumber(pMenuObj)), BuildingSystem()->GetPresetData(0, 2, CPRESET_Clonk), pMenuObj, 0, 0, "");

  //Zurück zum Hauptmenü
  AddMenuItem("$BackToMainMenu$", "OpenBuildingMenu", NONE, pMenuObj, 0, pMenuObj, "");

  return true; 
}

public func OpenSpecPresetMenu(id dummy, object pMenuObj, int iSelection, int iButton, int iPresetSlotPlusOne)
{
  var iSlot = iPresetSlotPlusOne-1, plr = GetOwner(pMenuObj);
  
  var preset = BuildingSystem()->GetFullPresetSlotData(plr, iSlot), value = 0;
  
  //Kopfzeile
  MenuHeader(pMenuObj, "$ChooseSpecPresetMenu$");
  
  //Clonkauswahl
  AddMenuItem("$PresetClonk$", 0, NULL, pMenuObj);

  var str = GetName(0, preset[CPRESET_Clonk]);
  if(!preset[CPRESET_Clonk])
    str = "$ChooseClonk$";
  
  AddMenuItem(str, Format("ChoosePresetClonk(0, Object(%d), %d)", ObjectNumber(pMenuObj), iSlot), preset[CPRESET_Clonk], pMenuObj);
  
  //Weiteres Menü nur wenn Clonk ausgewählt
  if(preset[CPRESET_Clonk])
  {
    //Collection-Limitierungen ermitteln
		var maxwpn = preset[CPRESET_Clonk]->~WeaponCollectionLimit(), maxeq = preset[CPRESET_Clonk]->~ObjectCollectionLimit(), fWpn = true;
		AddMenuItem("$PresetWeapons$", 0, NONE, pMenuObj);
		for(var i = 0; i < 5; i++)
		{
		  //Ausrüstung auflisten, wenn Limit erreicht
		  if(i == maxwpn)
		  {
		    fWpn = false;
		    AddMenuItem("$PresetEquipment$", 0, NONE, pMenuObj);
		  }
		
		  //String anpassen, je nach dem ob PresetDaten existieren oder nicht
		  if(!preset[i+1])
		  {
		    str = "$ChooseWeaponSlot$";
		    if(!fWpn)
		      str = "$ChooseEquipmentSlot$";
		  }
		  else
		    str = GetName(0, preset[i+1]);
		  AddMenuItem(str, Format("ChoosePresetInventory(0, Object(%d), %d, %d, %d)", ObjectNumber(pMenuObj), iSlot, i+1, fWpn), preset[i+1], pMenuObj, 0, 0, "");
		}
  }

  //Kosten anzeigen
  AddMenuItem("$Total$", 0, MNYS, pMenuObj, PresetValue(plr, iSlot));
  
  //Leerzeile
  AddMenuItem(" ", 0, NONE, pMenuObj);
  
  //Kann Preset nicht kaufen?
  if(GetWealth(plr) < PresetValue(plr, iSlot) || DeliveryInQueue(plr))
    AddMenuItem("$NoBuySpecPreset$", 0, NONE, pMenuObj);
  else
    AddMenuItem("$GoToCart$", Format("BuyPreset(Object(%d), %d)", ObjectNumber(pMenuObj), iSlot), NONE, pMenuObj, 0, 0, "");
  
  //Zurück
  AddMenuItem("$Back$", "OpenPresetMenu", NONE, pMenuObj, 0, pMenuObj, "");
  
  SelectMenuItem(iSelection, pMenuObj);
  
  //Presetslot speichern
  BuildingSystem()->SavePresetSlot(plr, iSlot);
  
  return true;
}

/* Preset kaufen */

public func NoSubFromCart(int iExtraBuyInfo) { return iExtraBuyInfo; }

public func PresetValue(int iPlr, int iSlot)
{
  var preset = BuildingSystem()->GetFullPresetSlotData(iPlr, iSlot), value;
  for(var def in preset)
    if(def)
      value += GetValue(0, def);
  
  return value;
}

public func BuyPreset(object pMenuObj, int iSlot)
{
  var plr = GetOwner(pMenuObj), preset = BuildingSystem()->GetFullPresetSlotData(plr, iSlot);
  if(GetWealth(plr) < PresetValue(plr, iSlot) || DeliveryInQueue(plr))
    return OpenSpecPresetMenu(0, pMenuObj, 6, 0, iSlot+1);
  
  //Einkaufswagen leeren
  EffectCall(FindObject(BGRL), GetBuyMenuEffect(GetOwner(pMenuObj)), "Clear");

  //Preset-Items in Einkaufswagen legen
  for(var def in preset)
    if(def)
      AddToCart(def, pMenuObj, 0, true);

  //Einkaufswagenmenü öffnen
  OpenCartMenu(pMenuObj, 0, 0, 0, "OpenSpecPresetMenu", iSlot+1);

  return true;
}

/* Preset liefern */

public func SelfDeliver(array aCart, int iExtra, int iPlr) { return iExtra; }

public func ProcessSelfDeliver(array aCart, int iExtra, int iPlr, object pTarget)
{
  var iSlot = iExtra-1;
  
  var clonk, equip = [];
  //Clonks und Ausrüstung trennen
  for(var data in aCart)
  {
    if(data[0]->~IsClonk())
      clonk = data[0];
    else
      equip[GetLength(equip)] = data[0];
  }
  
  //Clonk erstellen und an Fallschirm hängen
  var crew = CreateObject(clonk, AbsX(BoundBy(GetX()+Random(60)-30, 15, LandscapeWidth()-15)), AbsY(60), iPlr);
  MakeCrewMember(crew, iPlr);
	crew->SetAction("Jump");
  CreateObject(PARA, GetX(crew), GetY(crew), iPlr)->Set(crew);
  
  //Ausrüstungsgegenstände erstellen
  for(var def in equip)
  {
    var wpn = CreateContents(def, crew);
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
  
  return true;
}

/* Preset Auswahlmenüs */

public func ChoosePresetInventory(id idItem, object pMenuObj, int iSlot, int iInv, bool fWeapon)
{
  var iSelection;

  //Kopfzeile je nach Waffe/Equipment
  if(!fWeapon)
    MenuHeader(pMenuObj, "$ChooseWeaponSlot$");
  else
    MenuHeader(pMenuObj, "$ChooseEquipmentSlot$", C4MN_Extra_Value);
  
  //Kaufliste laden
  var list = ListBuyEquipment(), entry;
  var preset = BuildingSystem()->GetFullPresetSlotData(GetOwner(pMenuObj), iSlot);
  for(var def in list)
  {
    if(def == idItem)
      iSelection = entry;
    
    var index = GetIndexOf(def, preset);
    //Wurde schon wo anders ausgewählt
    if(index != -1 && index != iInv)
      continue;
  
    //Waffen oder Equipment anzeigen
    if((fWeapon && def->~IsWeapon()) || (!fWeapon && def->~IsEquipment() && !def->~IsGrenade()))
    {
      AddMenuItem(GetName(0, def), Format("SetPresetInventoryItem(%i, Object(%d), %d, %d)", def, ObjectNumber(pMenuObj), iSlot, iInv), def, pMenuObj, (index == iInv));
      entry++;
    }
  }
  
  //Zurück
  AddMenuItem("$BackToPresets$", Format("OpenSpecPresetMenu(0, Object(%d), %d, 0, %d)", ObjectNumber(pMenuObj), iInv, iSlot+1), NULL, pMenuObj, 0, 0, "");

  SelectMenuItem(iSelection, pMenuObj);
  
  return true;
}

public func SetPresetInventoryItem(id idItem, object pMenuObj, int iSlot, int iInv)
{
  var plr = GetOwner(pMenuObj);
  BuildingSystem()->SetPresetData(plr, iSlot, iInv, idItem);
  
  return ChoosePresetInventory(idItem, pMenuObj, iSlot, iInv, idItem->~IsWeapon());
}

public func ChoosePresetClonk(id idClonk, object pMenuObj, int iSlot)
{
  var plr = GetOwner(pMenuObj), preset = BuildingSystem()->GetFullPresetSlotData(plr, iSlot);
  if(!idClonk)
    idClonk = preset[CPRESET_Clonk];
  
  //Kopfzeile
  MenuHeader(pMenuObj, "$ChooseClonkMenu$", C4MN_Extra_Value);
  
  //Kaufbare Clonks auflisten
  var clonks = ListBuyClonks(), entry, iSelection;
  for(var clonk in clonks)
  {
    if(clonk == idClonk)
      iSelection = entry;
  
    AddMenuItem(GetName(0, clonk), Format("SetPresetClonk(Object(%d), %d, %i, %d)", ObjectNumber(pMenuObj), iSlot, clonk, entry), clonk, pMenuObj, clonk == idClonk);
    entry++;
  }
  
  //Zurück
  AddMenuItem("$BackToPresets$", Format("OpenSpecPresetMenu(0, Object(%d), 0, 0, %d)", ObjectNumber(pMenuObj), iSlot+1), NULL, pMenuObj, 0, 0, "");
  
  SelectMenuItem(iSelection, pMenuObj);

  return true;
}

public func SetPresetClonk(object pMenuObj, int iSlot, id idClonk)
{
  //Clonkdaten ändern
  var plr = GetOwner(pMenuObj);
  BuildingSystem()->SetPresetData(plr, iSlot, CPRESET_Clonk, idClonk);
  
  return ChoosePresetClonk(idClonk, pMenuObj, iSlot);
}

public func SwitchStorageSupply(id dummy, object pMenuObj)
{
  var storage = GetEffect("AmmoStorage", this);
  EffectVar(0, this, storage) = !EffectVar(0, this, storage);
  OpenBuildingMenu(0, pMenuObj);
  
  return true;
}

/* Kaufmenüs */

public func OpenClonkBuyMenu(id dummy, object pMenuObj)
{
  return OpenBuyMenu(0, pMenuObj, 0, 0, ListBuyClonks());
}

public func OpenEquipmentBuyMenu(id dummy, object pMenuObj)
{
  return OpenBuyMenu(0, pMenuObj, 0, 0, ListBuyEquipment());
}

public func ListBuyClonks() { return [PCMK, MDIC]; }

public func ListBuyEquipment()
{
  //Einzelne Itemlisten
  var weapons = [PSTL, RVLR, SMGN, ASTR, MNGN, PPGN], weaponadd = [];
  var grenades = [FRAG, STUN, SGRN, SRBL], grenadeadd = [];
  var ammo = [ABOX, GBOX, MBOX];
  var equipment = [BWTH, DGNN, CDBT, AMP2, FAP2], equipmentadd = [];
  if(GetUpgrade(U_XA))
  {
    //Erweiterte Itemlisten
    weaponadd = [RTLR, SGST, ATWN];
    grenadeadd = [FGRN, PGRN];
    equipmentadd = [BTBG, C4PA, RSHL];
  }

  var items = [];
  AddArray4K(weaponadd, weapons);
  AddArray4K(grenadeadd, grenades);
  AddArray4K(equipmentadd, equipment);

  //Liste zusammensetzen
  AddArray4K(weapons, items);
  AddArray4K(ammo, items);
  AddArray4K(grenades, items);
  AddArray4K(equipment, items);
  
  return items;
}

/* Zerstörung */

public func Destroyed()
{
  //Effekt

  return _inherited(...);
}

/* Upgrades */

public func OnUpgrade(id idUpgrade)
{
  //[Allgemeine Effekte/Sounds]

  if(idUpgrade == U_AS)
    SetupAmmoStorage();
}

/* Munitionsspeicher */

local iCreditStorage;

public func GetStorage() { return iCreditStorage; }
public func DoStorage(int iAmount) { return SetStorage(GetStorage()+iAmount); }
public func SetStorage(int iAmount) { return iCreditStorage = BoundBy(iAmount, 0, MaxStorage()); }

public func MaxStorage() {return 200;} //Maximale Punktzahl (und Creditszahl) die gespeichert werden kann
public func StorageRange() {return 150;} //Wirkbereich des Speichers

public func SetupAmmoStorage()
{
  AddEffect("AmmoStorage", this, 1, 3, this);
  
  //Effekte/Grafik verändern
  
  return true;
}

public func FxAmmoStorageTimer(object pTarget, int iNr)
{
  //Mit Credits beladen, sofern die Zufuhr nicht gesperrt ist
  if(GetStorage() < MaxStorage() && !EffectVar(0, pTarget, iNr) && GetWealth(GetOwner()))
  {
    DoStorage(1);
    SetWealth(GetOwner(), GetWealth(GetOwner())-1);
  }
  
  //Speicher leer
  if(!GetStorage())
    return;

  //Clonks im Inneren heilen
  for(var obj in FindObjects(Find_Container(this), Find_OCF(OCF_CrewMember), Find_Allied(GetOwner())))
  {
    //Speicher ist leer
    if(!GetStorage())
      return;
  
    if(GetEnergy(obj) >= GetPhysical("Energy", 0, obj)/1000)
      continue;

    DoEnergy(+1, obj);
    DoStorage(-1);
  }
  
  //Clonks im Inneren/in Umgebung munitionieren
  for(var obj in FindObjects(Find_Distance(StorageRange()), Find_OCF(OCF_CrewMember), Find_Allied(GetOwner())))
  {
    //Speicher ist leer
    if(!GetStorage())
      return;
    
    //Wurde schon beliefert
    if(GetEffect("AmmoSupplyCooldown", obj))
      continue;

    //Munitionsbedarf feststellen
    var highestammo = 0, ammoID = 0;
    for(var i = 0; i < ContentsCount(0, obj); i++)
      if(Contents(i, obj)->~IsWeapon())
        for (var j = 0; j < Contents(i, obj)->GetFMCount(); j++)
        {
          var ammocount, weapon = Contents(i, obj);
          if(weapon->GetFMData(FM_NoAmmoModify, j)) continue;
          if(weapon->GetFMData(FM_AmmoLoad, j) <= 3)
            ammocount = weapon->GetFMData(FM_AmmoLoad, j) * 10;
          else
          ammocount = weapon->GetFMData(FM_AmmoLoad,j) * 3;
          if(GetAmmo(weapon->GetFMData(FM_AmmoID, j), obj) < ammocount)
          {
            if(!ammoID)
              ammoID = weapon->GetFMData(FM_AmmoID,j);
            if(highestammo < ammocount)
              highestammo = ammocount;
          }
        }

    if(!ammoID)
      continue;

    //Munition hinzufügen
    var factor = ammoID->~GetPointFactor();
    
    //Kosten prüfen (Halb so hoch wie bei Munitionspaketen)
    if((ammoID->MaxAmmo() / 10 * factor)/2 > GetStorage() || GetAmmo(ammoID, obj) >= highestammo)
      continue;

    PlayerMessage(GetOwner(Contained()), "$AmmoReceived$", obj, ammoID->MaxAmmo() / 10, ammoID);
    PlayerMessage(GetOwner(obj),"$AmmoReceived$", obj, ammoID->MaxAmmo() / 10, ammoID);
    DoAmmo(ammoID, ammoID->MaxAmmo()/10, obj);
    Sound("Resupply.ogg");
    
    //Speicher leeren
    DoStorage(-(ammoID->MaxAmmo()/10*factor)/2);
    
    //Für die nächsten 3 Sekunden nicht mehr beliefern
    AddEffect("AmmoSupplyCooldown", obj, 1, 35*3);
  }
  
  //Packs auffüllen
  for(var obj in FindObjects(Find_Container(this), Find_Func("IsStaticPack")))
  {
    //Speicher leer?
    if(!GetStorage())
      return;

    //Pack ist nicht voll?
    if(obj->~GetPackPoints() < obj->~MaxPoints())
    {
      //Auffüllen
      obj->DoPackPoints(1);
      DoStorage(-1);
    }
  }

  return true;
}

public func SetAdditionalEnergy(int iValue)	{return iAdditionalEnergy = iValue;}
