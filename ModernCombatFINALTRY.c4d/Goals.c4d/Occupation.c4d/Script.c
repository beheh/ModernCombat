/*-- Besitznahme --*/

#strict 2
#include TEAM

local iStartTickets;
local iWarningTickets;
local aTicket;
local aTeamTimers;

static const GOCC_Horizontal		= 1;
static const GOCC_Vertical		= 2;

private func StartTickets()		{return 30;}		//Standardticketzahl
public func IsConfigurable()		{return true;}
public func GoalExtraValue()		{return iStartTickets;}	//Spielzielinformationen an Scoreboard weitergeben
public func CustomSpawnSystem()		{return true;}
public func RejectChoosedClassInfo()	{return true;}
global func GetOccupationTimerSpeed()	{return 30;}		//Standard-Timer-Geschwindigkeit


/* Initialisierung */

protected func Initialize()
{
  aTicket = [];
  //Ticketzahl vorgeben
  iStartTickets = StartTickets();

  return _inherited();
}

public func Activate(iPlr)
{
  var szText = "";
  if(GetWinningTeam() > 0)
  {
    if(GetWinningTeam() == GetPlayerTeam(iPlr))
    {
      szText = "$MsgGoalFulfilled$";
    }
    else
    {
      szText = "$MsgGoalLost$";
    }
  }
  else
  {
    //Alle Teamtickets anzeigen
    for(var i = 0; i < GetTeamCount(); i++)
    {
      var iTeam = GetTeamByIndex(i);
      if(TeamAlive(iTeam)) szText = Format("%s<c %x>%s</c>: %d $Tickets$|", szText, GetTeamColor(iTeam), GetTeamName(iTeam), GetTickets(iTeam));
    }
  }
  return MessageWindow(szText ,iPlr);
}

public func ChooserFinished()
{
  ScheduleCall(this(),"InitScoreboard",1);

  //Ticketalarm an Ticketzahl anpassen
  if(iStartTickets < 4)
    iWarningTickets = 0;
  else
    iWarningTickets = Max(iStartTickets/4, 5);

  //Bei Klassenwahl Spawnsystem anpassen
  if(!FindObject(MCSL))
    ScheduleCall(0,"CreateSpawners",1);

  //Tickets verteilen
  for(var i = 0; i < GetTeamCount(); i++)
    DoTickets(GetTeamByIndex(i), iStartTickets);

  //Scoreboards und Spielzielhinweise erstellen
  for(var i = 0; i < GetPlayerCount(); i++)
  {
    DoScoreboardShow(1, GetPlayerByIndex(i) + 1);
    CreateObject(TK01, 0, 0, GetPlayerByIndex(i));
    Sound("Info_Round.ogg", true, 0, 100, GetPlayerByIndex(i) + 1);
  }

  Schedule("AddEffect(\"OccupationGame\", this, 100, 1, this, GOCC);", 1, 0, this);
}

public func CreateSpawners()
{
  for(var i = 0; i < GetPlayerCount(); i++)
  {
    CreateGOCCSpawner(GetCrew(GetPlayerByIndex(i)));
  }
}

/* Globale Funktionen */

public func GetDirection()
{
  var call = GameCall("OccupationDir");
  if(call) return call;
  return GOCC_Horizontal;
}

global func GetFlags()
{
  return FindObjects(Find_Func("IsFlagpole"));
}

global func GetSpawnableFlags()
{
  return FindObjects(Find_And(Find_Func("IsFlagpole"), Find_Func("IsSpawnable")));
}

global func CreateFlagpole(int iX, int iY, string szName, int iRange, int iSpeed)
{
  var point = CreateObject(OFPL, iX, iY, NO_OWNER);
  point->Set(szName, iRange, iSpeed);
  return point;
}

global func GetTeamFlags(int iTeam)
{
  var result = [];

  for(flag in GetFlags())
    if(flag->GetTeam() == iTeam)
      result[GetLength(result)] = flag;

  return result;
}

global func GetFlagCount(int iTeam, bool bCountBlankFlags)
{
  var count = 0;
  for(var flag in FindObjects(Find_Func("IsFlagpole")))
  {
    if(iTeam)
    {
      if(bCountBlankFlags)
      {
        if(flag->GetTeam())
          if(flag->GetTeam() != iTeam)
            continue;
      }
      else
      {
        if(flag->GetTeam() != iTeam)
          continue;
      }
    }
    count++;
  }
  return count;
}

