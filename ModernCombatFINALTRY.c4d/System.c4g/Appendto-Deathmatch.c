/*-- Deathmatch --*/

//Das Eliminieren von Teammitgliedern oder sich selbst zieht einen Punkt ab.

#strict 2

#appendto GTDM


public func RelaunchPlayer(int iPlr, object pClonk, int iMurdererPlr)
{
  if(iMurdererPlr == -1)
    DoTeamScore(GetPlayerTeam(GetOwner(pClonk)),-1);  
  //Teamkill?
  if(GetPlayerTeam(iPlr) == GetPlayerTeam(iMurdererPlr))
    //Punktestand erhöhen
    DoTeamScore(GetPlayerTeam(iMurdererPlr),-1);
    
  inherited(iPlr, pClonk, iMurdererPlr);
}

public func GetHUDInfo(int player) {
	var team = GetPlayerTeam(player);
	
	// Rückstand
	var diff;
	diff = Max(TeamGetScore(team)) - BestTeamGetScore(team);
	
	var secondstring;
	if(!diff) secondstring= "";
	if(diff > 0) secondstring = Format("(+%d)", diff);
	if(diff < 0) secondstring = Format("(%d)", diff);
	
	return Format("%d Kills %s", TeamGetKills(team), secondstring);
}