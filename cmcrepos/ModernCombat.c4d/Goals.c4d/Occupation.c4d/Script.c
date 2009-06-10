/*-- Besitznahme --*/

#strict 2
#include TEAM


/* Globale Funktionen */

global func DoTickets(int iTeam, int iChange)
{
  var f = FindObject(GOCC);
  if(f)
    f->TicketChange(iTeam, iChange);
}

global func CreateFlagpole(int iX, int iY, string szName, int iRange, int iSpeed)
{
  var point = CreateObject(OFPL, iX, iY, NO_OWNER);
  point->Set(szName,iRange,iSpeed);
  return point;
}

/* Allgemein */

local iStartTickets;
local aTicket;

protected func Initialize()
{
  aTicket = [];
  iStartTickets = 10;

  return _inherited();
}

public func Activate(iPlr)
{
  var text = "";
  for(var i = 1; i <= GetTeamCount();i++)
    text = Format("%s<c %x>%s</c>: %d $Tickets$",text,GetTeamColor(i),GetTeamName(i),GetTickets(i));
  
  return MessageWindow(text,iPlr);
}

private func GetFlagCount(int iTeam, bool bCountBlankFlags)
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

public func ChooserFinished()
{
  ScheduleCall(this(),"InitScoreboard",1);
  
  if(!FindObject(MCSL))//Klassenwahl?
    ScheduleCall(0,"CreateSpawners",1);
}

private func CreateSpawners()
{
  for(var i = 0; i < GetPlayerCount(); i++)
    for(var j = 0, pCrew ; pCrew = GetCrew(GetPlayerByIndex(i), j) ; j++)
      CreateGOCCSpawner(pCrew);
}

public func GetHUDInfo(int player, object hud)
{
	var team = GetPlayerTeam(player);
  var str;
  
  if(GetTeamCount() > 0)
  {
    str = Format("<c %x>%d</c>",GetTeamColor(1),GetTickets(1));
    
    if(GetTeamCount() > 1)
      for(var i = 2; i <= GetTeamCount(); i++)
        Format("%s : <c %x>%d</c>",GetTeamColor(i),GetTickets(i));
  }
	
	return str;
}


/* Konfiguration */
public func IsConfigurable() { return true; }

public func ConfigMenu(object pCaller)
{
  OpenGoalMenu();
  return 1;
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
  // Stand verändern
  iStartTickets = BoundBy(iStartTickets+iChange,0,100);//100 ist Maximum.
  // Geräusch
  Sound("Grab", 1,0,0,1);
  // Menü wieder öffnen
  var iSel = 1;
  if(iChange == -1) iSel = 2;
  OpenGoalMenu(0, iSel);
}

private func ConfigFinished()
{
  var chos = FindObject(CHOS);
  if(chos)
    chos->OpenMenu();
}


/* Scoreboard */
private func InitScoreboard()
{  
  SetScoreboardData(SBRD_Caption, TEAM_TeamColumn, "{{KILC}}", SBRD_Caption);
  SetScoreboardData(SBRD_Caption, TEAM_PlayerColumn, "{{PCMK}}", SBRD_Caption);
  SetScoreboardData(SBRD_Caption, TEAM_GoalColumn, "{{TIKT}}", SBRD_Caption);
  SetScoreboardData(SBRD_Caption, TEAM_KillColumn, "{{PIWP}}", SBRD_Caption);
  SetScoreboardData(SBRD_Caption, TEAM_DeathColumn, "{{KAMB}}", SBRD_Caption);

  var teams = [];
  var team;

  //Daten
  //erstmal rausfinden welche Teams wir anzeigen
  for(var i = 0 ; i < GetPlayerCount() ; i++)
  {
    if(!GetPlayerName(GetPlayerByIndex(i)))
	    continue;

	  var done=0, plrteam = GetPlayerTeam(GetPlayerByIndex(i));
	  if(plrteam < 1) continue;

	  for(var team in teams)
	    if(team == plrteam)
	      done++;
        
	  if(!done)
	  {
	    teams[GetLength(teams)] = plrteam;
	  }
  }

  //Nach Teamnummern sortieren.
  var t1, t2;
  for(t1 = 0; t1 < GetLength(teams); t1++)
  {
    for(t2 = t1; t2 < GetLength(teams); t2++)
  	  if(teams[t2] < teams[t1])
      {
	      var tmp = teams[t1];
    		teams[t1] = teams[t2];
		    teams[t2] = tmp;
      }
  }
  
  //Darstellen. (InitSingleplayer/Multiplayer/Player)
  for(var x=0; x < GetLength(teams); x++)
  {
    var t = teams[x];

    if(!GetTeamPlayerCount(t)) continue;

    if(GetTeamPlayerCount(t) == 1)
    {
      InitSingleplayerTeam(GetTeamPlayer(t, 1));
      continue;
    }

    InitMultiplayerTeam(t);

    var j = 0;
    while(GetTeamPlayer(t, ++j) != -1)
    {
      InitPlayer(GetTeamPlayer(t, j));
    }
  }
  
  for(var i = 1; i <= GetTeamCount(); i++)
  {
    Log("Team %d mit %d Starttickets.",i,iStartTickets);//TODO: Übersetzung.
    aTicket[i-1] = iStartTickets;
  }
  
  SortTeamScoreboard();
  init = true;

  UpdateScoreboardTotal();
}