global func GetEnemyFlagCount(int iTeam)
{
  var count = 0;

  if(!iTeam)
    return 0;

  for(var flag in FindObjects(Find_Func("IsFlagpole")))
  {
    if(HostileTeam(iTeam, flag->GetTeam()))
      count++;
  }

  return count;
}

/* HUD */

public func GetHUDInfo(int player, object hud)
{
  var str;

  if(GetTeamCount() > 0)
  {
    str = Format("<c %x>%d</c>",GetTeamColor(1),GetTickets(1));

    if(GetTeamCount() > 1)
      for(var i = 2; i <= GetTeamCount(); i++)
        Format("%s : <c %x>%d</c>",GetTeamName(i),GetTeamColor(i),GetTickets(i));
  }

  return str;
}

/* Konfiguration */

local iChoosedPlr;

public func ConfigMenu(object pCaller)
{
  iChoosedPlr = GetOwner(pCaller);
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
  var pClonk = GetCursor(iChoosedPlr);
  CreateMenu(GetID(),pClonk,0,0,0,0,1);

  AddMenuItem(" ", "OpenGoalMenu", GetID(), pClonk, iStartTickets, 0, " ");
  AddMenuItem("$MoreTickets$", "ChangeStartTickets", CHOS, pClonk, 0, +1, "$MoreTickets$",2,1);
  AddMenuItem("$LessTickets$", "ChangeStartTickets", CHOS, pClonk, 0, -1, "$LessTickets$",2,2);
  AddMenuItem("$Finished$", "ConfigFinished", CHOS, pClonk,0,0,"$Finished$",2,3);

  SelectMenuItem(iSelection, pClonk);
}

private func ChangeStartTickets(id dummy, int iChange)
{
  //Zu erreichenden Punktestand verändern (maximal 100 (normal) oder 32 (Liga) Tickets)
  if(!GetLeague())
    iStartTickets = BoundBy(iStartTickets+iChange,0,100);
  else
    iStartTickets = BoundBy(iStartTickets+iChange,10,32);
  //Sound
  Sound("Grab", 1,0,0,1);
  //Menü wieder öffnen
  var iSel = 1;
  if(iChange == -1) iSel = 2;
  OpenGoalMenu(0, iSel);
}

/* Scoreboard */

static const GOCC_IconColumn		= 1;
static const GOCC_FlagColumn		= 2;
static const GOCC_TimerColumn		= 3;
static const GOCC_ProgressColumn	= 4;

protected func InitScoreboard()
{  
  //Wird noch eingestellt
  if(FindObject(CHOS)) return;

  UpdateHUDs();

  //Titelzeile
  SetScoreboardData(SBRD_Caption, SBRD_Caption, Format("%s",GetName()), SBRD_Caption);

  //Spaltentitel
  SetScoreboardData(SBRD_Caption, GOCC_IconColumn, " ", SBRD_Caption);
  SetScoreboardData(SBRD_Caption, GOCC_FlagColumn, "{{SM21}}", SBRD_Caption);
  SetScoreboardData(SBRD_Caption, GOCC_TimerColumn, " ", SBRD_Caption);
  SetScoreboardData(SBRD_Caption, GOCC_ProgressColumn, "{{SM02}}", SBRD_Caption);

  UpdateScoreboard();

  init = true;
}

