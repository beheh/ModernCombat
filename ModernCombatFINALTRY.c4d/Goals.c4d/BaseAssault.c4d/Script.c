/*-- Base Assault --*/

#strict 2
#include CASS


/* Bekanntgabe */

public func ReportAssaultTargetDestruction(object pTarget, int iTeam)
{
  if (GetIndexOf(pTarget, aTargets[iTeam]) == -1)
    return;

  _inherited(pTarget, iTeam, ...);
  
  //Und gleich mal bekanntgeben
  EventInfo4K(0, Format("$TargetDestruction$", GetTeamColor(iTeam), GetName(pTarget)), GBAS, 0, 0, 0, "Info.ogg");
  GameCall("OnAssaultTargetDestruction", pTarget, iTeam, FindInArray4K(aTargets[iTeam], pTarget));
  if (pTarget)
    Explode(50, pTarget);

  //Alle Ziele des Teams wurden zerstört! Warnung ausgeben
  if (!ObjectCount2(Find_InArray(aTargets[iTeam])))
    for (var i = 0; i < GetPlayerCount(); i++)
	  if (GetPlayerTeam(GetPlayerByIndex(i)) == iTeam)
	    EventInfo4K(GetPlayerByIndex(i)+1, "$NoTargets$", GBAS, 0, 0, 0, "Alarm.ogg");
}

public func GetAssaultTarget(int iIndex, int iTeam) {
  return aTargets[iTeam][iIndex];
}

/* Timer */

protected func FxIntGoalTimer()
{
  UpdateScoreboard();
  IsFulfilled();
}

/* Scoreboard */

static const GBAS_Name = -1;
static const GBAS_Count = 0;
static const GBAS_Clonks = 1;

public func UpdateScoreboard()
{
  //Titelleiste
  SetScoreboardData(SBRD_Caption, GBAS_Name, GetName());
  SetScoreboardData(SBRD_Caption, GBAS_Count, "{{GBAS}}");
  SetScoreboardData(SBRD_Caption, GBAS_Clonks, "{{PCMK}}");

  //Alle Teams durchgehen...
  for (var i, team; i < GetTeamCount(); team++)
  {
    if (GetTeamName(team))
	  i++;
	//Team ist raus
	if (!GetTeamName(team) || !GetTeamPlayerCount(team))
        {
	  SetScoreboardData(team, GBAS_Name);
	  SetScoreboardData(team, GBAS_Count);
	  SetScoreboardData(team, GBAS_Clonks);
	  continue;
	}
	var count = ObjectCount2(Find_InArray(aTargets[team]));
	SetScoreboardData(team, GBAS_Name, GetTaggedTeamName(team));
	SetScoreboardData(team, GBAS_Count, Format("<c %x>%d</c>", GetTeamColor(team), count), count);
	SetScoreboardData(team, GBAS_Clonks, Format("<c %x>%d</c>", GetTeamColor(team), GetTeamPlayerCount(team)), GetTeamPlayerCount(team));
  }
  SortScoreboard(GBAS_Clonks, true);
  SortScoreboard(GBAS_Count, true);
}

/* Sieg */

local fulfilled;

public func IsFulfilled()
{
  if (FindObject(CHOS)) return;
  if (fulfilled) return 1;
  
  //Teams durchgehen.
  for (var i; i < GetTeamCount(); i++)
  {
    var team = GetTeamByIndex(i);
	if (!GetTeamPlayerCount(team))
	  EliminateTeam(team);
  }
  
  //Gegen Camping während Klassenwahl oder im Menü
  for (var obj in FindObjects(Find_Func("IsClonk")))
    if ((GetID(Contained(obj)) == TIM1 || GetID(Contained(obj)) == TIM2) && !ObjectCount2(Find_InArray(aTargets[GetPlayerTeam(GetOwner(obj))])))
      EliminatePlayer(GetOwner(obj));

  //Nur noch ein Team übrig - Sieg!
  if (GetActiveTeamCount() == 1)
  {
    //Spielende planen
    Schedule("GameOver()", 150);

    //Auswertung
    RewardEvaluation();

    RemoveAll(GOAL);
    Sound("Cheer.ogg", true);
    Message("@$TeamHasWon$", 0, GetTaggedTeamName(team));
    return fulfilled = true;
  }
}

