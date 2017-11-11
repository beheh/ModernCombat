/*-- Versionsupdates --*/

//Aktualisiert CMC betreffende, veraltete Datensätze in Spielerdateien.

#strict 2

static const CMC_CURRENT_VERSION = 3;


global func UpdatePlayerData(int iPlr)
{
  var plrversion = GetPlrExtraData(iPlr, "CMC_PlayerData_Version");
  if(plrversion == CMC_CURRENT_VERSION)
    return true;

  var aUpdatedData = [["CMC_PlayerData_Version", CMC_CURRENT_VERSION]], iDMModules = GetPlrExtraData(iPlr, "CMC_DeathMenuModules");

  if(plrversion < 3)
  {
    if(!GetPlrExtraData(iPlr, "CMC_ScoreDisplayType"))
      SetPlrExtraData(iPlr, "CMC_ScoreDisplayType", 1);
  }

  if(plrversion < 2)
  {
    if(!iDMModules)
      iDMModules = FKDT_DeathMenu_DefaultSetting;
    if(GetPlrExtraData(iPlr, "CMC_DeathMenuMode"))
    {
      iDMModules = FKDT_DeathMenu_CompactSetting;
      aUpdatedData[GetLength(aUpdatedData)] = ["CMC_DeathMenuMode", 0];
    }
    iDMModules |= FKDT_DeathMenu_SpectateMenuItem;

    aUpdatedData[GetLength(aUpdatedData)] = ["CMC_RadioMusicMode", 0];
  }

  aUpdatedData[GetLength(aUpdatedData)] = ["CMC_DeathMenuModules", iDMModules];

  for(var updatedata in aUpdatedData)
    //Bei 0-Wert prüfen ob überhaupt vorhanden
    if(updatedata[1] || GetPlrExtraData(iPlr, updatedata[0]))
      SetPlrExtraData(iPlr, updatedata[0], updatedata[1]); 
}