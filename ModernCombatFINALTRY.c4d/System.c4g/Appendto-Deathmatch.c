/*-- Deathmatch: Minuspunkte bei Self- oder Teamkill --*/

#strict 2
#appendto GTDM

public func RelaunchPlayer(int iPlr, object pClonk, int iMurdererPlr)
{
  inherited(iPlr, pClonk, iMurdererPlr);
  if(iPlr == -1 || !GetPlayerName(iPlr)) return;
  if(iMurdererPlr == -1) //Selfkill? Tja, Pech gehabt.
    return DoTeamScore(GetPlayerTeam(GetOwner(pClonk)),-1);
  
  // kein Teamkill?
  if(GetPlayerTeam(iPlr) != GetPlayerTeam(iMurdererPlr))
    //Punktestand erhöhen
    DoTeamScore(GetPlayerTeam(iMurdererPlr),+1);
  else
    //Teamkill0r!!111einself D:
    DoTeamScore(GetPlayerTeam(iMurdererPlr),-1);
}
