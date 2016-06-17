/*-- Standardscript --*/

#strict 2


/* Wahl abgeschlossen */

static g_chooserFinished;

public func ChooserFinished()
{
  if(!(FindObject2(Find_Category(C4D_Goal)) && FindObject2(Find_Category(C4D_Goal))->~CustomSpawnSystem()))
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

  //Alle Spieler relaunchen
  for(var i = 0; i < GetPlayerCount(); i++)
  {
    RelaunchPlayer(GetPlayerByIndex(i),GetCrew(GetPlayerByIndex(i)), 0, GetPlayerTeam(GetPlayerByIndex(i)));
    SetFoW(true, GetPlayerByIndex(i));
  }

  //Ohne Klassenwahl
  if(!FindObject(MCSL))
  {
    for(var i = 0; i < GetPlayerCount(); i++)
      for(var j = 0, pCrew; pCrew = GetCrew(GetPlayerByIndex(i), j) ; j++)
        GameCallEx("OnClassSelection",pCrew);
  }
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
  UpdatePlayerData(iPlr);

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

global func IsDeveloper(int iPlayerID)	{return ((GetType(CSTD_CMCDevelopers) != C4V_Array) || CSTD_CMCDevelopers[iPlayerID]);}

/* Relaunch */

static g_aPlayerRespawnTimer, g_aPlayerRelaunchInfo, g_rejectPlayerRespawn;

public func SetPlayerRelaunchInformation(int iPlr, int iKiller)
{
  if(!g_aPlayerRelaunchInfo)
    g_aPlayerRelaunchInfo = [];

  g_aPlayerRelaunchInfo[GetPlayerID(iPlr)] = [iKiller];
  return true;
}

public func GetPlayerRelaunchInformation(int iPlr)
{
  if(g_aPlayerRelaunchInfo)
    return g_aPlayerRelaunchInfo[GetPlayerID(iPlr)];
  return;
}

public func SetPlayerRespawnTime(int iPlr, int iTime)
{
  if(!g_aPlayerRespawnTimer)
    g_aPlayerRespawnTimer = [];

  g_aPlayerRespawnTimer[GetPlayerID(iPlr)] = Max(iTime);
  return true;
}

public func GetPlayerRespawnTime(int iPlr, bool fRaw)
{
  if(RespawnDelayRejected() && !fRaw)
    return 0;
  if(g_aPlayerRespawnTimer)
    return g_aPlayerRespawnTimer[GetPlayerID(iPlr)];
}

public func RespawnDelayRejected()
{
  if(FindObject2(Find_Category(C4D_Rule|C4D_Goal|C4D_Environment), Find_Func("RejectRespawnTimer")))
    return true;

  if(g_rejectPlayerRespawn)
    return true;

  return false;
}

static g_PlayerViewRange;

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
    if(!(pCrew = RelaunchClonk(iPlr, pCrew)))
      return;

  //Ausrüsten
  OnClonkEquip(pCrew);
	//Standardverhalten falls kein eigenes Spawnsystem in Kraft tritt
  if(!FindObject2(Find_Category(C4D_Goal|C4D_Rule), Find_Func("CustomSpawnSystem"))) {
  	if(!Contained(pCrew) || (g_chooserFinished && FindObject(CHOS))) {
  		if(!FindObject2(Find_Category(C4D_Goal|C4D_Rule), Find_Func("AvoidDefaultSpawnObject"))) {
  			//Falls nicht in einem Spawnpunkt, dann in einen Spawnpunkt verschieben
				var tim = CreateObject(TIM2, GetX(pCrew), GetY(pCrew), -1);
				Enter(tim, pCrew);
			}
			else
				//In der Mitte der Map platzieren
	  		SetPosition(LandscapeWidth()/2, LandscapeHeight()/2, pCrew);
  	}
  	else
  		//In der Mitte der Map platzieren
	  	SetPosition(LandscapeWidth()/2, LandscapeHeight()/2, Contained(pCrew));

		//Vorherige Sicht speichern
		if(!g_PlayerViewRange)
			g_PlayerViewRange = [];
		var index = GetLength(g_PlayerViewRange);
		for(var i = 0; i < index; i++)
			if(!g_PlayerViewRange[i] || !g_PlayerViewRange[i][0]) {
				index = i;
				break;
			}
		g_PlayerViewRange[index] = [pCrew, GetObjPlrViewRange(pCrew)];

		//Sicht sperren
		if(g_chooserFinished) {
			SetPlrViewRange(0, pCrew);
			//Cursor auf pCrew setzen, falls Cursor auf pCrew ist. Ja.
			//(Auf diese Weise ist SetPlrViewMode auf Cursor statt Target. Irgendwie ignoriert letzteres die PlayerViewRange und zeigt die normale Sichtweite an.)
			if(GetCursor(GetOwner(pCrew)) == pCrew)
				SetCursor(GetOwner(pCrew), pCrew, true, true);
		}
  }
  if(!FindObject(MCSL) && !FindObject(CHOS))
    GameCallEx("OnClassSelection", pCrew);
}

