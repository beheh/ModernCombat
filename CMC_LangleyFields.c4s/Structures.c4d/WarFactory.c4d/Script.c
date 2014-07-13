/*-- Fahrzeugfabrik --*/

#strict 2
#include CCBS

public func TechLevel()		{return 3;}	//Techstufe
public func BuildingRadius()	{return 200;}	//Bauradius
public func RequiredEnergy() 	{return 100;}	//Energieverbraucher
public func BasementID()	{return BT01;}	//Fundament

/* Initalisierung */

protected func Initialize()
{
  //Effekte
  if(HasEnergy())
    Sound("Building_PowerOn.ogg");
  else
    Sound("Building_PowerOff.ogg");

  return _inherited(...);
}

/* Menüführung */

public func AdditionalBuildingMenu(object pMenuObj)
{
  //Fahrzeuge bauen
  AddMenuItem("$CreateVehicles$", "OpenVehicleMenu", CVHC, pMenuObj, 0, pMenuObj, "$VehicleMenuDesc$");
  //Ausrüstung kaufen
  AddMenuItem("$BuyEquipment$", "OpenEquipmentMenu", GEAR, pMenuObj, 0, pMenuObj, "$EquipmentMenuDesc$");

  return true;
}

static const CWFY_Category_Weapon	= 1;
static const CWFY_Category_Equipment	= 2;
static const CWFY_Category_Grenades	= 3;

public func OpenEquipmentMenu(id dummy, object pMenuObj)
{
  MenuHeader(pMenuObj, "$EquipmentMenu$");

  AddMenuItem("$ChooseCategory$", 0, NONE, pMenuObj);

  AddMenuItem("$CategoryWeapons$", Format("OpenEquipCategory(Object(%d), %d)", ObjectNumber(pMenuObj), CWFY_Category_Weapon), WPN2, pMenuObj);
  AddMenuItem("$CategoryEquipment$", Format("OpenEquipCategory(Object(%d), %d)", ObjectNumber(pMenuObj), CWFY_Category_Equipment), C4PA, pMenuObj);
  AddMenuItem("$CategoryGrenades$", Format("OpenEquipCategory(Object(%d), %d)", ObjectNumber(pMenuObj), CWFY_Category_Grenades), NADE, pMenuObj);
}

public func OpenEquipCategory(object pMenuObj, int iCat, int iSelection)
{
  MenuHeader(pMenuObj, "$CategoryWeapons$");

  var def, i, sel;
  while(def = GetPlrKnowledge(GetOwner(pMenuObj), 0, i++, C4D_Object))
  {
    if(iCat == CWFY_Category_Weapon)
    {
      if(!def->~IsWeapon())
        continue;
    }
    else if(iCat == CWFY_Category_Equipment)
    {
      if(!def->~IsEquipment())
        continue;
      if(def->~IsGrenade())
        continue;
    }
    else if(iCat == CWFY_Category_Grenades)
    {
      if(!def->~IsGrenade())
        continue;
    }

    AddMenuItem(GetName(0, def), Format("ProcessBuy(%i, Object(%d), %d, %d)", def, ObjectNumber(pMenuObj), iCat, sel++), def, pMenuObj);
  }

  AddMenuItem("$Back$", "OpenEquipmentMenu", NONE, pMenuObj, 0, pMenuObj); 
  SelectMenuItem(iSelection, pMenuObj);

  return true;
}

public func ProcessBuy(id idItem, object pMenuObj, int iCategory, int iSelection)
{
  if(GetWealth(GetOwner(pMenuObj)) > GetValue(0, idItem))
    Buy(idItem, GetOwner(pMenuObj), GetOwner(pMenuObj), this);

  return OpenEquipCategory(pMenuObj, iCategory, iSelection);
}

public func OpenVehicleMenu(id dummy, object pMenuObj)
{
  MenuHeader(pMenuObj, "$VehicleMenu$");

  var def, i, plr = GetOwner(pMenuObj);
  while(def = GetPlrKnowledge(plr, 0, i++, C4D_Vehicle))
    if(def)
      AddMenuItem(GetName(0, def), "StartVehicleCreatingProcess", def, pMenuObj);

  return true;
}

/* Fahrzeugkonstruktion */

public func StartVehicleCreatingProcess(id idDef, object pMenuObj)
{
  //Checks ob bauen soll etc. fehlen

  AddEffect("CreateVehicle", this, 1, 36, this, 0, GetOwner(pMenuObj), idDef);
}

public func FxCreateVehicleStart(object pTarget, int iNr, int iTemp, int iOwner, id idDef)
{
  EffectVar(0, pTarget, iNr) = iOwner;
  EffectVar(1, pTarget, iNr) = idDef;

  Sound("SF_Build", 0, pTarget, 0, 0, +1);
}

public func FxCreateVehicleTimer(object pTarget, int iNr, int iTime)
{
  //Entrancewerte
  for(var i = 10; i > 0; i--)
    pTarget->CreateParticle("Frazzle", -43+Random(48), 10+Random(35), RandomX(-20, 20), RandomX(-20, 20), 30, HSL(60, 150, RandomX(150, 250)), pTarget);

  if(iTime > 360)
    return -1;
}

public func FxCreateVehicleStop(object pTarget, int iNr)
{
  Sound("SF_Build", 0, pTarget, 0, 0, -1);

  CreateObject(EffectVar(1, pTarget, iNr), -22, 35, EffectVar(0, pTarget, iNr));
  return true;
}