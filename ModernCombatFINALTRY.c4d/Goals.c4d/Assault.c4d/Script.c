/*-- Assault --*/

#strict 2
#include CASS

local iDefender;		//Verteidiger-Team
local iTickets;			//Tickets für die Angreifer
local iStartTickets;
local aSpawns;			//Spawnpunkte
local Connected;		//Verbundene Ziele

/* Initialisierung */

protected func Initialize()
{
  iDefender = -1;
  iStartTickets = 10;
  aSpawns = [[],[]];
  Connected = [];
  return _inherited(...);
}

public func IsConfigurable()		{return true;}

public func ConfigMenu(object pCaller)
{
  OpenGoalMenu();
  return 1;
}

private func ConfigFinished()
{
  var chos = FindObject(CHOS);
  if(chos)
    chos->OpenMenu();
}

private func OpenGoalMenu(id dummy, int iSelection)
{
  var pClonk = GetCursor();
  CreateMenu(GetID(),pClonk,0,0,0,0,1);

  AddMenuItem(" ", "OpenGoalMenu", GetID(), pClonk, iStartTickets, 0, " ");
  AddMenuItem("$MoreTickets$", "ChangeTickets", CHOS, pClonk, 0, +1, "$MoreTickets$",2,1);
  AddMenuItem("$LessTickets$", "ChangeTickets", CHOS, pClonk, 0, -1, "$LessTickets$",2,2);
  AddMenuItem("$Finished$", "ConfigFinished", CHOS, pClonk,0,0,"$Finished$",2,3);

  SelectMenuItem(iSelection, pClonk);
}

protected func ChangeTickets(id dummy, int iChange)
{
  //Stand verändern
  iStartTickets = BoundBy(iStartTickets+iChange,3,30);
  //Sound
  Sound("Grab", 1,0,0,1);
  //Menü wieder öffnen
  var iSel = 1;
  if(iChange == -1) iSel = 2;
  OpenGoalMenu(0, iSel);
}

public func ChooserFinished()
{
  iTickets = iStartTickets;
  return _inherited(...);
}

/* Funktionen */

static const GASS_Spawn_Def = 0;
static const GASS_Spawn_Att = 1;

public func AddAssaultTarget(id idTarget, int iX, int iY, int iMaxDamage, int iTeam, string szName, int iIndex, array aSpawn, bool fNoBar)
{
  if (_inherited(idTarget, iX, iY, iMaxDamage, iTeam, szName, iIndex, aSpawns, fNoBar))
  {
    //Team setzen
    if (iTeam && iDefender == -1)
	  iDefender = iTeam;
	//Spawnpunkte anders behandeln
	aSpawns[iIndex] = aSpawn;
	return true;
  }
}

public func ReportAssaultTargetDestruction(object pTarget, int iTeam)
{
  if (GetIndexOf(pTarget, aTargets[iTeam]) == -1)
    return;

  _inherited(pTarget, iTeam, ...);

  //Und gleich mal bekanntgeben
  EventInfo4K(0, Format("$TargetDestruction$", GetTeamColor(iTeam), GetName(pTarget)), GBAS, 0, 0, 0, "Info4.ogg");
  var extra = GameCall("OnAssaultTargetDestruction", pTarget, iTeam, FindInArray4K(aTargets[iTeam], pTarget));
  if (pTarget && !(extra & AS_NoDestruction))
    Explode(50, pTarget);
  RemoveEffect("IntAssaultTarget", pTarget);

  //Verteidiger haben keine Ziele mehr
  if (!ObjectCount2(Find_InArray(aTargets[iTeam])))
    for (var i = 0; i < GetPlayerCount(); i++)
	  if (GetPlayerTeam(GetPlayerByIndex(i)) == iTeam)
	    EventInfo4K(GetPlayerByIndex(i)+1, "$NoTargets$", GASS, 0, 0, 0, "Alarm.ogg");

  //Tickets resetten
  if (!(extra & AS_NoTicketReset))
    iTickets = iStartTickets;
}

