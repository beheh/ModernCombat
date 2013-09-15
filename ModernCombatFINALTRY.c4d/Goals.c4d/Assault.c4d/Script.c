/*-- Assault --*/

#strict 2
#include CASS

local iAttacker;				//Angreifer-Team
local iDefender;				//Verteidiger-Team
local iTickets;					//Tickets der Angreifer
local iWarningTickets;				//Ticketwarnung
local aSpawns;					//Spawnpunkte
local Connected;				//Verbundene Ziele
local iTicketSubtrTime;				//Ticketabzugszeit

public func PlantTime()		{return 3*35;}	//Zeit zur Plazierung einer Sprengladung
public func DefuseTime()	{return 3*35;}	//Zeit zur Entschärfung einer Sprengladung
public func GoalExtraValue()
{
  var team = GameCall("AssaultDefenderTeam");
  if(GetTeamCount() > 2)
    return 0;

  if(!team)
    return;

  var att = GetTeamByIndex();
  if(att == team)
    att = GetTeamByIndex(1);

  return Format("{{SM19}}: <c %x>%s</c>", GetTeamColor(att), GetTeamName(att));
}

static const BAR_AssaultBar = 4;
static const GASS_PlantRadius = 100;


/* Initialisierung */

protected func Initialize()
{
  iAttacker = -1;
  iDefender = -1;
  iTickets = 1;
  aSpawns = [[],[]];
  Connected = [];
  iTicketSubtrTime = GASS_TicketIdleTime;

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
  for(var i = 0, j; i < GetPlayerCount(); i++)
  {
    var TaskID = TK02;
    if(GetPlayerTeam(j = GetPlayerByIndex(i)) == iDefender)
      TaskID = TK03;
    CreateObject(TaskID, 0, 0, GetPlayerByIndex(i));
    Sound("Info_Round.ogg", true, 0, 100, GetPlayerByIndex(i) + 1);
  }
}

/* Funktionen */

static const GASS_Spawn_Def = 0;
static const GASS_Spawn_Att = 1;

public func CalcTickets()
{
  var A, D;
  for(var i; i < GetPlayerCount(); i++)
    if(GetPlayerTeam(GetPlayerByIndex(i)) == iDefender)
      D++;
    else
      A++;

  if(!D) return 1; //Keine Verteidiger, für korrektes IsFulfilled

  //Ticketformel
  return D + (D * (4 + (20 - D) * D / 12)) / A;
}

public func AddAssaultTarget(id idTarget, int iX, int iY, int iMaxDamage, int iTeam, string szName, int iIndex, array aSpawn, bool fNoBar)
{
  if(_inherited(idTarget, iX, iY, iMaxDamage, iTeam, szName, iIndex, aSpawns, fNoBar))
  {
    //Team setzen
    if(iTeam && iDefender == -1)
      iDefender = iTeam;
    //Spawnpunkte anders behandeln
    aSpawns[iIndex] = aSpawn;
    return true;
  }
}

global func SetTicketSubtractionTime()
{
  return FindObject(GASS)->SetTicketSubtractionTime(...);
}

public func SetTicketSubtractionTime(int iTime)
{
  iTicketSubtrTime = iTime;
  if(!iTime)
    iTicketSubtrTime = GASS_TicketIdleTime;

  var effect = GetEffect("TicketSubtraction", this);
  EffectCall(this, effect, "Reset");

  return effect;
}

/* Zielobjektzerstörung */