// Team eintragen, dass mehrere Spieler hat
private func InitMultiplayerTeam(int iTeam)
{
  if(iTeam < 1) return;

  var col = GetTeamColor(iTeam);

  SetScoreboardData(TEAM_iRow + iTeam, SBRD_Caption, "");
  SetScoreboardData(TEAM_iRow + iTeam, TEAM_TeamColumn,  Format("<c %x>%s</c>", col, GetTeamName(iTeam)), iTeam);
  SetScoreboardData(TEAM_iRow + iTeam, TEAM_PlayerColumn, "");
  SetScoreboardData(TEAM_iRow + iTeam, TEAM_GoalColumn,  Format("<c %x>%d</c>", col, aTicket[iTeam-1]), aTicket[iTeam-1]);
  SetScoreboardData(TEAM_iRow + iTeam, TEAM_KillColumn,  Format("<c %x>%d</c>", col, TeamGetKills(iTeam)), TeamGetKills(iTeam)+1);
  SetScoreboardData(TEAM_iRow + iTeam, TEAM_DeathColumn, Format("<c %x>%d</c>", col, TeamGetDeath(iTeam)));
  SortTeamScoreboard();
}

// Team eintragen, dass nur einen Spieler hat
private func InitSingleplayerTeam(int iPlr)
{
  if(iPlr == -1) return;

  var col = GetPlrColorDw(iPlr);

  SetScoreboardData(iPlr, SBRD_Caption, "");
  SetScoreboardData(iPlr, TEAM_TeamColumn, GetTaggedPlayerName(iPlr), GetPlayerTeam(iPlr));
  SetScoreboardData(iPlr, TEAM_PlayerColumn, "");
  SetScoreboardData(iPlr, TEAM_GoalColumn,  Format("<c %x>%d</c>", col, aTicket[GetPlayerTeam(iPlr)-1]), aTicket[GetPlayerTeam(iPlr)-1]);
  SetScoreboardData(iPlr, TEAM_KillColumn,  Format("<c %x>%d</c>", col, TeamGetKills(GetPlayerTeam(iPlr))), TeamGetKills(GetPlayerTeam(iPlr))+1);
  SetScoreboardData(iPlr, TEAM_DeathColumn, Format("<c %x>%d</c>", col, aDeath[iPlr]), aDeath[iPlr]);
  SortTeamScoreboard();
}

// Spieler eintragen
private func InitPlayer(int iPlr)
{
  // Team 0 oder -1 können wir nicht behandeln
  if(GetPlayerTeam(iPlr) < 1) return;
  if(iPlr == -1) return;
  
  // Eintragen
  SetScoreboardData(iPlr, SBRD_Caption, "");
  SetScoreboardData(iPlr, TEAM_TeamColumn, Format("<c %x>\\</c>",GetTeamColor(GetPlayerTeam(iPlr))), GetPlayerTeam(iPlr));
  SetScoreboardData(iPlr, TEAM_PlayerColumn, GetTaggedPlayerName(iPlr));
  SetScoreboardData(iPlr, TEAM_GoalColumn, "", aTicket[GetPlayerTeam(iPlr)-1]);
  SetScoreboardData(iPlr, TEAM_KillColumn, Format("%d", aKill[iPlr]), aKill[iPlr]);
  SetScoreboardData(iPlr, TEAM_DeathColumn, Format("%d", aDeath[iPlr]), aDeath[iPlr]);
  SortTeamScoreboard();
}

