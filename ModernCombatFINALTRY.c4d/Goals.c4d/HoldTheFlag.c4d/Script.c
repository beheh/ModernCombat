/*-- Hold the Flag --*/

#strict 2
#include TEAM

local pFlag;					//HTF-Flaggenposten
local aTeamPoints;				//Punkte aller Teams
local iProgress;				//Fortschritt des Besitzerteams
local iGoal;					//Zu erreichende Punktezahl

public func GoalExtraValue()	{return iGoal;}	//Spielzielinformationen an Scoreboard weitergeben


/* Initialisierung */

protected func Initialize()
{
  //Standardpunktezahl
  iGoal = 10;
  aKill = [];
  aDeath = [];}

public func Activate(iPlr)
{
  return MessageWindow(GetDesc(), iPlr);
}

public func ChooserFinished()
{
  aTeamPoints = [];
  ScheduleCall(this, "UpdateFlag", 1);

  //Effekt überwacht den Spielverlauf
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
  //Flaggenposten ermitteln
  pFlag = FindObject2(Find_Func("IsFlagpole"));
  if(!pFlag) return;

  //Einnahmegeschwindigkeit ermitteln
  pFlag->Set(GetName(pFlag), pFlag->GetRange(), Max(18 - GetActiveTeamCount() * 4, 4));

  return true;
}

/* Konfiguration */

public func IsConfigurable()	{return true;}

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

/* Hostmenü */

private func OpenGoalMenu(id dummy, int iSelection)
{
  var pClonk = GetCursor(iChoosedPlr);
  CreateMenu(GetID(),pClonk,0,0,0,0,1);

  //Punkteanzeige
  AddMenuItem("$Points$", 0, IC12, pClonk, iGoal);

  //Punktelimits ermitteln
  var uplimit = 40; var downlimit = 5;
  if(GetLeague())
  {
    uplimit = 30; downlimit = 5;
  }

  //Mehr Punkte
  if(iGoal < uplimit)
    AddMenuItem("$MorePoints$", "ChangeWinpoints", GOCC, pClonk, 0, +1, "$MorePoints$",2,1); else
    AddMenuItem("<c 777777>$MorePoints$</c>", "ChangeWinpoints", GOCC, pClonk, 0, +1, "$MorePoints$",2,1);

  //Weniger Punkte
  if(iGoal > downlimit)
    AddMenuItem("$LessPoints$", "ChangeWinpoints", GOCC, pClonk, 0, -1, "$MorePoints$",2,2); else
    AddMenuItem("<c 777777>$LessPoints$</c>", "ChangeWinpoints", GOCC, pClonk, 0, -1, "$MorePoints$",2,2);

  //Fertig
  AddMenuItem("$Finished$", "ConfigFinished", GOCC, pClonk,0,0,"$Finished$",2,3);

  SelectMenuItem(iSelection, pClonk);
}

private func ChangeWinpoints(id dummy, int iChange)
{
  //Sound
  Sound("Grab", 1,0,0,1);

  //Zu erreichenden Punktezahl verändern
  if(!GetLeague())
    iGoal = BoundBy(iGoal+iChange,5,40);
  else
    iGoal = BoundBy(iGoal+iChange,5,30);

  //Menü erneut öffnen
  var iSel = 1;
  if(iChange == -1) iSel = 2;
  OpenGoalMenu(0, iSel);
}

/* Effekt */

