/*-- LMS-Bugs --*/

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
	if(!chooser) return _inherited();
	if(init) return ScheduleCall(this, "InitScoreboard", 1);
	init = true;
	_inherited();
	ScheduleCall(this, "InitScoreboard", 1);
 	for(var i = 0 ; i < GetPlayerCount() ; i++) {
	 	var iPlr = GetPlayerByIndex(i);
		aPlayerLives[iPlr] = iWinScore * GetCrewCount(iPlr) -1;
		
		// anzeigen
		if(GetTeamPlayerCount(GetPlayerTeam(iPlr)) > 1)
			SetScoreboardData(iPlr, TEAM_GoalColumn, Format("%d",aPlayerLives[iPlr]), aPlayerLives[iPlr]);
		else
			SetScoreboardData(iPlr, TEAM_GoalColumn, Format("<c %x>%d</c>", GetPlrColorDw(iPlr),aPlayerLives[iPlr]), aPlayerLives[iPlr]);
	}
}
