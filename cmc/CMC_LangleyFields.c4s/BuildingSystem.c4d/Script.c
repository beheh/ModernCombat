/*-- Gebäudesystem --*/

#strict 2

local aTechLevels, aTeamUpgrades, aTeamResources, aOrders, aPresets;


/* Auswahlmenü */

protected func Activate(iByPlayer)
{
  return true;
}

/* Initialisierung */

public func Initialize()
{
  aTechLevels = [];
  aTeamUpgrades = [];
  aTeamResources = [];
  aOrders = [];
  aPresets = [];
  AddEffect("EnergyManagement", this, 1, 36, this);
  return true;
}

//Vorrübergehend für Tests: Geldcheat.
public func InitializePlayer(int iPlr)
{
  if(!aTechLevels[GetPlayerTeam(iPlr)])
    aTechLevels[GetPlayerTeam(iPlr)] = TECHLEVEL_Start;

  SetWealth(iPlr, 2000/*0x7FFFFFFF*/);
  LoadPresets(iPlr);
  return true;
}

/* Presets (Kaserne) */

public func MaxSlotCount() { return 3; } //Maximale Anzahl an Presets (jeweils 6 ExtraData Slots)

public func LoadPresets(int iPlr)
{
  //Einzelne Presets laden
  var presets = [];
  for(var i = 0; i < MaxSlotCount(); i++)
    presets[i] = LoadPresetSlot(iPlr, i);

  aPresets[GetPlayerID(iPlr)] = presets;
  return true;
}

public func LoadPresetSlot(int iPlr, int iSlot)
{
  var preset = [];

  preset[0] = GetPlrExtraData(iPlr, Format("CMC_Preset%d_Clonk", iSlot));
  
  //Muss Clonk sien
  if(preset[0] && !preset[0]->~IsClonk())
    preset[0] = 0;
  
  //Presetdaten sammeln und auf Richtigkeit überprüfen
  preset[1] = ValidPresetInvData(GetPlrExtraData(iPlr, Format("CMC_Preset%d_Inv1", iSlot)), preset);
  preset[2] = ValidPresetInvData(GetPlrExtraData(iPlr, Format("CMC_Preset%d_Inv2", iSlot)), preset);
  preset[3] = ValidPresetInvData(GetPlrExtraData(iPlr, Format("CMC_Preset%d_Inv3", iSlot)), preset);
  preset[4] = ValidPresetInvData(GetPlrExtraData(iPlr, Format("CMC_Preset%d_Inv4", iSlot)), preset);
  preset[5] = ValidPresetInvData(GetPlrExtraData(iPlr, Format("CMC_Preset%d_Inv5", iSlot)), preset);

  return preset;
}

public func ValidPresetInvData(id data, array aPreset)
{
  //Keine Daten
  if(!data)
    return;
  
  //Nur Ausrüstungsgegenstände oder Waffen
  if(!data->~IsEquipment() && !data->~IsWeapon())
    return;
  
  //Wird schon im Preset benutzt
  if(GetIndexOf(data, aPreset) != -1)
    return;

  return data;
}

public func SavePresets(int iPlr)
{
  //Einzelne Presets speichern
  for(var i = 0; i < MaxSlotCount(); i++)
    SavePresetSlot(iPlr, i);
  
  return true;
}

public func SavePresetSlot(int iPlr, int iSlot)
{
  var preset = aPresets[GetPlayerID(iPlr)][iSlot];
  
  //Einzelne Preset-Daten abspeichern
  SetPlrExtraData(iPlr, Format("CMC_Preset%d_Clonk", iSlot), preset[0]);
  
  //Inventar
  SetPlrExtraData(iPlr, Format("CMC_Preset%d_Inv1", iSlot), preset[1]);
  SetPlrExtraData(iPlr, Format("CMC_Preset%d_Inv2", iSlot), preset[2]);
  SetPlrExtraData(iPlr, Format("CMC_Preset%d_Inv3", iSlot), preset[3]);
  SetPlrExtraData(iPlr, Format("CMC_Preset%d_Inv4", iSlot), preset[4]);
  SetPlrExtraData(iPlr, Format("CMC_Preset%d_Inv5", iSlot), preset[5]);
  
  //Fertig
  return true;
}

static const CPRESET_Clonk = 0;
static const CPRESET_Inv1 = 1;
static const CPRESET_Inv2 = 2;
static const CPRESET_Inv3 = 3;
static const CPRESET_Inv4 = 4;
static const CPRESET_Inv5 = 5;

public func GetPresetData(int iPlr, int iSlot, int iData)
{
  var preset = aPresets[GetPlayerID(iPlr)][iSlot];
  return preset[iData];
}

public func GetFullPresetSlotData(int iPlr, int iSlot)
{
  return aPresets[GetPlayerID(iPlr)][iSlot];
}

public func SetPresetData(int iPlr, int iSlot, int iData, id idNew)
{
  return aPresets[GetPlayerID(iPlr)][iSlot][iData] = idNew;
}

/* Energieverwaltung */

public func FxEnergyManagementTimer()
{
  //Energie für jedes Team managen
  for(var i = 0; i < GetTeamCount(); i++)
  {
    var team = GetTeamByIndex(i), plr;
    if((plr = GetTeamMemberByIndex(team)) < 0)
      continue;

    var buildings = FindObjects(Find_Func("IsCMCBuilding"), Find_Allied(plr));
    if(!buildings)
      return false;

    var iEnergySupply, iEnergyNeed;
    for(var obj in buildings)
      iEnergySupply += obj->GetEnergyData(CCBS_EnergyProd);

    for(var obj in buildings)
      iEnergyNeed += obj->GetEnergyData(CCBS_EnergyNeed);

    var fEnergy = false;
    if(iEnergySupply-iEnergyNeed >= 0)
      fEnergy = true;

    for(var obj in buildings)
      obj->~EnergySupply(fEnergy, iEnergySupply, iEnergyNeed);
  }

  return true;
}

