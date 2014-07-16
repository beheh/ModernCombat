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
  
  return true;
}

public func SwitchStorageSupply(id dummy, object pMenuObj)
{
  var storage = GetEffect("AmmoStorage", this);
  EffectVar(0, this, storage) = !EffectVar(0, this, storage);
  OpenBuildingMenu(0, pMenuObj);
  
  return true;
}

public func OpenClonkBuyMenu(id dummy, object pMenuObj)
{
  return OpenBuyMenu(0, pMenuObj, 0, 0, [PCMK, MDIC]);
}

public func OpenEquipmentBuyMenu(id dummy, object pMenuObj)
{
  var weapons = [PSTL, RVLR, SMGN, ASTR, MNGN, PPGN], weaponadd = [];
  var grenades = [FRAG, STUN, SGRN, SRBL], grenadeadd = [];
  var ammo = [ABOX, GBOX, MBOX];
  var equipment = [BWTH, DGNN, CDBT], equipmentadd = [];
  if(GetUpgrade(U_XA))
  {
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

  return OpenBuyMenu(0, pMenuObj, 0, 0, items);
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
  //TODO
  
  return true;
}

public func SetAdditionalEnergy(int iValue)	{return iAdditionalEnergy = iValue;}
