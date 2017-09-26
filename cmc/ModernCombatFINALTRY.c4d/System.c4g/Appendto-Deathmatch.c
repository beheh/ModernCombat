/*-- Deathmatch --*/

//Das Eliminieren von Teammitgliedern oder sich selbst zieht einen Punkt ab. Spielzielbeschreibung eingef�gt.

#strict 2
#appendto GTDM

local aMessages;

public func GoalDescription()	{return "$DMGoalDesc$";}


protected func Initialize()
{
  aMessages = [];
  return _inherited(...);
}

public func GoalExtraValue()	{return iWinScore;}	//Spielzielinformationen an Scoreboard weitergeben

public func RelaunchPlayer(int iPlr, object pClonk, int iMurdererPlr)
{
  var mTeam = GetPlayerTeam(iMurdererPlr);
  if(iMurdererPlr == -1 && GetPlayerName(GetOwner(pClonk)))
    DoTeamScore(GetPlayerTeam(GetOwner(pClonk)),-1);  
  //Teamkill?
  if(GetPlayerTeam(iPlr) == mTeam)
    //Punktestand erh�hen
    DoTeamScore(mTeam,-1);

  inherited(iPlr, pClonk, iMurdererPlr);

  var score, winscore = GetWinScore(FindObject(GTDM));
  if(score = TeamGetScore(mTeam))
  {
    if(score >= winscore - (20 * winscore / 100) && !aMessages[mTeam])
    {
      aMessages[mTeam] = true;
      var message = Format("$DMTeamIsWinning$", GetTaggedTeamName(mTeam), winscore - score);
      if(!Teams() || GetTeamPlayerCount(mTeam) == 1)
        message = Format("$DMPlayerIsWinning$", GetTaggedPlayerName(iMurdererPlr), winscore - score);

      EnemyEventInfo(iMurdererPlr, message, GTDM, 0, 0, 0, "Info_Alarm.ogg");
    }
    else
      aMessages[mTeam] = false;
  }
}

public func OnPlayerRankUp(int iPlr)
{
  //Scoreboard-Spielerr�nge aktualisieren
  if(GetTeamPlayerCount(GetPlayerTeam(iPlr)) > 1)
    SetScoreboardData(iPlr, TEAM_PlayerColumn, GetTaggedPlayerName(iPlr, true));
  else
    SetScoreboardData(iPlr, TEAM_TeamColumn, GetTaggedPlayerName(iPlr, true), GetPlayerTeam(iPlr));

  return true;
}

public func WinScoreChange(int iNewScore)
{
  //Einstellbarer Punktestand bei Ligarunden begrenzen
  if(GetLeague())
    iNewScore = BoundBy(iNewScore, 5, 30);
  else
    iNewScore = BoundBy(iNewScore, 2, 50);

  return _inherited(iNewScore, ...);
}