private func UpdateScoreboard()
{
  //Wird noch eingestellt
  if(FindObject(CHOS)) return;

  var i = 0;
  var data, base;

  //Entsprechend der Ausrichtung des Szenarios sortieren
  if(GetDirection() == GOCC_Horizontal)
    base = LandscapeWidth();
  if(GetDirection() == GOCC_Vertical)
    base = LandscapeHeight();

  //Flaggenposten
  for(var flag in GetFlags())
  {
    //Textfarbe ermitteln
    var color = GetTeamColor(flag->GetTeam()),
    prog = flag->GetProcess();

    color = RGBa(Interpolate2(255, GetRGBaValue(color, 1), prog, 100),
    Interpolate2(255, GetRGBaValue(color, 2), prog, 100), 
    Interpolate2(255, GetRGBaValue(color, 3), prog, 100));

    //Entsprechend der Ausrichtung des Szenarios sortieren
    if(GetDirection() == GOCC_Horizontal)
      data = GetX(flag);
    if(GetDirection() == GOCC_Vertical)
      data = GetY(flag);

    SetScoreboardData(i, GOCC_IconColumn, Format(" "));
    SetScoreboardData(i, GOCC_FlagColumn, Format("<c %x>%s</c>", color, GetName(flag)), data);
    SetScoreboardData(i, GOCC_TimerColumn, Format(" "));
    SetScoreboardData(i, GOCC_ProgressColumn, Format("<c %x>%d%</c>", color, flag->GetProcess()));
    i++;
  }

  if(i != 1)
  {
    SetScoreboardData(i, GOCC_IconColumn, "");
    SetScoreboardData(i, GOCC_FlagColumn, "{{SM26}}", base+1);
    SetScoreboardData(i, GOCC_TimerColumn, "{{SM27}}");
    SetScoreboardData(i, GOCC_ProgressColumn, "{{SM03}}");
    i++;
  }

  //Tickets
  for(var j = 0; j < GetTeamCount(); j++)
  {
    var iTeam = GetTeamByIndex(j);
    if(TeamAlive(iTeam))
    {
      if(GetIndexOf(iTeam, GetHighestTeams()) != -1)
        SetScoreboardData(i, GOCC_IconColumn, "{{SM14}}");
      else
        SetScoreboardData(i, GOCC_IconColumn, " ");
      SetScoreboardData(i, GOCC_FlagColumn, Format("<c %x>%s</c>", GetTeamColor(iTeam), GetTeamName(iTeam)), base+2+GetTickets(iTeam));
      SetScoreboardData(i, GOCC_TimerColumn, Format("<c 777777>%d</c>", GetTeamTimer(iTeam)));
      SetScoreboardData(i, GOCC_ProgressColumn, Format("%d", GetTickets(iTeam)));
    }
    else
    {
      SetScoreboardData(i, GOCC_IconColumn, 0);
      SetScoreboardData(i, GOCC_FlagColumn, 0);
      SetScoreboardData(i, GOCC_TimerColumn, 0);
      SetScoreboardData(i, GOCC_ProgressColumn, 0);
    }
    i++;
  }

  SortScoreboard(GOCC_FlagColumn);
}

/* GameCalls */

public func FlagAttacked(object pFlag, int iTeam)
{
  for(var i = 0; i < GetPlayerCount(); i++)
  {
    if(GetPlayerTeam(GetPlayerByIndex(i)) == iTeam)
    {
      DoScoreboardShow(1, GetPlayerByIndex(i)+1);
      Schedule(Format("DoScoreboardShow(-1, %d)", GetPlayerByIndex(i)+1), 150);
    }
  }
  UpdateScoreboard();
}

public func FlagLost(object pFlag, int iTeam, int iTeamAttacker, array pAttackers)
{
  //Punkte bei Belohnungssystem
  var i = 0;
  for(var pClonk in pAttackers)
  {
    if(!i)
    {
      //Punkte bei Belohnungssystem (Flaggenpostenneutralisierung)
      DoPlayerPoints(BonusPoints("OPNeutralize"), RWDS_TeamPoints, GetOwner(pClonk), pClonk, IC13);
      //Geldbonus: 10 Clunker
      DoWealth(GetOwner(pClonk), 10);
    }
    else
    {
      //Punkte bei Belohnungssystem (Hilfe bei Flaggenpostenneutralisierung)
      DoPlayerPoints(BonusPoints("OPAssist"), RWDS_TeamPoints, GetOwner(pClonk), pClonk, IC10);
      //Geldbonus: 5 Clunker
      DoWealth(GetOwner(pClonk), 5);
    }
    i++;
  }
  if(iTeam)
  {
    for(var i = 0; i < GetPlayerCount(); i++)
    {
      if(GetPlayerTeam(GetPlayerByIndex(i)) == iTeam)
      {
        //Eventnachricht: Flaggenposten verloren
        EventInfo4K(GetPlayerByIndex(i)+1, Format("$MsgFlagLost$", GetName(pFlag), GetTeamColor(iTeamAttacker), GetTeamName(iTeamAttacker)), SM23, 0, GetTeamColor(iTeamAttacker), 0, "Info_Event.ogg");
      }
    }
  }
  UpdateScoreboard();
}

