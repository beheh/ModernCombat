/*-- Deathmatch --*/

//Das Eliminieren von Teammitgliedern oder sich selbst zieht einen Punkt ab. Zudem werden Spielzielhinweise bei Rundenstart erstellt.

#strict 2

#appendto GTDM


public func RelaunchPlayer(int iPlr, object pClonk, int iMurdererPlr)
{
  if(iMurdererPlr == -1 && GetPlayerName(GetOwner(pClonk)))
    DoTeamScore(GetPlayerTeam(GetOwner(pClonk)),-1);  
  //Teamkill?
  if(GetPlayerTeam(iPlr) == GetPlayerTeam(iMurdererPlr))
    //Punktestand erhöhen
    DoTeamScore(GetPlayerTeam(iMurdererPlr),-1);

  inherited(iPlr, pClonk, iMurdererPlr);
}

public func ChooserFinished()
{
  //Spielzielhinweise erstellen
  for(var i = 0; i < GetPlayerCount(); i++)
  {
    DoScoreboardShow(1, GetPlayerByIndex(i) + 1);
    CreateObject(TK07, 0, 0, GetPlayerByIndex(i));
    Sound("RadioConfirm*.ogg", true, 0, 100, GetPlayerByIndex(i) + 1);
  }
  return _inherited(...);
}