public func ReportAssaultTargetDestruction(object pTarget, int iTeam, array aAttacker)
{
  var index = GetIndexOf(pTarget, aTargets[iTeam]);
  if(index == -1)
    return;

  //Auf Zielobjektverbund prüfen
  var fConnectedDestruction = true;;
  if(GetType(Connected[index]) != C4V_Array)
    fConnectedDestruction = false;
  else
    for(var i in Connected[index])
      if(aTargets[iDefender][i])
        fConnectedDestruction = false;

  if(aAttacker)
  {
    var assist = false;
    for(var clonk in aAttacker)
    {
      if(clonk)
        if(assist)
        {
          //Punkte bei Belohnungssystem (Hilfe bei Zielobjektzerstörung)
          DoPlayerPoints(BonusPoints("ASDestructionAssist"), RWDS_TeamPoints, GetOwner(clonk), clonk, IC03);
          //Geldbonus: 25 Clunker
          DoWealth(GetOwner(clonk), 25);
        }
        else
        {
          //Punkte bei Belohnungssystem (Zielobjektzerstörung)
          DoPlayerPoints(BonusPoints("ASDestruction"), RWDS_TeamPoints, GetOwner(clonk), clonk, IC03);
          //Geldbonus: 30 Clunker
          DoWealth(GetOwner(clonk), 30);
        }

      assist = true;
    }
  }

  //Eventnachricht: Zielobjekt zerstört
  EventInfo4K(0, Format("$TargetDestruction$", GetTeamColor(iTeam), GetName(pTarget)), GetID(), 0, 0, 0, "Info_Objective.ogg");
  GameCall("OnAssaultTargetDestruction", pTarget, iTeam, FindInArray4K(aTargets[iTeam], pTarget), fConnectedDestruction);
  Sound("RadioConfirm*.ogg", true);

  //Zielobjekt explodieren lassen sofern vorhanden
  if(pTarget)
  {
    Explode(50, pTarget);
    Sound("StructureHeavyHit*.ogg", false, pTarget);
  }

  if(ObjectCount2(Find_InArray(aTargets[iDefender])) == 1)
    for(var i, j; i < GetPlayerCount(); i++)
      if(GetPlayerTeam(j = GetPlayerByIndex(i)) == iDefender)
        //Eventnachricht: Letztes Zielobjekt verteidigen
        EventInfo4K(j+1, "$DefendLastStation$", GetID(), 0, 0, 0, "Info_Alarm.ogg");
      else
        //Eventnachricht: Letztes Zielobjekt zerstören
        EventInfo4K(j+1, "$DestroyLastStation$", GetID(), 0, 0, 0, "Info_Objective.ogg");

  //Tickets zurücksetzen (bei verbundenen Zielobjekten nur, wenn alle Ziele zerstört wurden)
  if(GetType(Connected[index]) != C4V_Array)
    InitializeTickets();
  else if(fConnectedDestruction)
    InitializeTickets();
}

public func TeamGetScore(int iTeam)
{
  if(iTeam == iDefender)
    return ObjectCount2(Find_InArray(aTargets[iDefender]));
  return iTickets;
}

public func GetHUDInfo(int iPlayer)
{
  var clr = GetTeamColor(iDefender), team = GetPlayerTeam(iPlayer);
  if(team!= iDefender)
    clr = GetTeamColor(team);
  return Format("<c %x>%d</c>", clr, TeamGetScore(team));
}

global func ConnectAssaultTargets()
{
  var goal = FindObject2(Find_Func("IsCMCAssaultGoal"));
  if(goal)
    return goal->~ConnectAssaultTargets(...);
}

public func ConnectAssaultTargets(array a)
{
  for(var i in a)
  {
    if(!Connected[i])
      Connected[i] = [];
    for(var j in a)
      if(j != i)
        Connected[i][GetLength(Connected[i])] = j;
  }
  return true;
}

private func GetNextTarget()
{
  for(var i; i < GetLength(aTargets[iDefender]); i++)
    if(aTargets[iDefender][i])
      return i;
  return -1;
}

public func GetAssaultTarget(int iIndex, int iTeam)
{
  return aTargets[iDefender][iIndex];
}

/* Assault-Effekt */

protected func FxIntAssaultTargetStart(object pTarget, int iEffect, int iTemp, int iMaxTime, bool fNoBar, id idTarget)
{
  if(iTemp)
    return;

  if(!iMaxTime)
    iMaxTime = 35 * 30;

  var bar = EffectVar(0, pTarget, iEffect) = CreateObject(SBAR, 0, 0, -1);
  bar->Set(pTarget, 0xFF0000, BAR_AssaultBar, 200, 0, SM16, 0, 0, true, true);
  bar->ChangeDefHeight(GetObjHeight(pTarget));
  bar->ChangeDefOffset(-GetObjHeight(pTarget)/2+5);
  bar->Update(0, true, true);
  bar->SetIcon(0, SM16, 0, 0, 32);
  bar->PositionToVertex(0, true);

  EffectVar(1, pTarget, iEffect) = idTarget;
  EffectVar(2, pTarget, iEffect) = 0;
  EffectVar(3, pTarget, iEffect) = 0;		//Status
  EffectVar(4, pTarget, iEffect) = 0;		//Entschärfung
  EffectVar(5, pTarget, iEffect) = iMaxTime;

  //Entschärfungsbalken
  bar = EffectVar(6, pTarget, iEffect) = CreateObject(SBAR, 0, 0, -1);
  bar->Set(pTarget, 0x33CCFF, BAR_AssaultBar+1, 200, 0, 0, 0, 0, 0, true);
  bar->ChangeDefHeight(GetObjHeight(pTarget));
  bar->ChangeDefOffset(-GetObjHeight(pTarget)/2+5);
  bar->Update(0, true);
  bar->PositionToVertex(0, true);

  EffectVar(7, pTarget, iEffect) = [];

  //Bepunktung
  EffectVar(8, pTarget, iEffect) = [];
  EffectVar(9, pTarget, iEffect) = [];
}

