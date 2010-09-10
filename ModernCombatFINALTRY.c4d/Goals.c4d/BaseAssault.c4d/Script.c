/*-- Assault --*/

#strict 2
#include TEAM

local aTargets;		//Die Ziele
local aSpawn;		//Spawnpoints


/* Initialisierung */

protected func Initialize()
{
  aTargets = [];
  aKill = [];
  aDeath = [];
  aSpawn = [];
}

protected func ChooserFinished()
{
  AddEffect("IntGoal", this, 1, 5, this);

  //Keine Klassenwahl? Alle relaunchen
  if (!FindObject(MCSL))
    for (var obj in FindObjects(Find_Func("IsClonk")))
      ScheduleCall(this, "OnClassSelection", 1, 0, obj);
}

/* Globales Zeug */

global func ReportAssaultTargetDestruction()
{
  var goal = FindObject2(Find_ID(GBAS));
  if (goal)
    return goal->ReportAssaultTargetDestruction(...);
}

global func AddAssaultTarget()
{
  var goal = FindObject2(Find_ID(GBAS));
  if (goal)
    return goal->AddAssaultTarget(...);
}

global func IsAssaultTarget()	{return GetEffect("IntAssaultTarget", this);}

global func Find_InArray(array a)
{
  var end = [C4FO_Or];
  for (var i; i < GetLength(a); i++)
    if (GetType(a[i]) == C4V_C4Object)
      end[i+1] = [C4FO_Not, [C4FO_Exclude, a[i]]];
  return end;
}

/* Zeug */

public func AddAssaultTarget(id idTarget, int iX, int iY, int iMaxDamage, int iTeam, string szName, int iIndex, array aSpawns, bool fNoBar) {
  //Grundobjekt erstellen
  var fake = CreateObject(STCR, iX, iY+GetDefCoreVal("Offset", 0, idTarget, 1)+2, -1);
  //Und Original imitieren
  SetShape(GetDefCoreVal("Offset", 0, idTarget), GetDefCoreVal("Offset", 0, idTarget, 1), GetDefCoreVal("Width", 0, idTarget), GetDefCoreVal("Height", 0, idTarget), fake);
  SetGraphics(0, fake, idTarget, 1, 1);
  fake->SetImitationID(idTarget);
  //Team setzen
  fake->SetTeam(iTeam);
  //Name
  if (szName)
    SetName(szName, fake);
  else
    SetName(GetName(0, idTarget), fake);
  //Ins Array packen
  if (!aTargets[iTeam])
    aTargets[iTeam] = [];
  aTargets[iTeam][iIndex] = fake;
  //Relaunchpositionen
  if (!aSpawn[iTeam])
    aSpawn[iTeam] = [];
  aSpawn[iTeam][iIndex] = aSpawns;
  //Assault-Effekt
  AddEffect("IntAssaultTarget", fake, 1, 1, this, 0, iMaxDamage, fNoBar, idTarget);
  //Done.
  return true;
}

public func ReportAssaultTargetDestruction(object pTarget, int iTeam)
{
  if (FindInArray4K(aTargets[iTeam], pTarget) == -1)
    return;

  if (GetID(pTarget) == STCR)
  {
    var aDmg = pTarget->GetDamager();
    var iPlr = aDmg[1];
	aDmg = aDmg[0];
	//Der letzte Killer bekommt 50 Punkte. -50 wenn er im selben Team war.
	if (GetPlayerTeam(iPlr) == iTeam)
    {
	  DoPlayerPoints(-50, RWDS_MinusPoints, iPlr, GetCrew(iPlr), IC03);
	  DoWealth(iPlr, -50);
	}
	else
	{
	  DoPlayerPoints(50, RWDS_TeamPoints, iPlr, GetCrew(iPlr), IC03);
	  DoWealth(iPlr, 50);
	}

	//Array durchgehen. Jeder gegnerische Helfer bekommt 20 Punkte, jeder Teamabschuss -20
	for (var i; i < GetLength(aDmg); i++)
	  if (GetPlayerName(i) && i != iPlr && aDmg[i]) {
        if (GetPlayerTeam(i) == iTeam) {
		  DoPlayerPoints(-20, RWDS_MinusPoints, i, GetCrew(i), IC03);
		  DoWealth(i, -20);
		}
		else
                {
		  DoPlayerPoints(20, RWDS_TeamPoints, i, GetCrew(i), IC03);
		  DoWealth(i, 20);
		}
	  }
  }

  //Und gleich mal bekanntgeben
  EventInfo4K(0, Format("$TargetDestruction$", GetTeamColor(iTeam), GetName(pTarget)), GBAS, 0, 0, 0, "Info4.ogg");
  GameCall("OnAssaultTargetDestruction", pTarget, iTeam, FindInArray4K(aTargets[iTeam], pTarget));
  if (pTarget)
    Explode(50, pTarget);

  //Alle Ziele des Teams wurden zerstört! Warnung ausgeben
  if (!ObjectCount2(Find_InArray(aTargets[iTeam])))
    for (var i = 0; i < GetPlayerCount(); i++)
	  if (GetPlayerTeam(GetPlayerByIndex(i)) == iTeam)
	    EventInfo4K(GetPlayerByIndex(i)+1, "$NoTargets$", GBAS, 0, 0, 0, "Alarm.ogg");
}