public func FlagCaptured(object pFlag, int iTeam, array pAttackers, bool fRegained)
{
  //Punkte bei Belohnungssystem
  if(fRegained)
  {
    for(var pClonk in pAttackers)
    {
      //Punkte bei Belohnungssystem (Flaggenpostenrückeroberung)
      DoPlayerPoints(BonusPoints("OPDefend"), RWDS_TeamPoints, GetOwner(pClonk), pClonk, IC12);
      //Geldbonus: 5 Clunker
      DoWealth(GetOwner(pClonk), 5);
    }
  }
  else
  {
    var i = 0;
    for(var pClonk in pAttackers)
    {
      if(!i)
      {
        //Punkte bei Belohnungssystem (Flaggenposteneroberung)
        DoPlayerPoints(BonusPoints("OPConquer"), RWDS_TeamPoints, GetOwner(pClonk), pClonk, IC10);
        //Achievement-Fortschritt (Flagship)
        DoAchievementProgress(1, AC07, GetOwner(pClonk));
        //Geldbonus: 15 Clunker
        DoWealth(GetOwner(pClonk), 15);
      }
      else
      {
        //Punkte bei Belohnungssystem (Hilfe bei Flaggenposteneroberung)
        DoPlayerPoints(BonusPoints("OPAssist"), RWDS_TeamPoints, GetOwner(pClonk), pClonk, IC11);
        //Achievement-Fortschritt (Flagship)
        DoAchievementProgress(1, AC07, GetOwner(pClonk));
        //Geldbonus: 10 Clunker
        DoWealth(GetOwner(pClonk), 10);
      }
      i++;
    }
  }
  //Eventnachricht: Flaggenposten erobert
  EventInfo4K(0, Format("$MsgCaptured$", GetTeamColor(iTeam), GetTeamName(iTeam), GetName(pFlag)), SM22, 0, GetTeamColor(iTeam), 0, "Info_Objective.ogg");
  UpdateScoreboard();
}

/* Tickets */

public func TicketChange(int iTeam, int iChange)
{
  DoTickets(iChange);
  UpdateScoreboard(iTeam);
}

public func GetTeamTimer(int iTeam)
{
  if(!aTeamTimers)
    return;

  return (aTeamTimers[iTeam-1] / 100);
}

public func GetHighestTeams()
{
  var result = [];
  var highest = GetMaxArrayVal(aTicket, false, true);

  for(var i = 0; i < GetLength(aTicket); i++)
    if(aTicket[i] == highest)
      result[GetLength(result)] = i + 1;

  return result;
}

public func GetTickets(int iTeam)
{
  return aTicket[iTeam-1];
}

public func SetTickets(int iTeam, int iTickets)
{
  aTicket[iTeam-1] = Max(iTickets, 0);
  UpdateScoreboard(iTeam);
  return true;
}

public func DoTickets(int iTeam, int iChange, bool fNoWarn)
{
  SetTickets(iTeam, GetTickets(iTeam) + iChange);
  if(!fNoWarn)
  {
    if(iWarningTickets != 0 && iWarningTickets == aTicket[iTeam-1])
    {
      Schedule(Format("GameCallEx(\"TicketsLow\", %d, %d)", aTicket[iTeam-1], iTeam), 1);
    }
    if(aTicket[iTeam-1] == 0)
    {
      Schedule(Format("GameCallEx(\"NoTickets\", %d)", iTeam), 1);
    }
  }
  return true;
}

/* Eventnachrichten */

public func TicketsLow(int iRemaining, int iTeam)
{
  for(var i = 0; i < GetPlayerCount(); i++)
  {
    if(GetPlayerTeam(GetPlayerByIndex(i)) == iTeam)
    {
      //Eventnachricht: Warnung vor niedrigen Tickets
      EventInfo4K(GetPlayerByIndex(i)+1,Format("$MsgTicketsLow$",iRemaining),SM03,0,0,0,"Info_Event.ogg");
    }
  }
  return true;
}

public func NoTickets(int iTeam)
{
  for(var i = 0; i < GetPlayerCount(); i++)
  {
    if(GetPlayerTeam(GetPlayerByIndex(i)) == iTeam)
    {
      //Eventnachricht: Hinweis auf aufgebrauchte Tickets
      EventInfo4K(GetPlayerByIndex(i)+1,Format("$MsgNoTickets$"),SM03,0,0,0,"Info_Alarm.ogg");
    }
  }
  return true;
}

/* Spiellogik */

local fFulfilled;