protected func FxIntAssaultTargetTimer(object pTarget, int iNr, int iTime)
{
  //Ziel an der Reihe?
  var iTarget = GetIndexOf(pTarget, aTargets[iDefender]),
  iNext = GetNextTarget();

  var fConnected = GetType(Connected[iNext]) == C4V_Array;

  var bar = EffectVar(0, pTarget, iNr);

  if(iTarget != iNext && (fConnected && GetIndexOf(iTarget, Connected[iNext]) == -1 || !fConnected))
  {
    bar->Update(0, true);
    return true;
  }

  var idTarget = EffectVar(1, pTarget, iNr);

  var status = EffectVar(2, pTarget, iNr);
  var process = EffectVar(3, pTarget, iNr);
  var def_process = EffectVar(4, pTarget, iNr); 
  var maxTime = EffectVar(5, pTarget, iNr);

  var attacker = EffectVar(8, pTarget, iNr), defender = EffectVar(9, pTarget, iNr);

  var clonks = FindObjects(pTarget->Find_Distance(GASS_PlantRadius), Find_OCF(OCF_CrewMember|OCF_Alive), Find_NoContainer());
  var aEnemies = [], aAllies = [], team = pTarget->~GetTeam();

  //Fraktionen identifizieren
  for(var clonk in clonks)
  {
    if(GetOwner(clonk) == NO_OWNER) continue;
    if(!GetPlayerName(GetOwner(clonk)) || !GetPlayerTeam(GetOwner(clonk))) continue;
    if(!PathFree4K(GetX(pTarget),GetY(pTarget)-GetObjHeight(pTarget)/2,GetX(clonk),GetY(clonk),4)) continue;

    if(GetPlayerTeam(GetOwner(clonk)) == team)
    {
      if(status == 3 && GetIndexOf(clonk, defender) == -1)
        defender[GetLength(defender)] = clonk;

      aAllies[GetLength(aAllies)] = clonk;
    }
    else
    {
      if(status == 1 && GetIndexOf(clonk, attacker) == -1)
        attacker[GetLength(attacker)] = clonk;

      aEnemies[GetLength(aEnemies)] = clonk;
    }
  }

  //Angreifer- und Verteidigeraufkommen jeweils zusammenrechnen
  var alliescnt = GetLength(aAllies), enemycnt = GetLength(aEnemies);

  //Status 0: Keine Aktivität
  if(!status)
  {
    bar->Update(0, true, true);

    //Angreifer vorhanden: Sprengladung wird platziert
    if(!GetEffect("TeamBorder", this) && enemycnt)
    {
      status = 1;
      process = 1;
      bar->SetIcon(0, SM17, 0, 0, 32);
      ShowPlantRadius(pTarget);
    }
  }

  //Status 1: Sprengladung wird platziert
  else if(status == 1)
  {
    bar->Update(process * 100 / PlantTime(), false);

    //Nur Angreifer vorhanden: Platzierung voranbringen
    if(enemycnt && !alliescnt)
    {
      process++;
      bar->SetIcon(0, SM17, 0, 0, 32);

      Sound("AHBS_Progress*.ogg", false, pTarget);
    }
    //Keine Angreifer vorhanden: Platzierung abbrechen
    else if(!enemycnt)
    {
      process = 0;
      status = 0;
      attacker = [];

      bar->Update(0, true, true);
      bar->SetIcon(0, SM16, 0, 0, 32);
    }
    //Angreifer und Verteidiger vorhanden: Platzierung blockieren
    else if(enemycnt && alliescnt)
      bar->SetIcon(0, SM19, 0, 0, 32);

    //Platzierung erfolgreich: Sprengladung ist scharf
    if(process >= PlantTime())
    {
      status = 2;
      process = maxTime;
      bar->SetIcon(0, SM18, 0, 0, 32);

      //Ticketabzug-Timer zurücksetzen
      var effect = GetEffect("TicketSubtraction", this);
      if(effect)
        EffectCall(this, effect, "Reset");

      //Eventnachricht: Ladung plaziert, verteidigen
      TeamEventInfo(iAttacker, Format("$TargetArmedAttacker$", GetName(pTarget)), SM16, 0, 0, 0, "Info_Event.ogg");
      //Eventnachricht: Ladung plaziert, entschärfen
      TeamEventInfo(iDefender, Format("$TargetArmedDefender$", GetName(pTarget)), SM17, 0, 0, 0, "Info_Event.ogg");

      var assist = false;
      for(var clonk in attacker)
      {
        if(clonk)
          if(assist)
          {
            //Punkte bei Belohnungssystem (Hilfe bei Sprengladungsplazierung)
            DoPlayerPoints(BonusPoints("ASTargetArmedAssist"), RWDS_TeamPoints, GetOwner(clonk), clonk, IC15);
          }
          else
          {
            //Punkte bei Belohnungssystem (Sprengladungsplazierung)
            DoPlayerPoints(BonusPoints("ASTargetArmed"), RWDS_TeamPoints, GetOwner(clonk), clonk, IC15);

            //Achievement-Fortschritt (Bomberman)
            DoAchievementProgress(1, AC43, GetOwner(clonk));
          }

        assist = true;
      }

      AddEffect("IntAlarmBlink", pTarget, 1, 1, this);

      Sound("AHBS_Fused.ogg", false, pTarget);
    }
  }

  //Status 2: Sprengladung ist scharf
  else if(status == 2)
  {
    //Ticketabzug-Timer zurücksetzen
    var effect = GetEffect("TicketSubtraction", this);
    if(effect)
      EffectCall(this, effect, "Reset");

    bar->Update(process * 100 / maxTime, false);
    bar->SetIcon(0, SM18, 0, 0, 32);
    process--;

    //Verteidiger vorhanden: Entschärfung starten
    if(alliescnt)
    {
      def_process = 1;
      status = 3;
      ShowPlantRadius(pTarget);
    }
    //Countdown abgelaufen: Sprengsatz explodiert
    else if(process <= 0)
    {
      SetController(GetOwner(attacker[0]), pTarget);
      return ReportAssaultTargetDestruction(pTarget, team, attacker);
    }
  }

  //Status 3: Sprengladung wird entschärft
  else if(status == 3)
  {
    EffectVar(6, pTarget, iNr)->Update(def_process * 100 / DefuseTime(), false);

    //Nur Verteidiger vorhanden: Entschärfung voranbringen
    if(!enemycnt && alliescnt)
    {
      def_process++;
      bar->SetIcon(0, SM16, 0, 0, 32);

      Sound("AHBS_Progress*.ogg", false, pTarget);
    }
    //Keine Verteidiger vorhanden: Entschärfung abbrechen
    else if(!alliescnt)
    {
      def_process = 0;
      status = 2;
      EffectVar(6, pTarget, iNr)->Update(0, true);
      defender = [];
    }
    //Angreifer und Verteidiger vorhanden: Entschärfung blockieren
    else if(enemycnt && alliescnt)
      bar->SetIcon(0, SM19, 0, 0, 32);

    //Entschärfung erfolgreich: Status zurücksetzen
    if(def_process >= DefuseTime())
    {
      process = 0;
      def_process = 0;
      status = 0;
      EffectVar(6, pTarget, iNr)->Update(0, true);

      //Eventnachricht: Ladung entschärft, neue setzen
      TeamEventInfo(iAttacker, Format("$TargetDefusedAttacker$", GetName(pTarget)), SM17, 0, 0, 0, "Info_Event.ogg");
      //Eventnachricht: Ladung entschärft
      TeamEventInfo(iDefender, Format("$TargetDefusedDefender$", GetName(pTarget)), SM16, 0, 0, 0, "Info_Event.ogg");

      var assist = false;
      for(var clonk in defender)
      {
        if(clonk)
          if(assist)
          {
            //Punkte bei Belohnungssystem (Hilfe bei Sprengladungsentschärfung)
            DoPlayerPoints(BonusPoints("ASTargetDefusedAssist"), RWDS_TeamPoints, GetOwner(clonk), clonk, IC15);
          }
          else
          {
            //Punkte bei Belohnungssystem (Sprengladungsentschärfung)
            DoPlayerPoints(BonusPoints("ASTargetDefused"), RWDS_TeamPoints, GetOwner(clonk), clonk, IC15);

            //Achievement-Fortschritt (Firewall)
            DoAchievementProgress(1, AC44, GetOwner(clonk));
          }
        assist = true;
      }

      attacker = [];
      defender = [];

      bar->Update(0, true, true);
      bar->SetIcon(0, SM16, 0, 0, 32);

      Sound("AHBS_Defused.ogg", false, pTarget);
    }
  }

  EffectVar(2, pTarget, iNr) = status;		//Bomben-Zustand
  EffectVar(3, pTarget, iNr) = process;		//Status
  EffectVar(4, pTarget, iNr) = def_process;	//Entschärfung
  EffectVar(8, pTarget, iNr) = attacker;	//Angreifer
  EffectVar(9, pTarget, iNr) = defender;	//Verteidiger
}

