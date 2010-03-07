/*-- Standartscript --*/

#strict

public func ChooserFinished()
{
  //"Insta Gib"-Regel?
  if(FindObject(IGIB))
  {
    //Keine Waffen und Spawns.
    for(var spwn in FindObjects(Find_ID(SPNP)))
      RemoveObject(spwn);
    for(var clonk in FindObjects(Find_OCF(OCF_CrewMember)))
      while(Contents(0, clonk))
        RemoveObject(Contents(0, clonk));
    //Keine Kisten mit Munition.
    for(var ammobox in FindObjects(Find_ID(AMCT)))
      if(ammobox->GetSpawnID())
        if(ammobox->GetSpawnID()->~IsAmmoPacket())
          RemoveObject(ammobox);
  }
  
  //"Keine Munition"-Regel?
  if(FindObject(NOAM))
  {
    //Keine Munitions-Spawns.
    for(var spwn in FindObjects(Find_ID(SPNP)))
      if(Contents(0, spwn)->~IsAmmoPacket())
        RemoveObject(spwn);

    //Keine Kisten mit Munition.
    for(var ammobox in FindObjects(Find_ID(AMCT)))
      if(ammobox->GetSpawnID())
        if(ammobox->GetSpawnID()->~IsAmmoPacket())
          RemoveObject(ammobox);
  }
  
  //Waffenwahl/Klassenwahl-Regel?
  if(FindObject(WPCH) || FindObject(MCSL))
  {
    for(var spwn in FindObjects(Find_ID(SPNP)))
      if(Contents(0, spwn)->~IsWeapon())
        RemoveObject(spwn);
  }
  
  //Ohne Klassenwahl?
  if(!FindObject(MCSL))
  {
    for(var i = 0; i < GetPlayerCount(); i++)
      for(var j = 0, pCrew ; pCrew = GetCrew(GetPlayerByIndex(i), j) ; j++)
        OnClassSelection(pCrew);
  }
}

protected func InitializePlayer(int iPlr, int iX, int iY, object pBase, int iTeam)
{
  for(var i=0, pCrew ; pCrew = GetCrew(iPlr, i) ; i++)
    RelaunchPlayer(iPlr, pCrew, 0, iTeam, true);
}

public func RelaunchPlayer(int iPlr, object pCrew, object pKiller, int iTeam, bool bFirst)
{
  // Kein ordentlicher Spieler?
  if(GetOwner(pCrew) == NO_OWNER || iPlr == NO_OWNER || !GetPlayerName(iPlr))
    return();
    
  // Kein Team
  if(!iTeam) iTeam = GetPlayerTeam(iPlr);
  
  // Reject?
  if(!bFirst)
    if(RejectRelaunch(iPlr,iTeam))
      return();
  
  // Clonk tot?
  if(!GetAlive(pCrew))
    pCrew = RelaunchClonk(iPlr, pCrew);
    
  if(GetPlayerType(iPlr) == C4PT_Script)
    pCrew->~SetupBot4K();
    
  // Zufallsposition
  var iX, iY;
  RelaunchPosition(iX, iY, iTeam);
  
  //Ausrüsten
  if(!FindObject(IGIB))
    OnClonkEquip(pCrew);
  
  if(Contained(pCrew))
    SetPosition(iX, iY, Contained(pCrew));
  else
    SetPosition(iX, iY, pCrew);

  if(!FindObject(MCSL) && !FindObject(CHOS))
    OnClassSelection(pCrew);
}

public func RelaunchClonk(int iPlr, object pCursor)
{
  var pClonk = CreateObject(PCMK, 10, 10, iPlr);
  if(pCursor)
    GrabObjectInfo(pCursor, pClonk);
  else
    MakeCrewMember(pClonk, iPlr);

  DoEnergy(+150, pClonk);
  SetCursor(iPlr, pClonk);
  SetPlrView(iPlr, pClonk);
  
  // Wegstecken
  var tim = CreateObject(TIM2, 10, 10, -1);
  pClonk->Enter(tim);
  PlayerMessage(iPlr, Format("@%s", GetName(pClonk)), tim);

  return(pClonk);
}


/* Sollte Überladen werden: */
public func RelaunchPosition(& iX, & iY, int iTeam)
{
  var wipf = PlaceAnimal(WIPF); 
  iX = GetX(wipf);
  iY = GetY(wipf);
  RemoveObject(wipf);
}

public func OnClonkEquip(object pClonk)
{
  var wpn = CreateContents(92FS, pClonk);
  wpn->DoAmmo(wpn->GetFMData(FM_AmmoID),wpn->GetFMData(FM_AmmoLoad));
  pClonk->DoAmmo(wpn->GetFMData(FM_AmmoID),wpn->GetFMData(FM_AmmoLoad)*2);
}

public func OnClassSelection(object pClonk)
{
  //Dummy...
}

public func RejectRelaunch(int iPlr, int iTeam)
{
  return(false);
}
