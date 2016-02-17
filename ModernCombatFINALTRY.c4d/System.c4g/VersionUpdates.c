/*-- Versionsupdates --*/

//Aktualisiert CMC betreffende, veraltete Datensätze in Spielerdateien.

#strict 2

static const CMC_CURRENT_VERSION = 1;


global func UpdatePlayerData(int iPlr)
{
  var plrversion = GetPlrExtraData(iPlr, "CMC_PlayerData_Version");
  if(plrversion == CMC_CURRENT_VERSION)
    return true;

  var aUpdatedData = [["CMC_PlayerData_Version", CMC_CURRENT_VERSION]], iDMModules = GetPlrExtraData(iPlr, "CMC_DeathMenuModules");

  if(plrversion < 1) {
  	if(!iDMModules)
  		iDMModules = FKDT_DeathMenu_DefaultSetting;
  	if(GetPlrExtraData(iPlr, "CMC_DeathMenuMode")) {
  		iDMModules = FKDT_DeathMenu_CompactSetting;
  		aUpdatedData[GetLength(aUpdatedData)] = ["CMC_DeathMenuMode", 0];
  	}
    iDMModules |= FKDT_DeathMenu_SpectateMenuItem;
  }

  aUpdatedData[GetLength(aUpdatedData)] = ["CMC_DeathMenuModules", iDMModules];

  for(var updatedata in aUpdatedData)
    SetPlrExtraData(iPlr, updatedata[0], updatedata[1]); 
}
