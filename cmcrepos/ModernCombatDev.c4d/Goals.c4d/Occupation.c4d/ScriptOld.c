/*-- Besitznahme --*/

#strict 2
#include TEAM

protected func Activate(iPlr) // Defaultnachrichten
{
  if (IsFulfilled())
  {
    //if(aTicket[GetPlayerTeam(iPlr)-1] >= iWinScore)
      //return(MessageWindow("$MsgGoalFulfilled$",iPlr));
    //else
      return MessageWindow("$MsgGoalLost$",iPlr);
  }
  
  var text = "";
  
  for(var i = 1; i <= GetTeamCount();i++)
  {
    text = Format("%s<c %x>%s</c>: %d $Tickets$",text,GetTeamColor(i),GetTeamName(i),aTicket[i-1]);
  }
  
  return MessageWindow(text,iPlr);
}

//Einstellungen! >:D
public func IsConfigurable() { return 1; }

public func ConfigMenu(object pCaller)
{
  OpenGoalMenu();
  return 1;
}

protected func OpenGoalMenu(id dummy, int iSelection)
{
  var pClonk = GetCursor();
  // Menü aufmachen
  CreateMenu(GetID(), pClonk, 0,0,0,0, 1);
  // Anzeige
  AddMenuItem(" ", "OpenGoalMenu", GetID(), pClonk, iStartTickets, 0, " ");
  // Zähler erhöhen
  AddMenuItem("$MoreTickets$", "ChangeStartTickets", CHOS, pClonk, 0, +1, "$MoreTickets$",2,1);
  // Zähler senken
  AddMenuItem("$LessTickets$", "ChangeStartTickets", CHOS, pClonk, 0, -1, "$LessTickets$",2,2);
  // Fertig
  AddMenuItem("$Finished$", "ConfigFinished", CHOS, pClonk,0,0,"$Finished$",2,3);
  // Letzten Eintrag auswählen
  SelectMenuItem(iSelection, pClonk);
}

protected func ChangeStartTickets(id dummy, int iChange)
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

protected func ConfigFinished()
{
  var chos = FindObject(CHOS);
  if(chos)
    chos->OpenMenu();
}

// Variablen zum Speichern des Punktestandes
local iStartTickets;
local aTicket;

protected func Initialize()
{
  aTicket = CreateArray();
  iStartTickets = 10;
  return _inherited();
}

