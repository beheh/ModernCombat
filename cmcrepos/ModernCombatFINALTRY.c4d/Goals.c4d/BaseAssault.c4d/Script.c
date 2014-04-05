/*-- Base Assault --*/

#strict 2
#include CASS

public func RejectChoosedClassInfo()	{return true;}

static const GBAS_BombRespawnDelay = 360;


/* Initialisierung */

public func ChooserFinished()
{
  //Spielzielhinweise erstellen
  for(var i = 0; i < GetPlayerCount(); i++)
  {
    CreateObject(TK04, 0, 0, GetPlayerByIndex(i));
    Sound("Info_Round.ogg", true, 0, 100, GetPlayerByIndex(i) + 1);
  }
  return _inherited(...);
}

/* Bekanntgabe */

public func ReportAssaultTargetDestruction(object pTarget, int iTeam)
{
  if(GetIndexOf(pTarget, aTargets[iTeam]) == -1)
    return;

  //Eventnachricht: Bombe wird gesucht
  EventInfo4K(0, "$BombSpawnDelay$", C4P2, 0, 0, 0, "Info_Objective.ogg");
  ScheduleCall(this, "PlaceBombSpawnpoint", GBAS_BombRespawnDelay);

  _inherited(pTarget, iTeam, ...);

  //Eventnachricht: Zielobjekt zerstört
  EventInfo4K(0, Format("$TargetDestruction$", GetTeamColor(iTeam), GetName(pTarget)), GBAS, 0, 0, 0, "Info_Objective.ogg");
  GameCall("OnAssaultTargetDestruction", pTarget, iTeam, FindInArray4K(aTargets[iTeam], pTarget));
  if(pTarget)
    Explode(50, pTarget);

  if(!ObjectCount2(Find_InArray(aTargets[iTeam])))
    for(var i = 0; i < GetPlayerCount(); i++)
      if(GetPlayerTeam(GetPlayerByIndex(i)) == iTeam)
        //Eventnachricht: Alle Zielobjekte zerstört
        EventInfo4K(GetPlayerByIndex(i)+1, "$NoTargets$", GBAS, 0, 0, 0, "Info_Alarm.ogg");
}

public func GetAssaultTarget(int iIndex, int iTeam)
{
  return aTargets[iTeam][iIndex];
}

/* Timer */

protected func FxIntGoalTimer()
{
  UpdateScoreboard();
  IsFulfilled();
}

/* Scoreboard */

static const GBAS_Icon = 0;
static const GBAS_TargetName = 1;
static const GBAS_TargetState = 2;

