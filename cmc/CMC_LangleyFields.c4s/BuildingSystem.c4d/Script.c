/*-- Gebäudesystem --*/

#strict 2

local aTechLevels, aTeamUpgrades, aTeamResources;


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
  AddEffect("EnergyManagement", this, 1, 36, this);
  return true;
}

//Vorrübergehend für Tests: Geldcheat.
public func InitializePlayer(int iPlr)
{
  if(!aTechLevels[GetPlayerTeam(iPlr)])
    aTechLevels[GetPlayerTeam(iPlr)] = TECHLEVEL_Start;

  SetWealth(iPlr, 2000/*0x7FFFFFFF*/);
  return true;
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
  if(GetTeamTechLevel(iTeam, iLevel) == fSet)
    return true;

  SwitchTeamTechLevel(iTeam, iLevel);
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
