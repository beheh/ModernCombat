/*-- Teams --*/

//Damit in jedem Falle mehrere Spielziele zur Auswahl stehen können.

#strict 2

#appendto TEAM

protected func MenuQueryCancel()	{return true;}


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

global func GetActiveTeamCount(bool fAliveOnly)
{
  var aTeams = [];
  for (var i; i < GetPlayerCount(); i++)
    if (GetPlayerName(GetPlayerByIndex(i)) && GetPlayerTeam(GetPlayerByIndex(i)) != -1)
    {
      if(fAliveOnly)
      {
        var pCrew;
        var j = 0;
        while (pCrew = GetCrew(GetPlayerByIndex(i), j))
        {
          j++;
          if(pCrew->~IsFakeDeath()) continue;
          aTeams[GetPlayerTeam(GetPlayerByIndex(i))] = 1;
          break;
        }
      }
      else
        aTeams[GetPlayerTeam(GetPlayerByIndex(i))] = 1;
    }
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

global func GetTeamMemberByIndex(int iTeam, int iIndex)
{
  var index;
  for(var i = 0; i < GetPlayerCount(); i++)
  {
    if(GetPlayerTeam(GetPlayerByIndex(i)) == iTeam)
    {
      if(iIndex == index)
        return GetPlayerByIndex(i);

      ++index;
    }
  }

  return -1;
}

global func GetActiveTeamByIndex(int iIndex)
{
  var aTeams = [];
  for(var i, iTeam; i != GetPlayerCount(); i++)
    if(iTeam = GetPlayerTeam(GetPlayerByIndex(i)))
      if(GetIndexOf(iTeam, aTeams) == -1)
        aTeams[GetLength(aTeams)] = iTeam;

  if (aTeams[iIndex])
  {
    return aTeams[iIndex];
  }
  else
  {
    return -1;
  }
}

/* Auswertung */

public func IsFulfilled() // Siegreiches Team?
{
  //Menschliche Spieler suchen
  if(!GetPlayerCount(C4PT_User))
  {
    if(IsNetwork()) //Bei nein im lokalen Netzwerk beenden
    {
      return(1);
    }
  }

  var team;
  for(var iScore in aPoints)
  {
    if(iScore >= iWinScore)
    {
      EliminateLosers();

      Message("@<c %x>$WinMsg$</c>",0,GetTeamColor(team),GetTeamName(team));

      //Leben noch Verlierer? = Ligainkompatibilität
      if(LosersAlive())
        return(0);

      return(1);
    }
    team++;
 }
  //Nur noch ein Team und das hat schon Punkte? = Sieg (Verlierer sind rausgegangen)
  team = OneTeam();
  if(team != -1)
    if(aPoints[team] > 0)
    {
      Message("@<c %x>$WinMsg$</c>",0,GetTeamColor(team),GetTeamName(team));
      return(1);
    }
}

public func Evaluate()
{
  return RewardEvaluation();
}

/* Eigene Icons */

func DoEvaluation(int plr)
{
  return false;
}

//Team eintragen, dass nur einen Spieler hat
private func InitSingleplayerTeam(int iPlr)
{
  if(iPlr == -1) return 0;

  var col = GetPlrColorDw(iPlr);

  SetScoreboardData(iPlr, SBRD_Caption, "");
  SetScoreboardData(iPlr, TEAM_TeamColumn, GetTaggedPlayerName(iPlr, true), GetPlayerTeam(iPlr));
  SetScoreboardData(iPlr, TEAM_PlayerColumn, "");
  SetScoreboardData(iPlr, TEAM_GoalColumn,  Format("<c %x>%d</c>", col, aPoints[GetPlayerTeam(iPlr)]), aPoints[GetPlayerTeam(iPlr)]);
  SetScoreboardData(iPlr, TEAM_KillColumn,  Format("<c %x>%d</c>", col, TeamGetKills(GetPlayerTeam(iPlr))), TeamGetKills(GetPlayerTeam(iPlr))+1);
  SetScoreboardData(iPlr, TEAM_DeathColumn, Format("<c %x>%d</c>", col, aDeath[iPlr]), aDeath[iPlr]);
  SortTeamScoreboard();
}

//Spieler eintragen
private func InitPlayer(int iPlr)
{
  // Team 0 oder -1 können nicht behandelt werden
  if(GetPlayerTeam(iPlr) < 1) return 0;
  if(iPlr == -1) return 0;

  UpdateHUDs();

  // Eintragen
  SetScoreboardData(iPlr, SBRD_Caption, "");
  SetScoreboardData(iPlr, TEAM_TeamColumn, Format("<c %x>\\</c>",GetTeamColor(GetPlayerTeam(iPlr))), GetPlayerTeam(iPlr));
  SetScoreboardData(iPlr, TEAM_PlayerColumn, GetTaggedPlayerName(iPlr, true));
  SetScoreboardData(iPlr, TEAM_GoalColumn, "", aPoints[GetPlayerTeam(iPlr)]);
  SetScoreboardData(iPlr, TEAM_KillColumn, Format("%d", aKill[iPlr]), aKill[iPlr]);
  SetScoreboardData(iPlr, TEAM_DeathColumn, Format("%d", aDeath[iPlr]), aDeath[iPlr]);
  SortTeamScoreboard();
}

/* Geldverteilung */

public func Money(int iPlr, object pClonk, int iMurdererPlr)
{
  if(iMurdererPlr != NO_OWNER)
  {
    //Teamkill
    if(GetPlayerTeam(iPlr) == GetPlayerTeam(iMurdererPlr))
      //Geldbonus: -20 Clunker
      DoWealth(iMurdererPlr, -20);
    //Kill
    else
      //Geldbonus: 25 Clunker
      DoWealth(iMurdererPlr, 25);
  }
  //Eigener Tod
  //Geldbonus: 10 Clunker
  DoWealth(iPlr, 10);
}

public func TeamGetScore(int iTeam)
{
  return aPoints[iTeam];
}

global func HostileTeam(int iTeam1, int iTeam2)
{
  if(iTeam1 == iTeam2 || iTeam1 == 0 || iTeam2 == 0) 
    return;

  return 1;
}