public func TeamGetScore(int iTeam)
{
  if (iTeam == iDefender)
    return ObjectCount2(Find_InArray(aTargets[iDefender]));
  return iTickets;
}

global func ConnectAssaultTargets()
{
  var goal = FindObject2(Find_Func("IsCMCAssaultGoal"));
  if (goal)
    return goal->~ConnectAssaultTargets(...);
}

public func ConnectAssaultTargets(array a)
{
  for (var i in a)
  {
    if (!Connected[i])
	  Connected[i] = [];
	for (var j in a)
	  if (j != i)
	    Connected[i][GetLength(Connected[i])] = j;
  }
  return true;
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

  var index = GASS_Spawn_Att;
  //Verteidiger?
  if (GetPlayerTeam(iPlr) == iDefender)
    index = GASS_Spawn_Def;

  //Spieler darf gar nicht mehr joinen?
  if (index == GASS_Spawn_Att)
  {
    //Angreifer: Keine Tickets?
	if (!iTickets)
	  return ScheduleCall(this, "WaitForJoin", 5, 0, iPlr);
  }
  else
    //Verteidiger: Keine Ziele?
	if (!ObjectCount2(Find_InArray(aTargets[iDefender])))
	  return EliminatePlayer(iPlr);

  //Kein Verteidiger? Ticket-Abzug
  if (GetPlayerTeam(iPlr) != iDefender && iKiller != -2)
	iTickets = Max(iTickets-1);	  
	
  //Clonk wegstecken
  var pCrew = GetCrew(iPlr);
  if (!pCrew)
    return;
  var cont = Contained(pCrew);
  var tim = CreateObject(TIM2, 0, 0, -1);
  Enter(tim, pCrew);
  if (cont)
    RemoveObject(cont);

  //Spawnpunkt raussuchen
  if (ObjectCount2(Find_InArray(aTargets[iDefender])))
  {
    for (var target_index; target_index < GetLength(aTargets[iDefender]); target_index++)
      if (aTargets[iDefender][target_index])
	    break;
  }
  else
    var target_index = GetLength(aTargets[iDefender])-1;

  var rand = Random(GetLength(aSpawns[target_index][index]));
  SetPosition(aSpawns[target_index][index][rand][0], aSpawns[target_index][index][rand][1]-10, tim);
  tim->Spawn();
}

protected func WaitForJoin(int iPlr)
{
  //Es gibt wieder Tickets!
  if (iTickets)
    return RelaunchPlayer(iPlr, GetCrew(iPlr), -2);

  //Alle anderen Angreifer sind tot -> verloren!
  var alive;
  for (var obj in FindObjects(Find_Func("IsClonk")))
  {
    var iPlr = GetOwner(obj);
    if (GetPlayerTeam(iPlr) == iDefender)
	  continue;
	if (GetAlive(obj) && GetID(Contained(obj)) != TIM1 && GetID(Contained(obj)) != TIM2)
	  alive = true;
  }

  if (alive)
    return ScheduleCall(this, "WaitForJoin", 5, 0, iPlr);
  EliminatePlayer(iPlr);
}

public func GetRespawnPoint(int &iX, int &iY, int iTeam)
{
  var index = GASS_Spawn_Att;
  if (iTeam == iDefender)
    index = GASS_Spawn_Def;

  //Erstmal checken
  if (GetType(aTargets) != C4V_Array || !GetLength(aTargets))
    return;

  //Nächstes Ziel suchen
  var target;
  if (ObjectCount2(Find_InArray(aTargets[iDefender])))
    for (target = 0; target < GetLength(aTargets); target++)
	  if (aTargets[iDefender][target])
	    break;

  //Kein Ziel? Einfach 0 zurückgeben
  if (!aTargets[iDefender][target])
    return;

  iX = aSpawns[target][index][0][0];
  iY = aSpawns[target][index][0][1];
  return true;
}

/* Scoreboard */

static const GASS_Icon = 0;
static const GASS_Name = 1;
static const GASS_Count = 2;
static const GASS_TargetRow = 4;

