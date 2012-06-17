/*-- Assault --*/

#strict 2
#include CASS

local iAttacker;
local iDefender;	//Verteidiger-Team
local iTickets;		//Tickets für die Angreifer
local iWarningTickets;	//Tickets bei denen gewarnt wird
local aSpawns;		//Spawnpunkte
local Connected;	//Verbundene Ziele


/* Initialisierung */

protected func Initialize()
{
  iAttacker = -1;
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

  //Ticketabzug-Effekt starten
  AddEffect("TicketSubtraction", this, 101, 35, this);

  return _inherited(...);
}

protected func InitializeTickets()
{
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
    Sound("ObjectiveReceipt.ogg", true, 0, 100, GetPlayerByIndex(i) + 1);
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

  //Ticketformel
  //return D + (A + 2 * D + D * D) / (A + 1);
  return D + (D * (4 + (20 - D) * D / 12)) / A;
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

/* Zielobjektzerstörung */

public func ReportAssaultTargetDestruction(object pTarget, int iTeam)
{
  var index = GetIndexOf(pTarget, aTargets[iTeam]);
  if (index == -1)
    return;

  _inherited(pTarget, iTeam, ...);

  //Auf Zielobjektverbund prüfen
  var fConnectedDestruction = true;;
  if (GetType(Connected[index]) != C4V_Array)
    fConnectedDestruction = false;
  else
    for (var i in Connected[index])
      if (aTargets[iDefender][i])
        fConnectedDestruction = false;

  //Bekanntgabe der Zerstörung
  EventInfo4K(0, Format("$TargetDestruction$", GetTeamColor(iTeam), GetName(pTarget)), GBAS, 0, 0, 0, "RadioConfirm*.ogg");
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

  //Tickets zurücksetzen, bei verbundenen Zielobjekten nur wenn alle Ziele zerstört sind
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
    if(!Connected[i])
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
  //Ticketabzug-Timer zurücksetzen
  var effect = GetEffect("TicketSubtraction", this);
  if(effect)
    EffectCall(this, effect, "Reset");

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

/* Ticketabzug-Effekt */

static const GASS_TicketIdleTime = 40; //Zeit in Sekunden, die benötigt werden, damit ein Ticket verloren geht
static const GASS_TicketCooldown = 20; //Zeit in Sekunden, die benötigt werden, damit nach letztem verursachten Schaden der Ticketabzug-Timer beginnt

protected func FxTicketSubtractionStart(object pTarget, int iEffect)
{ 
  EffectVar(0, pTarget, iEffect) = GASS_TicketCooldown; 
  return true;
}

protected func FxTicketSubtractionTimer(object pTarget, int iEffect)
{
  if(EffectVar(1, pTarget, iEffect))
  {
    EffectVar(0, pTarget, iEffect)++;
    if(EffectVar(0, pTarget, iEffect) == GASS_TicketIdleTime && iTickets > 0)
    {
      if(iTickets > 0)
      {
        iTickets--;
        //Event-Nachrichten
        if(iTickets && iTickets == iWarningTickets)
          Schedule(Format("GameCallEx(\"TicketsLow\", %d, %d, true)", iTickets, iDefender), 1);
        else if(!iTickets)
          Schedule(Format("GameCallEx(\"NoTickets\", %d, true)", iDefender), 1);
        else
        {
          var cnt = GetTeamPlayerCount(iAttacker);
          for(var i = 0; i < cnt; i++)
            EventInfo4K(1+GetTeamMemberByIndex(iAttacker, i), "$TicketLossAttacker$", GetID(), 0, 0, 0, "Alarm.ogg");

          for(var i = 0, cnt = GetTeamPlayerCount(iDefender); i < cnt; i++)
            EventInfo4K(1+GetTeamMemberByIndex(iDefender, i), "$TicketLossDefender$", GetID());
        }

        EffectVar(0, pTarget, iEffect) = 0;
      }
    }
  }
  else
  {
    EffectVar(0, pTarget, iEffect)--;
    if(EffectVar(0, pTarget, iEffect) <= 0)
    {
      EffectVar(1, pTarget, iEffect) = true;
      var cnt = GetTeamPlayerCount(iAttacker);
      for(var i = 0; i < cnt; i++)
        EventInfo4K(1+GetTeamMemberByIndex(iAttacker, i), "$TicketLossWarningAttacker$", GetID(), 0, 0, 0, "Alarm.ogg");

      for(var i = 0, cnt = GetTeamPlayerCount(iDefender); i < cnt; i++)
        EventInfo4K(1+GetTeamMemberByIndex(iDefender, i), "$TicketLossWarningDefender$", GetID());
    }
  }

  return true;
}

protected func FxTicketSubtractionReset(object pTarget, int iEffect)
{
  EffectVar(1, pTarget, iEffect) = false;
  EffectVar(0, pTarget, iEffect) = GASS_TicketCooldown;

  return true;
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

  //Kein Angreiferteam definiert?
  if(iAttacker == -1 && GetPlayerTeam(iPlr) != iDefender)
    iAttacker = GetPlayerTeam(iPlr);

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
      if (iTickets && iTickets == iWarningTickets)
        Schedule(Format("GameCallEx(\"TicketsLow\", %d, %d, true)", iTickets, iDefender), 1);
      if (!iTickets)
        Schedule(Format("GameCallEx(\"NoTickets\", %d, true)", iDefender), 1);
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

/* EventInfos */

public func TicketsLow(int iRemaining, int iTeam, bool fExclude)
{
  for (var i = 0; i < GetPlayerCount(); i++)
    if ((!fExclude && GetPlayerTeam(GetPlayerByIndex(i)) == iTeam) || (fExclude && GetPlayerTeam(GetPlayerByIndex(i)) != iTeam))
      //Nachricht über Tickettiefstand
      EventInfo4K(GetPlayerByIndex(i) + 1, Format("$MsgTicketsLow$", iRemaining), SM03, 0, 0, 0, "Alarm.ogg");
  return true;
}

public func NoTickets(int iTeam, bool fExclude)
{
  for (var i = 0; i < GetPlayerCount(); i++)
    if ((!fExclude && GetPlayerTeam(GetPlayerByIndex(i)) == iTeam) || (fExclude && GetPlayerTeam(GetPlayerByIndex(i)) != iTeam))
      //Nachricht über Verlust aller Tickets
      EventInfo4K(GetPlayerByIndex(i) + 1, Format("$MsgNoTickets$"), SM03, 0, 0, 0, "Alarm.ogg");
  return true;
}

/* Scoreboard */

static const GASS_Icon = 0;
static const GASS_Name = 1;
static const GASS_Count = 2;
static const GASS_TargetRow = 4;

static const GASS_DominationInfo = 100;

public func UpdateScoreboard()
{
  //Wird noch eingestellt
  if (FindObject(CHOS)) return;

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
  if (GetActiveTeamCount(true) == 2 - !GetTeamPlayerCount(iDefender))
  {
    if (team == iDefender)
      team = GetTeamByIndex(1);
    string = GetTaggedTeamName(team);
    color = GetTeamColor(team);
  }
  SetScoreboardData(2, GASS_Icon, "{{SM03}}");
  SetScoreboardData(2, GASS_Name, string);
  SetScoreboardData(2, GASS_Count, Format("<c %x>%d</c>", color, iTickets), 201);
  
  var icon = IC10;
  var str = "$AttackerDominate$";
  var effect = GetEffect("TicketSubtraction", this);

  if(EffectVar(1, this, effect))
  {
    icon = IC12;
    str = "$DefenderDominate$";
  }

  SetScoreboardData(GASS_DominationInfo, GASS_Icon, Format("{{%i}}", icon));
  SetScoreboardData(GASS_DominationInfo, GASS_Name, str);
  SetScoreboardData(GASS_DominationInfo, GASS_Count, "", 202);

  //Sortieren
  SortScoreboard(GASS_Count);
}

private func ClearScoreboard()
{
  for (var i = 0; i < 20; i++)
    for (var j = 0; j < 3; j++)
      SetScoreboardData(i, j);
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

protected func FxIntGoalTimer()
{
  UpdateScoreboard();
}

/* Rundenauswertung */

local fulfilled;

private func IsFulfilled()
{
  if (FindObject(CHOS))
    return;
  if (fulfilled)
    return fulfilled;

  var won = false;

  //Zielobjekte zerstört: Angreifer gewinnen
  if (!ObjectCount2(Find_InArray(aTargets[iDefender])))
  {
    //Verteidiger eliminieren
    EliminateTeam(iDefender);

    //Nachricht über Gewinner
    Message("@$AttackersWon$");

    //Achievement-Ausgabe
    for (var i = 0, j; i < GetPlayerCount(); i++)
    {
      if (GetPlayerTeam(j = GetPlayerByIndex(i)) == iDefender)
        continue;
      //Achievement-Fortschritt (Stormtrooper)
      DoAchievementProgress(1, AC25, GetPlayerByIndex(i));
    }

    //Vorbei
    won = true;
  }

  //Keine Angreifer übrig: Verteidiger gewinnen
  else if (iAttacker != -1 && !TeamGetScore(iAttacker) && GetActiveTeamCount(true) == 1)
  {
    //Angreifer eliminieren 
    EliminateTeam(iAttacker);

    //Nachricht über Gewinner
    Message("@$DefendersWon$");

    //Vorbei
    won = true;
  }

  //Keine Verteidiger übrig: Angreifer gewinnen
  else if (iDefender != -1 && !TeamGetScore(iDefender) && GetActiveTeamCount(true) < 2)
  {
    //Verteidiger eliminieren 
    EliminateTeam(iDefender);

    //Nachricht über Gewinner
    Message("$AttackersWon$");

    //Vorbei
    won = true;
  }

  if (won)
  {
    //Spielende planen
    Schedule("GameOver()", 150);

    //Auswertung
    RewardEvaluation();

    //Sound
    Sound("Cheer.ogg", true);

    RemoveAll(GOAL);
    return fulfilled = true;
  }
}