protected func FxIntAssaultTargetDamage(object pTarget, int iEffect, int iDamage)		{}
protected func FxIntAssaultTargetStop(object pTarget, int iEffect, int iCause, bool fTemp)	{}

/* Umkreis-Effekt */

protected func ShowPlantRadius(object pTarget)
{
  //Kreis-Symbol erstellen
  var obj = CreateObject(SM09, 0, 0, -1);
  obj->Set(pTarget);

  //Symbolgröße anpassen
  var wdt = GASS_PlantRadius * 2000 / GetDefWidth(SM09);

  //Symbol konfigurieren
  obj->SetObjDrawTransform(wdt, 0, 0, 0, wdt, 0);
  obj->SetGraphics("Big");
  obj->SetColorDw(RGB(255,0,0));

  return true;
}

/* Alarm-Effekt */

protected func FxIntAlarmBlinkStart(object pTarget, int iNr)
{
  EffectVar(0, pTarget, iNr) = 18;
  EffectVar(2, pTarget, iNr) = AddLight(300, RGB(220, 0, 0), pTarget);
  SetVisibility(VIS_None, EffectVar(2, pTarget, iNr));
}

protected func FxIntAlarmBlinkTimer(object pTarget, int iNr, int iTime)
{
  var light = EffectVar(2, pTarget, iNr);

  var effect = GetEffect("IntAssaultTarget", pTarget);
  if(!effect)
    return;

  var process = EffectVar(3, pTarget, effect);
  var status = EffectVar(2, pTarget, effect);
  var maxTime = EffectVar(5, pTarget, effect);
  if(status < 2)
    return -1;

  //Zeit abgelaufen?
  if(iTime % EffectVar(0, pTarget, iNr))
    return;

  //Beep-Geräusch
  var sound = EffectVar(1, pTarget, iNr) = !EffectVar(1, pTarget, iNr);
  if(sound)
  {
    if(process > maxTime/3*2)
      Sound("AHBS_Beep1.ogg", false, pTarget);
    else if(Inside(process, maxTime/3, maxTime/3*2))
    {
      Sound("AHBS_Beep2.ogg", false, pTarget);
      EffectVar(0, pTarget, iNr) = 9;
    }
    else if(process < maxTime/3)
    {
      Sound("AHBS_Beep3.ogg", false, pTarget);
      EffectVar(0, pTarget, iNr) = 4;
    }
  }

  //Alarmlicht-Blinken
  if(GetVisibility(light) == VIS_None)
    SetVisibility(VIS_All, light);
  else
    SetVisibility(VIS_None, light);
}

