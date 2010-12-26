/*-- Assault --*/

#strict 2
#include CASS

local iDefender;		   //Verteidiger-Team
local iTickets;			   //Tickets für die Angreifer
local iWarningTickets; //Tickets bei denen gewarnt wird
local aSpawns;			   //Spawnpunkte
local Connected;		   //Verbundene Ziele


/* Initialisierung */

protected func Initialize()
{
  iDefender = -1;
  aSpawns = [[],[]];
  Connected = [];
  return _inherited(...);
}

public func ChooserFinished()
{
  //Sicherheitshalber einen Frame verzögert
  ScheduleCall(this, "InitializeTickets", 1);

  //Ebenfalls einen Frame verzögert, da der Szenarienscript die Zielobjekte erst noch setzt
  ScheduleCall(this, "LogTask", 1);    

  return _inherited(...);
}

protected func InitializeTickets() {
  iTickets = CalcTickets();
  if(iTickets < 4)
    iWarningTickets = 0;
  else
    iWarningTickets = BoundBy(iTickets/4, 1, 5);
  return true;
}

protected func LogTask()
{
  for (var i = 0, j; i < GetPlayerCount(); i++)
  {
    var TaskID = TK02;
    if (GetPlayerTeam(j = GetPlayerByIndex(i)) == iDefender)
      TaskID = TK03;
    CreateObject(TaskID, 0, 0, GetPlayerByIndex(i));
    Sound("RadioConfirm*.ogg", true, 0, 100, GetPlayerByIndex(i) + 1);
  }
}

/* Funktionen */

static const GASS_Spawn_Def = 0;
static const GASS_Spawn_Att = 1;

public func CalcTickets()
{
  var A, D;
  for (var i; i < GetPlayerCount(); i++)
    if (GetPlayerTeam(GetPlayerByIndex(i)) == iDefender)
	  D++;
	else
	  A++;

  //return D + (A + 2 * D + D * D) / (A + 1);
  return D + (D * (4 + (20 - D) * D / 12)) / (A + 1);
}

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
  var index = GetIndexOf(pTarget, aTargets[iTeam]);
  if (index == -1)
    return;

  _inherited(pTarget, iTeam, ...);

  var fConnectedDestruction = true;;
  if (GetType(Connected[index]) != C4V_Array)
    fConnectedDestruction = false;
  else
    for (var i in Connected[index])
	  if (aTargets[iDefender][i])
	    fConnectedDestruction = false;

  //Und gleich mal bekanntgeben
  EventInfo4K(0, Format("$TargetDestruction$", GetTeamColor(iTeam), GetName(pTarget)), GBAS, 0, 0, 0, "Info.ogg");
  GameCall("OnAssaultTargetDestruction", pTarget, iTeam, FindInArray4K(aTargets[iTeam], pTarget), fConnectedDestruction);
  if (pTarget)
    Explode(50, pTarget);

  //Letztes Ziel
  if (ObjectCount2(Find_InArray(aTargets[iDefender])) == 1)
    for (var i, j; i < GetPlayerCount(); i++)
	  if (GetPlayerTeam(j = GetPlayerByIndex(i)) == iDefender)
	    EventInfo4K(j+1, "$DefendLastStation$", GetID(), 0, 0, 0, "Alarm.ogg");
	  else
	    EventInfo4K(j+1, "$DestroyLastStation$", GetID(), 0, 0, 0, "Alarm.ogg");
	    

  //Tickets resetten, bei verbundenen nur wenn alle Ziele zerstört sind
  if (GetType(Connected[index]) != C4V_Array)
    InitializeTickets();
  else
    if (fConnectedDestruction)
	    InitializeTickets();
}

public func TeamGetScore(int iTeam)
{
  if (iTeam == iDefender)
    return ObjectCount2(Find_InArray(aTargets[iDefender]));
  return iTickets;
}

