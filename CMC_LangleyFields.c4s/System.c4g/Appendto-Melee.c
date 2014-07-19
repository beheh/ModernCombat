/*-- Neues Script --*/

#strict 2

#appendto MELE

public func OnBuildingDestroyed(id idBuilding)
{
  if(idBuilding == CBAS)
    return IsFulfilled();
  
  return true;
}

public func IsFulfilled()
{
  // Anfangszahl Spieler bestimmen
  if(!StartPlayerCount)
  {
    if((StartPlayerCount = GetPlayerCount()) < 2)
    {
      Log("$MsgTooFewPlayers$");
      return RemoveObject();
    }
  }
  
  // Wenn Teams.txt-Teams noch gewählt werden müssen, kann das Ziel nicht erfüllt sein
  if(GetPlayerTeam(GetPlayerByIndex()) == -1) return;
    
  //Bei Stützpunktverlust Team eliminieren
  for(var i = 0; i < GetTeamCount(); i++)
  {
    var team = GetTeamByIndex(i);
    if(GetTeamPlayerCount(team) && !GetTeamTechLevel(team, TECHLEVEL_Start) && !FindObject2(Find_ID(CBAS), Find_Allied(GetTeamMemberByIndex(team))))
      EliminateTeam(team);
  }

  // Alle Spieler durchgehen
  for(var i = 0; i < GetPlayerCount(); ++ i)
  {
    var plr = GetPlayerByIndex(i);
    // Mit den anderen Spielern vergleichen
    for(var j = i + 1; j < GetPlayerCount(); ++ j)
    {
      var plr2cmp = GetPlayerByIndex(j); 
      // Noch verfeindete Spieler da? Nicht erfüllt.
      if(CheckTeamHostile(plr, plr2cmp)) return;
    }
  }
    
  // Alle verbündet, also erfüllt
  return true;
}