public func UpdateScoreboard()
{
  //Wird noch eingestellt
  if(FindObject(CHOS)) return;

  //Titelzeile
  SetScoreboardData(SBRD_Caption, SBRD_Caption, GetName());

  //Informationen zur Bombe
  SetScoreboardData(SBRD_Caption, GBAS_Icon, Format("{{C4P2}}"));
  SetScoreboardData(SBRD_Caption, GBAS_TargetName, Format("$Bomb$"));

  //Zustand der Bombe ermitteln (Icons fehlen)
  var state = "", bomb = GetBomb();
  //Zuerst von Bombe im Freien ausgehen
  if(bomb)
    state = "$BombIdle$";
  else
  {
    var bomb_carrier, e;
    for(var obj in FindObjects(Find_OCF(OCF_CrewMember)))
      if(e = GetEffect("BaseAssaultBomb", obj))
      {
        bomb_carrier = obj;
        break;
      }

    //Anzeigen wer die Bombe trägt
    var status = 0;
    for(var i = 0; i < GetLength(aTargets); i++)
    {
      if(!aTargets[i])
        continue;
      for(var target in aTargets[i])
      {
        var e;
        if((e = GetEffect("IntAssaultTarget", target)) && (status = EffectVar(2, target, e)))
          break;
      }

      if(status)
        break;
    }

    if(!status)
    {
      //Kein Bombenträger verfügbar: Als gesucht melden
      if(!bomb_carrier)
        state = "$SearchingBomb$";
      //Ansonsten Träger anzeigen
      else
        state = Format("$BombInPossession$", GetTaggedPlayerName(GetOwner(bomb_carrier)));
    }
    //Bombe wird platziert
    else if(status == 1)
      state = Format("$BombPlacing$", GetTaggedPlayerName(GetOwner(bomb_carrier)));
    //Bombe ist platziert
    else if(status >= 2)
      state = "$BombPlaced$";
  }
  SetScoreboardData(SBRD_Caption, GBAS_TargetState, state);

  //Leerzeile
  SetScoreboardData(0, 0, " ", 0);

  //Teams auflisten
  for(var i = 0; i < GetTeamCount(); i++)
  {
    var team = GetTeamByIndex(i);
    var name = GetTaggedTeamName(team);
    var clr = GetTeamColor(team);
    var plr;

    //Kein Spieler im Team? Dann nicht anzeigen
    if(GetTeamMemberByIndex(team, 0) == -1)
      continue;

    for(var j = 0; j < GetLength(aTargets[team]); j++)
    {
      var target = aTargets[team][j], e = GetEffect("IntAssaultTarget", target), state = EffectVar(2, target, e);
      if(!target)
      {
        //Eintrag ausleeren
        SetScoreboardData(team*100+j, GBAS_Icon);
        SetScoreboardData(team*100+j, GBAS_TargetName);
        SetScoreboardData(team*100+j, GBAS_TargetState);
        continue;
      }

      SetScoreboardData(team*100+j, GBAS_Icon, Format("{{%i}}", EffectVar(1, target, e)));
      SetScoreboardData(team*100+j, GBAS_TargetName, Format("<c %x>%s</c>", GetTeamColor(team), GetName(target)));

      //Statusicon für Zielobjekt ermitteln
      var icon = SM16;	//Keine Aktivität
      if(state == 1)
        icon = SM17;	//Ladung wird plaziert
      else if(state == 2)
        icon = SM18;	//Ladung plaziert
      else if(state == 3)
        icon = SM16;	//Ladung wird entschärft

      SetScoreboardData(team*100+j, GBAS_TargetState, Format("{{%i}}", icon));
    }
  }
}

private func GetTeamPlayerByIndex(int iPlr, int iTeam)
{
  for(var i; i < GetPlayerCount(); i++)
    if(GetPlayerTeam(GetPlayerByIndex(i)) == iTeam)
    {
      if(!iPlr)
        return GetPlayerByIndex(i);
       --iPlr;
    }
  return -1;
}

public func RemoveScoreboardTeam(int iTeam)
{
  for(var i; i < aTargetCount[iTeam]; i++)
  {
    SetScoreboardData(100*iTeam+i, GBAS_Icon);
    SetScoreboardData(100*iTeam+i, GBAS_TargetName);
    SetScoreboardData(100*iTeam+i, GBAS_TargetState);
  }
}

/* Rundenauswertung */

local fulfilled;

public func IsFulfilled()
{
  if(FindObject(CHOS)) return;
  if(fulfilled) return 1;
  
  //Teams durchgehen
  for(var i; i < GetTeamCount(); i++)
  {
    var team = GetTeamByIndex(i);
    if(!GetTeamPlayerCount(team))
    {
      for(var obj in FindObjects(Find_InArray(aTargets[team])))
        DecoExplode(50, obj);
      EliminateTeam(team);
    }
    else
    {
      //Keine Zielobjekte mehr: Team eliminieren
      for(var target in aTargets[team])
        if(target)
          break;

      if(!target)
        EliminateTeam(team);
    }
  }

  //Gegen Camping während Klassenwahl oder im Menü
  for(var obj in FindObjects(Find_Func("IsClonk")))
    if((GetID(Contained(obj)) == TIM1 || GetID(Contained(obj)) == TIM2) && !ObjectCount2(Find_InArray(aTargets[GetPlayerTeam(GetOwner(obj))])))
      EliminatePlayer(GetOwner(obj));

  //Nur noch ein Team übrig - Sieg!
  if(GetActiveTeamCount() == 1)
  {
    //Spielende planen
    Schedule("GameOver()", 150);

    //Auswertung
    RewardEvaluation();

    //Nachricht über Gewinner
    Message("@$TeamHasWon$", 0, GetTaggedTeamName(team));

    //Sound
    Sound("Cheer.ogg", true);

    RemoveAll(GOAL);
    return fulfilled = true;
  }
}