protected func FxIntAddProgressTimer()
{
  if(!pFlag && !SetFlag(FindObject(OFPL))) return;

  UpdateScoreboard();

  //Besitzer des Flaggenpostens ermitteln
  var team = pFlag->~GetTeam();

  //Kein Besitzer: Nichts weiter unternehmen
  if(team == -1) return;

  //Nicht vollständig eingenommen: Fortschritt zurücksetzen
  if(!pFlag->IsFullyCaptured())
  {
    iProgress = 0;

    //Update um Prozente nicht falschen Teams zuzuschreiben
    return UpdateScoreboard();
  }

  var warning = BoundBy(iGoal*3/4, iGoal-5, iGoal-1);
  //Punkt erspielt: Entsprechendem Team zuschreiben
  if((++iProgress) >= 100)
  {
    //Punkt vergeben
    aTeamPoints[team]++;
    //Fortschritt zurücksetzen
    iProgress = 0;

    UpdateHUDs();

    //Punkte bei Belohnungssystem
    for(var i; i < GetPlayerCount(); i++)
    {
      if(GetPlayerTeam(GetPlayerByIndex(i)) == team)
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

  //Nach Gewinner prüfen
  if(!EffectVar(0, Par(), Par(1)))
    EffectVar(0, Par(), Par(1)) = IsFulfilled();
}

/* Zeug */

public func IsTeamGoal()	{return 1;}

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
  if(pFlagPole != pFlag)
    return;

  var first = true; //Der erste bekommt mehr Punkte, der Rest bekommt Assistpunkte
  for(var clonk in aAttackers)
    if(clonk)
    {
      if(fRegained)
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

static const GHTF_FlagColumn		= 1;
static const GHTF_ProgressColumn	= 2;
static const GHTF_PointsColumn		= 3;
static const GHTF_FlagRow		= 1024;

public func InitScoreboard() 
{
  //Wird noch eingestellt
  if(FindObject(CHOS) || IsFulfilled()) return;

  UpdateHUDs();

  //Titelzeile
  SetScoreboardData(SBRD_Caption, SBRD_Caption, Format("%s",GetName()), SBRD_Caption);

  //Spaltentitel
  SetScoreboardData(SBRD_Caption, GHTF_FlagColumn, "{{IC12}}", SBRD_Caption);
  SetScoreboardData(SBRD_Caption, GHTF_ProgressColumn, " ", SBRD_Caption);
  SetScoreboardData(SBRD_Caption, GHTF_PointsColumn, "{{SM02}}", SBRD_Caption);

  UpdateScoreboard();
}

public func UpdateScoreboard()
{
  //Wird noch eingestellt
  if(FindObject(CHOS) || !pFlag || IsFulfilled()) return;

  //Zeileniterator
  var i = 0;

  //Teamfarbe und Flaggenzustand ermitteln
  var teamclr = GetTeamFlagColor(pFlag->GetTeam()),
  prog = pFlag->GetProcess();
  var percentclr = InterpolateRGBa3(RGBa(255, 255, 255), teamclr, prog, 100);

  //Flaggennamenfarbe ermitteln
  if(!pFlag->~IsFullyCaptured())
    var nameclr = RGB(255,255,255);
  else
    var nameclr = teamclr;
    
  //Flaggenicon ermitteln
  var icon, trend = pFlag->GetTrend();
  if(!trend)		icon = SM21;	//Keine Aktivität
  if(trend == -1)	icon = SM23;	//Angriff
  if(trend == 1)	icon = SM22;	//Verteidigung

  SetScoreboardData(i, GHTF_FlagColumn, Format("<c %x>%s</c>", nameclr, GetName(pFlag)));
  SetScoreboardData(i, GHTF_ProgressColumn, Format("{{%i}}", icon), GHTF_FlagRow-i);
  SetScoreboardData(i, GHTF_PointsColumn, Format("<c %x>%d%</c>", percentclr, prog), GHTF_FlagRow-i);

  i++;

  //Leere Zeile
  SetScoreboardData(i, GHTF_FlagColumn, " ");
  SetScoreboardData(i, GHTF_ProgressColumn, " ", GHTF_FlagRow-i);
  SetScoreboardData(i, GHTF_PointsColumn, " ", GHTF_FlagRow-i);

  i++;

  //Benötigte Punktzahl
  SetScoreboardData(i, GHTF_FlagColumn, "$SbrdGoalPoints$");
  SetScoreboardData(i, GHTF_ProgressColumn, " ", GHTF_FlagRow-i);
  SetScoreboardData(i, GHTF_PointsColumn, Format("%d", iGoal), GHTF_FlagRow-i);

  i++;

  //Leere Zeile
  SetScoreboardData(i, GHTF_FlagColumn, " ");
  SetScoreboardData(i, GHTF_ProgressColumn, " ", GHTF_FlagRow-i);
  SetScoreboardData(i, GHTF_PointsColumn, " ", GHTF_FlagRow-i);

  i++;

  //Icons
  SetScoreboardData(i, GHTF_FlagColumn, "{{SM26}}");
  SetScoreboardData(i, GHTF_ProgressColumn, "{{SM27}}", GHTF_FlagRow-i);
  SetScoreboardData(i, GHTF_PointsColumn, "{{IC09}}", GHTF_FlagRow-i);

  i++;

  for(var j = 0; j < GetTeamCount(); j++)
  {
    var iTeam = GetTeamByIndex(j);

    if(GetTeamName(iTeam) && GetTeamPlayerCount(iTeam))
    {
      i++;

      SetScoreboardData(i, GHTF_FlagColumn, Format("<c %x>%s</c>", GetTeamFlagColor(iTeam), GetTeamName(iTeam)));

      if(iTeam == pFlag->~GetTeam())
        SetScoreboardData(i, GHTF_ProgressColumn, Format("<c %x>%d</c>", RGB(128, 128, 128), iProgress), iProgress);
      else
        SetScoreboardData(i, GHTF_ProgressColumn, Format("<c %x>%d</c>", RGB(128, 128, 128), 0), 0);

      SetScoreboardData(i, GHTF_PointsColumn, Format("%d", aTeamPoints[iTeam]), aTeamPoints[iTeam]);
    }
  }

  //Sortieren
  SortScoreboard(GHTF_ProgressColumn, true);
  SortScoreboard(GHTF_PointsColumn, true);
}

public func GetTeamFlagColor(int iTeam)
{
  //Team hat mehrere Spieler: Teamfarbe
  if(GetTeamPlayerCount(iTeam) != 1)
    return GetTeamColor(iTeam);
  //Ein Spieler: Dessen Spielerfarbe
  for(var i; i < GetPlayerCount(); i++)
    if(GetPlayerTeam(GetPlayerByIndex(i)) == iTeam)
      return GetPlrColorDw(GetPlayerByIndex(i));
  return GetTeamColor(iTeam);
}

/* Sieg */

local fulfilled;

public func IsFulfilled()
{
  if(FindObject(CHOS)) return;

  if(fulfilled) return true;

  //Punktestände prüfen
  for(var i; i < GetLength(aTeamPoints); i++)
    //Ein Team hat die Punktezahl erreicht: Sieg
    if(aTeamPoints[i] >= iGoal)
    {
      //Verlierer eliminieren
      for(var team, j = GetTeamCount(); j; team++)
      if(GetTeamName(team))
      {
        j--;
        if(team != i)
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

  //Nur ein Team vorhanden: Gewinnen lassen
  if(GetActiveTeamCount() <= 1)
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

private func InitMultiplayerTeam(int iTeam)					{}
private func RemoveMultiplayerTeam(int iTeam)					{}
private func InitSingleplayerTeam(int iPlr)					{}
private func RemoveSingleplayerTeam(int iPlr)					{}
private func InitPlayer(int iPlr)						{}
private func RemoveScoreboardPlayer(int iPlr)					{}
public func WinScoreChange(int iNewScore)					{}
private func SortTeamScoreboard()						{}
public func RelaunchScoreboard(int iPlr, object pClonk, int iMurdererPlr)	{}