/*-- Standartscript --*/

#strict


/* Wahl abgeschlossen */

public func ChooserFinished()
{
  //"Insta Gib"-Regel
  if(FindObject(IGIB))
  {
   //Spawnpoints entfernen
   for(var spwn in FindObjects(Find_ID(SPNP)))
    RemoveObject(spwn);

   //Eventuelle Clonkausrüstung entfernen
   for(var clonk in FindObjects(Find_OCF(OCF_CrewMember)))
    while(Contents(0, clonk))
     RemoveObject(Contents(0, clonk));

   //Munitionskisten entfernen
   for(var ammobox in FindObjects(Find_ID(AMCT)))
    RemoveObject(ammobox);
  }

  //"Keine Munition"-Regel
  if(FindObject(NOAM))
  {
   //Munitionsspawnpoints entfernen
   for(var spwn in FindObjects(Find_ID(SPNP)))
    if(Contents(0, spwn)->~IsAmmoPacket())
     RemoveObject(spwn);

   //Munitionskisten mit Munition entfernen
   for(var ammobox in FindObjects(Find_ID(AMCT)))
    if(ammobox->GetSpawnID())
     if(ammobox->GetSpawnID()->~IsAmmoPacket())
      RemoveObject(ammobox);
  }

  //Waffenwahl/Klassenwahl-Regel
  if(FindObject(WPCH) || FindObject(MCSL))
  {
   //Waffenspawnpoints entfernen
   for(var spwn in FindObjects(Find_ID(SPNP)))
    if(Contents(0, spwn)->~IsWeapon())
     RemoveObject(spwn);
  }

  //Ohne Klassenwahl
  if(!FindObject(MCSL))
  {
   for(var i = 0; i < GetPlayerCount(); i++)
    for(var j = 0, pCrew ; pCrew = GetCrew(GetPlayerByIndex(i), j) ; j++)
     OnClassSelection(pCrew);
  }
}

/* Spielerinitalisierung */

protected func InitializePlayer(int iPlr, int iX, int iY, object pBase, int iTeam)
{
  for(var i=0, pCrew ; pCrew = GetCrew(iPlr, i) ; i++)
   RelaunchPlayer(iPlr, pCrew, 0, iTeam, true);
}

/* Relaunch */

public func RelaunchPlayer(int iPlr, object pCrew, object pKiller, int iTeam, bool bFirst)
{
  //Kein ordentlicher Spieler?
  if(GetOwner(pCrew) == NO_OWNER || iPlr == NO_OWNER || !GetPlayerName(iPlr))
   return();
    
  //Kein Team?
  if(!iTeam) iTeam = GetPlayerTeam(iPlr);
  
  //Reject?
  if(!bFirst)
   if(RejectRelaunch(iPlr,iTeam))
    return();
  
  //Clonk tot?
  if(!GetAlive(pCrew))
   pCrew = RelaunchClonk(iPlr, pCrew);

  //KI-Spieler?
  if(GetPlayerType(iPlr) == C4PT_Script)
   pCrew->~SetupBot4K();
    
  //Zufallsposition setzen
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
  //Clonkerstellung
  var pClonk = CreateObject(PCMK, 10, 10, iPlr);
  if(pCursor)
   GrabObjectInfo(pCursor, pClonk);
  else
   MakeCrewMember(pClonk, iPlr);

  DoEnergy(+150, pClonk);
  SetCursor(iPlr, pClonk);
  SetPlrView(iPlr, pClonk);
  
  //In Spawnpoint verschieben
  var tim = CreateObject(TIM2, 10, 10, -1);
  pClonk->Enter(tim);

  //Clonknamen anzeigen
  PlayerMessage(iPlr, Format("@%s", GetName(pClonk)), tim);

  return(pClonk);
}

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  var wipf = PlaceAnimal(WIPF); 
  iX = GetX(wipf);
  iY = GetY(wipf);
  RemoveObject(wipf);
}

/* Bei Clonkausrüstung */

public func OnClonkEquip(object pClonk)
{
  //Standardausrüstung geben: Pistole und Handgranate
  var wpn = CreateContents(PSTL, pClonk);
  wpn->DoAmmo(wpn->GetFMData(FM_AmmoID),wpn->GetFMData(FM_AmmoLoad));
  CreateContents(FGRN, pClonk);

  //Etwas Zusatzmunition für die Pistole sofern welche benötigt wird
  if(!FindObject(NOAM))
  pClonk->DoAmmo(wpn->GetFMData(FM_AmmoID),wpn->GetFMData(FM_AmmoLoad)*2);
}

/* Sobald eine Klasse gewählt wurde */

public func OnClassSelection(object pClonk)
{
}

/* Relaunch ablehnen */

public func RejectRelaunch(int iPlr, int iTeam)
{
  return(false);
}