/* Bombenspawnmechanik */

global func SetupBombSpawnpoint()
{
  if(FindObject(GBAS))
    return FindObject(GBAS)->~SetupBombSpawnpoint(...);

  return false;
}

global func PlaceBombSpawnpoint()
{
  if(FindObject(GBAS))
    return FindObject(GBAS)->~PlaceBombSpawnpoint(...);

  return false;
}

public func GetBomb()	{return FindObject(C4P2);}

local bombSpawns;

public func DelayedBombRespawn(object pBomb, int iX, int iY)
{
  //Eventnachricht: Bombe verloren
  EventInfo4K(0, "$BombLost$", C4P2, 0, 0, 0, "Info_Objective.ogg");

  RemoveObject(pBomb);
  ScheduleCall(this, "PlaceBombSpawnpoint", GBAS_BombRespawnDelay, 0, iX, iY, true);
}

public func SetupBombSpawnpoint(array aSpawnCoordinates)
{
  bombSpawns = aSpawnCoordinates;
  if(!bombSpawns)
    bombSpawns = [[]];

  PlaceBombSpawnpoint();
  return true;
}

public func PlaceBombSpawnpoint(int iX, int iY, bool fNoDelay)
{
  if(!iX && !iY)
  {
    //Eventnachricht: Bombe gesichtet
    EventInfo4K(0, "$BombSpawned$", C4P2, 0, 0, 0, "Info_Objective.ogg");

    var sp = bombSpawns[Random(GetLength(bombSpawns))];
    iX = sp[0]; iY = sp[1];
  }

  var bomb = CreateObject(C4P2, AbsX(iX), AbsY(iY), NO_OWNER);
  if(!SpawningConditions(bomb))
  {
    if(!fNoDelay)
      DelayedBombRespawn(bomb, 0, 0);
    else
    {
      var sp = bombSpawns[Random(GetLength(bombSpawns))];
      iX = sp[0]; iY = sp[1];
      bomb->SetPosition(iX, iY);
    }
  }

  return true;
}

public func SpawningConditions(object pBomb)
{
  var x = GetX(pBomb), y = GetY(pBomb)+2;
  //Befindet sich in Grenzgebieten?
  if(FindObject2(Find_ID(BRDR), Find_Func("IsDangerous", pBomb, x, y)))
    return false;
  //Außerhalb der Map?
  if(!Inside(x, 0, LandscapeWidth()) || !Inside(y, 0, LandscapeHeight()))
    return false;
  //Innerhalb von Lava/Säure?
  var mat = GetMaterial(AbsX(x), AbsY(y));
  if(GetMaterialVal("Corrosive", "Material", mat) || GetMaterialVal("Incindiary", "Material", mat))
    return false;

  return true;
}

public func PlantingCondition(object pTarget, object pAssaultTarget)
{
  if(GetEffect("BaseAssaultBomb", pTarget) || EffectVar(2, pAssaultTarget, GetEffect("IntAssaultTarget", pAssaultTarget)) > 1)
    return true;

  return false;
}