protected func FxIntAlarmBlinkStop(object pTarget, int iNr)
{
  if(EffectVar(2, pTarget, iNr))
    RemoveObject(EffectVar(2, pTarget, iNr));
}

/* Ticketabzug-Effekt */

static const GASS_TicketIdleTime = 90;	//Zeit in Sekunden, bis bei aktivem Ticketabzug-Timer ein Ticket verloren geht
static const GASS_TicketCooldown = 180; //Zeit in Sekunden, bis der Ticketabzug-Timer beginnt

protected func FxTicketSubtractionStart(object pTarget, int iEffect)
{
  EffectVar(0, pTarget, iEffect) = GASS_TicketCooldown; 
  EffectVar(2, pTarget, iEffect) = iTicketSubtrTime;
  if(!EffectVar(2, pTarget, iEffect))
    EffectVar(2, pTarget, iEffect) = GASS_TicketIdleTime;

  return true;
}

protected func FxTicketSubtractionTimer(object pTarget, int iEffect)
{
  //Ende bei Spielzielerfüllung
  if(fulfilled)
    return false;

  if(EffectVar(1, pTarget, iEffect))
  {
    EffectVar(0, pTarget, iEffect)++;
    if(EffectVar(0, pTarget, iEffect) == EffectVar(2, pTarget, iEffect) && iTickets > 0)
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
            //Eventnachricht: Ticketverlust, Vorsicht
            EventInfo4K(1+GetTeamMemberByIndex(iAttacker, i), "$TicketLossAttacker$", GetID(), 0, 0, 0, "Info_Alarm.ogg");

          for(var i = 0, cnt = GetTeamPlayerCount(iDefender); i < cnt; i++)
            //Eventnachricht: Ticketverlust
            EventInfo4K(1+GetTeamMemberByIndex(iDefender, i), "$TicketLossDefender$", GetID(), 0, 0, 0, "Info_Event.ogg");
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
        //Eventnachricht: Warnung vor Ticketverlust
        EventInfo4K(1+GetTeamMemberByIndex(iAttacker, i), "$TicketLossWarningAttacker$", GetID(), 0, 0, 0, "Info_Event.ogg");

      for(var i = 0, cnt = GetTeamPlayerCount(iDefender); i < cnt; i++)
        //Eventnachricht: Hinweis auf Ticketverlust
        EventInfo4K(1+GetTeamMemberByIndex(iDefender, i), "$TicketLossWarningDefender$", GetID(), 0, 0, 0, "Info_Event.ogg");
    }
  }

  return true;
}

