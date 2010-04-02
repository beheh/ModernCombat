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