public func OnPlantingComplete(array aAttackers, object pTarget)
{
  for(var clonk in aAttackers)
    if(GetEffect("BaseAssaultBomb", clonk))
      RemoveEffect("BaseAssaultBomb", clonk);

  //Eventnachricht: Ladung plaziert, verteidigen
  for(var i = 0,team; i < GetTeamCount(); i++)
    if((team = GetTeamByIndex(i)) && team != GetTeam(pTarget))
      TeamEventInfo(team, Format("$TargetArmedAttacker$", GetName(pTarget)), SM16, 0, 0, 0, "Info_Event.ogg");
  //Eventnachricht: Ladung plaziert, entschärfen
  TeamEventInfo(GetTeam(pTarget), Format("$TargetArmedDefender$", GetName(pTarget)), SM17, 0, 0, 0, "Info_Event.ogg");

  return true;
}

public func OnDefusingComplete(array aDefenders, object pTarget)
{
  var fBomb = false;
  for(var clonk in aDefenders)
    if(clonk && !(clonk->~IsFakeDeath()))
    {
      fBomb = true;
      C4P2->AddBombObject(clonk);
      break;
    }

  if(!fBomb)
    PlaceBombSpawnpoint();
  /*if(GetLength(aDefenders) > 0)
    C4P2->AddBombObject(aDefenders[0]);*/

  //Eventnachricht: Ladung entschärft, neue setzen
  for(var i = 0,team; i < GetTeamCount(); i++)
    if((team = GetTeamByIndex(i)) && team != GetTeam(pTarget))
      TeamEventInfo(team, Format("$TargetDefusedAttacker$", GetName(pTarget)), SM17, 0, 0, 0, "Info_Event.ogg");
  //Eventnachricht: Ladung entschärft
  TeamEventInfo(GetTeam(pTarget), Format("$TargetDefusedDefender$", GetName(pTarget)), SM16, 0, 0, 0, "Info_Event.ogg");

  return true;
}

/* Relaunch */

public func OnClassSelection(object pCrew, int iClass)
{
  RelaunchPlayer(GetOwner(pCrew), 0, -2, iClass);
}

public func RelaunchPlayer(int iPlr, pClonk, int iKiller, int iClass)
{
  if(iKiller != -2)
  {
    aDeath[iPlr]++;
    if(iKiller != -1 && GetPlayerTeam(iPlr) != GetPlayerTeam(iKiller))
      aKill[iKiller]++;
    Money(iPlr, pClonk, iKiller);
  }

  var pCrew = GetCrew(iPlr);

  if(!pCrew || iPlr == -1) return;

  //Team hat keine Ziele mehr - Spieler eliminieren
  if(GetLength(aTargets))
    if(!ObjectCount2(Find_InArray(aTargets[GetPlayerTeam(iPlr)])))
      return EliminatePlayer(iPlr);

  //Clonk wegstecken
  var tim = CreateObject(TIM2, LandscapeWidth()/2, LandscapeHeight()/2, -1);
  Enter(tim, pCrew);
  ClearScheduleCall(tim, "SpawnOk");
  OpenRelaunchMenu(pCrew, 0, iClass);
}

public func OpenRelaunchMenu(object pCrew, int iSelection, int iClass)
{
  //Sichtweite setzen
  SetPlrViewRange(200, pCrew);
  //Zwischendurch alle Ziele vernichtet? Stirb!
  if(!ObjectCount2(Find_InArray(aTargets[GetPlayerTeam(GetOwner(pCrew))])))
    return EliminatePlayer(GetOwner(pCrew));
  CloseMenu(pCrew);
  CreateMenu(GBAS, pCrew, this, 0, "$ChoosePoint$", 0, 3, 0, GBAS);
  //Alle vorhandenen Ziele ins Menü setzen
  var array = FindObjects(Find_InArray(aTargets[GetPlayerTeam(GetOwner(pCrew))]));
  SortTargets(array);
  for(var obj in array)
  {
    var id = GetID(obj);
    if(id == AHBS) id = obj->GetImitationID();
    AddMenuItem(GetName(obj), Format("DoRelaunch(Object(%d), Object(%d), %d)", ObjectNumber(pCrew), ObjectNumber(obj), iClass), id, pCrew, 0, 0, GetName(obj));
  }
  SelectMenuItem(iSelection, pCrew);
}