protected func FxTicketSubtractionReset(object pTarget, int iEffect)
{
  EffectVar(1, pTarget, iEffect) = false;
  EffectVar(0, pTarget, iEffect) = GASS_TicketCooldown;

  EffectVar(2, pTarget, iEffect) = iTicketSubtrTime;
  if(!EffectVar(2, pTarget, iEffect))
    EffectVar(2, pTarget, iEffect) = GASS_TicketIdleTime;

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
    if(iKiller != -1 && GetPlayerTeam(iPlr) != GetPlayerTeam(iKiller))
      aKill[iKiller]++;
        Money(iPlr, pClonk, iKiller);
  }

  //Kein Angreiferteam definiert?
  if(iAttacker == -1 && GetPlayerTeam(iPlr) && GetPlayerTeam(iPlr) != iDefender)
  {
    iAttacker = GetPlayerTeam(iPlr);
  }

  //Noch gar keine Ziele: Kurz warten
  if(!GetLength(aTargets[iDefender]))
    return;

  var index = GASS_Spawn_Att;

  //Verteidiger?
  if(GetPlayerTeam(iPlr) == iDefender)
    index = GASS_Spawn_Def;

  if(iKiller != -2)
  {
    //Spieler darf gar nicht mehr joinen?
    if(index == GASS_Spawn_Att)
    {
      //Angreifer: Keine Tickets?
      if(!iTickets)
        return AddEffect("WaitForJoin", this, 1, 5, this, 0, iPlr);
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
      if(iTickets && iTickets == iWarningTickets)
        Schedule(Format("GameCallEx(\"TicketsLow\", %d, %d, true)", iTickets, iDefender), 1);
      if(!iTickets)
        Schedule(Format("GameCallEx(\"NoTickets\", %d, true)", iDefender), 1);
    }
  }

  //Clonk wegstecken
  var pCrew = GetCrew(iPlr);
  if(!pCrew)
    return;
  var cont = Contained(pCrew);
  var tim = CreateObject(TIM2, 0, 0, iPlr);
  Enter(tim, pCrew);
  if(cont)
    RemoveObject(cont);

  //Spawnpunkt raussuchen
  if(ObjectCount2(Find_InArray(aTargets[iDefender])))
    var target_index = GetNextTarget();
  else
    var target_index = GetLength(aTargets[iDefender])-1;

  var x, y;
  GetBestSpawnpoint(aSpawns[target_index][index], iPlr, x, y);
  SetPosition(x, y-10, tim);
  AddEffect("IntAssaultSpawn", tim, 1, 1, this);
}

protected func FxIntAssaultSpawnTimer(object pTarget)
{
  if(GetAlive(Contents(0, pTarget)))
    pTarget->~Spawn();
  if(pTarget)
    RemoveObject(pTarget);
}

protected func FxWaitForJoinStart(object pTarget, int iNr, iTemp, int iPlr)
{
  EffectVar(0, pTarget, iNr) = iPlr;
}

protected func FxWaitForJoinTimer(object pTarget, int iNr)
{
  var iPlr = EffectVar(0, pTarget, iNr);
  //Es gibt wieder Tickets!
  if(iTickets || !ObjectCount2(Find_InArray(aTargets[iDefender])))
  {
    RelaunchPlayer(iPlr, GetCrew(iPlr), -2);
    return -1;
  }

  //Wegstecken falls nötig
  if(!Contained(GetCrew(iPlr)))
  {
    var target = aTargets[iDefender][GetNextTarget()];
    var tim = CreateObject(TIM1, GetX(target)-GetX(), GetY(target)-GetY(), iPlr);
    Enter(tim, GetCrew(iPlr));
    SetPlrViewRange(200, tim);
    AddEffect("IntAssaultWaitObject", tim, 1, 0, tim);
    RemoveEffect("Spawn", GetCrew(iPlr));
  }

  //Alle anderen Angreifer sind eliminiert: Verlieren
  var alive = false;
  for(var obj in FindObjects(Find_Func("IsClonk")))
  {
    if(GetPlayerTeam(GetOwner(obj)) == iDefender)
      continue;
    if(GetAlive(obj) && !GetEffect("IntAssaultWaitObject", Contained(obj))/* && (GetOwner(obj) != iPlr || GetMenu(obj) != MCSL)*/)
      alive = true;
  }

  if(alive)
    return;
  EliminatePlayer(iPlr);
}