public func GetHUDInfo(int iPlayer)
{
  var clr = GetTeamColor(iDefender), team = GetPlayerTeam(iPlayer);
  if (team!= iDefender)
    clr = GetTeamColor(team);
  return Format("<c %x>%d</c>", clr, TeamGetScore(team));
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

private func GetNextTarget()
{
  for (var i; i < GetLength(aTargets[iDefender]); i++)
    if (aTargets[iDefender][i])
	  return i;
  return -1;
}

public func GetAssaultTarget(int iIndex, int iTeam)
{
  return aTargets[iDefender][iIndex];
}

/* Assault-Effekt */

protected func FxIntAssaultTargetDamage(object pTarget, int iEffect, int iDamage)
{
  //Nur durchlassen, wenn das Ziel an der Reihe ist
  var iTarget = GetIndexOf(pTarget, aTargets[iDefender]),
  iNext = GetNextTarget();
  if (iTarget == iNext || (GetType(Connected[iNext]) == C4V_Array && GetIndexOf(iTarget, Connected[iNext]) != -1))
    return iDamage;
  var id = pTarget->~GetImitationID();
  if (!id)
    id = GetID(pTarget);
  var size = Distance(0, 0, GetDefWidth(id), GetDefHeight(id)) * 6;
  CreateParticle("TargetShield", GetX(pTarget) - GetX(), GetY(pTarget) - GetY(), Sin(Random(360), 10), -Cos(Random(360), 10), size, InterpolateRGBa3(RGB(255, 255, 255), GetTeamColor(iDefender), 3, 4));
  Sound("Shield", false, pTarget);
  return 0;
}

/* Relaunch */

public func OnClassSelection(object pCrew)
{
  RelaunchPlayer(GetOwner(pCrew), 0, -2);
}

public func RelaunchPlayer(int iPlr, pClonk, int iKiller)
{
  if(iKiller != -2)
  {
    aDeath[iPlr]++;
    if (iKiller != -1 && GetPlayerTeam(iPlr) != GetPlayerTeam(iKiller))
      aKill[iKiller]++;
        Money(iPlr, pClonk, iKiller);
  }

  //Noch gar keine Ziele: Kurz warten
  if(!GetLength(aTargets[iDefender]))
    return;

  var index = GASS_Spawn_Att;
  //Verteidiger?
  if (GetPlayerTeam(iPlr) == iDefender)
    index = GASS_Spawn_Def;

  if (iKiller != -2)
  {
    //Spieler darf gar nicht mehr joinen?
    if(index == GASS_Spawn_Att)
    {
      //Angreifer: Keine Tickets?
      if(!iTickets)
        return ScheduleCall(this, "WaitForJoin", 5, 0, iPlr);
    }
    else
      //Verteidiger: Keine Ziele?
      if(GetLength(aTargets))
        if(!ObjectCount2(Find_InArray(aTargets[iDefender])))
          return EliminatePlayer(iPlr);

    //Kein Verteidiger? Ticket-Abzug
    if(GetPlayerTeam(iPlr) != iDefender && iKiller != -2)
    {
      iTickets = Max(iTickets-1);
      //Keine Tickets mehr?
      if (iTickets != 0 && iTickets == iWarningTickets) {
        Schedule(Format("GameCallEx(\"TicketsLow\", %d, %d, true)", iTickets, iDefender), 1);
      }
      if (!iTickets) {
      	Schedule(Format("GameCallEx(\"NoTickets\", %d, true)", iDefender), 1);
      }
    }
  }

  //Clonk wegstecken
  var pCrew = GetCrew(iPlr);
  if (!pCrew)
    return;
  var cont = Contained(pCrew);
  var tim = CreateObject(TIM2, 0, 0, iPlr);
  Enter(tim, pCrew);
  if (cont)
    RemoveObject(cont);

  //Spawnpunkt raussuchen
  if (ObjectCount2(Find_InArray(aTargets[iDefender])))
    var target_index = GetNextTarget();
  else
    var target_index = GetLength(aTargets[iDefender])-1;

  var rand = Random(GetLength(aSpawns[target_index][index]));
  SetPosition(aSpawns[target_index][index][rand][0], aSpawns[target_index][index][rand][1]-10, tim);
  AddEffect("IntAssaultSpawn", tim, 1, 1, this);
}

/* EventInfos */

public func TicketsLow(int iRemaining, int iTeam, bool fExclude)
{
  for(var i = 0; i < GetPlayerCount(); i++)
  {
    if((!fExclude && GetPlayerTeam(GetPlayerByIndex(i)) == iTeam) || (fExclude && GetPlayerTeam(GetPlayerByIndex(i)) != iTeam))
    {
      //Nachricht über Tickettiefstand
      EventInfo4K(GetPlayerByIndex(i)+1,Format("$MsgTicketsLow$",iRemaining),SM03,0,0,0,"Alarm.ogg");
    }
  }
  return true;
}

public func NoTickets(int iTeam, bool fExclude)
{
  for(var i = 0; i < GetPlayerCount(); i++)
  {
    if((!fExclude && GetPlayerTeam(GetPlayerByIndex(i)) == iTeam) || (fExclude && GetPlayerTeam(GetPlayerByIndex(i)) != iTeam))
    {
      //Nachricht über Verlust aller Tickets
      EventInfo4K(GetPlayerByIndex(i)+1,Format("$MsgNoTickets$"),SM03,0,0,0,"Alarm.ogg");
    }
  }
  return true;
}

protected func FxIntAssaultSpawnTimer(object pTarget)
{
  if (GetAlive(Contents(0, pTarget)))
    pTarget->~Spawn();
  if (pTarget)
    RemoveObject(pTarget);
}

protected func WaitForJoin(int iPlr)
{
  //Es gibt wieder Tickets!
  if(iTickets || !ObjectCount2(Find_InArray(aTargets[iDefender])))
    return RelaunchPlayer(iPlr, GetCrew(iPlr), -2);

  //Wegstecken falls nötig
  if (!Contained(GetCrew(iPlr)))
  {
    var target = aTargets[iDefender][GetNextTarget()];
    var tim = CreateObject(TIM1, GetX(target)-GetX(), GetY(target)-GetY(), iPlr);
	Enter(tim, GetCrew(iPlr));
	SetPlrViewRange(150, tim);
	AddEffect("IntAssaultWaitObject", tim, 1, 0, tim);
    RemoveEffect("Spawn", GetCrew(iPlr));
  }

  //Alle anderen Angreifer sind tot -> verloren!
  var alive = false;
  for (var obj in FindObjects(Find_Func("IsClonk")))
  {
    if (GetPlayerTeam(GetOwner(obj)) == iDefender)
      continue;
	
    if (GetAlive(obj) && !GetEffect("IntAssaultWaitObject", Contained(obj))/* && (GetOwner(obj) != iPlr || GetMenu(obj) != MCSL)*/)
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
  if (GetType(aTargets) != C4V_Array || !GetLength(aTargets[iDefender]))
    return;

  //Nächstes Ziel suchen
  var target;
  if (ObjectCount2(Find_InArray(aTargets[iDefender])))
    target = GetNextTarget();

  //Kein Ziel? Einfach 0 zurückgeben
  if (!aTargets[iDefender][target])
    return;

  iX = aSpawns[target][index][0][0];
  iY = aSpawns[target][index][0][1]-10;
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

  if (IsFulfilled())
    return;

  //Erst das nächste Ziel raussuchen
  var obj = aTargets[iDefender][GetNextTarget()];

  //Titelzeile
  SetScoreboardData(SBRD_Caption, SBRD_Caption, GetName());
  SetScoreboardData(SBRD_Caption, GASS_Icon, Format("{{%i}}", GetID()));
  SetScoreboardData(SBRD_Caption, GASS_Name, Format("<c %x>$Targets$</c>", GetTeamColor(iDefender)));
  SetScoreboardData(SBRD_Caption, GASS_Count, Format("<c %x>%d</c>", GetTeamColor(iDefender), ObjectCount2(Find_InArray(aTargets[iDefender]))), 0);

  //Zeile fürs nächste Ziel
  if (obj)
  {
    //Ziel und zusammenhängende Ziele anzeigen
	var index = GetIndexOf(obj, aTargets[iDefender]);
	AddScoreboardTarget(obj, index);
	if (GetType(Connected[index]) == C4V_Array)
	  for (var i; i < GetLength(Connected[index]); i++)
	    if (Connected[index][i])
		  AddScoreboardTarget(aTargets[iDefender][Connected[index][i]], Connected[index][i]);
  }

  //Leerzeilen
  SetScoreboardData(0, GASS_Count, "<c ffffffff> </c>", 1);
  SetScoreboardData(1, GASS_Count, "<c ffffffff> </c>", 200);

  //Tickets
  var string = Format("<c %x>$Attackers$</c>", RGB(255, 255, 255));
  var color = RGB(255, 255, 255);
  var team = GetTeamByIndex();
  //Nur ein Angreiferteam
  if (GetActiveTeamCount() == 2 - !GetTeamPlayerCount(iDefender))
  {
    if (team == iDefender)
	  team = GetTeamByIndex(1);
	string = GetTaggedTeamName(team);
	color = GetTeamColor(team);
  }
  SetScoreboardData(2, GASS_Icon, "{{SM03}}");
  SetScoreboardData(2, GASS_Name, string);
  SetScoreboardData(2, GASS_Count, Format("<c %x>%d</c>", color, iTickets), 201);
  
  //Sortieren
  SortScoreboard(GASS_Count);
}

private func ClearScoreboard()
{
  for (var i = 0; i < 20; i++)
    for (var j = 0; j < 3; j++)
	  SetScoreboardData(i,j);
}

private func AddScoreboardTarget(object pTarget, int iRow)
{
  if (!pTarget)
    return;
  var effect = GetEffect("IntAssaultTarget", pTarget);
  var percent = 100-GetDamage(pTarget)*100/EffectVar(0, pTarget, effect);
  var color = InterpolateRGBa3(RGB(255, 255, 255), GetTeamColor(iDefender), percent, 100);

  SetScoreboardData(iRow+GASS_TargetRow, GASS_Icon, Format("{{%i}}", pTarget->~GetImitationID()));
  SetScoreboardData(iRow+GASS_TargetRow, GASS_Name, Format("<c %x>%s</c>", color, GetName(pTarget)));
  SetScoreboardData(iRow+GASS_TargetRow, GASS_Count, Format("<c %x>%d%</c>", color, percent), percent + 50);
}

/* Ziel */

protected func FxIntGoalTimer()
{
  UpdateScoreboard();
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

    //Nachricht über Gewinner
    Message("@$TeamHasWon$", 0, GetTaggedTeamName(GetTeamByIndex()));

    //Sound
    Sound("Cheer.ogg", true);

    RemoveAll(GOAL);
    return fulfilled = true;
  }
}