public func UpdateScoreboard()
{
  //Wird noch eingestellt
  if (FindObject(CHOS))
    return;

  //Komplett leeren
  ClearScoreboard();

  //Erst das nächste Ziel raussuchen
  var obj;
  for (var index; index < GetLength(aTargets[iDefender]); index++)
    if (obj = aTargets[iDefender][index])
	  break;

  //Titelzeile
  SetScoreboardData(SBRD_Caption, SBRD_Caption, GetName());
  SetScoreboardData(SBRD_Caption, GASS_Icon, Format("{{%i}}", GetID()));
  SetScoreboardData(SBRD_Caption, GASS_Name, Format("<c %x>$Targets$</c>", GetTeamColor(iDefender)));
  SetScoreboardData(SBRD_Caption, GASS_Count, Format("<c %x>%d</c>", GetTeamColor(iDefender), ObjectCount2(Find_InArray(aTargets[iDefender]))), 0);

  //Zeile fürs nächste Ziel
  if (obj)
  {
    //Ziel und zusammenhängende Ziele anzeigen
	index = GetIndexOf(obj, aTargets[iDefender]);
	AddScoreboardTarget(obj, index);
	if (GetType(Connected[index]) == C4V_Array)
	  for (var i; i < GetLength(Connected[index]); i++)
	    if (Connected[index][i])
		  AddScoreboardTarget(aTargets[iDefender][Connected[index][i]], Connected[index][i]);
  }

  //Leerzeile
  SetScoreboardData(1, GASS_Count, "<c ffffffff> </c>", 2);

  //Tickets
  var string = Format("<c %x>$Attackers$</c>", RGB(255, 255, 255));
  var color = RGB(255, 255, 255);
  var team = GetTeamByIndex();
  //Nur ein Angreiferteam
  if (GetActiveTeamCount() == 2)
  {
    if (team == iDefender)
	  team = GetTeamByIndex(1);
	string = GetTaggedTeamName(team);
	color = GetTeamColor(team);
  }
  SetScoreboardData(2, GASS_Icon, "{{SM03}}");
  SetScoreboardData(2, GASS_Name, string);
  SetScoreboardData(2, GASS_Count, Format("<c %x>%d</c>", color, iTickets), 3);
  
  //Sortieren
  SortScoreboard(GASS_Count);
}

private func ClearScoreboard()
{
  for (var i = 0; i < 32; i++)
    for (var j = 0; j < 4; j++)
	  SetScoreboardData(i,j);
}

private func AddScoreboardTarget(object pTarget, int iRow)
{
  if (!pTarget)
    return;
  SetScoreboardData(iRow+GASS_TargetRow, GASS_Icon, Format("{{%i}}", pTarget->GetImitationID()));
  SetScoreboardData(iRow+GASS_TargetRow, GASS_Name, Format("<c %x>%s</c>", GetTeamColor(iDefender), GetName(pTarget)));
  var effect = GetEffect("IntAssaultTarget", pTarget);
  var percent = 100-GetDamage(pTarget)*100/EffectVar(0, pTarget, effect);
  SetScoreboardData(iRow+GASS_TargetRow, GASS_Count, Format("<c %x>%d%</c>", GetTeamColor(iDefender), percent), 1);
}

/* Ziel */

protected func FxIntGoalTimer()
{
  UpdateScoreboard();
  IsFulfilled();
}

local fulfilled;

private func IsFulfilled()
{
  if (FindObject(CHOS))
    return;
  if (fulfilled)
    return fulfilled;

  //Keine Ziele mehr -> Verteidiger eliminiert
  if (!ObjectCount2(Find_InArray(aTargets[iDefender])))
    EliminateTeam(iDefender);

  //Nur noch ein Team übrig - Sieg!
  if (GetActiveTeamCount() == 1)
  {
    //Spielende planen
    Schedule("GameOver()", 150);

    //Auswertung
    RewardEvaluation();

    RemoveAll(GOAL);
    Sound("Cheer.ogg", true);
    Message("@$TeamHasWon$", 0, GetTaggedTeamName(GetTeamByIndex()));
    return fulfilled = true;
  }
}