public func GetRespawnPoint(int &iX, int &iY, int iTeam)
{
  var index = GASS_Spawn_Att;
  if(iTeam == iDefender)
    index = GASS_Spawn_Def;

  //Erstmal checken
  if(GetType(aTargets) != C4V_Array || !GetLength(aTargets[iDefender]))
    return;

  //Nächstes Ziel suchen
  var target;
  if(ObjectCount2(Find_InArray(aTargets[iDefender])))
    target = GetNextTarget();

  //Kein Ziel? 0 zurückgeben
  if(!aTargets[iDefender][target])
    return;

  iX = aSpawns[target][index][0][0];
  iY = aSpawns[target][index][0][1]-10;
  return true;
}

/* Event-Nachrichten */

public func TicketsLow(int iRemaining, int iTeam, bool fExclude)
{
  for(var i = 0; i < GetPlayerCount(); i++)
    if((!fExclude && GetPlayerTeam(GetPlayerByIndex(i)) == iTeam) || (fExclude && GetPlayerTeam(GetPlayerByIndex(i)) != iTeam))
      //Eventnachricht: Hinweis auf niedrige Ticketzahl
      EventInfo4K(GetPlayerByIndex(i) + 1, Format("$MsgTicketsLow$", iRemaining), SM03, 0, 0, 0, "Info_Alarm.ogg");
  return true;
}