public func OnClassSelection(object pCrew) {
	if(FindObject2(Find_Category(C4D_Goal|C4D_Rule), Find_Func("CustomSpawnSystem")))
		return;

	//Zufallsposition setzen (iX und iY für Abwärtskompatibilität)
  var iX, iY, aSpawnpoints, iPlr = GetOwner(pCrew);
  aSpawnpoints = RelaunchPosition(iX, iY, GetPlayerTeam(iPlr));

  //Szenario nutzt neue Spawnmechanik?
  if(GetType(aSpawnpoints) == C4V_Array)
    GetBestSpawnpoint(aSpawnpoints, iPlr, iX, iY);

  if(Contained(pCrew))
    SetPosition(iX, iY, Contained(pCrew));
  else
    SetPosition(iX, iY, pCrew);

	if(!Contained(pCrew))
		ResetSpawnViewRange(pCrew);
}

public func ResetSpawnViewRange(object pCrew) {
	for(var i = 0; i < GetLength(g_PlayerViewRange); i++)
		if(g_PlayerViewRange[i] && g_PlayerViewRange[i][0] == pCrew) {
			SetPlrViewRange(g_PlayerViewRange[i][1], pCrew);
			g_PlayerViewRange[i] = 0;
			break;
		}
	return true;
}

public func RelaunchClonk(int iPlr, object pCursor)
{
  //Clonkerstellung
  var pClonk, killmsg = KILL->GetKillMsgByObject(pCursor);
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

  //Gegebenenfalls Informationen mitliefern
  LocalN("Respawn_KillMsg", tim) = killmsg;
  LocalN("Respawn_Position", tim) = [GetX(pCursor), Min(GetY(pCursor), LandscapeHeight()-10)];

  //Wartezeit von vorherigem Ableben vorhanden?
  if(GetPlayerRespawnTime(iPlr))
  {
    //An den Todesort verschieben (allerdings nicht in Abgründe rein)
    SetPosition(GetX(pCursor), Min(GetY(pCursor), LandscapeHeight()-10), tim);
    AddEffect("WaitingObject", tim, 100, 10, tim, 0, killmsg);

    return;
  }

  //Clonknamen anzeigen (Brauchen wir das? Habs mal deaktiviert, macht sonst nur Probleme)
  //PlayerMessage(iPlr, Format("@%s", GetName(pClonk)), tim);
	if(tim)
  	ScheduleCall(tim, "SpawnOk", 1);

  return pClonk;
}

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  var wipf = PlaceAnimal(WIPF); 
  iX = GetX(wipf);
  iY = GetY(wipf);
  RemoveObject(wipf);
}

/* Intelligente Spawnmechanik */

static const SPAWNSYS_Allies = 50;
static const SPAWNSYS_Enemies = -50;
static const SPAWNSYS_Traps = -30;

global func GetBestSpawnpoint(array aSpawnpoints, int iPlr, int &x, int &y)
{
  var team = GetPlayerTeam(iPlr);
  var spawn_grading = [];
  for(var spawn in aSpawnpoints)
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

  var spawnpoint = chosen_spawns[Random(GetLength(chosen_spawns))];
  x = spawnpoint[0];
  y = spawnpoint[1];

  return spawnpoint;
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
