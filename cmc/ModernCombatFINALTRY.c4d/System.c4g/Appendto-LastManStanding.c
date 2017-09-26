/*-- Last Man Standing --*/

//Last Man Standing-Fehler korrigiert. Spielzielbeschreibung eingefügt.

#strict 2
#appendto GLMS

local init;

public func GoalExtraValue()	{return iWinScore;}	//Spielzielinformationen an Scoreboard weitergeben
public func GoalDescription()	{return "$LMSGoalDesc$";}


protected func InitializePlayer(int iPlr, int iX, int iY, object pBase, int iTeam)
{
  if(!chooser) return _inherited(iPlr, iX, iY, pBase, iTeam, ...);
  return;
}

private func InitScoreboard()
{
  if(!init && !chooser)
  {
    init = true;
    ScheduleCall(this, "InitScoreboard", 1);
    for(var i = 0 ; i < GetPlayerCount() ; i++)
    {
      var iPlr = GetPlayerByIndex(i);
      aPlayerLives[iPlr] = iWinScore * GetCrewCount(iPlr) -1;

      //Anzeigen
      if(GetTeamPlayerCount(GetPlayerTeam(iPlr)) > 1)
        SetScoreboardData(iPlr, TEAM_GoalColumn, Format("%d",aPlayerLives[iPlr]), aPlayerLives[iPlr]);
      else
        SetScoreboardData(iPlr, TEAM_GoalColumn, Format("<c %x>%d</c>", GetPlrColorDw(iPlr),aPlayerLives[iPlr]), aPlayerLives[iPlr]);
    }
  }
  else
  if(!init)
  {
    ScheduleCall(this, "InitScoreboard", 1);
  }
  return _inherited(...);
}

/* Auswertung */

local rewarded;

public func IsFulfilled()
{
  if (_inherited(...))
  {
    if (!rewarded)
      RewardEvaluation();
    rewarded = true;
    return true;
  }
}

/* Warnmeldung */

public func RelaunchPlayer(int iPlr, object pClonk, int iMurdererPlr) 
{
  inherited(iPlr, pClonk, iMurdererPlr);

  if(iPlr == -1 || !GetPlayerName(iPlr)) 
    return;

  var warning = 30 * iWinScore / 100;
  if(aPlayerLives[iPlr] == warning)
  {
    if(warning)
      //Eventnachricht: Warnung vor niedriger Respawnzahl
      EventInfo4K(iPlr+1, Format("$LivesLow$", aPlayerLives[iPlr]), SM03, 0, 0, 0, "Info_Alarm.ogg");
    else
      if(iWinScore > 1)
        //Eventnachricht: Hinweis auf aufgebrauchte Respawns
        EventInfo4K(iPlr+1, Format("$NoLivesLeft$"), SM03, 0, 0, 0, "Info_Alarm.ogg");
  }
}

public func OnPlayerRankUp(int iPlr)
{
  //Scoreboard-Spielerränge aktualisieren
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