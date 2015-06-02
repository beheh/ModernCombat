/*-- Hold the Flag --*/

#strict 2
#include TEAM

local pFlag;		//Diese Flagge gilt es zu erobern
local aTeamPoints;	//Die Punkte jedes Teams
local iProgress;	//Bei 100% bekommt das Team einen Punkt
local iGoal;		//So viele Punkte gilt es zu erreichen

public func GoalExtraValue()	{return iGoal;}	//Spielzielinformationen an Scoreboard weitergeben


/* Initialisierung */

protected func Initialize()
{
  iGoal = 10;
  aKill = [];
  aDeath = [];
}

public func ChooserFinished()
{
  aTeamPoints = [];
  ScheduleCall(this, "UpdateFlag", 1);
  //Punktegeschwindgikeit
  AddEffect("IntAddProgress", this, 1, Max(14 - GetActiveTeamCount() * 2, 5), this);  
  UpdateHUDs();

  //Spielzielhinweise erstellen
  for(var i = 0; i < GetPlayerCount(); i++)
  {
    CreateObject(TK05, 0, 0, GetPlayerByIndex(i));
    Sound("Info_Round.ogg", true, 0, 100, GetPlayerByIndex(i));
  }
}

protected func UpdateFlag()
{
  pFlag = FindObject2(Find_Func("IsFlagpole"));
  if(!pFlag) return;
  //Einnahmegeschwindigkeit
  pFlag->Set(GetName(pFlag), pFlag->GetRange(), Max(18 - GetActiveTeamCount() * 4, 4));
  return true;
}

/* Konfiguration */

public func IsConfigurable()		{return true;}

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

  AddMenuItem(" ", "OpenGoalMenu", GetID(), pClonk, iGoal, 0, " ");
  AddMenuItem("$MorePoints$", "ChangeWinpoints", CHOS, pClonk, 0, +1, "$MorePoints$",2,1);
  AddMenuItem("$LessPoints$", "ChangeWinpoints", CHOS, pClonk, 0, -1, "$LessPoints$",2,2);
  AddMenuItem("$Finished$", "ConfigFinished", CHOS, pClonk,0,0,"$Finished$",2,3);

  SelectMenuItem(iSelection, pClonk);
}

private func ChangeWinpoints(id dummy, int iChange)
{
  //Zu erreichenden Punktestand verändern (maximal 100 (normal) oder 25 (Liga) Punkte)
  if(!GetLeague())
    iGoal = BoundBy(iGoal+iChange,1,100);
  else
    iGoal = BoundBy(iGoal+iChange,10,25);

  //Sound
  Sound("Grab", 1,0,0,1);
  //Menü wieder öffnen
  var iSel = 1;
  if(iChange == -1) iSel = 2;
  OpenGoalMenu(0, iSel);
}

/* Effekt */

protected func FxIntAddProgressTimer()
{
  //Keine Flagge?
  if (!pFlag && !SetFlag(FindObject(OFPL)))
    return;

  //Scoreboard
  UpdateScoreboard();

  var team = pFlag->~GetTeam();
  //Keiner hat sie?
  if (team == -1) return;
  //Nicht vollständig eingenommen?
  if (!pFlag->IsFullyCaptured())
  {
    iProgress = 0;
    //Nochmal updaten damit nicht das falsche Team die Prozente zugeschrieben bekommt
    return UpdateScoreboard();
  }

  //Punkte vergeben
  var warning = BoundBy(iGoal*3/4, iGoal-5, iGoal-1);
  if ((++iProgress) >= 100)
  {
    aTeamPoints[team]++;
    iProgress = 0;
    UpdateHUDs();
    //Und jedem Spieler im Team 10 Punkte geben.
    for (var i; i < GetPlayerCount(); i++)
    {
      if (GetPlayerTeam(GetPlayerByIndex(i)) == team)
      {
        //Punkte bei Belohnungssystem (Flaggenverteidigung)
        DoPlayerPoints(BonusPoints("Protection"), RWDS_TeamPoints, GetPlayerByIndex(i), GetCrew(GetPlayerByIndex(i)), IC12);
        Sound("Info_Event.ogg", true, 0, 0, GetPlayerByIndex(i)+1);
      }
      else if(aTeamPoints[team] == warning)
      {
        //Eventnachricht: Hinweis auf Team, das dem Ziel nahe ist
        EventInfo4K(GetPlayerByIndex(i)+1, Format("$TeamReachingGoal$", GetTaggedTeamName(team), iGoal-warning), GHTF, 0, 0, 0, "Info_Alarm.ogg");
      }
    }
  }

  //Gewonnen?
  if (!EffectVar(0, Par(), Par(1)))
    EffectVar(0, Par(), Par(1)) = IsFulfilled();
}