//Die Initialisierung des Scoreboards. Damit alle Spalten/Zeilen etc. angezeigt werden
private func InitScoreboard()
{
  //bool SetScoreboardData (int iRowID, int iColID, string sText, int iData); 

  //Überschriften
  
  //SetScoreboardData(SBRD_Caption, SBRD_Caption, Format("%d $Score$",iWinScore), SBRD_Caption);
  //Es gibt ja kein Maximum.
  
  SetScoreboardData(SBRD_Caption, TEAM_TeamColumn, "{{KILC}}", SBRD_Caption);
  SetScoreboardData(SBRD_Caption, TEAM_PlayerColumn, "{{PCMK}}", SBRD_Caption);
  SetScoreboardData(SBRD_Caption, TEAM_GoalColumn, "{{TIKT}}", SBRD_Caption);
  SetScoreboardData(SBRD_Caption, TEAM_KillColumn, "{{PIWP}}", SBRD_Caption);
  SetScoreboardData(SBRD_Caption, TEAM_DeathColumn, "{{KAMB}}", SBRD_Caption);

  var teams = CreateArray(), team;

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

  //so, jetzt wissen wir die Teams. Jetzt noch nach Teamnummern ordnen. %(
  //Bubblesort!
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
  //puh, ganz schön viel Aufwand für sowas..
  //aber jetzt müssen wir nur noch darstellen. :)
  for(var x=0; x < GetLength(teams); x++)
  {
    var t = teams[x];
    // Team hat keinen Spieler -> Höh?
    if(!GetTeamPlayerCount(t)) continue;
    // Team hat nur einen Spieler -> "Einzelspielerteam"
    if(GetTeamPlayerCount(t) == 1)
    {
      InitSingleplayerTeam(GetTeamPlayer(t, 1));
      continue;
    }
    // Team einfügen, 1024 Spieler werden maximal unterstützt!
    InitMultiplayerTeam(t);
    // Spieler aus dem Team einfügen
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
  
  // Sortieren
  SortTeamScoreboard();
  // Fertig
  init = true;
  
  //>.< Argh!
  UpdateScoreboardTotal();
}

/* Scoreboard */

// Funktionen zum Eintragen

// Team eintragen, dass mehrere Spieler hat
private func InitMultiplayerTeam(int iTeam)
{
  if(iTeam < 1) return ;

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
  if(iPlr == -1) return ;

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
  if(GetPlayerTeam(iPlr) < 1) return ;
  if(iPlr == -1) return ;
  
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
  if(iPlr == -1) return ;

  if(GetTeamPlayerCount(GetPlayerTeam(iPlr)) == 1) return RemoveSingleplayerTeam(iPlr);

  SetScoreboardData(iPlr, TEAM_TeamColumn);
  SetScoreboardData(iPlr, TEAM_PlayerColumn);
  SetScoreboardData(iPlr, TEAM_GoalColumn);
  SetScoreboardData(iPlr, TEAM_KillColumn);
  SetScoreboardData(iPlr, TEAM_DeathColumn);

  SortTeamScoreboard();

  // Team nicht prüfen
  if(fNoTeamCheck) return ;
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

/* Aufrufe */

public func ScoreChange(int iTeam, int iChange)
{
  TicketChange(iTeam,iChange);
}

public func TicketChange(int iTeam, int iChange)
{
  aTicket[iTeam-1] += iChange;
  UpdateScoreboard(iTeam);
}

public func IsFulfilled() // Siegreiches Team?
{
  if(ObjectCount(CHOS)) return 0;
  if(GetPlayerCount() >= 1){return 0;}
  for(var i = 1; i <= GetTeamCount();i++)
  {
    //Alles eingenommen oder gefraggt!
    if((GetFlagCount() <= GetFlagCount(i))||OnlyTeamAlife(i))
    {
      EliminateLosers();
      Message("@<c %x>$WinMsg$</c>",0,GetTeamColor(i),GetTeamName(i));
      
      //Leben noch Verlierer? -> Ligainkompatibilität!
      if(LosersAlive())
        return 0;
        
      // Auswertungsdialog
      if(!evaluation)
      for(var j=0 ; j < GetPlayerCount() ; j++)
      {
    		var plr = GetPlayerByIndex(j);
				AddEvaluationData(Format("{{PIWP}}$Kills$: %d", aKill[plr]), GetPlayerID(plr));
				AddEvaluationData(Format("{{KAMB}}$Death$: %d", aDeath[plr]), GetPlayerID(plr));
			}
      
      evaluation = true;
      return 1;
    }
  }
}

private func OnlyTeamAlife(int iTeam)
{
  for(var i = 1; i <= GetTeamCount(); i++)
  {
    if(iTeam != i)
      if(aTicket[i-1])
        return false;
  }
  return true;
}

private func EliminateLosers()  // Hier trennt sich die Spreu vom Weizen
{
  for(var i=0 ; i < GetPlayerCount() ; i++)
    if( (!GetFlagCount(GetPlayerTeam(GetPlayerByIndex(i))))
     || (!aTicket[GetPlayerTeam(GetPlayerByIndex(i))-1]))
        EliminatePlayer(GetPlayerByIndex(i)); // HARHAR
}

private func LosersAlive()
{
  for(var x = 0 ; x < GetPlayerCount() ; x++)
    if(!GetFlagCount(GetPlayerTeam(GetPlayerByIndex(x))))
      if(GetPlayerByIndex(x) >= 0)
        return 1;

  return 0;
}

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


private func Overtaken(int iTeam)
{
  Sound("Domination", true);
  Log("<c %x>%s</c> hat alle Fahnen geownt!!11elf", GetTeamColor(iTeam),GetTeamName(iTeam));//TODO: Übersetzung.
  
  ResetFlagpoles();

  for(var pClonk in FindObjects(Find_OCF(OCF_CrewMember), Find_OCF(OCF_Alive)))
  {
    ScheduleCall(pClonk,"CreateGOCCSpawner",35*5);
  }
}

public func GetFlagCount(int iTeam)
{
  var count;
  for(var flag in FindObjects(Find_Func("IsFlagpole")))
  {
    if(iTeam)
      if(flag->GetTeam() != iTeam)
        continue;
    count++;
  }
  return count;
}

public func ResetFlagpoles()
{
  var flagpoles = FindObjects(Find_Func("IsFlagpole"));
  for(var flag in flagpoles)
    flag->NoTeam();
    
  for(var i = 1; i <= GetTeamCount();i++)
  {
    var j = Random(GetLength(flagpoles));
    var sel = flagpoles[j];
    DelArrayItem4K(flagpoles,j);//HAR HAR
  }
}

/** Respawn **/
protected func InitializePlayer(int iPlr, int iX, int iY, object pBase, int iTeam)
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
      Log("1");
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

public func RelaunchPlayer(int iPlr, object pCrew, int iMurdererPlr, int iTeam, no_relaunch)
{
  // Kein ordentlicher Spieler?
  if(GetOwner(pCrew) == NO_OWNER || iPlr == NO_OWNER || !GetPlayerName(iPlr))
    return ;
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
    // Geld
    Money(iPlr, pCrew, iMurdererPlr);
    
    //Log("Team %d -> %d-1 Tickets (wegen %s)",iTeam,aTicket[iTeam-1],GetPlayerName(iPlr));
    
    if(!aTicket[iTeam-1])
    {
      EliminatePlayer(iPlr);
      //Log("Keine Tickets kein Respawn! (%d->%s)",iTeam,GetPlayerName(iPlr));
      UpdateScoreboard(iTeam);
      return ;
    }

    aTicket[iTeam-1]--;
  }
  
  //UpdateScoreboard(iTeam);
  UpdateScoreboardTotal();

  if(!FindObject(CHOS) && !FindObject(MCSL))//Regelwähler oder Klassenwahl?
    CreateGOCCSpawner(pCrew);
    
  return ;
}

public func ChooserFinished()
{
  ScheduleCall(this,"InitScoreboard",1);
  
  if(!FindObject(MCSL))//Klassenwahl?
    ScheduleCall(0,"CreateSpawners",1);
}

public func OnClassSelection(object pClonk)
{
  if(FindObject(CHOS))
    return ;
    
  CreateGOCCSpawner(pClonk);
}

func CreateSpawners()
{
  for(var i = 0; i < GetPlayerCount(); i++)
    for(var j = 0, pCrew ; pCrew = GetCrew(GetPlayerByIndex(i), j) ; j++)
      CreateGOCCSpawner(pCrew);
}

/* Status */

public func IsTeamGoal() { return 1; }
