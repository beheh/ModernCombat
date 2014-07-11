/*-- Effektmanager --*/

#strict 2


/* Auswahlmenü */

protected func Activate(iByPlayer)
{
  /*var pClonk = GetCursor(iByPlayer);
  //Wenn Spieler 1 und keine Liga
  if(iByPlayer == GetPlayerByIndex(0, C4PT_User) && !GetLeague())
  {
    //Menü öffnen
    if(GetMenu(pClonk)) return;
    CreateMenu(GetID(), pClonk, 0,0,GetName(),0, 1);
    for(var i = 1; i <= 3; i++)
    {
      var szText = Format("$EffectLevel$", i);
      if(i == EFSM_Level) szText = Format("<c ffff33>%s</c>",szText);
      AddMenuItem(szText, "SetEffectLevel", EFMN, pClonk, 0, i, "$SetEffectLevel$");
    }
  }
  else
  {
    //Beschreibung anzeigen
    var szLevel = Format("Level %d", EFSM_Level);
    if(EFSM_Level == 0) szLevel = "$Custom$";
    MessageWindow(Format("$Desc$", szLevel),iByPlayer);
  }*/
  return(1);
}

local aTechLevels, aTeamUpgrades;

public func Initialize()
{
	aTechLevels = [];
	aTeamUpgrades = [];
	AddEffect("EnergyManagement", this, 1, 36, this);
	return true;
}

//Vorrübergehend für Tests: Geldcheat.
public func InitializePlayer(int iPlr) { SetWealth(iPlr, 0x7FFFFFFF); }

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

global func BuildingSystem() { return FindObject2(Find_ID(BGRL)); }

global func GetTeamTechLevel(int iTeam) { return LocalN("aTechLevels", BuildingSystem())[iTeam]; }
global func SetTeamTechLevel(int iTeam, int iLevel) { return LocalN("aTechLevels", BuildingSystem())[iTeam] = iLevel; } 

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

global func GetTeamUpgrade(int iTeam, id idUpgrade)
{
	return (GetIndexOf(idUpgrade, LocalN("aTeamUpgrades", BuildingSystem())[iTeam]) > -1);
}

global func GetTeamUpgradeList(int iTeam)
{
	return LocalN("aTeamUpgrades", BuildingSystem())[iTeam];
}