/* Zeug */

protected func Activate(iPlr)
{
  return MessageWindow(GetDesc(), iPlr);
}

public func IsTeamGoal()		{return 1;}

public func SetFlag(object pFlagPole)
{
  if(!pFlagPole || !pFlagPole->~IsFlagpole()) return;
  pFlag = pFlagPole;
  return !!pFlag;
}

public func GetFlag()
{
  return pFlag;
}

public func FlagLost(object pFlagPole, int iOldTeam, int iNewTeam, array aAttackers)
{
  //Ist es die Flagge?
  if(pFlagPole != pFlag)
    return;
  //Punkte für die Angreifer
  for(var clonk in aAttackers)
    if(clonk)
      //Punkte bei Belohnungssystem (Flaggenpostenneutralisierung)
      DoPlayerPoints(BonusPoints("Protection"), RWDS_TeamPoints, GetOwner(clonk), clonk, IC13);

  for(var i; i < GetPlayerCount(); i++)
    if(GetPlayerTeam(GetPlayerByIndex(i)) == iOldTeam)
      //Eventnachricht: Flaggenposten verloren
      EventInfo4K(GetPlayerByIndex(i)+1, Format("$MsgFlagLost$", GetName(pFlag), GetTeamFlagColor(iNewTeam), GetTeamName(iNewTeam)), SM23, 0, GetTeamFlagColor(iNewTeam), 0, "Info_Event.ogg");
}

public func FlagCaptured(object pFlagPole, int iTeam, array aAttackers, bool fRegained)
{
  //Ist es die Flagge?
  if (pFlagPole != pFlag)
    return;

  var first = true; //Der erste bekommt mehr Punkte, der Rest bekommt Assistpunkte
  for (var clonk in aAttackers)
    if (clonk)
    {
      if (fRegained)
        //Punkte bei Belohnungssystem (Flaggenpostenrückeroberung)
        DoPlayerPoints(BonusPoints("OPDefend"), RWDS_TeamPoints, GetOwner(clonk), clonk, IC12);
      else
      {
        if(first)
          //Punkte bei Belohnungssystem (Flaggenposteneroberung)
          DoPlayerPoints(BonusPoints("OPConquer"), RWDS_TeamPoints, GetOwner(clonk), clonk, IC10);
          else
          //Punkte bei Belohnungssystem (Hilfe bei Flaggenposteneroberung)
          DoPlayerPoints(BonusPoints("OPAssist"), RWDS_TeamPoints, GetOwner(clonk), clonk, IC11);
      }
      first = false;
    }

  //Eventnachricht: Flaggenposten erobert
  EventInfo4K(0, Format("$MsgCaptured$", GetTeamFlagColor(iTeam), GetTeamName(iTeam), GetName(pFlag)), SM22, 0, GetTeamFlagColor(iTeam), 0, "Info_Objective.ogg");
}

/* Scoreboard */

static const GHTF_Name = SBRD_Caption;
static const GHTF_Points = 0;
static const GHTF_Progress = 1;
static const GHTF_FlagRow = 1024;

