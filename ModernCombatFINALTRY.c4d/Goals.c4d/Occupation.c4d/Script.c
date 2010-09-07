/*-- Besitznahme --*/

#strict 2
#include TEAM

static const GOCC_Horizontal = 1;
static const GOCC_Vertical = 2;
static const GOCC_FlagColumn = 1;
static const GOCC_ProgressColumn = 2;

private func StartTickets()		{return 15;}	//Standardticketzahl


/* Globale Funktionen */

global func GetGOCCDirection()
{
  var call = GameCall("OccupationDir");
  if(call) return call;
  return GOCC_Horizontal;
}

global func GetGOCCFlags()
{
  var list = [];
  var iPos = 0;
  for(var pFlag in FindObjects(Find_Func("IsFlagpole")))
  {
    if(GetGOCCDirection() == GOCC_Horizontal)
      iPos = GetX(pFlag);
    if(GetGOCCDirection() == GOCC_Vertical)
      iPos = GetY(pFlag);
    list[iPos] = pFlag;
  }
  var pFlags = [];
  for(var pFlag in list)
  {
    if(pFlag) pFlags[GetLength(pFlags)] = pFlag;
  }
  return pFlags;
}

global func CreateFlagpole(int iX, int iY, string szName, int iRange, int iSpeed)
{
  var point = CreateObject(OFPL, iX, iY, NO_OWNER);
  point->Set(szName, iRange, iSpeed);
  return point;
}

/* Flaggen */

public func GetFlagCount(int iTeam, bool bCountBlankFlags)
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

/* Allgemein */

local iStartTickets;
local iWarningTickets;
local aTicket;

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
  if(iStartTickets < 4)
  {
    iWarningTickets = 0;
  }
  else
  {
    iWarningTickets = Max(iStartTickets/4, 5);
  }
  //Klassenwahl?
  if(!FindObject(MCSL))
    ScheduleCall(0,"CreateSpawners",1);

  for(var i = 0; i < GetTeamCount(); i++)
    DoTickets(GetTeamByIndex(i), iStartTickets);

  for(var i = 0; i < GetPlayerCount(); i++)
  {
    DoScoreboardShow(1, GetPlayerByIndex(i)+1);
  }
}

public func CreateSpawners()
{
  for(var i = 0; i < GetPlayerCount(); i++)
  {
    CreateGOCCSpawner(GetCrew(GetPlayerByIndex(i)));
  }
}

/* HUD */

public func GetHUDInfo(int player, object hud)
{
  var team = GetPlayerTeam(player);
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
  AddMenuItem("$MoreTickets$", "ChangeStartTickets", CHOS, pClonk, 0, +1, "$MoreTickets$",2,1);
  AddMenuItem("$LessTickets$", "ChangeStartTickets", CHOS, pClonk, 0, -1, "$LessTickets$",2,2);
  AddMenuItem("$Finished$", "ConfigFinished", CHOS, pClonk,0,0,"$Finished$",2,3);

  SelectMenuItem(iSelection, pClonk);
}

private func ChangeStartTickets(id dummy, int iChange)
{
  //Stand verändern (maximal 100 Tickets)
  iStartTickets = BoundBy(iStartTickets+iChange,0,100);
  //Sound
  Sound("Grab", 1,0,0,1);
  //Menü wieder öffnen
  var iSel = 1;
  if(iChange == -1) iSel = 2;
  OpenGoalMenu(0, iSel);
}

/* Scoreboard */

protected func InitScoreboard()
{  
  if(FindObject(CHOS)) return;
  UpdateHUDs();

  //Überschriften
  SetScoreboardData(SBRD_Caption, SBRD_Caption, Format("%s",GetName()), SBRD_Caption);
  SetScoreboardData(SBRD_Caption, GOCC_FlagColumn, "{{OSPW}}", SBRD_Caption);
  SetScoreboardData(SBRD_Caption, GOCC_ProgressColumn, "{{AFTN}}", SBRD_Caption);

  UpdateScoreboard();

  init = true;
}

