/*-- Standardscript --*/

#strict 2


/* Wahl abgeschlossen */

static g_chooserFinished;

public func ChooserFinished()
{
	if(!(FindObject2(Find_Category(C4D_Goal))->~CustomSpawnSystem()))
  	g_chooserFinished = true;

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
  //Effektmanager erstellen
  CreateObject(EFMN);

  //Ohne Klassenwahl
  if(!FindObject(MCSL))
  {
    for(var i = 0; i < GetPlayerCount(); i++)
      for(var j = 0, pCrew; pCrew = GetCrew(GetPlayerByIndex(i), j) ; j++)
        GameCallEx("OnClassSelection",pCrew);
  }

  //Alle Spieler relaunchen
  for(var i = 0; i < GetPlayerCount(); i++)
  {
    RelaunchPlayer(GetPlayerByIndex(i),GetCrew(GetPlayerByIndex(i)), 0, GetPlayerTeam(GetPlayerByIndex(i)));
    SetFoW(true, GetPlayerByIndex(i));
  }
  
  g_chooserFinished = true;
}

/* Regelvoreinstellung */

func ChooserRuleConfig()
{
  //Standardregelsatz: Belohnungssystem, Kein FriendlyFire, Waffen bleiben, Arena, Limitierte Ausr�stung
  var array = [RWDS, NOFF, WPST, NODR, LIMT];
  return array;
}

/* Spielerinitalisierung */

static CSTD_CMCDevelopers;

protected func InitializePlayer(int iPlr, int iX, int iY, object pBase, int iTeam)
{
  AddMsgBoardCmd("Votekick", "Votekick(%player%, \"%s\")");

  if(GetType(CSTD_CMCDevelopers) != C4V_Array)
    CSTD_CMCDevelopers = [];

  CSTD_CMCDevelopers[GetPlayerID(iPlr)] = IsCMCTeamMember(iPlr);

  LoadRanks2Cache();
  var db;
  if(db = FindObject(RWDS))
    db->SetPlayerData(db->GetFullPlayerData(iPlr, RWDS_BattlePoints) + db->GetFullPlayerData(iPlr, RWDS_TeamPoints), RWDS_StartTotalPoints, iPlr);

  for(var i=0, pCrew ; pCrew = GetCrew(iPlr, i) ; i++)
    RelaunchPlayer(iPlr, pCrew, 0, iTeam, true);
}

global func IsDeveloper(int iPlayerID) { return ((GetType(CSTD_CMCDevelopers) != C4V_Array) || CSTD_CMCDevelopers[iPlayerID]); }

/* Relaunch */

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

  //Ausr�sten
  OnClonkEquip(pCrew);

  //Zufallsposition setzen
  var iX, iY;
  RelaunchPosition(iX, iY, iTeam);

  if(Contained(pCrew))
    SetPosition(iX, iY, Contained(pCrew));
  else
    SetPosition(iX, iY, pCrew);

  if(!FindObject(MCSL) && !FindObject(CHOS))
    GameCallEx("OnClassSelection",pCrew);
}

public func RelaunchClonk(int iPlr, object pCursor)
{
  //Clonkerstellung
  var pClonk;
  if(pCursor && pCursor->~GetRealCursor()) pCursor = pCursor->~GetRealCursor();
  if(pCursor)
  {
    pClonk = CreateObject(GetID(pCursor), 10, 10, iPlr);
      GrabObjectInfo(pCursor, pClonk);
  }
  else
  {
    pClonk = CreateObject(PCMK, 10, 10, iPlr);
      MakeCrewMember(pClonk, iPlr);
  }
  DoEnergy(+150, pClonk);
  SetCursor(iPlr, pClonk);
  SetPlrView(iPlr, pClonk);

  //In Spawnpoint verschieben
  var tim = CreateObject(TIM2, LandscapeWidth()/2, LandscapeHeight()/2, -1);
  Enter(tim, pClonk);

  //Clonknamen anzeigen
  PlayerMessage(iPlr, Format("@%s", GetName(pClonk)), tim);

  return pClonk;
}

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  var wipf = PlaceAnimal(WIPF); 
  iX = GetX(wipf);
  iY = GetY(wipf);
  RemoveObject(wipf);
}

/* Bei Clonkausr�stung */

public func OnClonkEquip(object pClonk)
{
  //Falls kein Clonk, nicht ausf�hren
  if(!pClonk)
    return;

  //Falls Clonk bereits ausger�stet, nicht nochmal ausr�sten
  if(Contents(0,pClonk))
    return;

  //Keine Klassenwahl: Standardausr�stung vergeben: Pistole und Granaten
  if(!FindObject(MCSL))
  {
    var wpn = CreateContents(PSTL, pClonk);
    wpn->DoAmmo(wpn->GetFMData(FM_AmmoID),wpn->GetFMData(FM_AmmoLoad));
    CreateContents(FGRN, pClonk);
    CreateContents(FRAG, pClonk);
    CreateContents(STUN, pClonk);

    //Zusatzmunition f�r die Pistole sofern ben�tigt
    if(!FindObject(NOAM))
      pClonk->DoAmmo(wpn->GetFMData(FM_AmmoID),wpn->GetFMData(FM_AmmoLoad)*2);
  }
}

/* Relaunch ablehnen */

public func RejectRelaunch(int iPlr, int iTeam)
{
  return false;
}

/* Spieler zuschauen lassen */

public func ForceObservation(int iPlr)
{
  if(GetCursor(iPlr)) SetPlrViewRange(0, GetCursor(iPlr));
  EliminatePlayer(iPlr);
}

/* Runde auswerten */

public func OnGameOver()
{
  //RewardEvaluation();

  //Belohnungen-Regel
  if(FindObject(RWDS))
  {
    Sound("ObjectiveReceipt.ogg");

    //Endmusik einstellen
    SetPlayList("CMC_End of War.ogg", true);
    Music("CMC_End of War.ogg");

    Schedule("ResetPlayList(true)", 10);
  }
}

/* Spieler verl�sst die Runde */

protected func RemovePlayer(iPlr)
{
  var pCursor = GetCursor(iPlr);
  if(!pCursor) return;
  if(pCursor->~GetRealCursor()) pCursor = pCursor->~GetRealCursor();
  if(!pCursor->~IsFakeDeath()) return;
  pCursor->Kill();
}

/* Zusatzfunktionen */

public func OnClassSelection()	{}
public func OnWeaponChoice()	{}