public func UpdateScoreboard()
{
  //Wird noch eingestellt
  if (FindObject(CHOS)) return;

  //Titelzeile
  SetScoreboardData(SBRD_Caption, GHTF_Name, GetName());

  //Spaltentitel
  SetScoreboardData(SBRD_Caption, GHTF_Points, "{{GHTF}}");
  SetScoreboardData(SBRD_Caption, GHTF_Progress, "{{SM02}}");

  //Flaggenzeile
  var color = GetTeamFlagColor(pFlag->GetTeam()),
  prog = pFlag->GetProcess();
  color = InterpolateRGBa3(RGBa(255, 255, 255), color, prog, 100);
  SetScoreboardData(GHTF_FlagRow, GHTF_Name, Format("<c %x>%s</c>", color, GetName(pFlag)));
  SetScoreboardData(GHTF_FlagRow, GHTF_Progress, Format("<c %x>%d%</c>", color, prog), GHTF_FlagRow);
  var icon, trend = pFlag->GetTrend();
  if (!trend)		icon = SM21;	//Keine Aktivität
  if (trend == -1)	icon = SM23;	//Angriff
  if (trend == 1)	icon = SM22;	//Verteidigung
  SetScoreboardData(GHTF_FlagRow, GHTF_Points, Format("{{%i}}", icon), GHTF_FlagRow-1);

  //Leere Zeile
  SetScoreboardData(GHTF_FlagRow-1, GHTF_Progress, "", GHTF_FlagRow-1);
  SetScoreboardData(GHTF_FlagRow-1, GHTF_Points, "", GHTF_FlagRow-1);

  //Benötigte Punktzahl
  SetScoreboardData(GHTF_FlagRow-2, GHTF_Name, "$SbrdGoalPoints$", GHTF_FlagRow-1);
  SetScoreboardData(GHTF_FlagRow-2, GHTF_Points, Format("%d", iGoal), GHTF_FlagRow-1);

  //Alle Teams
  var iFlagTeam = pFlag->~GetTeam();
  for (var i, j; i < GetTeamCount(); j++)
  {
    //Team gibts. Hochzählen
    if (GetTeamName(j))
      i++;
    //Team gibts nicht oder keine Spieler drin
    if (!GetTeamName(j) || !GetTeamPlayerCount(j))
    {
      SetScoreboardData(j, GHTF_Name);
      SetScoreboardData(j, GHTF_Points);
      SetScoreboardData(j, GHTF_Progress);
      continue;
    }
    SetScoreboardData(j, GHTF_Name, Format("<c %x>%s</c>", GetTeamFlagColor(j), GetTeamName(j)));
    SetScoreboardData(j, GHTF_Points, Format("<c %x>%d</c>", GetTeamFlagColor(j), aTeamPoints[j]), aTeamPoints[j]);
    //Team hat die Flagge
    if (j == iFlagTeam)
      SetScoreboardData(j, GHTF_Progress, Format("<c %x>%02d%</c>", GetTeamFlagColor(j), iProgress), iProgress);
    else
      SetScoreboardData(j, GHTF_Progress, Format("<c %x>00%</c>", GetTeamFlagColor(j)), -1);
  }

  //So... Erstmal nach Flaggenstatus sortieren
  SortScoreboard(GHTF_Progress, true);
  //Und dann nochmal nach Punkten. Damit bei gleicher Punktzahl das Team vorne ist, das gerade die Flagge hält
  SortScoreboard(GHTF_Points, true);
}

public func GetTeamFlagColor(int iTeam)
{
  //Team hat mehrere Spieler: Teamfarbe
  if(GetTeamPlayerCount(iTeam) != 1)
    return GetTeamColor(iTeam);
  //Ein Spieler: Dessen Spielerfarbe
  for (var i; i < GetPlayerCount(); i++)
    if(GetPlayerTeam(GetPlayerByIndex(i)) == iTeam)
      return GetPlrColorDw(GetPlayerByIndex(i));
  return GetTeamColor(iTeam);
}

/* Sieg */

local fulfilled;

public func IsFulfilled()
{
  if(FindObject(CHOS)) return;

  if (fulfilled) return true;
  for (var i; i < GetLength(aTeamPoints); i++)
    if (aTeamPoints[i] >= iGoal)
    {
      //Team hat gewonnen. Alle anderen durchgehen und eliminieren
      for (var team, j = GetTeamCount(); j; team++)
      if (GetTeamName(team))
      {
        j--;
        if (team != i)
          EliminateTeam(team);
      }
      //Spielende planen
      Schedule("GameOver()", 150);

      //Auswertung
      RewardEvaluation();

      //Nachricht über Gewinner
      Message("@$TeamHasWon$", 0, GetTeamFlagColor(i), GetTeamName(i));

      //Sound
      Sound("Cheer.ogg", true);

      return fulfilled = true;
    }
  //Nur noch eins übrig
  if (GetActiveTeamCount() <= 1)
  {
    var i = GetPlayerTeam(GetPlayerByIndex());

    //Spielende planen
    Schedule("GameOver()", 150);

    //Auswertung
    RewardEvaluation();

    //Nachricht über Gewinner
    Message("@$TeamHasWon$", 0, GetTeamFlagColor(i), GetTeamName(i));

    //Sound
    Sound("Cheer.ogg", true);

    return fulfilled = true;  
  }
}

private func TeamGetScore(int iTeam)
{
  return aTeamPoints[iTeam];
}

/* Ungenutze Funktionen */

private func InitScoreboard()							{}
private func InitMultiplayerTeam(int iTeam)					{}
private func RemoveMultiplayerTeam(int iTeam)					{}
private func InitSingleplayerTeam(int iPlr)					{}
private func RemoveSingleplayerTeam(int iPlr)					{}
private func InitPlayer(int iPlr)						{}
private func RemoveScoreboardPlayer(int iPlr)					{}
public func WinScoreChange(int iNewScore)					{}
private func SortTeamScoreboard()						{}
public func RelaunchScoreboard(int iPlr, object pClonk, int iMurdererPlr)	{}