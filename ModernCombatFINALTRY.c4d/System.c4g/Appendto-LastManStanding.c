/*-- LMS-Fix --*/

//Last Man Standing Bugs �berladen und korrigiert.

#strict 2
#appendto GLMS

local init;


protected func InitializePlayer(int iPlr, int iX, int iY, object pBase, int iTeam)
{
  if(!chooser) return _inherited(iPlr, iX, iY, pBase, iTeam);
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
  return _inherited();
}

//Auswertung

local rewarded;

public func IsFulfilled() {
  if (_inherited(...)) {
    if (!rewarded)
      RewardEvaluation();
    rewarded = true;
	return true;
  }
}