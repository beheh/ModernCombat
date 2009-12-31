#strict 2

global func IsAIControlled() {} // Dummy-Funktion wegen 4K-Bot. ;)
global func SetupBot4K() {}

global func AddAI(int iTeam)
{
  return _inherited(iTeam);
}

global func AddBot(int iTeam)
{
  // Spieler suchen
  for(var i=0, found = -1 ; i < GetPlayerCount(C4PT_Script) ; i++)
    if(GetPlayerTeam(GetPlayerByIndex(i, C4PT_Script)) == iTeam)
    {
      found = GetPlayerByIndex(i, C4PT_Script);
      break;
    }
  // Spieler vorhanden?
  if(found == -1) return AddAI(iTeam);
  // Clonk erstellen
  var bot = CreateObject(CLNK, 0,0, found);
  MakeCrewMember(bot, found);
  GameCall("RelaunchPlayer", found, bot, -1);
  return bot;
}
