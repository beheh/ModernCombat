/*-- Standardscript --*/

#strict 2

public func Initialize()
{
  //falls mal kein Chooser verwendet wird
  if(!FindObject(CHOS))
	GameCall("PlaceSpawnplaces");

  return _inherited(...);
}

/* Wahl abgeschlossen */

static g_chooserFinished;

public func ChooserFinished()
{
  if(!(FindObject2(Find_Category(C4D_Goal)) && FindObject2(Find_Category(C4D_Goal))->~CustomSpawnSystem()))
    g_chooserFinished = true;

  //"Keine Munition"-Regel
  if(FindObject(NOAM))
  {
    //Munitionsspawnplaces entfernen
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

  GameCall("PlaceSpawnplaces");
}

/* Regelvoreinstellung */

func ChooserRuleConfig()
{
  //Standardregelsatz: Belohnungssystem, Kein FriendlyFire, Fallschaden, Waffen bleiben, Arena, Limitierte Ausrüstung
  var array = [RWDS, NOFF, FDMG, WPST, NODR, LIMT];
  return array;
}

/* Spielerinitalisierung */

static CSTD_CMCDevelopers;

protected func InitializePlayer(int iPlr, int iX, int iY, object pBase, int iTeam)
{
  AddMsgBoardCmd("votekick", "Votekick(%player%, \"%s\")");

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

  //Ausrüsten
  OnClonkEquip(pCrew);

  //Zufallsposition setzen (iX und iY für Abwärtskompatibilität)
  var iX, iY, aSpawnplaces;
  aSpawnplaces = RelaunchPosition(iX, iY, iTeam);
  
  //Szenario nutzt neue Spawnmechanik?
  if(GetType(aSpawnplaces) == C4V_Array)
    GetBestSpawnplace(aSpawnplaces, iPlr, iX, iY);

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
  if(pCursor && pCursor->~GetRealCursor()) 
	pCursor = pCursor->~GetRealCursor();

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

  //In Spawnplace verschieben
  var tim = CreateObject(TIM2, LandscapeWidth()/2, LandscapeHeight()/2, -1);
  Enter(tim, pClonk);

  //Clonknamen anzeigen
  PlayerMessage(iPlr, Format("@%s", GetName(pClonk)), tim);

  return pClonk;
}

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  if(!g_chooserFinished)
	return ChooserMenuPosition();
  else
  {
	var rsp = FindObject2(Find_Func("IsTeamRespawnplace", iTeam));
	if(rsp)
	  return [[GetX(rsp), GetY(rsp)]];
  }
}

public func OnClassSelection(object pCrew, int iClass)
{
  CreateRelaunchMenu(pCrew, iClass);
}

global func ChooserMenuPosition() { return [LandscapeWidth()/2, LandscapeHeight()/2];}

/* Intelligente Spawnmechanik */

static const SPAWNSYS_Allies = 50;
static const SPAWNSYS_Enemies = -50;
static const SPAWNSYS_Traps = -30;

global func GetBestSpawnplace(array aSpawnplaces, int iPlr, int &x, int &y)
{
  var team = GetPlayerTeam(iPlr);
  var spawn_grading = [];
  for(var spawn in aSpawnplaces)
  {
    var i = GetLength(spawn_grading);
    spawn_grading[i] = [spawn, 0];
    for(var obj in FindObjects(Find_Distance(100, spawn[0], spawn[1]), Find_Or(Find_OCF(OCF_CrewMember), Find_And(Find_Hostile(iPlr), Find_Func("IsSpawnTrap")))))
    {
      if(GetOCF(obj) & OCF_CrewMember)
      {
        if(Hostile(iPlr, GetOwner(obj)))
          spawn_grading[i][1] += SPAWNSYS_Enemies;
        else
          spawn_grading[i][1] += SPAWNSYS_Allies;
      }
      else
        spawn_grading[i][1] += SPAWNSYS_Traps;
    }
  }

  var highest = 0x80000000;
  var chosen_spawns = [];
  for(var spawn_data in spawn_grading)
  {
    if(spawn_data[1] > highest)
    {
      chosen_spawns = [spawn_data[0]];
      highest = spawn_data[1];
    }
    else if(spawn_data[1] == highest)
      chosen_spawns[GetLength(chosen_spawns)] = spawn_data[0];
  }

  var spawnplace = chosen_spawns[Random(GetLength(chosen_spawns))];
  x = spawnplace[0];
  y = spawnplace[1];

  return spawnplace;
}

/* Bei Clonkausrüstung */

public func OnClonkEquip(object pClonk)
{
  //Falls kein Clonk, nicht ausführen
  if(!pClonk)
    return;

  //Falls Clonk bereits ausgerüstet, nicht nochmal ausrüsten
  if(Contents(0,pClonk))
    return;

  //Keine Klassenwahl: Standardausrüstung vergeben: Pistole und Granaten
  if(!FindObject(MCSL))
  {
    var wpn = CreateContents(PSTL, pClonk);
    wpn->DoAmmo(wpn->GetFMData(FM_AmmoID),wpn->GetFMData(FM_AmmoLoad));
    CreateContents(FGRN, pClonk);
    CreateContents(FRAG, pClonk);
    CreateContents(STUN, pClonk);

    //Zusatzmunition für die Pistole sofern benötigt
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
    Sound("Info_Round.ogg");

    //Endmusik einstellen
    SetPlayList("CMC_End of War.ogg", true);
    Music("CMC_End of War.ogg");

    Schedule("ResetPlayList(true)", 10);
  }
}

/* Spieler verlässt die Runde */

protected func RemovePlayer(iPlr)
{
  var pCursor = GetCursor(iPlr);
  if(!pCursor) return;
  if(pCursor->~GetRealCursor()) pCursor = pCursor->~GetRealCursor();
  if(!pCursor->~IsFakeDeath()) return;
  pCursor->Kill();
}

/* Wartemusik */

global func SongCount()	{return 2;}	//Anzahl Lieder

global func SetWaitingMusic()
{
  var song = Random(SongCount()) + 1;

  //Musikliste setzen
  SetPlayList(Format("CMC_WaitingForSpawn%d.ogg", song));

  //Titel abspielen
  Music("", 1);
}

/* Zusatzfunktionen */

public func OnWeaponChoice()	{}
