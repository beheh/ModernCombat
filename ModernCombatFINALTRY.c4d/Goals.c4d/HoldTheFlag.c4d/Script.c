/*-- Hold the Flag --*/

#strict 2

local pFlag;		//Diese Flagge gilt es zu erobern
local aTeamPoints;	//Die Punkte jedes Teams
local iProgress;	//Bei 100% bekommt das Team einen Punkt
local iGoal;		//So viele Punkte gilt es zu erreichen

/* Initialisierung */

protected func Initialize() {
  iGoal = 20;
}

public func ChooserFinished() {
  pFlag = FindObject(OFPL);
  aTeamPoints = [];
  AddEffect("IntAddProgress", this, 1, 10, this);
}

/* Konfiguration */

public func IsConfigurable() { return true; }

public func ConfigMenu(object pCaller) {
  OpenGoalMenu();
  return 1;
}

private func ConfigFinished() {
  var chos = FindObject(CHOS);
  if(chos)
    chos->OpenMenu();
}

private func OpenGoalMenu(id dummy, int iSelection) {
  var pClonk = GetCursor();
  CreateMenu(GetID(),pClonk,0,0,0,0,1);

  AddMenuItem(" ", "OpenGoalMenu", GetID(), pClonk, iGoal, 0, " ");
  AddMenuItem("$MorePoints$", "ChangeWinpoints", CHOS, pClonk, 0, +1, "$MorePoints$",2,1);
  AddMenuItem("$LessPoints$", "ChangeWinpoints", CHOS, pClonk, 0, -1, "$LessPoints$",2,2);
  AddMenuItem("$Finished$", "ConfigFinished", CHOS, pClonk,0,0,"$Finished$",2,3);

  SelectMenuItem(iSelection, pClonk);
}

private func ChangeWinpoints(id dummy, int iChange) {
  //Stand ver�ndern
  iGoal = BoundBy(iGoal+iChange,1,100); //100 ist Maximum.
  //Ger�usch
  Sound("Grab", 1,0,0,1);
  //Men� wieder �ffnen
  var iSel = 1;
  if(iChange == -1) iSel = 2;
  OpenGoalMenu(0, iSel);
}

/* Effekt */

protected func FxIntAddProgressTimer() {
  //Keine Flagge?
  if (!pFlag && !(pFlag = FindObject(OFPL)))
    return;

  //Scoreboard
  UpdateScoreboard();

  var team = pFlag->~GetTeam();
  //Keiner hat sie?
  if (team == -1) return;
  //Nicht vollst�ndig eingenommen?
  if (!pFlag->IsFullyCaptured()) {
    iProgress = 0;
	//Nochmal updaten damit nicht das falsche Team die Prozente zugeschrieben bekommt
	return UpdateScoreboard();
  }

  //Punkte vergeben
  if ((++iProgress) >= 100) {
    aTeamPoints[team]++;
	iProgress = 0;
  }
  
  //Gewonnen?
  if (!EffectVar(0, Par(), Par(1))) //Bla.
    EffectVar(0, Par(), Par(1)) = IsFulfilled();
}

/* Zeug */

protected func Activate(iPlr) {
  return MessageWindow(GetDesc(), iPlr);
}

global func EliminateTeam(int iTeam) {
  for(var i; i < GetPlayerCount(); i++)
    if(GetPlayerTeam(GetPlayerByIndex(i)) == iTeam)
      EliminatePlayer(GetPlayerByIndex(i));
}

global func GetTaggedTeamName(int iTeam) {
  if (!GetTeamName(iTeam)) return;
  return Format("<c %x>%s</c>", GetTeamColor(iTeam), GetTeamName(iTeam));
}

protected func RelaunchPlayer(int iPlr, object pCrew, int iMurdererPlr, int iTeam, no_relaunch) {
  if(!FindObject(CHOS) && !FindObject(MCSL))
    CreateGOCCSpawner(pCrew);
}

public func IsTeamGoal() {return 1;}

public func SetFlag(object pFlagPole) {
  if (pFlagPole)
    return pFlag = pFlagPole;
}

public func GetFlag() {
  return pFlag;
}

public func FlagLost(object pFlagPole, int iOldTeam, int iNewTeam, array aAttackers) {
  //Ist es die Flagge?
  if (pFlagPole != pFlag)
    return;
  //Punkte f�r die Angreifer
  for (var clonk in aAttackers)
    if (clonk)
	  DoPlayerPoints(10, 13, GetOwner(clonk), clonk, IC13);
}

public func FlagCaptured(object pFlagPole, int iTeam, array aAttackers, bool fRegained) {
  //Ist es die Flagge?
  if (pFlagPole != pFlag)
	return;
	
  var first = true; //Der erste bekommt mehr Punkte, der Rest bekommt Assistpunkte
  for (var clonk in aAttackers)
    if (clonk) {
	  if (fRegained)
	    DoPlayerPoints(30, 12, GetOwner(clonk), clonk, IC12);
	  else {
	    if (first)
	      DoPlayerPoints(50, 10, GetOwner(clonk), clonk, IC10);
		else
		  DoPlayerPoints(10, 11, GetOwner(clonk), clonk, IC11);
	  }
	  first = false;
	}
}

/* Scoreboard */

static const GHTF_Name = SBRD_Caption;
static const GHTF_Points = 0;
static const GHTF_Progress = 1;

public func UpdateScoreboard() {
  //Wird noch eingestellt
  if (FindObject(CHOS))
    return;
	
  //Titelzeile
  SetScoreboardData(SBRD_Caption, GHTF_Name, Format("%s (%d)", GetName(), iGoal));
  SetScoreboardData(SBRD_Caption, GHTF_Points, "{{GHTF}}");
  SetScoreboardData(SBRD_Caption, GHTF_Progress, "{{OFPL}}");
  
  //Und... alle Teams...
  var iFlagTeam = pFlag->~GetTeam();
  for (var i, j; i < GetTeamCount(); j++) {
    //Team gibts gar nicht
    if (!GetTeamName(j)) {
	  SetScoreboardData(j, GHTF_Name);
	  SetScoreboardData(j, GHTF_Points);
	  SetScoreboardData(j, GHTF_Progress);
	  continue;
	}
	//Team gibts. Hochz�hlen
	i++;
	SetScoreboardData(j, GHTF_Name, GetTaggedTeamName(j));
	SetScoreboardData(j, GHTF_Points, Format("<c %x>%d</c>", GetTeamColor(j), aTeamPoints[j]), aTeamPoints[j]);
	//Team hat die Flagge
	if (j == iFlagTeam)
	  SetScoreboardData(j, GHTF_Progress, Format("<c %x>%02d%</c>", GetTeamColor(j), iProgress), iProgress);
	else
	  SetScoreboardData(j, GHTF_Progress, Format("<c %x>00%</c>", GetTeamColor(j)), -1);
  }
  
  //So... Erstmal nach Flaggenstatus sortieren
  SortScoreboard(GHTF_Progress, true);
  //Und dann nochmal nach Punkten. Damit bei gleicher Punktzahl das Team vorne ist, das gerade die Flagge h�lt
  SortScoreboard(GHTF_Points, true);
}

/* Sieg */

public func IsFulfilled() {
  for (var i; i < GetLength(aTeamPoints); i++)
    if (aTeamPoints[i] >= iGoal) {
	  //Team hat gewonnen. Alle anderen durchgehen und eliminieren
	  for (var team, j = GetTeamCount(); j; team++)
	    if (GetTeamName(team)) {
		  j--;
		  if (team != i)
		    EliminateTeam(team);
		}
	  Schedule("GameOver()", 150);
	  return true;
    }
}