// Spieler austragen
private func RemoveScoreboardPlayer(int iPlr)
{
  if(iPlr == -1) return;

  if(GetTeamPlayerCount(GetPlayerTeam(iPlr)) == 1) return RemoveSingleplayerTeam(iPlr);

  SetScoreboardData(iPlr, TEAM_TeamColumn);
  SetScoreboardData(iPlr, TEAM_PlayerColumn);
  SetScoreboardData(iPlr, TEAM_GoalColumn);
  SetScoreboardData(iPlr, TEAM_KillColumn);
  SetScoreboardData(iPlr, TEAM_DeathColumn);

  SortTeamScoreboard();

  // Team nicht prüfen
  if(fNoTeamCheck) return;
  fNoTeamCheck = true;

  // Team wechselt zum Einzelspielerteam
  if(GetTeamPlayerCount(GetPlayerTeam(iPlr)) == 2) // 2, weil der Spieler in diesem Moment noch im Spiel ist
  {
    var j = 0, p;
    while(GetTeamPlayer(GetPlayerTeam(iPlr), ++j) != -1)
    {
      p = GetTeamPlayer(iPlr, j);
      if(p == iPlr) continue;
      //Log("lol: %d", p);//TODO: Wasn das?!
      RemoveMultiplayerTeam(GetPlayerTeam(iPlr));
      InitSingleplayerTeam(p);
    }
  }
  fNoTeamCheck = false;
}

//aktualisiert den Punktestand
private func UpdateScoreboardTotal()
{
  for(var i = 1; i <= GetTeamCount(); i++)
    UpdateScoreboard(i);
}

private func UpdateScoreboard(int iTeam)
{
  if(GetTeamPlayerCount(iTeam) > 1)
  {
    SetScoreboardData(TEAM_iRow + iTeam, TEAM_GoalColumn, Format("<c %x>%d</c>", GetTeamColor(iTeam), aTicket[iTeam-1]), aTicket[iTeam-1]);
    var p, j=0;
    while(GetTeamPlayer(iTeam, ++j) != -1)
    {
      p = GetTeamPlayer(iTeam, j);
      SetScoreboardData(p, TEAM_GoalColumn, "", aTicket[iTeam-1]);
    }
  }
  else
    SetScoreboardData(GetTeamPlayer(iTeam, 1), TEAM_GoalColumn, Format("<c %x>%d</c>", GetPlrColorDw(GetTeamPlayer(iTeam, 1)), aTicket[iTeam-1]), aTicket[iTeam-1]);

  SortTeamScoreboard();
}


/* Tickets */
public func TicketChange(int iTeam, int iChange)
{
  DoTickets(iChange);
  UpdateScoreboard(iTeam);
}

public func ScoreChange(int iTeam, int iChange)
{
  DoTickets(iChange);
  UpdateScoreboard(iTeam);
}

public func GetTickets(int iTeam)
{
  return aTicket[iTeam-1];
}

public func DoTickets(int iTeam, int iChange)
{
  aTicket[iTeam-1] = Max(aTicket[iTeam-1] + iChange, 0);
  UpdateScoreboard(iTeam);
}

public func SetTickets(int iTeam, int iTickets)
{
  aTicket[iTeam-1] = Max(iTickets, 0);
  UpdateScoreboard(iTeam);
}


/* Spiellogik */
public func IsFulfilled()
{
  if(ObjectCount(CHOS)) return false;
  if(GetTeamCount() == 1) return false;//Sinnlos.

  var alive = [];

  for(var i = 0; i <= GetTeamCount(); i++)
  {
    if(GetTeamPlayerCount(i+1))
      alive[GetLength(alive)] = i+1;
  }
  
  //Nur ein Team am Leben? (-> Gewonnen!)
  if(GetLength(alive) == 1)
  {
    EliminateLosers(alive[0]);
    if(LosersAlive(alive[0]))
      return false;

    Evaluation();
    Message("@<c %x>$WinMsg$</c>",0,GetTeamColor(i),GetTeamName(i));
    Sound("Cheer.ogg",true);
    return true;
  }
  
  //Draw! D=
  if(!GetLength(alive))
  {
    EliminateLosers(0);
    if(LosersAlive(0))
      return false;
      
    Evaluation();
      
    return true;
  }
}

private func Evaluation()
{
  if(!evaluation)
  for(var j=0 ; j < GetPlayerCount() ; j++)
  {
    var plr = GetPlayerByIndex(j);
    AddEvaluationData(Format("{{PIWP}}$Kills$: %d", aKill[plr]), GetPlayerID(plr));
    AddEvaluationData(Format("{{KAMB}}$Death$: %d", aDeath[plr]), GetPlayerID(plr));
  }
  
  evaluation = true;
}

