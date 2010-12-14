/*-- Teams --*/

//Damit in jedem Falle mehrere Spielziele zur Auswahl stehen können.

#strict 2

#appendto TEAM


private func InitScoreboard()
{
  if(FindObject(CHOS)) return;
  return _inherited();
}

protected func Initialize()
{
  aPoints = CreateArray();
  aKill = CreateArray();
  aDeath = CreateArray();
  if(!FindObject(CHOS))
    ChooserFinished();

  if(!FindObject(GOAL)) CreateObject(GOAL,0,0,-1);

  return inherited(...);
}

private func UpdateHUD()
{
  //Deaktiviert
  return false;
}

private func UpdateHUDs()
{
  //Deaktiviert
  return false;
}

private func GetGoalHUD(player)
{
  //Deaktiviert
  return false;
}

//Globales für Ziele und Teams

global func EliminateTeam(int iTeam)
{
  for(var i; i < GetPlayerCount(); i++)
    if(GetPlayerTeam(GetPlayerByIndex(i)) == iTeam)
      EliminatePlayer(GetPlayerByIndex(i));
}

global func GetTaggedTeamName(int iTeam)
{
  if (!GetTeamName(iTeam)) return;
  return Format("<c %x>%s</c>", GetTeamColor(iTeam), GetTeamName(iTeam));
}

global func GetActiveTeamCount()
{
  var aTeams = [];
  for (var i; i < GetPlayerCount(); i++)
    if (GetPlayerName(GetPlayerByIndex(i)))
      aTeams[GetPlayerTeam(GetPlayerByIndex(i))] = 1;
  i = 0;
  for (var item in aTeams)
    i += item;
  return i;
}

global func GetTeamPlayerCount(int iTeam)
{
  for (var i, count; i < GetPlayerCount(); i++)
    if (GetPlayerTeam(GetPlayerByIndex(i)) == iTeam && GetPlayerName(GetPlayerByIndex(i)))
	  count++;
  return count;
}

/* Eigene Icons */

func DoEvaluation(int plr)
{
  AddEvaluationData(Format("{{PSTL}}$Kills$: %d", aKill[plr]), GetPlayerID(plr));
  AddEvaluationData(Format("{{KAMB}}$Death$: %d", aDeath[plr]), GetPlayerID(plr));
}