public func IsFulfilled()
{
  if(ObjectCount(CHOS)) return false;

  if(fFulfilled) return true;

  var iWinningTeam = GetWinningTeam();

  //Existiert nurnoch ein Team, gewinnt es
  if(iWinningTeam > 0)
  {
    //Teameliminierung
    for(var i = 0; i < GetTeamCount(); i++)
      if(GetTeamByIndex(i) != iWinningTeam) EliminateTeam(GetTeamByIndex(i));

    if(LosersAlive(iWinningTeam)) return;

    //Spielende planen
    Schedule("GameOver()",150);

    //Auswertung
    RewardEvaluation();

    //Nachricht über Gewinner
    Message("@$TeamHasWon$",0 , GetTeamColor(iWinningTeam), GetTeamName(iWinningTeam));

    //Sounds
    Sound("Cheer.ogg", true);

    return fFulfilled = true;
  }

  //Unentschieden
  if(iWinningTeam == -1)
  {
    for(var i = 0; i < GetTeamCount(); i++)
      EliminateTeam(GetTeamByIndex(i));
    if(LosersAlive(0))
      return false;

    //Spielende planen
    Schedule("GameOver()",150);

    //Auswertung
    RewardEvaluation();

    return fFulfilled = true;
  }
}

public func FxOccupationGameStart(object pTarget, int iEffectNumber)
{
  aTeamTimers = [];

  for(var i = 0; i < GetTeamCount(); i++)
    aTeamTimers[i] = 10000;

  if(!GetLength(GetFlags()))
  {
    fFulfilled = true;
    Log("ERROR: No valuable flags found! Aborting!");
    return -1;
  }

  return 1;
}

public func FxOccupationGameTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  for(var i = 0; i < GetTeamCount(); i++)
  {
    var decrease = GetFlagCount(i+1) - GetEnemyFlagCount(i+1);
    if(decrease < 0)
    {
      decrease *= GetOccupationTimerSpeed();
      decrease = decrease / GetLength(GetFlags());
      aTeamTimers[i] += decrease;
      if(aTeamTimers[i] <= 0)
      {
        aTicket[i] -= 1;
        aTeamTimers[i] = 10000;
        UpdateScoreboard();
      }
    }
  }

  return;
}

private func TeamAlive(int iTeam)
{
  var alive = [], poles = [];
  var i = iTeam;
  
  //Regelwähler vorhanden: Teamanzahl zurückgeben
  if(FindObject(CHOS))
  {
    return GetTeamPlayerCount(iTeam);
  }
  //Ticketzahl 0: Team eliminieren
  if(aTicket[i-1] <= 0)
  {
    EliminateTeam(i);
    return false;
  }

  //Zwei Siegbedingungen: Alle Spieler eines Teams eliminiert und alle Flaggen des Teams eingenommen
  poles[i] = 0;
  for(var pole in FindObjects(Find_ID(OFPL)))
    if(pole->GetTeam() == i && pole->IsFullyCaptured())
      poles[i]++;
  //Keine Flaggen?
  if(poles[i] == 0)
  {
    alive[i] = 0;
    for(var clonk in FindObjects(Find_OCF(OCF_Alive), Find_OCF(OCF_CrewMember)))
      if(GetPlayerTeam(GetOwner(clonk)) == i)
      {
        if(Contained(clonk))
        {
          if(IsFakeDeath(clonk) || (GetID(Contained(clonk)) == OSPW && GetAction(Contained(clonk)) != "Counter") || GetID(Contained(clonk)) == TIM1 || GetID(Contained(clonk)) == TIM2)
            continue;
          alive[i]++;
          break;
        }
        alive[i]++;
      }
  }
  else
    //Keine Spieler in einem Team?
    for(var j = 0; j < GetPlayerCount(); j++)
      if(GetPlayerTeam(GetPlayerByIndex(j)) == i)
        alive[i]++;
  if(alive[i] > 0) return true;
  return false;
}

private func GetWinningTeam()
{
  var alive = [];

  //Zwei Siegbedingungen: Alle Spieler eines Teams eliminiert und alle Flaggen des Teams eingenommen
  for(var i = 0; i < GetTeamCount(); i++)
  {
    alive[GetTeamByIndex(i)] = TeamAlive(GetTeamByIndex(i));
  }

  //Wie viele Teams existent?
  var teamA = 0;
  for(var i = 0; i < GetLength(alive); i++)
    if(alive[i])
    {
      if(teamA) //Zwei oder mehr Teams lebendig?
        return 0;
      else
        teamA = i; //Ein lebendiges Team gefunden?
    }
  if(teamA < 1) //Falls keine Teams lebendig (wieso auch immer)
    return 0;
  else
    return teamA; //Ansonsten das einzig lebendige Team
}

