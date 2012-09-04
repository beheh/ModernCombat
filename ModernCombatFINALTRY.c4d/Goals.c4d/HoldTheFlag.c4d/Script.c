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
  SetFlag(FindObject(OFLP));
  aTeamPoints = [];
  AddEffect("IntAddProgress", this, 1, 10, this);
  UpdateHUDs();

  //Spielzielhinweise erstellen
  for(var i = 0; i < GetPlayerCount(); i++)
  {
    CreateObject(TK05, 0, 0, GetPlayerByIndex(i));
    Sound("ObjectiveReceipt.ogg", true, 0, 100, GetPlayerByIndex(i));
  }
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
  //Stand ver�ndern
  iGoal = BoundBy(iGoal+iChange,1,100); //100 ist Maximum.
  //Sound
  Sound("Grab", 1,0,0,1);
  //Men� wieder �ffnen
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
  //Nicht vollst�ndig eingenommen?
  if (!pFlag->IsFullyCaptured())
  {
    iProgress = 0;
	//Nochmal updaten damit nicht das falsche Team die Prozente zugeschrieben bekommt
	return UpdateScoreboard();
  }

  //Punkte vergeben
  var warning = BoundBy(iGoal*3/4, iGoal-5, iGoal-1);
  if ((++iProgress) >= 100) {
    aTeamPoints[team]++;
	iProgress = 0;
    UpdateHUDs();
	//Und jedem Spieler im Team 10 Punkte geben.
	for (var i; i < GetPlayerCount(); i++)
	  if (GetPlayerTeam(GetPlayerByIndex(i)) == team)
          {
            //Punkte bei Belohnungssystem (Flaggenverteidigung)
	    DoPlayerPoints(BonusPoints("Protection"), RWDS_TeamPoints, GetPlayerByIndex(i), GetCrew(GetPlayerByIndex(i)), IC12);
	      Sound("Info.ogg", true, 0, 0, GetPlayerByIndex(i)+1);
	  }
	  //Die anderen warnen falls n�tig
	  else
	    if(aTeamPoints[team] == warning)
	      EventInfo4K(GetPlayerByIndex(i)+1, Format("$TeamReachingGoal$", GetTaggedTeamName(team), iGoal-warning), GHTF, 0, 0, 0, "Alarm.ogg");
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

protected func RelaunchPlayer(int iPlr, object pCrew, int iKiller, int iTeam, no_relaunch)
{
  Money(iPlr, pCrew, iKiller);

  aDeath[iPlr]++;
  if (iKiller != -1 && GetPlayerTeam(iKiller) != GetPlayerTeam(iPlr))
    aKill[iKiller]++;

  if(!FindObject(CHOS) && !FindObject(MCSL))
    CreateGOCCSpawner(pCrew);
}

public func IsTeamGoal()		{return 1;}

public func SetFlag(object pFlagPole)
{
  pFlag = pFlagPole;
  //GameCall f�r Szenarien.
  //Die Funktion sollte anhand der Anzahl der aktiven Teams (1. Parameter) die Zeit berechnen und zur�ckgeben.
  var iTime = GameCall("GetHTFTime", GetActiveTeamCount());
  if (!iTime)
    iTime = 15/GetActiveTeamCount();
  if (pFlag)
    pFlag->~Set(GetName(pFlag), 100, Max(iTime, 1));
  return pFlag;
}

public func GetFlag()
{
  return pFlag;
}

public func FlagLost(object pFlagPole, int iOldTeam, int iNewTeam, array aAttackers)
{
  //Ist es die Flagge?
  if (pFlagPole != pFlag)
    return;
  //Punkte f�r die Angreifer
  for (var clonk in aAttackers)
    if(clonk)
      //Punkte bei Belohnungssystem (Flaggenpostenneutralisierung)
      DoPlayerPoints(BonusPoints("Protection"), RWDS_TeamPoints, GetOwner(clonk), clonk, IC13);

  //Eventmessages
  for (var i; i < GetPlayerCount(); i++)
    if (GetPlayerTeam(GetPlayerByIndex(i)) == iOldTeam)
      EventInfo4K(GetPlayerByIndex(i)+1, Format("$MsgFlagLost$", GetName(pFlag), GetTeamColor(iNewTeam), GetTeamName(iNewTeam)), OFLG, 0, GetTeamColor(iNewTeam), 0, "Info.ogg");
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
        //Punkte bei Belohnungssystem (Flaggenpostenr�ckeroberung)
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

  //Nachricht �ber Flaggeneroberung
  EventInfo4K(0, Format("$MsgCaptured$", GetTeamColor(iTeam), GetTeamName(iTeam), GetName(pFlag)), OFLG, 0, GetTeamColor(iTeam), 0, "Info.ogg");
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
  SetScoreboardData(SBRD_Caption, GHTF_Name, Format("%d $Points$", iGoal));
  SetScoreboardData(SBRD_Caption, GHTF_Points, "{{GHTF}}");
  SetScoreboardData(SBRD_Caption, GHTF_Progress, "{{OFPL}}");

  //Flaggenzeile
  var color = GetTeamFlagColor(pFlag->GetTeam()),
  prog = pFlag->GetProcess();
  color = InterpolateRGBa3(RGBa(255, 255, 255), color, prog, 100);
  SetScoreboardData(GHTF_FlagRow, GHTF_Name, Format("<c %x>%s</c>", color, GetName(pFlag)));
  SetScoreboardData(GHTF_FlagRow, GHTF_Progress, Format("<c %x>%d%</c>", color, prog), GHTF_FlagRow);
  var icon, trend = pFlag->GetTrend();
  if (!trend) icon = IC12;
  if (trend == -1) icon = IC13;
  if (trend == 1) icon = IC10;
  SetScoreboardData(GHTF_FlagRow, GHTF_Points, Format("{{%i}}", icon), GHTF_FlagRow-1);

  //Leere Zeile
  SetScoreboardData(GHTF_FlagRow-1, GHTF_Progress, "", GHTF_FlagRow-1);
  SetScoreboardData(GHTF_FlagRow-1, GHTF_Points, "", GHTF_FlagRow-1);

  //Alle Teams
  var iFlagTeam = pFlag->~GetTeam();
  for (var i, j; i < GetTeamCount(); j++)
  {
    //Team gibts. Hochz�hlen
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
  //Und dann nochmal nach Punkten. Damit bei gleicher Punktzahl das Team vorne ist, das gerade die Flagge h�lt
  SortScoreboard(GHTF_Points, true);
}

public func GetTeamFlagColor(int iTeam)
{
  //Team hat mehrere Spieler: Teamfarbe
  if (GetTeamPlayerCount(iTeam) != 1)
    return GetTeamColor(iTeam);
  //Ein Spieler: Dessen Spielerfarbe
  for (var i; i < GetPlayerCount(i); i++)
    if (GetPlayerTeam(GetPlayerByIndex(i)) == iTeam)
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

      //Nachricht �ber Gewinner
      Message("@$TeamHasWon$", 0, GetTeamColor(i), GetTeamName(i));

      //Sound
      Sound("Cheer.ogg", true);

      return fulfilled = true;
    }
  //Nur noch eins �brig
  if (GetActiveTeamCount() <= 1)
  {
    var i = GetPlayerTeam(GetPlayerByIndex());

    //Spielende planen
    Schedule("GameOver()", 150);

    //Auswertung
    RewardEvaluation();

    //Nachricht �ber Gewinner
    Message("@$TeamHasWon$", 0, GetTeamColor(i), GetTeamName(i));

    //Sound
    Sound("Cheer.ogg", true);

    return fulfilled = true;  
  }
}

/* Ungenutze Funktionen */

private func InitScoreboard()			{}
private func InitMultiplayerTeam(int iTeam)	{}
private func RemoveMultiplayerTeam(int iTeam)	{}
private func InitSingleplayerTeam(int iPlr)	{}
private func RemoveSingleplayerTeam(int iPlr)	{}
private func InitPlayer(int iPlr)		{}
private func RemoveScoreboardPlayer(int iPlr)	{}
public func WinScoreChange(int iNewScore)	{}
private func SortTeamScoreboard()		{}

private func TeamGetScore(int iTeam)
{
  return aTeamPoints[iTeam];
}