public func NoTickets(int iTeam, bool fExclude)
{
  for(var i = 0; i < GetPlayerCount(); i++)
    if((!fExclude && GetPlayerTeam(GetPlayerByIndex(i)) == iTeam) || (fExclude && GetPlayerTeam(GetPlayerByIndex(i)) != iTeam))
      //Eventnachricht: Hinweis auf aufgebrauchte Tickets
      EventInfo4K(GetPlayerByIndex(i) + 1, Format("$MsgNoTickets$"), SM03, 0, 0, 0, "Info_Alarm.ogg");
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
  if(FindObject(CHOS)) return;

  //Komplett leeren
  ClearScoreboard();

  if(IsFulfilled())
    return;

  //Erst das nächste Ziel raussuchen
  var obj = aTargets[iDefender][GetNextTarget()];

  //Titelzeile
  SetScoreboardData(SBRD_Caption, SBRD_Caption, GetName());
  SetScoreboardData(SBRD_Caption, GASS_Icon, Format("{{%i}}", GetID()));
  SetScoreboardData(SBRD_Caption, GASS_Name, Format("<c %x>$Targets$</c>", GetTeamColor(iDefender)));
  SetScoreboardData(SBRD_Caption, GASS_Count, Format("<c %x>%d</c>", GetTeamColor(iDefender), ObjectCount2(Find_InArray(aTargets[iDefender]))), 0);

  //Zeile für das nächste Ziel
  if(obj)
  {
    //Ziel und zusammenhängende Ziele anzeigen
    var index = GetIndexOf(obj, aTargets[iDefender]);
    AddScoreboardTarget(obj, index);
    if(GetType(Connected[index]) == C4V_Array)
      for(var i; i < GetLength(Connected[index]); i++)
        if(Connected[index][i])
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
  if(GetTeamPlayerCount(iDefender) && GetActiveTeamCount() == 2)
  {
    if(team == iDefender)
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
  for(var i = 0; i < 20; i++)
    for(var j = 0; j < 3; j++)
      SetScoreboardData(i, j);
}

private func AddScoreboardTarget(object pTarget, int iRow)
{
  if(!pTarget)
    return;
  var effect = GetEffect("IntAssaultTarget", pTarget);
  var status = EffectVar(2, pTarget, effect);
  var color = GetTeamColor(iDefender);//InterpolateRGBa3(RGB(255, 255, 255), GetTeamColor(iDefender), percent, 100);

  SetScoreboardData(iRow+GASS_TargetRow, GASS_Icon, Format("{{%i}}", pTarget->~GetImitationID()));
  SetScoreboardData(iRow+GASS_TargetRow, GASS_Name, Format("<c %x>%s</c>", color, GetName(pTarget)));

  //Statusicon für Zielobjekt ermitteln
  var icon = SM16;	//Keine Aktivität
  if(status == 1)
    icon = SM17;	//Ladung wird plaziert
  else if(status == 2)
    icon = SM18;	//Ladung plaziert
  else if(status == 3)
    icon = SM16;	//Ladung wird entschärft

  SetScoreboardData(iRow+GASS_TargetRow, GASS_Count, Format("{{%i}}", icon), status + 50);
}

protected func FxIntGoalTimer()
{
  UpdateScoreboard();
}

/* Rundenauswertung */

local fulfilled;

private func IsFulfilled()
{
  if(FindObject(CHOS))
    return;
  if(fulfilled)
    return fulfilled;

  var won = false;

  //Zielobjekte zerstört: Angreifer gewinnen
  if(!ObjectCount2(Find_InArray(aTargets[iDefender])))
  {
    //Verteidiger eliminieren
    EliminateTeam(iDefender);

    //Nachricht über Gewinner
    Message("@$AttackersWon$");

    //Achievement-Ausgabe
    for(var i = 0, j; i < GetPlayerCount(); i++)
    {
      if(GetPlayerTeam(j = GetPlayerByIndex(i)) == iDefender)
        continue;
      //Achievement-Fortschritt (Stormtrooper)
      DoAchievementProgress(1, AC25, GetPlayerByIndex(i));
    }

    //Vorbei
    won = true;
  }

  //Keine Angreifer übrig: Verteidiger gewinnen
  else if(iAttacker != -1 && ((!GetTeamPlayerCount(iAttacker, true) && !TeamGetScore(iAttacker)) || !GetTeamPlayerCount(iAttacker)))
  {
    var fEliminate = true;

    //Check nach noch laufenden Bombentimer 
    for(var i = 0; i < GetLength(aTargets[iDefender]); i++)
      if(EffectVar(2, aTargets[iDefender][i], GetEffect("IntAssaultTarget", aTargets[iDefender][i])))
        fEliminate = false;

    if(fEliminate)
    {
      //Angreifer eliminieren 
      EliminateTeam(iAttacker);

      //Nachricht über Gewinner
      Message("@$DefendersWon$");

      //Vorbei
      won = true;
    }
  }

  //Keine Verteidiger übrig: Angreifer gewinnen
  else if(iAttacker != -1 && (!GetTeamPlayerCount(iDefender) || (!TeamGetScore(iDefender) && GetActiveTeamCount() < 2)))
  {
    //Verteidiger eliminieren 
    EliminateTeam(iDefender);

    //Nachricht über Gewinner
    Message("$AttackersWon$");

    //Vorbei
    won = true;
  }

  if(won)
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

public func GetTeamPlayerCount(int iTeam, bool fAliveOnly)
{
  var count = 0;
  for(var i = 0; i < GetPlayerCount(); i++)
    if(GetPlayerTeam(GetPlayerByIndex(i)) == iTeam && GetPlayerName(GetPlayerByIndex(i)))
    {
      var pCrew, j = 0, fAlive = true;
      if(fAliveOnly)
      {
        fAlive = false;
        while(pCrew = GetCrew(GetPlayerByIndex(i), j))
        {
          ++j;
          if(pCrew->~IsFakeDeath())  continue;

          var fWait = false, k = 0, effect;
          while(effect = GetEffect("WaitForJoin", this, k++))
            if(EffectVar(0, this, effect) == GetPlayerByIndex(i))
              fWait = true;

          if(fWait) continue;

          fAlive = true;
          break;
        }
      }
      if(fAlive)
        count++;
    }

  return count;
}

public func CreateTeamBorder(int iDefenderTeam, int iX, int iY, int iDir, bool fCreateBorderObject, int iTime)
{
  //Wartezeit übernehmen oder Standard setzen
  if(iTime <= 0) iTime = 30;
  iTime *= 35;

  if(fCreateBorderObject)
    for(var i = 0; i < GetPlayerCount(); i++)
      if(GetPlayerTeam(GetPlayerByIndex(i)) != iDefenderTeam)
        //Eventnachricht: Bereitmachen
        EventInfo4K(GetPlayerByIndex(i) + 1, "$MsgRegroup$", GASS, 0, 0, 0, "Info_Event.ogg");
      else
        //Eventnachricht: Zurückfallen
        EventInfo4K(GetPlayerByIndex(i) + 1, "$MsgFallBack$", GASS, 0, 0, 0, "Info_Event.ogg");

  var border = CreateObject(BRDR, iX, iY, NO_OWNER);
  border->Set(iDir, true, false, true, iDefenderTeam);

  var effect = AddEffect("TeamBorder", this, 1, Max(1, iTime*fCreateBorderObject), 0, GASS);
  EffectVar(0, this, effect) = iDefenderTeam;
  EffectVar(1, this, effect) = border;
}

public func FxTeamBorderStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  for(var i = 0; i < GetPlayerCount(); i++)
    if(GetPlayerTeam(GetPlayerByIndex(i)) != EffectVar(0, pTarget, iEffectNumber))
      //Eventnachricht: Vorrücken
      EventInfo4K(GetPlayerByIndex(i) + 1, "$MsgForward$", GASS, 0, 0, 0, "Info_Objective.ogg");
    else
      //Eventnachricht: Verteidigen
      EventInfo4K(GetPlayerByIndex(i) + 1, "$MsgDefend$", GASS, 0, 0, 0, "Info_Objective.ogg"); 

  //Grenze und Effekt entfernen
  if(EffectVar(1, pTarget, iEffectNumber))
  {
    EffectVar(1, pTarget, iEffectNumber)->Destruction();
    RemoveObject(EffectVar(1, pTarget, iEffectNumber));
  }
}