private func EliminateTeam(int iTeam)
{
  for(var i = 0; i < GetPlayerCount() ; i++)
    if(GetPlayerTeam(GetPlayerByIndex(i)) == iTeam)
    {
      EliminatePlayer(GetPlayerByIndex(i));
    }
}

private func LosersAlive(int iTeam)
{
  for(var i = 0; i < GetPlayerCount() ; i++)
    if(GetPlayerTeam(GetPlayerByIndex(i)) != iTeam)
      return true;

  return false;
}

/* Alarmleuchtensteuerung */

public func FxIntWarnStart(object pTarget, int iEffectNumber, int iTemp, object pPoint)
{
  EffectVar(0, pTarget, iEffectNumber) = pPoint;
  return 1;
}

public func FxIntWarnTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  if(!pTarget)
   return -1;

  var pPoint = EffectVar(0,pTarget,iEffectNumber);
  var iLast  = EffectVar(1,pTarget,iEffectNumber);
  var iNow;
  if(pPoint->GetTrend() || pPoint->GetAttacker())
   iNow = true;
  
  if(iNow != iLast)
  {
     if((pPoint->GetTrend() >= 0) || !pPoint->GetAttacker())
      pTarget->TurnOff();
    else
      pTarget->TurnOn();

    EffectVar(1, pTarget, iEffectNumber) = iNow;
  }

  return;
}

global func AddGOCCWarnEffect(object pTarget, object pPoint)
{
  AddEffect("IntWarn", pTarget, 1, 35, 0, GOCC, pPoint);
  return true;
}

/* Respawn */

protected func InitializePlayer(int iPlr, int iX, int iY, object pBase, int iTeam)
{
  if(!init) return;

  if(IsFulfilled()) return EliminatePlayer(iPlr);

  //Verfeindung setzen
  Hostility(iPlr);
  
  RelaunchPlayer(iPlr, GetCrew(iPlr), 0, iTeam, true);
}

protected func RelaunchPlayer(int iPlr, object pCrew, int iMurdererPlr, int iTeam, no_relaunch)
{
  if(iPlr == -1 || !GetPlayerName(iPlr)) return;

  //Schauen wir mal ob noch geht
  IsFulfilled();

  //Kein Team?
  if(!iTeam) iTeam = GetPlayerTeam(iPlr);

  //Kills
  aDeath[iPlr]++;
  //Tode
  if(iMurdererPlr != -1 && GetPlayerTeam(iPlr) != GetPlayerTeam(iMurdererPlr))
  {
    aKill[iMurdererPlr]++;
  }

  //Geld verteilen
  Money(iPlr, pCrew, iMurdererPlr);
  
  if(GetWinningTeam() > 0 && GetWinningTeam() != iTeam)
  {
    if(GetCursor(iPlr)) SetPlrViewRange(0, GetCursor(iPlr));
    return;
  }

  DoTickets(iTeam,-1);

  if(!FindObject(CHOS) && !FindObject(MCSL)) //Regelwähler oder Klassenwahl?
    CreateGOCCSpawner(pCrew);

  //Flagge anfokussieren
  DoFlag(iTeam, iPlr);
}

public func OnClassSelection(object pClonk, int iClass)
{
  if(FindObject(CHOS))
    return;

  CreateGOCCSpawner(pClonk, iClass);
}

public func DoFlag(int iTeam, int iPlr)
{
  var pCrew = GetCrew(iPlr);
  if(!pCrew) return Schedule(Format("DoFlag(%d, %d)", iTeam, iPlr), 1);
  var pObject = Contained(pCrew);

  if(!ShowFlagpole(GetBestFlag(iTeam), pCrew, pObject))
  {
    SetPlrViewRange(0, pCrew);
  }

  return true;
}

private func RemovePlayer(int iPlr)
{
  if(iPlr == -1) return;

  //Auswertungsdialog
  DoEvaluation(iPlr);

  UpdateHUDs();
  aDeath[iPlr] = 0;
  aKill[iPlr] = 0;
}

/* Ungenutzte Funktionen */

private func InitMultiplayerTeam(int iTeam)	{}
private func RemoveMultiplayerTeam(int iTeam)	{}
private func InitSingleplayerTeam(int iPlr)	{}
private func RemoveSingleplayerTeam(int iPlr)	{}
private func InitPlayer(int iPlr)		{}
private func RemoveScoreboardPlayer(int iPlr)	{}
public func WinScoreChange(int iNewScore)	{}
private func SortTeamScoreboard()		{}