/* Relaunch */

public func OnClassSelection(object pCrew)
{
  RelaunchPlayer(GetOwner(pCrew), 0, -2);
}

public func RelaunchPlayer(int iPlr, pClonk, int iKiller)
{
  if (iKiller != -2)
  {
    aDeath[iPlr]++;
    if (iKiller != -1 && GetPlayerTeam(iPlr) != GetPlayerTeam(iKiller))
      aKill[iKiller]++;
	Money(iPlr, pClonk, iKiller);
  }

  var pCrew = GetCrew(iPlr);

  if (!pCrew || iPlr == -1) return;

  //Team hat keine Ziele mehr - Spieler eliminieren
  if (GetLength(aTargets))
    if (!ObjectCount2(Find_InArray(aTargets[GetPlayerTeam(iPlr)])))
      return EliminatePlayer(iPlr);

  //Clonk wegstecken
  var tim = CreateObject(TIM2, LandscapeWidth()/2, LandscapeHeight()/2, -1);
  Enter(tim, pCrew);
  ClearScheduleCall(tim, "SpawnOk");
  OpenRelaunchMenu(pCrew);
}

public func OpenRelaunchMenu(object pCrew, int iSelection)
{
  SetPlrViewRange(200, pCrew);
  //Zwischendurch alle Ziele vernichtet? Stirb!
  if (!ObjectCount2(Find_InArray(aTargets[GetPlayerTeam(GetOwner(pCrew))])))
    return EliminatePlayer(GetOwner(pCrew));
  CreateMenu(GBAS, pCrew, this, 0, "$ChoosePoint$", 0, 3, 0, GBAS);
  //Alle vorhandenen Ziele ins Menü setzen
  for (var obj in FindObjects(Find_InArray(aTargets[GetPlayerTeam(GetOwner(pCrew))])))
  {
    var dmg = EffectVar(0, obj, GetEffect("IntAssaultTarget", obj));
    var id = GetID(obj);
    if (id == AHBS) id = obj->GetImitationID();
    AddMenuItem(GetName(obj), Format("DoRelaunch(Object(%d), Object(%d))", ObjectNumber(pCrew), ObjectNumber(obj)), id, pCrew, 100*(dmg-GetDamage(obj))/dmg, 0, GetName(obj));
  }
  SelectMenuItem(iSelection, pCrew);
}

public func OnMenuSelection(int iSelection, object pCrew)
{
  var array = FindObjects(Find_InArray(aTargets[GetPlayerTeam(GetOwner(pCrew))]));
  var obj = array[iSelection];
  //Ziel zerstört? Menü neu öffnen
  if (!obj)
    return OpenRelaunchMenu(pCrew);
  //Guckloch für das Ziel
  SetPosition(GetX(obj), GetY(obj), Contained(pCrew));
}

public func MenuQueryCancel(int iSelection, object pCrew)
{
  return GetMenu(pCrew) == GBAS;
}

public func DoRelaunch(object pCrew, object pTarget)
{
  //Ziel weg? Neu öffnen
  if (!pTarget)
    return OpenRelaunchMenu(pCrew);
  var container = Contained(pCrew);
  var id = GetID(pTarget);
  //Fake?
  if (id == AHBS)
    id = pTarget->GetImitationID();
  //Relaunchposition
  var x, y, iTeam = pTarget->GetTeam(), array = aSpawn[iTeam];
  if (GetType(array) == C4V_Array)
    array = array[GetIndexOf(pTarget, aTargets[iTeam])];
  //Da ist kein Array? Dann am Objekt respawnen lassen
  if (GetType(array) != C4V_Array) {
    x = GetX(pTarget);
    y = GetY(pTarget)+GetDefHeight(id)/2-10;
  }
  else {
    var i = Random(GetLength(array));
    x = array[i][0];
    y = array[i][1]-10;
  }
  SetPosition(x, y, Contained(pCrew));
  container->Spawn();
  SetPlrViewRange(500, pCrew);
}