private func UpdateScoreboard()
{
  if(FindObject(CHOS)) return;

  //Zeile
  var i = 0;
  var data;
  //Flaggen
  for(var flag in GetGOCCFlags())
  {
    //Erst mal weiß annehmen
    var color = GetTeamColor(flag->GetTeam()),
	  prog = flag->GetProcess();
		color = RGBa(Interpolate2(255, GetRGBaValue(color, 1), prog, 100), Interpolate2(255, GetRGBaValue(color, 2), prog, 100), Interpolate2(255, GetRGBaValue(color, 3), prog, 100));
    if(GetGOCCDirection() == GOCC_Horizontal)
      data = GetX(flag);
    if(GetGOCCDirection() == GOCC_Vertical)
      data = GetY(flag);
    SetScoreboardData(i, GOCC_FlagColumn, Format("<c %x>%s</c>", color, GetName(flag)), data);
    SetScoreboardData(i, GOCC_ProgressColumn, Format("<c %x>%d%</c>", color, flag->GetProcess()), flag->GetProcess());
    i++;
  }
  var base;
  if(GetGOCCDirection() == GOCC_Horizontal)
    base = LandscapeWidth();
  if(GetGOCCDirection() == GOCC_Vertical)
    base = LandscapeHeight();
  if(i != 1)
  {
    SetScoreboardData(i, GOCC_FlagColumn, " ", base+1);
    SetScoreboardData(i, GOCC_ProgressColumn, " ", base+1);
    i++;
  }

  //Tickets
  for(var j = 0; j < GetTeamCount(); j++)
  {
    var iTeam = GetTeamByIndex(j);
    if(TeamAlive(iTeam))
    {
      SetScoreboardData(i, GOCC_FlagColumn, Format("<c %x>%s</c>", GetTeamColor(iTeam), GetTeamName(iTeam)), base+2+GetFlagCount()-GetFlagCount(iTeam));
      SetScoreboardData(i, GOCC_ProgressColumn, Format("%d {{TIKT}}", GetTickets(iTeam)), base+2+iStartTickets-GetTickets(iTeam));
    }
    else
    {
      SetScoreboardData(i, GOCC_FlagColumn, 0);
      SetScoreboardData(i, GOCC_ProgressColumn, 0);
    }
    i++;
  }
  SortScoreboard(1);
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
      //Neutralisierung einer feindlichen Flagge
      DoPlayerPoints(BonusPoints("OPNeutralize"), RWDS_TeamPoints, GetOwner(pClonk), pClonk, IC13);
      DoWealth(GetOwner(pClonk), 20);
    }
    else
    {
      //Hilfe bei Flaggeneroberung
      DoPlayerPoints(BonusPoints("OPAssist"), RWDS_TeamPoints, GetOwner(pClonk), pClonk, IC10);
      DoWealth(GetOwner(pClonk), 10);
    }
    i++;
  }
  if(iTeam)
  {
    for(var i = 0; i < GetPlayerCount(); i++)
    {
      if(GetPlayerTeam(GetPlayerByIndex(i)) == iTeam)
      {
        //Nachricht über Flaggenverlust
        EventInfo4K(GetPlayerByIndex(i)+1, Format("$MsgFlagLost$", GetName(pFlag), GetTeamColor(iTeamAttacker), GetTeamName(iTeamAttacker)), OFLG, 0, GetTeamColor(iTeamAttacker), 0, "Info.ogg");
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
      //Rückeroberung einer attackierte Flagge
      DoPlayerPoints(BonusPoints("OPDefend"), RWDS_TeamPoints, GetOwner(pClonk), pClonk, IC12);
      DoWealth(GetOwner(pClonk), 15);
    }
  }
  else
  {
    var i = 0;
    for(var pClonk in pAttackers)
    {
      if(!i)
      {
        //Flaggeneroberung
        DoPlayerPoints(BonusPoints("OPConquer"), RWDS_TeamPoints, GetOwner(pClonk), pClonk, IC10);
        DoAchievementProgress(1, AC07, GetOwner(pClonk));
        DoWealth(GetOwner(pClonk), 30);
      }
      else
      {
        //Hilfe bei Flaggeneroberung
        DoPlayerPoints(BonusPoints("OPAssist"), RWDS_TeamPoints, GetOwner(pClonk), pClonk, IC13);
        DoAchievementProgress(1, AC07, GetOwner(pClonk));
        DoWealth(GetOwner(pClonk), 20);
      }
      i++;
    }
  }
  //Nachricht über Flaggeneroberung
  EventInfo4K(0, Format("$MsgCaptured$", GetTeamColor(iTeam), GetTeamName(iTeam), GetName(pFlag)), OFLG, 0, GetTeamColor(iTeam), 0, "Info.ogg");
  UpdateScoreboard();
}

/* Tickets */

public func TicketChange(int iTeam, int iChange)
{
  DoTickets(iChange);
  UpdateScoreboard(iTeam);
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
      Schedule(Format("GameCallEx(\"NoTickets\", %d, %d)", iTeam), 1);
    }
  }
  return true;
}

/* EventInfos */

