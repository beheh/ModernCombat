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
  if(GetWinningTeam() > 0) {
    if(GetWinningTeam() == GetPlayerTeam(iPlr)) {
      text = "$MsgGoalFulfilled$";
    }
    else {
      text = "$MsgGoalLost$";
    }
  }
  else {
    for(var i = 1; i <= GetTeamCount(); i++)
      text = Format("%s<c %x>%s</c>: %d $Tickets$|",text,GetTeamColor(i),GetTeamName(i),GetTickets(i));
  }
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
        Format("%s : <c %x>%d</c>",GetTeamName(i),GetTeamColor(i),GetTickets(i));
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
  SetScoreboardData(SBRD_Caption, SBRD_Caption, "{{GOCC}}", SBRD_Caption);
  SetScoreboardData(SBRD_Caption, 1, "{{OSPW}}", SBRD_Caption);
  SetScoreboardData(SBRD_Caption, 2, "{{AFTN}}", SBRD_Caption);
  
  UpdateScoreboard();
}

private func UpdateScoreboard()
{
  var i = 1;
  //Flaggen
  for(var flag in FindObjects(Find_ID(OFPL)))
  {
    var color=RGB(255,255,255);
    if(flag->GetTeam())
      if(flag->GetProcess() >= 100) {
        color = GetTeamColor(flag->GetTeam());
      }
      else {
        //color = DoColorBrightness(GetTeamColor(flag->GetTeam()), -80);
				color = SetRGBaValue(GetTeamColor(flag->GetTeam()), 180, 0);
      }
    SetScoreboardData(i,1,Format("<c %x>%s</c>",color,GetName(flag)), GetX(flag));
    SetScoreboardData(i,2,Format("%d%", flag->GetProcess()), flag->GetProcess());
    i++;
  }
  SetScoreboardData(i, 1, "", LandscapeWidth()+9); //ololo
  i++;
  //Tickets
  for(var j = 1; j <= GetTeamCount(); j++)
  {
    SetScoreboardData(i, 1, Format("<c %x>%s</c>", GetTeamColor(j), GetTeamName(j)), LandscapeWidth()+10);
    SetScoreboardData(i, 2, Format("%d {{TIKT}}", GetTickets(j)), i);
    i++;
  }
  SortScoreboard(1);
}

public func FlagAttacked(object pFlag) {
  for(var i = 0; i < GetPlayerCount(); i++) {
    DoScoreboardShow(1, GetPlayerByIndex(i)+1);
    Schedule(Format("DoScoreboardShow(-1, %d)", GetPlayerByIndex(i)+1), 150);
  }
}

/* Tickets */
public func TicketChange(int iTeam, int iChange)
{
  DoTickets(iChange);
  //UpdateScoreboard(iTeam);
}

public func ScoreChange(int iTeam, int iChange)
{
  DoTickets(iChange);
  //UpdateScoreboard(iTeam);
}

public func GetTickets(int iTeam)
{
  return aTicket[iTeam-1];
}

public func DoTickets(int iTeam, int iChange)
{
  aTicket[iTeam-1] = Max(aTicket[iTeam-1] + iChange, 0);
  //UpdateScoreboard(iTeam);
}

public func SetTickets(int iTeam, int iTickets)
{
  aTicket[iTeam-1] = Max(iTickets, 0);
  //UpdateScoreboard(iTeam);
}


/* Spiellogik */
public func IsFulfilled()
{
  if(ObjectCount(CHOS)) return false;

  var iWinningTeam = GetWinningTeam();

  //Nur ein Team am Leben? (-> Gewonnen!)
  if(iWinningTeam > 0)
  {
    if(LosersAlive(iWinningTeam))
      return false;

    Evaluation();
    Message("@$WinMsg$",0 , GetTeamColor(iWinningTeam), GetTeamName(iWinningTeam));
    Sound("Cheer.ogg",true);
    Schedule("GameOver()",150);
    return true;
  }
  //Draw! D=
  if(iWinningTeam == -1)
  {
    for(var i = 0; i < GetTeamCount(); i++)
      EliminateTeam(GetTeamByIndex(i));
    if(LosersAlive(0))
      return false;
      
    Evaluation();
    Schedule("GameOver()",150);
    return true;
  }
}

