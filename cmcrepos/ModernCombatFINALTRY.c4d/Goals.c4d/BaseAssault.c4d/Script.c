/*-- Base Assault --*/

#strict 2
#include CASS

public func RejectChoosedClassInfo()	{return true;}


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

static const GBAS_Icon = -1;
static const GBAS_Name = 0;
static const GBAS_Status = 1;
static const GBAS_MaxTargetCount = 8;	//8 Ziele darf jedes Team maximal haben

local aScoreboardTeams;

public func UpdateScoreboard()
{
  if(!aScoreboardTeams)
    aScoreboardTeams = [];

  //Titelzeile
  SetScoreboardData(SBRD_Caption, SBRD_Caption, GetName());

  //Teams auflisten
  for(var i, iTeam; i < GetTeamCount(); i++)
  {
    //Team eliminiert
    if(!GetTeamName(iTeam = GetTeamByIndex(i)) || !GetTeamPlayerCount(iTeam))
    { 
      if(aScoreboardTeams[iTeam])
        RemoveScoreboardTeam(iTeam);

      continue;
    }
    RemoveScoreboardTeam(iTeam);
    aScoreboardTeams[iTeam] = true;
    //Team hat noch Ziele
    if(ObjectCount2(Find_InArray(aTargets[iTeam])))
    {
      for(var k = 0, row = 0; k < GetLength(aTargets[iTeam]); k++)
      {
        var target = aTargets[iTeam][k];
        //Ziel noch da?
        if(target)
        {
          var dmg = Interpolate2(100, 0, GetDamage(target), EffectVar(0, target, GetEffect("IntAssaultTarget", target))),
          clr = InterpolateRGBa3(GetTeamColor(iTeam), RGB(255, 255, 255), 100 - dmg, 100);
          SetScoreboardData(row + iTeam * GBAS_MaxTargetCount, GBAS_Icon, Format("{{%i}}", target->~GetImitationID() || GetID(target)));
          SetScoreboardData(row + iTeam * GBAS_MaxTargetCount, GBAS_Name, Format("<c %x>%s</c>", clr, GetName(target)), iTeam);
          SetScoreboardData(row + iTeam * GBAS_MaxTargetCount, GBAS_Status, Format("<c %x>%d%</c>", clr, dmg), dmg);
          ++row;
        }
      }
    }
    //Keine Ziele mehr: Clonks anzeigen
    else
    {
      //Spieler abklappern
      for(var l = 0, iPlr; l < GetTeamPlayerCount(iTeam); l++)
      {
        iPlr = GetTeamPlayerByIndex(l, iTeam);
        var clonk = GetCrew(iPlr);
        //Kein Clonk?
        if(!clonk || !GetPlayerName(iPlr))
          continue;

        //Tot?
        var symbol = GetID(clonk),
        dmg = 1;
        if(GetID(Contained(clonk)) == FKDT || !GetAlive(clonk) || GetID(Contained(clonk)) == TIM1 || GetID(Contained(clonk)) == TIM2)
        {
          symbol = CDBT;
          dmg = 0;
        }
        SetScoreboardData(l + iTeam * GBAS_MaxTargetCount, GBAS_Icon, Format("{{%i}}", symbol));
        SetScoreboardData(l + iTeam * GBAS_MaxTargetCount, GBAS_Name, GetTaggedPlayerName(iPlr, true), iTeam);
        SetScoreboardData(l + iTeam * GBAS_MaxTargetCount, GBAS_Status, "", dmg);
      }
    }
  }
  SortScoreboard(GBAS_Status);
  SortScoreboard(GBAS_Name);
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
  aScoreboardTeams[iTeam] = false;
  for(var i; i < GBAS_MaxTargetCount; i++)
  {
    SetScoreboardData(i + iTeam * GBAS_MaxTargetCount, GBAS_Icon);
    SetScoreboardData(i + iTeam * GBAS_MaxTargetCount, GBAS_Name);
    SetScoreboardData(i + iTeam * GBAS_MaxTargetCount, GBAS_Status);
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
    var dmg = EffectVar(0, obj, GetEffect("IntAssaultTarget", obj));
    var id = GetID(obj);
    if(id == AHBS) id = obj->GetImitationID();
    AddMenuItem(GetName(obj), Format("DoRelaunch(Object(%d), Object(%d), %d)", ObjectNumber(pCrew), ObjectNumber(obj), iClass), id, pCrew, 100*(dmg-GetDamage(obj))/dmg, 0, GetName(obj));
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