public func TicketsLow(int iRemaining, int iTeam)
{
  for(var i = 0; i < GetPlayerCount(); i++)
  {
    if(GetPlayerTeam(GetPlayerByIndex(i)) == iTeam)
    {
      //Nachricht über Tickettiefstand
      EventInfo4K(GetPlayerByIndex(i)+1,Format("$MsgTicketsLow$",iRemaining),TIKT,0,0,0,"Alarm.ogg");
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
      //Nachricht über Verlust aller Tickets
      EventInfo4K(GetPlayerByIndex(i)+1,Format("$MsgNoTickets$"),TIKT,0,0,0,"Alarm.ogg");
    }
  }
  return true;
}

/* Spiellogik */

public func IsFulfilled()
{
  if(ObjectCount(CHOS)) return false;

  var iWinningTeam = GetWinningTeam();

  //Existiert nurnoch ein Team, gewinnt es
  if(iWinningTeam > 0)
  {
    //Teameliminierung
    for(var i = 0; i < GetTeamCount(); i++)
      if(GetTeamByIndex(i) != iWinningTeam) EliminateTeam(GetTeamByIndex(i));

    if(LosersAlive(iWinningTeam)) return;

    //Auswertung
    Evaluation();

    //Nachricht über Gewinner
    Message("@$WinMsg$",0 , GetTeamColor(iWinningTeam), GetTeamName(iWinningTeam));

    //Sounds
    Sound("Cheer.ogg", true);

    //Spielende planen
    Schedule("GameOver()",150);
    return true;
  }

  //Unentschieden
  if(iWinningTeam == -1)
  {
    for(var i = 0; i < GetTeamCount(); i++)
      EliminateTeam(GetTeamByIndex(i));
    if(LosersAlive(0))
      return false;

    //Auswertung
    Evaluation();

    //Spielende planen
    Schedule("GameOver()",150);
    return true;
  }
}

private func TeamAlive(int iTeam)
{
  var alive = 0, poles = 0, fakedeath = 0;
  
  //Regelwähler-Hack
  if(FindObject(CHOS))
  {
    return GetTeamPlayerCount(iTeam);
  }

  //Zwei Siegbedingungen: Alle Spieler eines Teams eliminiert und alle Flaggen des Teams eingenommen
  poles = 0;
  for(var pole in FindObjects(Find_ID(OFPL)))
    if(pole->GetTeam() == iTeam && pole->IsFullyCaptured())
      poles++;
  //Keine Flaggen?
  if(poles == 0)
  {
    alive = 0;
    for(var clonk in FindObjects(Find_OCF(OCF_Alive), Find_OCF(OCF_CrewMember)))
      if(GetPlayerTeam(GetOwner(clonk)) == iTeam)
      {
      	if(IsFakeDeath(clonk)) {
      		fakedeath++;
      	}
        if(Contained(clonk))
        {
          if((GetID(Contained(clonk)) == OSPW && GetAction(Contained(clonk)) != "Counter") || GetID(Contained(clonk)) == TIM1 || GetID(Contained(clonk)) == TIM2)
            continue;
        }
        alive++;
      }
  }
  else {
    //Keine Spieler in einem Team?
		for(var j = 0; j < GetPlayerCount(); j++) {
			if(GetPlayerTeam(GetPlayerByIndex(j)) == iTeam) {
				alive++;
			}
		}
	}
  if(alive > 0 && alive > fakedeath) return true;
  return false;
}

private func GetWinningTeam()
{
  var alive = [], poles = [];
  var add, id;

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

private func Evaluation()
{
  if(!evaluation)
  for(var j=0 ; j < GetPlayerCount() ; j++)
  {
    var plr = GetPlayerByIndex(j);
    AddEvaluationData(Format("{{PSTL}}$Kills$: %d", aKill[plr]), GetPlayerID(plr));
    AddEvaluationData(Format("{{KAMB}}$Death$: %d", aDeath[plr]), GetPlayerID(plr));
  }

  evaluation = true;
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
  
  //Tickets
  if(GetTickets(iTeam) <= 0)
  {
    if(GetCursor(iPlr)) SetPlrViewRange(0, GetCursor(iPlr));
    GameCall("ForceObservation",iPlr);
    return;
  }
  if(GetWinningTeam() > 0 && GetWinningTeam() != iTeam)
  {
    if(GetCursor(iPlr)) SetPlrViewRange(0, GetCursor(iPlr));
    return;
  }

  //Check ob noch am Leben
  if(!TeamAlive(iTeam))
  {
    return EliminatePlayer(iPlr);
  }  

  DoTickets(iTeam,-1);

  if(!FindObject(CHOS) && !FindObject(MCSL)) //Regelwähler oder Klassenwahl?
    CreateGOCCSpawner(pCrew);
    
  //Flagge anfokussieren
  DoFlag(iTeam, iPlr);
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


public func OnClassSelection(object pClonk)
{
  if(FindObject(CHOS))
    return;

  CreateGOCCSpawner(pClonk);
}