public func OnMenuSelection(int iSelection, object pCrew)
{
  var array = FindObjects(Find_InArray(aTargets[GetPlayerTeam(GetOwner(pCrew))]));
  SortTargets(array);
  var obj = array[iSelection];
  //Ziel zerstört? Menü neu öffnen
  if(!obj)
    return OpenRelaunchMenu(pCrew, 0, FindObject(MCSL) && LocalN("lastclass", FindObject(MCSL))[GetOwner(pCrew)]);
  //Guckloch für das Ziel
  SetPosition(GetX(obj), GetY(obj), Contained(pCrew));
}

public func MenuQueryCancel(int iSelection, object pCrew)
{
  return GetMenu(pCrew) == GBAS;
}

public func DoRelaunch(object pCrew, object pTarget, int iClass)
{
  //Ziel weg? Neu öffnen
  if(!pTarget)
    return OpenRelaunchMenu(pCrew, iClass);

  var container = Contained(pCrew);
  var id = GetID(pTarget);

  //Fake?
  if(id == AHBS)
    id = pTarget->GetImitationID();

  //Relaunchposition
  var x, y, iTeam = pTarget->GetTeam(), array = aSpawn[iTeam];
  if(GetType(array) == C4V_Array)
    array = array[GetIndexOf(pTarget, aTargets[iTeam])];

  //Da ist kein Array? Dann am Objekt respawnen lassen
  if(GetType(array) != C4V_Array) 
  {
    x = GetX(pTarget);
    y = GetY(pTarget)+GetDefHeight(id)/2-10;
  }
  else
  {
    GetBestSpawnpoint(array, GetOwner(pCrew), x, y);
    y -= 10;
    /*
    var i = Random(GetLength(array));
    x = array[i][0];
    y = array[i][1]-10;
    */
  }
  
  if(FindObject(MCSL))
    //Eventnachricht: Hinweis auf Spawnort
    FindObject(MCSL)->SpawnEventInfo(Format("$SpawnAt$", GetName(pTarget)), GetOwner(pCrew), iClass, this);

  SetPosition(x, y, Contained(pCrew));
  container->Spawn();

  //Sichtweite setzen
  SetPlrViewRange(500, pCrew);
}

/* Sortierung */

private func SortTargets(array &a)
{
  var result = [], array = a, next, val, dir = GameCall("OccupationDir");
  CleanArray4K(array);
  if(!dir)
    dir = GOCC_Horizontal;

  if(dir == GOCC_Horizontal)
  {
    while(GetLength(array))
  {
      val = next = 0;
    for(var obj in array)
    {
      if(!next)
    {
      val = Abs(GetX(obj) - LandscapeWidth()/2);
      next = obj;
    }
    else
      if(Abs(GetX(obj) - LandscapeWidth()/2) < val)
      {
      val = Abs(GetX(obj) - LandscapeWidth()/2);
      next = obj;
      }
    }
    result[GetLength(result)] = next;
    DelArrayItem4K(array, GetIndexOf(next, array));
  }
  }

  if(dir == GOCC_Vertical)
  {
    while(GetLength(array))
    {
      val = next = 0;
    for(var obj in array)
    {
      if(!next)
      {
        val = Abs(GetY(obj) - LandscapeHeight()/2);
        next = obj;
      }
      else
        if(Abs(GetY(obj) - LandscapeHeight()/2) < val)
        {
          val = Abs(GetY(obj) - LandscapeHeight()/2);
          next = obj;
        }
      }
      result[GetLength(result)] = next;
      DelArrayItem4K(array, GetIndexOf(next, array));
    }
  }

  return a = result;
}