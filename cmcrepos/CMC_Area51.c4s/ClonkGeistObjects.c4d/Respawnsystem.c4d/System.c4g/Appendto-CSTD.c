/*-- Neues Script --*/

#strict 2

#appendto CSTD

public func RelaunchPlayer(int iPlr, object pCrew, object pKiller, int iTeam, bool bFirst)
{
  //Kein ordentlicher Spieler?
  if(GetOwner(pCrew) == NO_OWNER || iPlr == NO_OWNER || !GetPlayerName(iPlr))
    return;

  //Kein Team?
  if(!iTeam) iTeam = GetPlayerTeam(iPlr);

  //Falscher Cursor?
  if (!pCrew || !pCrew->~IsClonk())
    pCrew = GetCrew(iPlr);

  //Reject?
  if(!bFirst)
    if(RejectRelaunch(iPlr,iTeam))
      return;

  //Clonk tot?
  if(!GetAlive(pCrew))
    pCrew = RelaunchClonk(iPlr, pCrew);

  //Ausrüsten
  OnClonkEquip(pCrew);

  //Zufallsposition setzen (iX und iY wegen Abwärtskompatibilität.)
  var iX, iY, aSpawnpoints;
  aSpawnpoints = RelaunchPosition(iX, iY, iTeam);
  
  //Szenario nutzt neue Spawnmechanik?
  if(GetType(aSpawnpoints) == C4V_Array)
  	GetBestSpawnpoint(aSpawnpoints, iPlr, iX, iY);

  if(Contained(pCrew))
    SetPosition(iX, iY, Contained(pCrew));
  else
    SetPosition(iX, iY, pCrew);

  if(!FindObject(MCSL) && !FindObject(CHOS))
    GameCallEx("OnClassSelection",pCrew);
}