public func IsTeamGoal()	{return true;}

/* Assault-Effekt */

//Effekt-Vars
//0: Maximalschaden
//1: Leiste oder nicht
//2: Leiste
//3: Aktuell angezeigter Schaden (für "flüssige" Leiste)
//4: ID. Wichtig für FakeTarget

protected func FxIntAssaultTargetStart(object pTarget, int iEffect, int iTemp, int iDmg, bool fNoBar, id idTarget)
{
  EffectVar(0, pTarget, iEffect) = iDmg;
  EffectVar(1, pTarget, iEffect) = fNoBar;
  EffectVar(4, pTarget, iEffect) = idTarget;
}

protected func FxIntAssaultTargetTimer(object pTarget, int iEffect)
{
  //Ziel will keine Leiste
  if (EffectVar(1, pTarget, iEffect))
    return;

  //Erstmal die Leiste prüfen
  if (!EffectVar(2, pTarget, iEffect))
    EffectVar(2, pTarget, iEffect) = CreateObject(EBAR, GetX(pTarget), GetY(pTarget)+GetDefHeight(EffectVar(4, pTarget, iEffect))/2+10, -1);
  var bar = EffectVar(2, pTarget, iEffect);

  //Schaden
  if (EffectVar(3, pTarget, iEffect) < GetDamage(pTarget))
    EffectVar(3, pTarget, iEffect)++;
  var dmg = EffectVar(3, pTarget, iEffect), maxdmg = EffectVar(0, pTarget, iEffect), team = pTarget->~GetTeam();

  //Bei zu viel Schaden zerstören
  if (GetDamage(pTarget) > maxdmg)
    return ReportAssaultTargetDestruction(pTarget, team);

  //Und die Leiste füllen...
  SetPosition(GetX(pTarget), GetY(pTarget)+GetDefHeight(EffectVar(4, pTarget, iEffect))/2+10, bar);
  SetGraphics("Row", bar, GetID(bar), 1, 1, 0, 4);
  var permill = 1000*(maxdmg-dmg)/maxdmg;
  SetObjDrawTransform(permill, 0, (permill-1000)*GetDefWidth(GetID(bar))/2, 0, 1000, 0, bar, 1);
  SetClrModulation(GetTeamColor(team), bar, 1);
}

protected func FxIntAssaultTargetStop(object pTarget, int iEffect, int iCause, bool fTemp)
{
  if (!fTemp && EffectVar(2, pTarget, iEffect))
    RemoveObject(EffectVar(2, pTarget, iEffect));
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
  }

  var pCrew = GetCrew(iPlr);
  if (!pCrew || iPlr == -1) return;

  //Team hat keine Ziele mehr - Spieler eliminieren
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
    if (id == STCR) id = obj->GetImitationID();
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
  if (id == STCR)
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

/* Ungenutzte Funktionen */

private func InitScoreboard()			{}
private func InitMultiplayerTeam(int iTeam)	{}
private func RemoveMultiplayerTeam(int iTeam)	{}
private func InitSingleplayerTeam(int iPlr)	{}
private func RemoveSingleplayerTeam(int iPlr)	{}
private func InitPlayer(int iPlr)		{}
private func RemoveScoreboardPlayer(int iPlr)	{}
public func WinScoreChange(int iNewScore)	{}
private func SortTeamScoreboard()		{}