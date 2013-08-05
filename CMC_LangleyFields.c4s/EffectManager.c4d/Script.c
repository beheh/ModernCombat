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

local aTechLevels;

public func Initialize()
{
	aTechLevels = [];
	AddEffect("EnergyManagement", this, 1, 36, this);
	return true;
}

public func FxEnergyManagementTimer()
{
	var buildings = FindObjects(Find_Func("IsCMCBuilding"));
	if(!buildings)
		return false;
	
	for(var obj in buildings)
		obj->ResetCurrentEnergy();
	
	for(var obj in buildings)
		obj->SendEnergy();

	for(var obj in buildings)
		obj->~EnergySupplyFinished();
	
	return true;
}



global func GetTeamTechLevel(int iTeam) { return LocalN("aTechLevels", FindObject2(Find_ID(BGRL)))[iTeam]; }
global func SetTeamTechLevel(int iTeam, int iLevel) { return LocalN("aTechLevels", FindObject2(Find_ID(BGRL)))[iTeam] = iLevel; } 