global func BuildingSystem()	{return FindObject2(Find_ID(BGRL));}

/* Lieferwarteschlange */

public func AddOrderToQueue(int iPlr, id idBuilding)
{
  var team = GetPlayerTeam(iPlr);
  if(!aOrders[team])
    aOrders[team] = [];
  
  var found;
  for(var i = 0; i < GetLength(aOrders[team]); i++)
  {
    var building = aOrders[team][i];
    if(building && building[0] == idBuilding)
    {
      found = true;
      aOrders[team][i][GetLength(building)] = iPlr;
      break;
    }
  }
  
  if(!found)
    aOrders[team][GetLength(aOrders[team])] = [idBuilding, iPlr];
  
  return true;
}

public func RemoveOrderFromQueue(int iPlr, id idBuilding)
{
  var team = GetPlayerTeam(iPlr);
  if(!aOrders[team])
    return;
  
  var order = [];
  for(var i = 0; i < GetLength(aOrders[team]); i++)
  {
    var b = aOrders[team][i];
    if(b && b[0] == idBuilding)
    {
      for(var j = 0; j < GetLength(b); j++)
      {
        if(b[j] != iPlr)
          order[GetLength(order)] = b[j];
      }
      
      aOrders[team][i] = order;
      break; 
    }
  }
  
  return true;
}

public func GetOrderQueueLength(int iPlr, id idBuilding)
{
  var team = GetPlayerTeam(iPlr);
  if(!aOrders[team])
    return;
  
  for(var building in aOrders[team])
    if(building && building[0] == idBuilding)
      return GetLength(building)-1;
  
  return 0;
}

public func GetOrderQueuePosition(int iPlr, id idBuilding)
{
  var team = GetPlayerTeam(iPlr);
  if(!aOrders[team])
    return;
  
  var pos = -1;
  for(var building in aOrders[team])
    if(building && building[0] == idBuilding)
    {
      for(var i = 0; i < GetLength(building); i++)
        if(building[i] == iPlr)
          break;
      
      if(i == GetLength(building))
        pos = -1;
      else
        pos = i-1;
      break;
    }
  
  return pos;
} 

/* Liefercooldown */

public func DeliveryCooldown(int iPlr, id idBuilding)
{
  AddEffect("DeliveryCooldown", this, 1, idBuilding->~DeliveryCooldownTime(), this, 0, iPlr, idBuilding);
  return true;
}

public func GetDeliveryCooldown(int iPlr, id idBuilding)
{
  var i,e;
  while(e = GetEffect("DeliveryCooldown", this, i))
  {
    if(EffectVar(0, this, e) == idBuilding && EffectVar(1, this, e) == GetPlayerTeam(iPlr))
      return true;

    i++;
  }
  
  return false;
}

public func FxDeliveryCooldownStart(object pTarget, int iNr, int iTemp, int iPlr, id idBuilding)
{
  if(iTemp)
    return;

  EffectVar(0, pTarget, iNr) = idBuilding;
  EffectVar(1, pTarget, iNr) = GetPlayerTeam(iPlr);
  return true;
}

/* Techstufen */

static const TECHLEVEL_None	= 0;
static const TECHLEVEL_Start	= 1;
static const TECHLEVEL_1	= 2;
static const TECHLEVEL_2	= 4;
static const TECHLEVEL_3	= 8;

global func GetTeamTechLevel(int iTeam, int iLevel)	{return LocalN("aTechLevels", BuildingSystem())[iTeam] & iLevel;}
global func SwitchTeamTechLevel(int iTeam, int iLevel)	{return LocalN("aTechLevels", BuildingSystem())[iTeam] ^= iLevel;}

global func SetTeamTechLevel(int iTeam, int iLevel, bool fSet)
{
  //Bleibt gleich?
  if(!!GetTeamTechLevel(iTeam, iLevel) == fSet)
    return true;

  SwitchTeamTechLevel(iTeam, iLevel);
  
  //Callback
  FindObjects(Find_Func("OnTechLevelChange", iLevel, fSet), Find_Allied(GetTeamMemberByIndex(iTeam)));
  return true;
}

/* Upgrades */

global func AddTeamUpgrade(int iTeam, id idUpgrade)
{
  if(!LocalN("aTeamUpgrades", BuildingSystem())[iTeam])
    LocalN("aTeamUpgrades", BuildingSystem())[iTeam] = [idUpgrade];
  else
    LocalN("aTeamUpgrades", BuildingSystem())[iTeam][GetLength(LocalN("aTeamUpgrades", BuildingSystem())[iTeam])] = idUpgrade;

  for(var obj in FindObjects(Find_Category(C4D_Structure), Find_Func("IsCMCBuilding")))
    if(GetPlayerTeam(GetOwner(obj)) == iTeam && GetIndexOf(idUpgrade, obj->~PossibleUpgrades()) > -1)
      obj->AddUpgrade(idUpgrade);


  return true;
}

global func GetTeamUpgrade(int iTeam, id idUpgrade)	{return (GetIndexOf(idUpgrade, LocalN("aTeamUpgrades", BuildingSystem())[iTeam]) > -1);}
global func GetTeamUpgradeList(int iTeam)		{return LocalN("aTeamUpgrades", BuildingSystem())[iTeam];}

global func DoTeamResources(int iTeam, int iAmount)	{return LocalN("aTeamResources", BuildingSystem())[iTeam] += iAmount;}
global func GetTeamResources(int iTeam)			{return LocalN("aTeamResources", BuildingSystem())[iTeam];}