private func EliminateLosers(int iTeam)
{
  //Log("EliminateLosers(%d)",iTeam);
  for(var i = 0; i < GetPlayerCount() ; i++)
    if(GetPlayerTeam(GetPlayerByIndex(i)) != iTeam)
    {
      //Log("->%d",GetPlayerTeam(GetPlayerByIndex(i)));
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

/*private func UpdateTicketLoss()
{
  if(ObjectCount(CHOS)) return;

  var cnt_div_teams = GetFlagCount()/GetTeamCount();
  var team_cnt;

  for(var i = 0; i < GetTeamCount(); i++)
  {
    team_cnt = GetFlagCount(i+1,true);
    if(team_cnt < cnt_div_teams)
      DoTickets(i+1,-1);
  }
}

private func FxTicketLossStart(object pTarget, int iEffectNumber, int iTemp, iTeam)
{
  if(!iTeam) return -1;
  EffectVar(0,pTarget,iEffectNumber) = iTeam;
}

private func FxTicketLossTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  pTarget->DoTickets(EffectVar(0,pTarget,iEffectNumber),-1);
}*/


/* Respawn */
private func InitializePlayer(int iPlr, int iX, int iY, object pBase, int iTeam)
{
  //Verfeindung setzen
  Hostility(iPlr);
  // Ins Scoreboard eintragen

  var teamp = GetTeamPlayerCount(iTeam);
  // Team ist neu
  if(teamp == 1)
  {
    //TODO: Theoretisch könnte man hier auch toll die Ticketverteilung tun. :)
    InitSingleplayerTeam(iPlr);
  }
  // Team wechselt von Einzelspieler auf Mehrspieler
  if(teamp == 2)
  {
    InitMultiplayerTeam(iTeam);
    RemoveSingleplayerTeam(iPlr);
    var j = 0;
    while(GetTeamPlayer(iTeam, ++j) != -1)
    {
      InitPlayer(GetTeamPlayer(iTeam, j));
    }
  }
  // Spieler normal einfügen
  if(teamp > 2)
    InitPlayer(iPlr);
  // Sortieren
  SortTeamScoreboard();
  
  RelaunchPlayer(iPlr, GetCrew(iPlr), 0, iTeam, true);
}

private func RelaunchPlayer(int iPlr, object pCrew, int iMurdererPlr, int iTeam, no_relaunch)
{
  // Kein ordentlicher Spieler?
  if(GetOwner(pCrew) == NO_OWNER || iPlr == NO_OWNER || !GetPlayerName(iPlr))
    return;
  // Kein Team
  if(!iTeam) iTeam = GetPlayerTeam(iPlr);
  
  if(!no_relaunch)
  {
    //Tode++
    aDeath[iPlr]++;
    if(GetTeamPlayerCount(GetPlayerTeam(iPlr)) > 1)
      SetScoreboardData(iPlr, TEAM_DeathColumn, Format("%d", aDeath[iPlr]), aDeath[iPlr]);
    else
      SetScoreboardData(iPlr, TEAM_DeathColumn, Format("<c %x>%d</c>", GetPlrColorDw(iPlr), aDeath[iPlr]), aDeath[iPlr]);
    if(GetTeamPlayerCount(GetPlayerTeam(iPlr)) > 1)
      SetScoreboardData(TEAM_iRow + GetPlayerTeam(iPlr), TEAM_DeathColumn, Format("<c %x>%d</c>", GetTeamColor(GetPlayerTeam(iPlr)), TeamGetDeath(GetPlayerTeam(iPlr))), TeamGetDeath(GetPlayerTeam(iPlr))+1);
    //kein Selfkill? kein Teamkill?
    if(iMurdererPlr != -1 && GetPlayerTeam(iPlr) != GetPlayerTeam(iMurdererPlr))
    {	
      //kill++
      aKill[iMurdererPlr]++;
      SetScoreboardData(iMurdererPlr, TEAM_KillColumn, Format("<c %x>%d</c>", GetPlrColorDw(iMurdererPlr), aKill[iMurdererPlr]), aKill[iMurdererPlr]);
      //team: kill++
      if(GetTeamPlayerCount(GetPlayerTeam(iMurdererPlr)) > 1)
      {
        SetScoreboardData(TEAM_iRow + GetPlayerTeam(iMurdererPlr), TEAM_KillColumn, Format("<c %x>%d</c>", GetTeamColor(GetPlayerTeam(iMurdererPlr)), TeamGetKills(GetPlayerTeam(iMurdererPlr))), TeamGetKills(GetPlayerTeam(iMurdererPlr))+1);
        SetScoreboardData(iMurdererPlr, TEAM_KillColumn, Format("%d", aKill[iMurdererPlr]), aKill[iMurdererPlr]);
      }
    }
    
    SortTeamScoreboard();
    Money(iPlr, pCrew, iMurdererPlr);
    
    if(!GetTickets(iTeam))
    {
      EliminatePlayer(iPlr);
      UpdateScoreboard(iTeam);
      return;
    }

    DoTickets(iTeam,-1);
  }
  
  UpdateScoreboardTotal();

  if(!FindObject(CHOS) && !FindObject(MCSL))//Regelwähler oder Klassenwahl?
    CreateGOCCSpawner(pCrew);
    
  return;
}

public func OnClassSelection(object pClonk)
{
  if(FindObject(CHOS))
    return;
    
  CreateGOCCSpawner(pClonk);
}