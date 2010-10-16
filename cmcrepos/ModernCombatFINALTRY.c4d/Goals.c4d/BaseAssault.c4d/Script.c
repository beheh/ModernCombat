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

static const GBAS_Name = 0;
static const GBAS_Status = 1;
static const GBAS_MaxTargetCount = 8;	//8 Ziele darf jedes Team maximal haben

local aScoreboardTeams;

public func UpdateScoreboard()
{
  if (!aScoreboardTeams)
    aScoreboardTeams = [];

  //Titelleiste
  SetScoreboardData(SBRD_Caption, SBRD_Caption, GetName());

  //Alle Teams durchgehen
  for(var i, iTeam; i < GetTeamCount(); i++)
  {
    //Team eliminiert
	if (!GetTeamName(iTeam = GetTeamByIndex(i)) || !GetTeamPlayerCount(iTeam))
	{
	  if (aScoreboardTeams[iTeam])
	    RemoveScoreboardTeam(iTeam);
	  continue;
	}
	RemoveScoreboardTeam(iTeam);
	aScoreboardTeams[iTeam] = true;
	//Team hat noch Ziele
	if (ObjectCount2(Find_InArray(aTargets[iTeam])))
	{
	  for (var k = 0, row = 0; k < GetLength(aTargets[iTeam]); k++)
	  {
	    var target = aTargets[iTeam][k];
	    //Ziel noch da?
		if (target)
		{
		  var dmg = Interpolate2(100, 0, GetDamage(target), EffectVar(0, target, GetEffect("IntAssaultTarget", target))),
		  clr = InterpolateRGBa3(GetTeamColor(iTeam), RGB(255, 255, 255), 100 - dmg, 100);
		  SetScoreboardData(row + iTeam * GBAS_MaxTargetCount, GBAS_Name, Format("<c %x>%s</c>", clr, GetName(target)), iTeam);
		  SetScoreboardData(row + iTeam * GBAS_MaxTargetCount, GBAS_Status, Format("<c %x>%d%</c>", clr, dmg), dmg);
		  ++row;
		}
	  }
	}
	//Keine Ziele mehr. Clonks anzeigen
	else
	{
	  //Spieler abklappern
	  for (var l = 0, iPlr; l < GetTeamPlayerCount(iTeam); l++)
	  {
		iPlr = GetTeamPlayerByIndex(l, iTeam);
		var clonk = GetCrew(iPlr);
		//Kein Clonk?
		if (!clonk || !GetPlayerName(iPlr))
		  continue;
		//Tot?
		var symbol = GetID(clonk),
		dmg = 1;
		if (GetID(Contained(clonk)) == FKDT || !GetAlive(clonk) || GetID(Contained(clonk)) == TIM1 || GetID(Contained(clonk)) == TIM2)
		{
		  symbol = CDBT;
		  dmg = 0;
		}
		SetScoreboardData(l + iTeam * GBAS_MaxTargetCount, GBAS_Name, GetTaggedPlayerName(iPlr), iTeam);
		SetScoreboardData(l + iTeam * GBAS_MaxTargetCount, GBAS_Status, Format("{{%i}}", symbol), dmg);
	  }
	}
  }
  SortScoreboard(GBAS_Status);
  SortScoreboard(GBAS_Name);
}

private func GetTeamPlayerByIndex(int iPlr, int iTeam)
{
  for (var i; i < GetPlayerCount(); i++)
    if (GetPlayerTeam(GetPlayerByIndex(i)) == iTeam)
	{
	  if (!iPlr)
	    return GetPlayerByIndex(i);
	  --iPlr;
	}
  return -1;
}

public func RemoveScoreboardTeam(int iTeam)
{
  aScoreboardTeams[iTeam] = false;
  //Uff.
  for (var i; i < GBAS_MaxTargetCount; i++)
  {
    SetScoreboardData(i + iTeam * GBAS_MaxTargetCount, GBAS_Name);
	SetScoreboardData(i + iTeam * GBAS_MaxTargetCount, GBAS_Status);
  }
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
	{
	  for (var obj in FindObjects(Find_InArray(aTargets[team])))
	    Explode(50, obj);
	  EliminateTeam(team);
	}
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
  CloseMenu(pCrew);
  CreateMenu(GBAS, pCrew, this, 0, "$ChoosePoint$", 0, 3, 0, GBAS);
  //Alle vorhandenen Ziele ins Menü setzen
  var array = FindObjects(Find_InArray(aTargets[GetPlayerTeam(GetOwner(pCrew))]));
  SortTargets(array);
  for (var obj in array)
  {
    var dmg = EffectVar(0, obj, GetEffect("IntAssaultTarget", obj));
    var id = GetID(obj);
    if (id == AHBS) id = obj->GetImitationID();
    AddMenuItem(GetName(obj), Format("DoRelaunch(Object(%d), Object(%d))", ObjectNumber(pCrew), ObjectNumber(obj)), id, pCrew, 100*(dmg-GetDamage(obj))/dmg, 0, GetName(obj));
  }
  SelectMenuItem(iSelection, pCrew);
}

//Sortieren ist immer so weird...
private func SortTargets(array &a)
{
  var result = [], array = a, next, val, dir = GameCall("OccupationDir");
  CleanArray4K(array);
  if (!dir)
    dir = GOCC_Horizontal;

  if (dir == GOCC_Horizontal)
  {
    while (GetLength(array))
	{
      val = next = 0;
	  for (var obj in array)
	  {
	    if (!next)
		{
		  val = Abs(GetX(obj) - LandscapeWidth()/2);
		  next = obj;
		}
		else
		  if (Abs(GetX(obj) - LandscapeWidth()/2) < val)
		  {
			val = Abs(GetX(obj) - LandscapeWidth()/2);
			next = obj;
		  }
	  }
	  result[GetLength(result)] = next;
	  DelArrayItem4K(array, GetIndexOf(next, array));
	}
  }

  if (dir == GOCC_Vertical)
  {
    while (GetLength(array))
	{
      val = next = 0;
	  for (var obj in array)
	  {
	    if (!next)
		{
		  val = Abs(GetY(obj) - LandscapeHeight()/2);
		  next = obj;
		}
		else
		  if (Abs(GetY(obj) - LandscapeHeight()/2) < val)
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