private func GetWinningTeam() {
  var alive = [], poles = [];
  var add, id;
  
  //Zwei Siegbedingungen: Alle Spieler eines Teams eliminiert und alle Flaggen des Teams eingenommen
  for(var i = 1; i <= GetTeamCount(); i++)
  {
    poles[i] = 0;
    for(var pole in FindObjects(Find_ID(OFPL)))
      if(pole->GetTeam() == i && pole->IsFullyCaptured())
        poles[i]++;
    if(poles[i] == 0) //Keine Flaggen?
    {
      alive[i] = 0;    //Hackig...
      for(var clonk in FindObjects(Find_OCF(OCF_Alive), Find_OCF(OCF_CrewMember)))
        if(GetPlayerTeam(GetOwner(clonk)) == i)
        {
          if(Contained(clonk))
          {
            if((GetID(Contained(clonk)) == OSPW && GetAction(Contained(clonk)) != "Counter") || GetID(Contained(clonk)) == TIM2)
              continue;
            alive[i]++;
            break;
          }
          alive[i]++;
        }
      if(alive[i] == 0) //Keine Clonks auf dem Feld oder kurz vorm Respawn?
        EliminateTeam(i); //Eliminieren!
    }
    else
      //Garkeine Spieler in einem Team?
      for(var j = 0; j < GetPlayerCount(); j++)
        if(GetPlayerTeam(GetPlayerByIndex(j)) == i)
          alive[i]++;
  }
  
  //Wie viele Teams existent?
  var teamA = 0;
  for(var i = 0; i < GetLength(alive); i++)
    if(alive[i] > 0)
    {
      if(teamA) //Zwei oder mehr Teams lebendig?
        return 0;
      else
        teamA = i; //Ein lebendiges Team gefunden?
    }
  if(teamA < 1) //Falls keine Teams lebendig (wieso auch immer)
    return -1;
  else
    return teamA; //Ansonsten das einzig lebendige Team
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
	//Neineinein, kein Rejoin
  if(!FindObject(CHOS) && !GetTickets(iTeam))
  {
    EliminatePlayer(iPlr);
    return false;
  }

  //Verfeindung setzen
  Hostility(iPlr);
  // Ins Scoreboard eintragen
 /*
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
  SortTeamScoreboard();*/
  
  RelaunchPlayer(iPlr, GetCrew(iPlr), 0, iTeam, true);
}

private func RelaunchPlayer(int iPlr, object pCrew, int iMurdererPlr, int iTeam, no_relaunch)
{
  // Kein ordentlicher Spieler?
  if(GetOwner(pCrew) == NO_OWNER || iPlr == NO_OWNER || !GetPlayerName(iPlr))
    return;
  // Kein Team
  if(!iTeam) iTeam = GetPlayerTeam(iPlr);
 
  //Kriegsnebel für alle
  SetFoW(true, iPlr);
  
  if(!no_relaunch)
  {
    //Tode++
    aDeath[iPlr]++;

    //kein Selfkill? kein Teamkill?
    if(iMurdererPlr != -1 && GetPlayerTeam(iPlr) != GetPlayerTeam(iMurdererPlr))
    {	
      //kill++
      aKill[iMurdererPlr]++;
    }
    
    Money(iPlr, pCrew, iMurdererPlr);
    
    if(!GetTickets(iTeam) || (GetWinningTeam() > 0 && GetWinningTeam() != iTeam))
    {
      EliminatePlayer(iPlr);
      return;
    }

    DoTickets(iTeam,-1);
  }

  if(!FindObject(CHOS) && !FindObject(MCSL))//Regelwähler oder Klassenwahl?
     CreateGOCCSpawner(pCrew);
     
  Schedule(Format("DoFlag(%d, %d)", iTeam, iPlr), 1);

  return;
}

public func DoFlag(int iTeam, int iPlr) {
  var pCrew = GetCrew(iPlr);
  var pObject = Contained(pCrew);
  if(!pObject) {
    pObject = pCrew;
  }
  else {
    SetVisibility(VIS_None, pCrew);
  }
  ShowFlagpole(GetBestFlag(iTeam), pCrew, pObject);
  return true;
}

private func RemovePlayer(int iPlr)
{
  if(iPlr == -1) return;

	// Auswertungsdialog
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

public func ChooserFinished()
{
  for(var i = 1; i <= GetTeamCount(); i++)
    DoTickets(i, iStartTickets);
  _inherited(...);
}
