/*-- Spielregelwahl --*/

//Erweitert die Spielregelwahl und erlaubt unter anderem Voreinstellungen der Regeln innerhalb eines Szenarios.

#strict 2
#appendto CHOS

local iEffectCount;
local iChosenPlr;
local iTeamCount, arTeams, aTeamCaptains;
local iTeamMode, iUsedTeamSort;
local fRandomMenu;
local iTeamCaptainState;

static const CHOS_LeagueAutostart = 60;	//Zeit bis zum automatischen Rundenstart bei Ligarunden

protected func RecommendedGoals()	{return GameCall("RecommendedGoals");}
protected func MinTeamCount()		{return Min(GetPlayerCount(), 2);}


/* Initialisierung */

protected func Initialize()
{
  //Scoreboard initialisieren
  ScheduleCall(this, "InitScoreboard", 3);

  SetPosition();
  aRules = CreateArray();
  aGoals = CreateArray();
  aTempGoalSave = CreateArray();
  aAI = CreateArray();

  //Spielziele verzögert entfernen
  ScheduleCall(this, "RemoveGoals", 1);

  //Host identifizieren
  iChosenPlr = -1;
  ScheduleCall(this, "ChoosePlayer", 15);

  //Dunkelheit ermitteln
  iDarkCount = ObjectCount(DARK);
  
  //Anz. der Teams angeben
  iTeamCount = GetTeamCount();
  arTeams = [];
  ScheduleCall(this, "InitializeTeam", 4);

  aPlayerSetting = [];
  aTeamCaptains = [];

  //Regeln voreinstellen
  LoadRuleCfg();

  //Effektstufe setzen
  iEffectCount = 3;
  EFSM_SetEffects(3);

  //Spielerränge ermitteln
  iAchievementCount = 0;
  while(GetName(0, C4Id(Format("AC%02d", iAchievementCount+1))))
  {
    iAchievementCount++;
  }
  
  //Falls in der Lobby zufällig & unsichtbar gewählt wurde, entsprechende Eigenvariante einsetzen
  if(!GetTeamConfig(TEAM_AutoGenerateTeams) && GetTeamConfig(TEAM_Dist) == 3)
    ScheduleCall(this, "CreateTeams", 5, 0, 2-GetTeamConfig(TEAM_AutoGenerateTeams), CHOS_TeamRandomInvisible, true);
}

public func FxLeagueAutostartTimer(object pTarget, int iNr, int iTime)
{
  if(CHOS_LeagueAutostart - (iTime/35) < 10)
    Sound("Select.ogg", true);

  if(iTime >= CHOS_LeagueAutostart*35)
  {
    CloseMenu(GetCursor(iChosenPlr));
    ConfigurationFinished();
    return -1;
  }
}

/* Statusanzeige im Scoreboard */

static const CHOS_SBRD_Chooser	= SBRD_Caption;
static const CHOS_SBRD_Goal	= 0;
static const CHOS_SBRD_Rules	= 1;
static const CHOS_SBRD_Effect	= 2;
static const CHOS_SBRD_Darkness	= 3;
static const CHOS_SBRD_Timer	= 4;
static const CHOS_SBRD_Teams	= 5;

public func InitScoreboard()
{
  //Scoreboard einblenden
  Schedule("DoScoreboardShow(1)", 1);

  //Zeilen setzen
  SetScoreboardData(SBRD_Caption, SBRD_Caption, 0, 0, true);
  SetScoreboardData(CHOS_SBRD_Chooser, 0, "$ScoreboardChooser$", CHOS_SBRD_Chooser, true);
  SetScoreboardData(CHOS_SBRD_Goal, 0, "$ScoreboardGoals$", CHOS_SBRD_Goal, true);
  SetScoreboardData(CHOS_SBRD_Rules, 0, "$ScoreboardRules$", CHOS_SBRD_Rules, true);
  SetScoreboardData(CHOS_SBRD_Effect, 0, "$ScoreboardEffects$", CHOS_SBRD_Effect, true);
  SetScoreboardData(CHOS_SBRD_Darkness, 0, "$ScoreboardDarkness$", CHOS_SBRD_Darkness, true);
  SetScoreboardData(CHOS_SBRD_Teams, 0, "$ScoreboardTeams$", CHOS_SBRD_Teams, true);
  SetScoreboardData(CHOS_SBRD_Teams, 1, "$TeamsSortedManually$", 0, true);
  if(GetLeague())
  {
    SetScoreboardData(CHOS_SBRD_Timer, 0, "$ScoreboardTimer$", CHOS_SBRD_Timer, true);
    SetScoreboardData(CHOS_SBRD_Timer, 1, "-", 0, true);
  }

  SortScoreboard(0);
  AddEffect("ChooserScoreboard", this, 21, 10, this);
}

global func SetScoreboardData(int iRowID, int iColID, string sText, int iData, bool fChooser)	//Bearbeitung des Scoreboard während des Choosers benötigt fChooser
{
  if(FindObject(CHOS) && !fChooser)
  {
    return false;
  }

  return _inherited(iRowID, iColID, sText, iData);
}

public func FxChooserScoreboardTimer(object target, nr, time)
{
  target->~UpdateScoreboard();
}

public func UpdateScoreboard()
{
  //Host-Spieler
  SetScoreboardData(CHOS_SBRD_Chooser, 1, GetPlayerName(iChosenPlr), 0, true);

  //Spielziele
  var str_goals = "", str_extra = "";
  if(!pGoal)
  {
    for(var goal2 in aGoals)
    {
      str_goals = Format("%s{{%i}}", str_goals, goal2);
    }
  }
  else
  {
    str_goals = Format("%s ", GetName(pGoal));
    var extraval = pGoal->~GoalExtraValue();
    //Extrawerte für Spielziele
    if(extraval)
    {
      if(GetType(extraval) == C4V_Int)
        str_extra = Format("(%d)", extraval);
      else
        if(GetType(extraval) == C4V_String)
          str_extra = Format("(%s)", extraval);
    }
  }
  SetScoreboardData(CHOS_SBRD_Goal, 1, Format("%s%s", str_goals, str_extra), 0, true);

  //Regeln
  var str_rules = "";
  for(var i = 0, j = 0, idR; idR = GetDefinition(i, Chooser_Cat) ; i++)
  {
    if(idR->~IsChooseable() && aRules[i])
    {
      str_rules = Format("%s{{%i}}", str_rules, idR);
      j++;
    }
  }
  SetScoreboardData(CHOS_SBRD_Rules, 1, str_rules, 0, true);

  //Effektstufe
  SetScoreboardData(CHOS_SBRD_Effect, 1, Format("%dx", iEffectCount), 0, true);

  //Dunkelheitsstufe
  if(FindObject(DARK))
  {
    SetScoreboardData(CHOS_SBRD_Darkness, 0, "$ScoreboardDarkness$", CHOS_SBRD_Darkness, true);
    SetScoreboardData(CHOS_SBRD_Darkness, 1, Format("%dx", iDarkCount), 0, true);
  }
  else
  {
    SetScoreboardData(CHOS_SBRD_Darkness, 0, "<c 777777>$ScoreboardDarkness$</c>", CHOS_SBRD_Darkness, true);
    SetScoreboardData(CHOS_SBRD_Darkness, 1, Format("<c 777777>%dx</c>", iDarkCount), 0, true);
  }

  //Teamverteilung und Spielerränge
  for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
  {
    var row_id = CHOS_SBRD_Teams + 1 + i;
    var plr = GetPlayerByIndex(i, C4PT_User);
    var player_name = GetTaggedPlayerName(plr, true);
    var team_name;
    if(iTeamMode == CHOS_TeamRotation)
    {
      if(aTeamCaptains[plr] || !iTeamCaptainState)
        team_name = GetTeamName(GetPlayerTeam(plr));
      else
        team_name = "$NoCaptain$";
    }
    else
    {
      if((iTeamMode != CHOS_TeamRandomInvisible && !fRandomMenu) || aPlayerSetting[plr])
        team_name = GetTeamName(GetPlayerTeam(plr));
      else
        team_name = "$Random$";
    }

    SetScoreboardData(row_id, 0, GetTaggedPlayerName(plr, true), CHOS_SBRD_Teams+plr, true);
    SetScoreboardData(row_id, 1, team_name, 0, true);
  }

  if(GetLeague() && GetEffect("LeagueAutostart", this))
    SetScoreboardData(CHOS_SBRD_Timer, 1, Format("%d", CHOS_LeagueAutostart - (GetEffect("LeagueAutostart", this, 0, 6)/35)), 0, true);

  SortScoreboard(0);
}

public func IsInRandomTeam(int iPlr)
{
  if(iTeamMode == CHOS_TeamRotation)
    if(!aTeamCaptains[iPlr] && iTeamCaptainState)
      return true;

  if(iTeamMode != CHOS_TeamRandomInvisible && !fRandomMenu)
    return false;
  if(!aPlayerSetting[iPlr])
    return true;

  return false;
}

public func RemovePlayer(int iPlr)
{
  if(GetType(aTeamMenu) == C4V_Array)
  {
    var sel = GetMenuSelection(GetCursor(iChosenPlr));
    for(var i = 0; i < GetPlayerCount(); i++)
      if(GetPlayerByIndex(i) == iPlr)
        break;

    if(i <= sel)
      sel--;

    CloseMenu(GetCursor(iChosenPlr));
    if(aTeamMenu[0] == 1)
      ScheduleCall(this, "OpenTeamMenu", 3, 0, aTeamMenu[0], sel);
    else
      ScheduleCall(this, "SelectPredefinedTeamMember", 3, 0, aTeamMenu[1], sel, aTeamMenu[3], aTeamMenu[4]);
  }

  var count = 0;
  for(var i = 0; i < GetLength(arTeams); i++) //Bei mehr Teams als Spielern verhindern
  {
    if(arTeams[i])
      count++;
  }

  if(GetPlayerCount(C4PT_User)-1 < count && count > 1)
    arTeams[GetIndexOf(true, arTeams)] = false;

  //Engine-erstellte Teams
  if(GetTeamConfig(TEAM_AutoGenerateTeams))
    iTeamCount = BoundBy(iTeamCount - 1, 1, GetPlayerCount(C4PT_User)-1);

  ClearScoreboard(CHOS_SBRD_Teams + GetPlayerCount()+2, 2);
  InitScoreboard();
  ScheduleCall(this, "UpdateScoreboard", 1);

  //Neuen Host identifizieren und Menü öffnen
  if(iChosenPlr == iPlr)
  {
    iChosenPlr = -1;
    ScheduleCall(this, "ChoosePlayer", 15);
    ScheduleCall(this, "OpenMenu", 16);
  }

  return true;
}

global func ClearScoreboard(int iRow, int iCol)
{
  var row = SBRD_Caption;
  var col = 0;
  var search = true;
  while(search)
  {
    //Eintrag vorhanden?
    if(GetScoreboardString(row, col))
    {
      //Eintrag überschreiben und somit löschen
      SetScoreboardData(row, col, 0, 0);
    }
    //Zelle nicht gegeben, Reaktion auf leere Zellen
    if(iCol == -2 || iRow == -2)
    {
      row++;
      col = SBRD_Caption;
      if(!GetScoreboardString(row, col))
      {
        search = false;
        break;
      }
      continue;
    }
    //Zelle gegeben
    else if(col == iCol)
    {
      if(row == iRow)
      {
        search = false;
        break;
      }
      row++;
      col = SBRD_Caption;
      continue;
    }
    col++;
  }

  return true;
}

/* Host-Identifizierung */

private func ChoosePlayer()
{
  var lowest = [0x7FFFFFFF, -1];
  for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
  {
    var plr = GetPlayerByIndex(i, C4PT_User);
    if(GetPlrClientNr(plr) < lowest[0])
      lowest = [GetPlrClientNr(plr), plr];
  }

  return iChosenPlr = lowest[1];
}

/* Nur eine Mitteilung für Neugierige */

protected func Activate(iPlr)
{
  if(GetEffect("TeamRotationChoosePlr", this) || GetEffect("EvaluateGoalVote", this))
    return false;

  if(iPlr == iChosenPlr)
    return OpenMenu();
  MessageWindow(Format("$Choosing$", GetPlayerName(iChosenPlr)), iPlr);
}

local blocked_teams;

protected func OpenMenu()
{
  aTeamMenu = 0;
  fRandomMenu = false;
  if(GetLength(aGoals))
    return OpenGoalChooseMenu();

  var pClonk = GetCursor(iChosenPlr);
  if(!pClonk)
    return ScheduleCall(this, "OpenMenu", 1);

  if(GetMenu(pClonk))
    CloseMenu(pClonk);

  blocked_teams = GameCall("ChooserBlockedTeams", GetID(pGoal), pGoal);
  if(blocked_teams)
  {
    if(!GetTeamConfig(TEAM_AutoGenerateTeams))
    {
      if(GetType(blocked_teams) == C4V_Array)
      {
        var teams = [];
        for(var i = 0; i < GetTeamCount(); i++)
        {
          var team = GetTeamByIndex(i);
          if(GetIndexOf(team, blocked_teams) == -1)
            teams[GetLength(teams)] = team;
          else
            arTeams[team] = false;
        }
        var j = 0;

        for(var i = 0; i < GetPlayerCount(); i++)
        {
          var plr = GetPlayerByIndex(i);
          if((iTeamMode != CHOS_TeamRandomInvisible && !fRandomMenu) || aPlayerSetting[plr] || GetIndexOf(GetPlayerTeam(plr), blocked_teams) != -1)
          {
            SetPlayerTeam(plr, teams[((++j) %= GetLength(teams))]);
            arTeams[teams[j]] = true;
          }
        }
      }
    }
    else if(GetType(blocked_teams) == C4V_Int)
    {
      if(!iTeamCount || (iTeamCount >= blocked_teams && (blocked_teams-1) <= GetPlayerCount(C4PT_User)))
        iTeamCount = blocked_teams - 1;

      for(var i = 0, j = 0; i < GetPlayerCount(); i++)
      {
        var plr = GetPlayerByIndex(i);
        if((iTeamMode != CHOS_TeamRandomInvisible && !fRandomMenu) || aPlayerSetting[plr] || GetPlayerTeam(plr) >= blocked_teams)
          SetPlayerTeam(plr, ((++j) %= blocked_teams));
      } 
    }
  }

  Message("", pClonk);

  CreateMenu(GetID(), pClonk, 0, 0, 0, 0, 1);
  if(!GetLeague())
  {
    //Spielregeln
    AddMenuItem("$CreateRules$", "OpenRuleMenu", CTFL, pClonk, 0,0, "$RuleInfo$");
    //Dunkelheit
    if(IsDark())
      AddMenuItem("%s", "OpenDarknessMenu", DARK, pClonk,0,0,"$DarknessChose$");
  }
  //Spielziel
  if(pGoal && pGoal->~IsConfigurable())
    AddMenuItem("%s", "OpenGoalMenu", GetID(pGoal), pClonk,0,0,"$GoalChose$");
  //Teameinteilung
  if(GetTeamCount() && !GetLeague())
    AddMenuItem("$TeamArrangement$", "OpenTeamMenu", TEAM, pClonk, 0, 0, "$TeamInfo$");
  //KI
  if(ObjectCount(WAYP) && !GetLeague())
    AddMenuItem("$AIMenu$", "OpenAIMenu", HZCK, pClonk, 0,0, "$AIInfo$");
  //Effekte
  AddMenuItem("$Effects$", "OpenEffectMenu", EFMN, pClonk, 0,0, "$EffectInfo$");
  //Menüweitergabe
  AddMenuItem("$ChangeHost$", "ChangeHostMenu", MCMC, pClonk, 0,0, "$ChangeHostInfo$");
  //Fertig
  AddMenuItem("$Finished$", "ConfigurationFinished", GOCC, pClonk,0,0,"$Finished$",2,3);
}

/* Regelmenü */

protected func OpenRuleMenu(id dummy, int iSelection)
{
  var pClonk = GetCursor(iChosenPlr);
  //Menü erstellen
  CreateMenu(GetID(), pClonk);
  //Regeln einfügen
  for(var i=0, idR, def, j, check ; idR = GetDefinition(i, Chooser_Cat) ; i++)
  {
    if(DefinitionCall(idR, "IsChooseable") && !GetLength(FindObjects(Find_ID(idR))))
    {
      def = CreateObject(idR, 0,0, -1);
      if(aRules[i])
      {
        SetGraphics("Chosen", def, WPCH, 1, GFXOV_MODE_Picture);
        SetObjDrawTransform(650, 0, 5000, 0, 650, 5000, def, 1);
      }
      
      AddMenuItem("%s", "ChangeRuleConf", idR, pClonk, 0, i, 0, 4, def);
      
      if(i == iSelection)
        check = true;
      
      if(!check)
        j++;
      
      RemoveObject(def);
    }
  }
  //Fertig-Eintrag hinzufügen
  AddMenuItem("$Finished$", "OpenMenu", GOCC, pClonk,0,0,0,2,3);
  //Menüeintrag auswählen
  SelectMenuItem(j, pClonk);
}

protected func OpenEffectMenu(id dummy, int iSelection)
{
  var pClonk = GetCursor(iChosenPlr);
  //Menü aufmachen
  CreateMenu(GetID(), pClonk, 0,0,0,0, 1);
  //Anzeige
  AddMenuItem("$Effects$", 0, EFMN, pClonk, iEffectCount);
  //Zähler erhöhen

  //Weniger Tickets
  if(GetEffectLevel() < 3)
  AddMenuItem("$MoreEffects$", "ChangeEffectConf", GOCC, pClonk, 0, +1, "$MoreEffects$",2,1); else
  AddMenuItem("<c 777777>$MoreEffects$</c>", "ChangeEffectConf", GOCC, pClonk, 0, +1, "$MoreEffects$",2,1);
  //Zähler senken
  if(GetEffectLevel() > 1)
  AddMenuItem("$LessEffects$", "ChangeEffectConf", GOCC, pClonk, 0, -1, "$LessEffects$",2,2); else
  AddMenuItem("<c 777777>$LessEffects$</c>", "ChangeEffectConf", GOCC, pClonk, 0, -1, "$LessEffects$",2,2);
  //Fertig
  AddMenuItem("$Finished$", "OpenMenu", GOCC, pClonk,0,0, "$Finished$",2,3);
  //Letzten Eintrag auswählen
  SelectMenuItem(iSelection, pClonk);
}

protected func ChangeEffectConf(id dummy, int iChange)
{
  //Stand verändern
  iEffectCount = BoundBy(iEffectCount+iChange, 1, 3);
  EFSM_SetEffects(iEffectCount);
  //Geräusch
  Sound("Grab", 1,0,0,1);
  //Menü wieder öffnen
  var iSel = 1;
  if(iChange == -1) iSel = 2;
  OpenEffectMenu(0, iSel);
}

protected func OpenGoalMenu(id dummy, int iSelection)
{
  if(!pGoal)
    return OpenMenu();
  var pClonk = GetCursor(iChosenPlr);
  if(pGoal->~ConfigMenu(pClonk))
    return 1;

  //Menü aufmachen
  CreateMenu(GetID(), pClonk, 0,0,0,0, 1);
  //Anzeige
  AddMenuItem(" ", "OpenGoalMenu", GetID(pGoal), pClonk, GetWinScore2(), 0, " ");
  //Zähler erhöhen
  AddMenuItem("$MoreWinScore$", "ChangeGoalConf", CHOS, pClonk, 0, +1, "$MoreWinScore$",2,1);
  //Zähler senken
  AddMenuItem("$LessWinScore$", "ChangeGoalConf", CHOS, pClonk, 0, -1, "$LessWinScore$",2,2);
  //Fertig
  AddMenuItem("$Finished$", "OpenMenu", CHOS, pClonk,0,0,"$Finished$",2,3);
  //Letzten Eintrag auswählen
  SelectMenuItem(iSelection, pClonk);
  return true;
}

protected func OpenDarknessMenu(id dummy, int iSelection)
{
  var pClonk = GetCursor(iChosenPlr);
  //Menü aufmachen
  CreateMenu(GetID(), pClonk, 0,0,0,0, 1);
  //Dunkelheitsanzeige
  AddMenuItem("$Darkness$", 0, DARK, pClonk, iDarkCount);
  //Zähler erhöhen
  if(iDarkCount < 10)
    AddMenuItem("$MoreDarkness$", "ChangeDARKConf", GOCC, pClonk, 0, +1, "$MoreDarkness$",2,1); else
    AddMenuItem("<c 777777>$MoreDarkness$</c>", "ChangeDARKConf", GOCC, pClonk, 0, +1, "$MoreDarkness$",2,1);
  //Zähler senken
  if(iDarkCount > 0)
    AddMenuItem("$LessDarkness$", "ChangeDARKConf", GOCC, pClonk, 0, -1, "$LessDarkness$",2,2); else
    AddMenuItem("<c 777777>$LessDarkness$</c>", "ChangeDARKConf", GOCC, pClonk, 0, -1, "$LessDarkness$",2,2);
  //Fertig
  AddMenuItem("$Finished$", "OpenMenu", GOCC, pClonk,0,0, "$Finished$",2,3);
  //Letzten Eintrag auswählen
  SelectMenuItem(iSelection, pClonk);
}

local aTeamMenu;

protected func OpenTeamMenu(id dummy, int iSelection)
{
  aTeamMenu = [1, dummy, iSelection];
  fRandomMenu = false;
  var pClonk = GetCursor(iChosenPlr);
  //Menü erstellen
  CreateMenu(GetID(), pClonk, 0, 0, 0, 0, 1);
  //Teams auflisten
  for(var j = 0; j < GetPlayerCount(); j++)
  {
    var plr = GetPlayerByIndex(j);
    var team_name = GetTeamName(GetPlayerTeam(plr));
    if(iTeamMode == CHOS_TeamRandomInvisible && !aPlayerSetting[plr])
      team_name = "$Random$";

    AddMenuItem(Format("%s (%s)", GetTaggedPlayerName(plr, true), team_name), "SwitchTeam", PCMK, pClonk, 0, plr);
  }
  
  //Zusätzliche Teameinstellungen
  AddMenuItem("$RandomTeams$", "ChoosePossibleTeams(CHOS_TeamRandom)", MCMC, pClonk);
  AddMenuItem("$AutoBalanceTeams$", "ChoosePossibleTeams(CHOS_TeamAutobalance)", MCMC, pClonk);
  AddMenuItem("$RotatedTeams$", "ChoosePossibleTeams(CHOS_TeamRotation)", MCMC, pClonk);

  //Fertig
  AddMenuItem("$Finished$", "OpenMenu", GOCC, pClonk, 0, 0, "$Finished$", 2, 3);
  //Letzten Eintrag auswählen
  SelectMenuItem(iSelection, pClonk);
}

protected func ChoosePossibleTeams(int iMode, bool fInvisible, int iSelection)
{
  aTeamMenu = 0;
  if(iMode == CHOS_TeamRandom)
    fRandomMenu = true;

  var pClonk = GetCursor(iChosenPlr);

  //Menü erstellen
  CreateMenu(GetID(), pClonk, 0, 0, 0, 0, 1);

  var caption = "$CreateTeams$";
  if(iMode == CHOS_TeamRotation)
    caption = "$ChooseCaptains$";

  if(GetTeamConfig(TEAM_AutoGenerateTeams)) //Engine-erstelltes Team
  {
    if(!iTeamCount)
      iTeamCount = GetTeamCount();

    AddMenuItem("$TeamCount$", 0, TEAM, pClonk, iTeamCount);
    AddMenuItem("$IncTeamCnt$", Format("ChangeTeamCount(1, %d, %d)", iMode, fInvisible), CHOS, pClonk, 0, 0, 0, 2, 1);
    AddMenuItem("$DecTeamCnt$", Format("ChangeTeamCount(-1, %d, %d)", iMode, fInvisible), CHOS, pClonk, 0, 0, 0, 2, 2);
    if(iMode == CHOS_TeamRandom)
    {
      if(fInvisible) 
        AddMenuItem("$SwitchInvisible$", Format("ChoosePossibleTeams(CHOS_TeamRandom, false, %d)", 3), GOCC, pClonk, 0, 0, 0, 2, 3);
      else
        AddMenuItem("$SwitchInvisible$", Format("ChoosePossibleTeams(CHOS_TeamRandom, true, %d)", 3), SM06, pClonk);

       AddMenuItem("$PredefinedTeamMember$", Format("SelectPredefinedTeamMember(%v, 4, 1)", fInvisible), TEAM, pClonk);
    }

    AddMenuItem(caption, Format("CreateTeams(1, %d)", iMode+fInvisible), CHOS, pClonk, 0, 0, 0, 2, 3);
    AddMenuItem("$Back$", "OpenTeamMenu", 0, pClonk, 0, 0, "$Back$");
  }
  else
  {
    var blocked_teams = GameCall("ChooserBlockedTeams", GetID(pGoal), pGoal);
    if(GetType(blocked_teams) != C4V_Array)
      blocked_teams = 0;

    for(var i = 0; i < GetTeamCount(); i++)
    {
      var team = GetTeamByIndex(i);
      var clr = GetTeamColor(team);
      var cmd = Format("SwitchTeam2(%d, %d, %d)", team, iMode, fInvisible);
      if(!arTeams[team])
        clr = 0x777777;
      if(blocked_teams && GetIndexOf(team, blocked_teams) != -1)
        cmd = 0;

      AddMenuItem(Format("<c %x>%s</c>", clr, GetTeamName(team)), cmd, PCMK, pClonk);
    }
    if(iMode == CHOS_TeamRandom)
    {
      if(fInvisible) 
        AddMenuItem("$SwitchInvisible$", Format("ChoosePossibleTeams(CHOS_TeamRandom, false, %d)", GetTeamCount()), CHOS, pClonk, 0, 0, 0, 2, 3);
      else
        AddMenuItem("$SwitchInvisible$", Format("ChoosePossibleTeams(CHOS_TeamRandom, true, %d)", GetTeamCount()), SM06, pClonk);

      AddMenuItem("$PredefinedTeamMember$", Format("SelectPredefinedTeamMember(%v, %d, 2)", fInvisible, GetTeamCount()+1), TEAM, pClonk);
    }

    var cmd = 0;
    if(CreateTeamsAllowed(iMode))
      cmd = Format("CreateTeams(2, %d)", iMode+fInvisible);
    else
      caption = "<c 777777>$ChooseCaptains$</c>";

    AddMenuItem(caption, cmd, GOCC, pClonk, 0, 0, 0, 2, 3);
    AddMenuItem("$Back$", "OpenTeamMenu", 0, pClonk, 0, 0, "$Back$");
  }

  if(iSelection)
    SelectMenuItem(iSelection, pClonk);

  return true;
}

local aPlayerSetting;

public func CreateTeamsAllowed(int iMode)
{
  if(iMode == CHOS_TeamRotation)
  {
    if(GetTeamConfig(TEAM_AutoGenerateTeams) && iTeamCount+2 > GetPlayerCount()) //Engine-erstelltes Team
      return false;
    else if(!GetTeamConfig(TEAM_AutoGenerateTeams))
    {
      var teamcnt;
      for(var team in arTeams)
        if(team)
          teamcnt++;

      if(teamcnt+2 > GetPlayerCount())
        return false;
    }
  }

  return true;
}

protected func SelectPredefinedTeamMember(bool fInvisible, int iSelection, int iTeamSort, int iPlr)
{
  aTeamMenu = [2, fInvisible, iSelection, iTeamSort, iPlr];
  var pClonk = GetCursor(iChosenPlr);
  //Menü erstellen
  CreateMenu(GetID(), pClonk, 0, 0, 0, 0, 1);
  //Teams auflisten
  for(var j = 0; j < GetPlayerCount(); j++)
  {
    var plr = GetPlayerByIndex(j);
    var team_name = GetTeamName(GetPlayerTeam(plr));
    if(!aPlayerSetting[plr])
      team_name = "$Random$";

    AddMenuItem(Format("%s (%s)", GetTaggedPlayerName(plr, true), team_name), Format("SwitchPredefinedTeam(%v, %d, %d, %d)", fInvisible, iSelection, plr, iTeamSort), PCMK, pClonk);
  }
  AddMenuItem("$Back$", Format("ChoosePossibleTeams(CHOS_TeamRandom, %v, %d)", fInvisible, iSelection), 0, pClonk, 0, 0, "$Back$");

  SelectMenuItem(iPlr, pClonk);

  return true;
}

public func CountArrayItems(array aArray, value)
{
  var i, j = 0;
  while((i = GetIndexOf(value, aArray)) != -1)
  {
    j++;
    aArray[i] = !value;
  }

  return j;
}

private func SwitchPredefinedTeam(bool fInvisible, int iSelection, int iPlr, int iTeamSort)
{
  if(iTeamSort == 2)
  {
    arTeams[0] = true;
    var i = (1 + aPlayerSetting[iPlr]) % GetLength(arTeams);
    for(; i < GetLength(arTeams); i++)
    {
      if(arTeams[i])
      {
        aPlayerSetting[iPlr] = i;
        break;
      }

      if(i+1 >= GetLength(arTeams))
        i = -1;
    }

    arTeams[0] = false;
  }
  else if(iTeamSort == 1)
    aPlayerSetting[iPlr] = (aPlayerSetting[iPlr] + 1) % (iTeamCount + 1);

  SetPlayerTeam(iPlr, Max(aPlayerSetting[iPlr], 1));

  return SelectPredefinedTeamMember(fInvisible, iSelection, iTeamSort, iPlr);
}

protected func SwitchTeam2(int iTeam, int iMode, bool fInvisible)
{
  arTeams[iTeam] = !arTeams[iTeam];
  //Deaktivierung aller Teams verhindern
  if(CountArrayItems(arTeams, true) < MinTeamCount())
  {
    Sound("Error", true, 0, 100, iChosenPlr+1);
    arTeams[iTeam] = true;
  }

  var count = 0;
  for(var i = 0; i < GetLength(arTeams); i++) //Mehr Teams als Spieler? Verhindern.
  {
    if(arTeams[i])
      count++;
  }
  
  if(GetPlayerCount() < count)
  {
    Sound("Error", true, 0, 100, iChosenPlr+1);
    arTeams[iTeam] = false;
  }

  for(var i = 0; i < GetLength(aPlayerSetting); i++)
  {
    if(aPlayerSetting[i] == iTeam)
      aPlayerSetting[i] = 0;
  }

  ChoosePossibleTeams(iMode, fInvisible, iTeam-1);
  return true;
}

protected func ChangeTeamCount(int iChange, int iMode, bool fInvisible)
{
  blocked_teams = GameCall("ChooserBlockedTeams", GetID(pGoal), pGoal);
  if(GetType(blocked_teams) != C4V_Int)
    blocked_teams = 0;

  blocked_teams = (GetPlayerCount(C4PT_User) - blocked_teams + MinTeamCount()) * (!!blocked_teams);
  blocked_teams *= (blocked_teams >= 0);

  iTeamCount = BoundBy(iTeamCount + iChange, MinTeamCount(), GetPlayerCount(C4PT_User)-blocked_teams);
  for(var i = 0; i < GetLength(aPlayerSetting); i++)
  {
    if(aPlayerSetting[i] > iTeamCount)
      aPlayerSetting[i] = 0;
  }
  ChoosePossibleTeams(iMode, fInvisible, !!(iChange-1)+1);
  return true;
}

protected func InitializeTeam()
{
  for(var i = 0; i < iTeamCount; i++)
    arTeams[i+1] = (i < GetPlayerCount(C4PT_User));

  return true;
}

static const CHOS_TeamManual		= 0;	//Manuell
static const CHOS_TeamRandom		= 1;	//Zufällig
static const CHOS_TeamRandomInvisible	= 2;	//Zufällig & unsichtbar
static const CHOS_TeamAutobalance	= 3;	//Autobalanced
static const CHOS_TeamRotation		= 4;	//Abwechselnd

protected func CreateTeams(int iTeamSort, int iMode, bool fNoTeamMenu)
{
  iTeamMode = iMode;
  iUsedTeamSort = iTeamSort;

  if(iMode == CHOS_TeamRandom) // Zufällige Teamzusammenstellung.
  {
    var players = []; var teams = [];
    for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
    {
      var plr = GetPlayerByIndex(i, C4PT_User);
      if(aPlayerSetting[plr])
        SetPlayerTeam(plr, aPlayerSetting[plr]);
      else
        players[GetLength(players)] = plr;
    }

    var i = 0;
    var tPlayerSetting = aPlayerSetting;
    while(GetLength(players))
    {
      ++i;

      //Teamnummer gegebenenfalls zurücksetzen
      if(iTeamSort == 2)
      {
        if(i >= GetLength(arTeams))
          i = 1;

        if(!arTeams[i])
          continue;
      }
      else
        if(i > iTeamCount)
          i = 1;

      //Vordefinierter Spieler für dieses Team gefunden: Das Team überspringen
      var plr2 = GetIndexOf(i, tPlayerSetting);
      if(plr2 != -1)
      {
        //DelArrayItem4K(players, GetIndexOf(plr2, players));
        tPlayerSetting[plr2] = 0;
        continue;
      }

      //Zufälligen Spieler ziehen
      var pos = Random(GetLength(players));
      var plr = players[pos];
      if(tPlayerSetting[plr])
        continue;
      DelArrayItem4K(players, pos);

      var team = i;

      if(GetType(teams[i]) != C4V_Array)
        teams[i] = [];

      teams[i][GetLength(teams[i])] = plr;
    }

    //Spielerteam setzen
    for(var i = 0; i < GetLength(teams); i++)
      if(GetType(teams[i]) == C4V_Array)
        for(var plr in teams[i])
          SetPlayerTeam(plr, i);
    
    SetScoreboardData(CHOS_SBRD_Teams, 1, "$TeamsSortedRandomly$", 0, true);
  }
  else if(iMode == CHOS_TeamRandomInvisible)
  {
    for(var i = 0; i < GetPlayerCount(); i++)
    {
      var plr = GetPlayerByIndex(i);
      if(!aPlayerSetting[plr])
        SetPlayerTeam(plr, 1);
    }

    SetScoreboardData(CHOS_SBRD_Teams, 1, "$TeamsSortedRandomlyAndInv$", 0, true);
  }
  else if(iMode == CHOS_TeamAutobalance)
  {
    var team_count;
    if(iTeamSort == 1)
      team_count = iTeamCount;
    else
      for(var elm in arTeams)
        team_count += elm;

    if(!team_count)
      return false;

    var arNumbers = [];
    for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
      arNumbers[i] = GetPlayerRank(GetPlayerByIndex(i, C4PT_User))+1;

    var teams = [];
    for(var i = 0; i < team_count; i++)
      teams[i] = [];

    var nr_cnt = GetLength(arNumbers);	//Anzahl Zahlen
    var sum_nrs;			//Summe dieser Zahlen (-> Ränge)
    for(var rank in arNumbers)
      sum_nrs += rank;

    var max_rows = nr_cnt / team_count + (nr_cnt % team_count);	//Max. Anzahl von Zeilen
    var max_nr = sum_nrs / team_count + (sum_nrs % team_count);	//Höchste zu erreichende Zahl

    if(AB_GetMaxPlayerRank(arNumbers, true) > max_nr)		//Sonderregelung: Höhere Zahl in der Liste als die höchste, zu erreichende Zahl
    {
      max_nr = AB_GetMaxPlayerRank(arNumbers, true);
      max_rows = 0x7FFFFFFF;
    }

    //Zusammenfügen
    for(var i = 0; i < team_count; i++)
      teams[i][0] = AB_GetMaxPlayerRank(arNumbers);

    i--;

    while(i > -1)
    {
      for(var j = 0; j < max_rows-1; j++)
      {
        var val = AB_GetMaxPlayerRank(arNumbers, false, AB_ArraySum(teams[i]), max_nr);
        if(val != -1)
          teams[i][j+1] = val; 
        else
          break;
      }

      i--;
    }

    var fAdditionalMethods = true;
    while(fAdditionalMethods)
    {
      if(AB_DifferenceCheck(teams))
        break;

      for(var i = iTeamCount-1; i >= 0; i--)
      {
        if(!AB_Method1(teams[i-1], teams[i]))
          fAdditionalMethods = false;
        else
          fAdditionalMethods = true;
      }
    }
    if(!fAdditionalMethods)
    {
      fAdditionalMethods = true;
      while(fAdditionalMethods)
      {
        var aSums = [];

        for(var team in teams)
        {
          if(GetType(team) != C4V_Array)
            continue;

          aSums[GetLength(aSums)] = AB_ArraySum(team);
        }

        if((GetLength(aSums) - CountArrayVal(aSums, max_nr)) <= 1) // Alle Zahlen (bis auf einer) entsprechen der max. erreichbaren Zahl?
          break;

        if(AB_DifferenceCheck(teams)) // Evtl nochmal schauen, ob alles vielleicht doch noch seine Richtigkeit hat...
          break;

        if(!AB_Method2(teams, max_nr))
          fAdditionalMethods = false;
      }
    }

    //Aufstellung fertigstellen
    var arTempTeams = arTeams;
    var aExcept = [];
    for(var i = 0; i < GetLength(teams); i++)
    {
      if(GetType(teams[i]) != C4V_Array)
        continue;

      if(iTeamSort == 2)
      {
        var pos = GetIndexOf(true, arTempTeams);
        arTempTeams[pos] = 0;

        for(var rank in teams[i])
        {
          var plr = GetPlayerByRank(rank-1, aExcept);
          aExcept[GetLength(aExcept)] = plr;
          SetPlayerTeam(plr, pos);
        }
      }
      else
      {
        var t = i+1;

        for(var rank in teams[i])
        {
          var plr = GetPlayerByRank(rank-1, aExcept);
          aExcept[GetLength(aExcept)] = plr;
          SetPlayerTeam(plr, t);
        }
      }
    }

    SetScoreboardData(CHOS_SBRD_Teams, 1, "$TeamsSortedByRank$", 0, true);
  }
  else if(iMode == CHOS_TeamRotation)
  {
    iTeamCaptainState = 1;

    var pClonk = GetCursor(iChosenPlr);
    CloseMenu(pClonk);

    //Menü erstellen
    CreateMenu(GetID(), pClonk, 0, 0, 0, 0, 1);

    //Teams auflisten
    for(var j = 0; j < GetPlayerCount(); j++)
    {
      var plr = GetPlayerByIndex(j);

      //Team des Teamcaptains wurde nachträglich deaktiviert
      if((!GetTeamConfig(TEAM_AutoGenerateTeams) && aTeamCaptains[plr] && !arTeams[aTeamCaptains[plr]]) || (GetTeamConfig(TEAM_AutoGenerateTeams) && aTeamCaptains[plr] > iTeamCount))
        aTeamCaptains[plr] = 0;

      var team_name = GetTeamName(aTeamCaptains[plr]);
      if(!aTeamCaptains[plr])
        team_name = "$NoCaptain$";

      AddMenuItem(Format("%s (%s)", GetTaggedPlayerName(plr, true), team_name), Format("SwitchTeamCaptain(%d)", plr), PCMK, pClonk);
    }

    //Nur wenn es genausoviele TeamCaptains gibt, wie es Teams gibt
    if(GetAvailableTeamCount()-GetTeamCaptainCount() > 0)
      AddMenuItem("$StartTeamRotationDeactivated$", 0, GOCC, pClonk, 0, 0, 0, 2, 3);
    else
      AddMenuItem("$StartTeamRotation$", "StartTeamRotation", GOCC, pClonk, 0, 0, 0, 2, 3);

    AddMenuItem("$Back$", Format("ChoosePossibleTeams(CHOS_TeamRotation)"), 0, pClonk, 0, 0, "$Back$");
    return;
  }

  if(fNoTeamMenu)
    return true;

  return OpenTeamMenu();
}

public func GetTeamCaptainCount()
{
  var count = 0;
  for(var i = 0; i < GetLength(aTeamCaptains); i++)
    if(aTeamCaptains[i] && GetPlayerID(aTeamCaptains[i]))
      count++;

  return count;
}

public func GetAvailableTeamCount()
{
  if(GetTeamConfig(TEAM_AutoGenerateTeams))
    return iTeamCount;
  else
  {
    var count, i;
    for(i = 0; i < GetLength(arTeams); i++)
      if(arTeams[i])
        count++;

    return count;
  }

  return 0;
}

public func SwitchTeamCaptain(int iPlr)
{
  var f = (GetAvailableTeamCount()-GetTeamCaptainCount() > 0);
  //var iSelection = GetMenuSelection(GetCursor(iChosenPlr));

  if(GetAvailableTeamCount()-GetTeamCaptainCount() <= 0)
    aTeamCaptains[iPlr] = 0;
  else if(GetTeamConfig(TEAM_AutoGenerateTeams))	//Engine-erstelltes Team
  {
    var i = aTeamCaptains[iPlr]+1;
    if(i > iTeamCount)
      i = 0;

    while(i != aTeamCaptains[iPlr] && i)
    {
      if(GetIndexOf(i, aTeamCaptains) == -1)
        break;

      if(i < iTeamCount)
        i++;
      else
        i = 0;
    }

    aTeamCaptains[iPlr] = i;
  }
  else
  {
    var team = aTeamCaptains[iPlr];
    for(var i = team; i < GetLength(arTeams); i++)
      if(arTeams[i] && GetIndexOf(i, aTeamCaptains) == -1)
      {
        team = i;
        break;
      }

    if(team == aTeamCaptains[iPlr])
      team = 0;

    aTeamCaptains[iPlr] = team;
  }

  SetPlayerTeam(iPlr, Max(1, aTeamCaptains[iPlr]));

  //Geräusch
  Sound("Grab", 1,0,0,1);

  CreateTeams(0, CHOS_TeamRotation);
  SelectMenuItem(iPlr, GetCursor(iChosenPlr));

  return true;
}

public func StartTeamRotation()
{
  iTeamCaptainState = 2;
  SetScoreboardData(CHOS_SBRD_Teams, 1, "$TeamSortedByRotation$", 0, true);

  var captains = [], players = [];
  for(var i = 0; i < GetPlayerCount(); i++)
  {
    var plr = GetPlayerByIndex(i);

    if(aTeamCaptains[plr])
      captains[GetLength(captains)] = plr;
    else
      players[GetLength(players)] = plr;
  }
  var start = Random(GetLength(captains));

  //Keine weiteren Spieler: Zum Teammenü springen
  if(!GetLength(players))
    return TeamRotationEnd();
  //Nur ein Captain: Spieler dessen Team zuweisen
  else if(GetLength(captains) == 1)
  {
    for(var plr in players)
      SetPlayerTeam(plr, GetPlayerTeam(captains[0]));

    return TeamRotationEnd();
  }

  //Eventnachricht: Teamrotation beginnt
  EventInfo4K(0, "$TeamRotationBegins$", CHOS, 0, 0, 0, "Info_Event.ogg");

  AddEffect("TeamRotationChoosePlr", this, 100, 36, this, 0, start, captains, players);
  return true;
}

public func TeamRotationEnd()
{
  iTeamCaptainState = 0;
  //Eventnachricht: Teamrotation beendet
  EventInfo4K(0, "$TeamRotationEnd$", CHOS, 0, 0, 0, "Info_Event.ogg");
  OpenTeamMenu();

  return true;
}

static const CHOS_TeamRotation_ChooseTime = 10;	//Zeit in Sekunden pro Wahl

public func FxTeamRotationChoosePlrStart(object pTarget, int iNr, temp, int iStartIndex, array aCaptains, array aAvailablePlayers)
{
  if(temp)
    return;

  EffectVar(0, pTarget, iNr) = iStartIndex;
  EffectVar(1, pTarget, iNr) = aCaptains;
  EffectVar(3, pTarget, iNr) = aAvailablePlayers;
  EffectVar(2, pTarget, iNr) = CHOS_TeamRotation_ChooseTime;

  FxTeamRotationChoosePlrTimer(pTarget, iNr);

  return true;
}

public func FxTeamRotationChoosePlrTimer(object pTarget, int iNr)
{
  var cpt = EffectVar(1, pTarget, iNr)[EffectVar(0, pTarget, iNr)];
  var obj = GetCursor(cpt);
  if(obj->~GetRealCursor())
    obj = obj->~GetRealCursor();

  if(EffectVar(2, pTarget, iNr) <= 0)
    return EffectCall(pTarget, iNr, "Choose", EffectVar(3, pTarget, iNr)[Random(GetLength(EffectVar(3, pTarget, iNr)))], cpt);

  if(GetLength(EffectVar(3, pTarget, iNr)) == 1)
    return EffectCall(pTarget, iNr, "Choose", EffectVar(3, pTarget, iNr)[0], cpt);

  var sel = 0;
  if(EffectVar(2, pTarget, iNr) != CHOS_TeamRotation_ChooseTime)
    sel = GetMenuSelection(obj);

  CloseMenu(obj);

  if(EffectVar(2, pTarget, iNr) <= 5)
    Sound("Select.ogg", true);

  CreateMenu(GetID(), obj, this, 0, 0, 0, 1);
  AddMenuItem(Format("$TimeRemaining$", EffectVar(2, pTarget, iNr)), 0, TEAM, obj);

  for(var plr in EffectVar(3, pTarget, iNr))
    AddMenuItem(GetTaggedPlayerName(plr, true), Format("EffectCall(Object(%d), %d, \"Choose\", %d, %d)", ObjectNumber(pTarget), iNr, plr, cpt), PCMK, obj);

  AddMenuItem("$ChooseRandom$", Format("EffectCall(Object(%d), %d, \"Choose\", %d, %d)", ObjectNumber(pTarget), iNr, EffectVar(3, pTarget, iNr)[Random(GetLength(EffectVar(3, pTarget, iNr)))], cpt), CXIN, obj);

  SelectMenuItem(sel, obj);
  EffectVar(2, pTarget, iNr)--;

  return true;
}

public func FxTeamRotationChoosePlrNext(object pTarget, int iNr, int iCpt)
{
  if(++EffectVar(0, pTarget, iNr) == GetLength(EffectVar(1, pTarget, iNr)))
    EffectVar(0, pTarget, iNr) = 0;

  EffectVar(2, pTarget, iNr) = CHOS_TeamRotation_ChooseTime;

  CloseMenu(GetCursor(iCpt));
  FxTeamRotationChoosePlrTimer(pTarget, iNr);

  return true;
}

public func FxTeamRotationChoosePlrChoose(object pTarget, int iNr, int iPlr, int iCpt)
{
  DelArrayItem4K(EffectVar(3, pTarget, iNr), GetIndexOf(iPlr, EffectVar(3, pTarget, iNr)));

  SetPlayerTeam(iPlr, aTeamCaptains[iCpt]);
  if(GetLength(EffectVar(3, pTarget, iNr)) >= 1)
    EffectCall(pTarget, iNr, "Next", iCpt);
  else
  {
    RemoveEffect("TeamRotationChoosePlr", pTarget);
    CloseMenu(GetCursor(iCpt));
    TeamRotationEnd();
  }

  return true;
}

/* Weitere Autobalance-Methoden, zur Übersichtlichkeit ausgelagert */

//Austausch: Es werden in beiden Listen versucht, Zahlen auszutauschen, um ein besseres Ergebnis zu erreichen.
public func AB_Method1(array &arListOne, array &arListTwo)
{
  if(!arListOne || !arListTwo)
    return false;

  var sumL1 = AB_ArraySum(arListOne);	//Summe beider Listen
  var sumL2 = AB_ArraySum(arListTwo);

  var savedL1 = arListOne;		//Temporäre Speicher
  var savedL2 = arListTwo;
  var savedSums = [sumL1, sumL2];	//Temporäre Summen beider Listen

  for(var i = GetLength(arListTwo)-1; i >= 0; i--)
  {
    for(var j = GetLength(arListOne)-1; j >= 0; j--)
    {
      var tempL1 = arListOne;
      var tempL2 = arListTwo;

      var a, b;
      a = arListTwo[i];
      b = arListOne[j];
      tempL2[i] = b;
      tempL1[j] = a;

      var sumTL1 = AB_ArraySum(tempL1); var sumTL2 = AB_ArraySum(tempL2);
      if(GetIndexOf(sumTL1, savedSums) != -1 || GetIndexOf(sumTL2, savedSums) != -1) //Wenn vorherige und aktuelle Summen gleich sind, kein Tausch
        continue;

      var tempSLists = [AB_ArraySum(tempL1), AB_ArraySum(tempL2)];
      if(GetMinArrayVal(tempSLists) > GetMinArrayVal(savedSums)) //Tauschen und speichern
      {
        savedL1 = tempL1; savedL2 = tempL2; savedSums = tempSLists;
        continue;
      }
    }
  }

  if(savedSums[0] == sumL1 || savedSums[0] == sumL2)
    return false;

  arListOne = savedL1;
  arListTwo = savedL2;

  return true;
}

//Es wird geprüft, ob es möglich ist, aus einem Array kleinere Zahlen in den anderen Array zu packen, um einen Ausgleich zu erhalten.
public func AB_Method2(array &arTeams, int iMaxNr)
{
  if(!arTeams)
    return false;

  var arraySums = [];
  for(var i = 0; i < GetLength(arTeams); i++)
    arraySums[i] = AB_ArraySum(arTeams[i]);

  var teamMin = GetMinArrayVal(arraySums, true); //Array-Positionen in arTeams
  var teamMax = GetMaxArrayVal(arraySums, true); 

  var arTempTeams = arTeams; //Zur Absicherung

  var val = GetMinArrayVal(arTempTeams[teamMax]); //Kleinste Zahl im größtem Array
  var pos = GetMinArrayVal(arTempTeams[teamMax], true); //und deren Position

  DelArrayItem4K(arTempTeams[teamMax], pos); // Zahl entfernen
  arTempTeams[teamMin][GetLength(arTempTeams[teamMin])] = val; // Zahl hinzufügen

  if(AB_ArraySum(arTempTeams[teamMin]) <= iMaxNr) //Änderung darf nicht über das Maximum
  {
    arTeams = arTempTeams; //Änderungen übernehmen
    return true;
  }
  else
    return false;
}

//Hinweis: Die Funktionen mit AB_-Präfix sollten möglichst nur vom Autobalancer eingesetzt werden.
protected func AB_GetMaxPlayerRank(array &arNumbers, bool fNoDelete, int iValue, int iMax, bool fPlr)
{
  var highest, pos = -1;
  for(var i = 0; i < GetLength(arNumbers); i++)
  {
    if(iMax)
    {
      if(arNumbers[i] > highest && iValue + arNumbers[i] <= iMax)
      {
        highest = arNumbers[i];
        pos = i;
      }
    }
    else if(arNumbers[i] > highest)
    {
      highest = arNumbers[i];
      pos = i;
    }
  }

  if(pos == -1)
  {
    if(GetLength(arNumbers) == 1)
    {
      pos = 0; 
      highest = arNumbers[0];
    }
    else
      return -1;
  }

  if(fPlr)
    highest = arNumbers[pos];

  if(pos+1 && !fNoDelete)
    DelArrayItem4K(arNumbers, pos);

  return highest;
}

public func AB_ArraySum(array arArray)
{
  var ret = 0;
  for(var elm in arArray)
    ret += elm;

  return ret;
}

public func AB_DifferenceCheck(array arArray)
{
  var aSums = [];

  for(var team in arArray)
  {
    if(GetType(team) != C4V_Array)
      continue;

    aSums[GetLength(aSums)] = AB_ArraySum(team);
  }

  if(AB_GetMaxPlayerRank(aSums) - GetMinArrayVal(aSums) <= 1)
    return true; 
}

public func CountArrayVal(array arArray, value)
{
  var i;
  for(var elm in arArray)
    if(elm == value)
      ++i;

  return i;
}

public func GetPlayerByRank(int iRank, array arExcept)
{
  var iResult = -1;
  for(var i = 0; i < GetPlayerCount(); i++)
  {
    var plr = GetPlayerByIndex(i);
    if(GetIndexOf(plr, arExcept) > -1)
      continue;

    if(GetPlayerRank(plr) == iRank)
    {
      iResult = plr;
      break;
    }
  }

  return plr;
}

protected func SwitchTeam(id dummy, int iPlr)
{
  var team = GetPlayerTeam(iPlr);
  var teams = [];

  blocked_teams = GameCall("ChooserBlockedTeams", GetID(pGoal), pGoal);
  if(blocked_teams)
  {
    if(!GetTeamConfig(TEAM_AutoGenerateTeams))
    {
      if(GetType(blocked_teams) == C4V_Array)
        for(var i = 0; i < GetTeamCount(); i++)
          if(GetIndexOf(GetTeamByIndex(i), blocked_teams) == -1)
            teams[GetLength(teams)] = GetTeamByIndex(i);
    }
    else if(GetType(blocked_teams) == C4V_Int)
    {
      for(var i = 0; i < GetTeamCount(); i++)
        if(GetTeamByIndex(i) < blocked_teams)
          teams[GetLength(teams)] = GetTeamByIndex(i);
    }
  }
  else
    for(var i = 0; i < GetTeamCount(); i++)
      teams[GetLength(teams)] = GetTeamByIndex(i);


  var p = Max(0, GetIndexOf(team, teams));
  p = (p+1) % GetLength(teams);

  SetPlayerTeam(iPlr, teams[p]);

  var sel = GetMenuSelection(GetCursor(iChosenPlr));

  //Geräusch
  Sound("Grab", 1,0,0,1);

  for(var j = 0; j < GetPlayerCount(); j++)
  {
    if(j == 0) continue;
    CloseMenu(GetCursor(GetPlayerByIndex(j)));
  }

  iTeamMode = CHOS_TeamManual;
  SetScoreboardData(CHOS_SBRD_Teams, 1, "$TeamsSortedManually$", 0, true);

  //Menü wieder eröffnen
  OpenTeamMenu(0, iPlr);
  return true;
}

protected func LoadRuleCfg()
{
  var a = GameCall("ChooserRuleConfig");
  if(!GetLength(a))
    return;

  for(var i=0, idR; idR = GetDefinition(i, Chooser_Cat) ; i++)
   if(DefinitionCall(idR, "IsChooseable") && !GetLength(FindObjects(Find_ID(idR))))
     if(FindInArray4K(a, idR) > -1)
       aRules[i] = true;
}

protected func InitializePlayer(int iPlr, int iX, int iY, object pBase, int iTeam, id idExtra)
{
  if(CountArrayItems(arTeams, true) < MinTeamCount())
  {
    arTeams[0] = true;
    var index = GetIndexOf(false, arTeams);
    if(index != -1)
      arTeams[index] = true;

    arTeams[0] = false;
  }

  if(GetPlayerType(iPlr) == C4PT_Script)
    for(var i = 1 ; i < aAI[iTeam] ; i++)
      CreateClonk(iPlr);
  aAI[iTeam] = 0;
  if(Death) return;
  //Alle Clonks des Spielers verstauen
  for(var i=0, pCrew, tmp ; pCrew = GetCrew(iPlr, i) ; i++)
  {
    tmp = CreateObject(TIM1, GetX(pCrew), GetY(pCrew), -1);
    SetCategory(GetCategory(tmp) | C4D_Foreground,tmp);
    SetGraphics(0, tmp, GetID(pCrew), 1, 5, 0, 1, pCrew);
    Enter(tmp, pCrew);
    Eastern(tmp);
  }
  //Falls ein Spieler während der Abstimmung beitritt
  if(GetEffect("EvaluateGoalVote", this))
    GoalVoteMenu(0, 0, iPlr);
}

/* Spielziel setzen */

protected func CreateGoal(id idGoal, int iScore, string szMessage)
{
  //Bei Ligarunden Autostart initialisieren
  if(GetLeague())
    AddEffect("LeagueAutostart", this, 1, 35, this);

  //Spielziel erstellen
  var goal = CreateObject(idGoal, 0,0, -1);
  //Spielziel als lokale Variable setzen
  pGoal = goal;
  //Alten Wert setzen
  SetWinScore(iScore, goal);
  if(!szMessage)
    //Eventnachricht: Spielziel gewählt
    EventInfo4K(0, Format("$Goal$", GetName(0, idGoal)), idGoal, 0, 0, 0, "Info_Event.ogg");
  else
    //Eventnachricht: Spielziele gewählt
    EventInfo4K(0, szMessage, idGoal, 0, 0, 0, "Info_Event.ogg");
  //Array leeren um erneuten Menüaufruf zu verhindern
  aGoals = CreateArray();
  //Normales Menü öffnen
  OpenMenu();
}

/* Konfiguration abschließen */

protected func ConfigurationFinished()
{
  if(GetEffect("LeagueAutostart", this))
    RemoveEffect("LeagueAutostart", this);

  return _inherited(...);
}

protected func ConfigurationFinished2()
{
  //Zufällige und unsichtbare Teams
  if(iTeamMode == CHOS_TeamRandomInvisible)
    CreateTeams(iUsedTeamSort, CHOS_TeamRandom);

  Death = true;
  //Regeln erzeugen
  var i = 0, j = 0, pCrew, tmp, log, def;
  if(IsStandardSetting())
    log = "$StdRules$";
  else
    log = "$Rules$";

  var activated = "";
  var deactivated = "";
  var optional = "";
  var comma = ["", "", ""];
  for(var check in aRules)
  {
    def = GetDefinition(i, Chooser_Cat);
    i++;
    if(check)
      CreateObject(def, 10, 10, -1);

    if(!(GetCategory(0, def) & C4D_Rule))
      continue;

    var fStandard = (GetIndexOf(def, GameCall("ChooserRuleConfig")) != -1);
    if(!fStandard && !check)
      continue;
    else if(fStandard && !check)
    {
      deactivated = Format("%s%s<c %x>%s</c>", deactivated, comma[0], GetRuleColor(def, true), GetName(0, def));
      comma[0] = ", ";
    }
    else if(fStandard)
    {
      activated = Format("%s%s<c %x>%s</c>", activated, comma[1], GetRuleColor(def), GetName(0, def));
      comma[1] = ", ";
    }
    else
    {
      optional = Format("%s%s<c %x>%s</c>", optional, comma[2], GetRuleColor(def), GetName(0, def));
      comma[2] = ", ";
    }
  }
  if(activated != "" && deactivated != "")
    deactivated = Format(", %s", deactivated);
  if((activated != "" || deactivated != "") && optional != "")
    optional = Format(", %s", optional);
  //Zusammenfassen
  log = Format("%s%s%s%s", log, activated, deactivated, optional);
  //Dunkelheit erzeugen
  log = Format("%s, %s x%d", log, GetName(0, DARK), iDarkCount);
  //Eventnachricht: Dunkelheit
  EventInfo4K(0,log,CHOS, 0, 0, 0, "Info_Event.ogg");

  //Schneller GameCall für Einstellungen
  GameCallEx("ChooserFinished");

  //Spieler freilassen
  for(i = 0 ; i < GetPlayerCount() ; i++)
  {
    for(j = 0 ; pCrew = GetCrew(GetPlayerByIndex(i), j) ; j++)
    {
      tmp = Contained(pCrew);
      if(GetID(tmp) == TIM1)
      	RemoveObject(tmp, 1);
      pCrew->~Recruitment(GetOwner(pCrew));
    }
    for(var rule in FindObjects(Find_Category(Chooser_Cat), Find_Exclude(this)))
      rule->~InitializePlayer(GetPlayerByIndex(i));
  }

  //Scoreboard leeren
  ClearScoreboard(CHOS_SBRD_Teams + GetPlayerCount(), 1);
  //Nach einem Frame die Auswahlsperre auflösen
  ScheduleCall(pGoal, "InitScoreboard", 1);
  //Selber entfernen
  RemoveObject();
}

private func IsStandardSetting()
{
  var a = GameCall("ChooserRuleConfig"), i;
  for (var i = 0; i < GetLength(aRules); i++)
  {
    if(GetIndexOf(GetDefinition(i, Chooser_Cat), a) != -1)
    {
      if(!aRules[i]) //Regel im Standardsatz, aber nicht ausgewählt
        return false;
    }
    else if(aRules[i]) //Regel ausgewählt, aber nicht Standard
      return false;
  }
  return true;
}

private func GetRuleColor(id idDef, bool fNotChoosed)
{
  if(GetIndexOf(idDef, GameCall("ChooserRuleConfig")) == -1)
    return RGB(255);
  if(fNotChoosed)
    return RGB(127, 127, 127);

  return RGB(255,255,255);
}

/* Easteregg Textnachrichten */

private func Eastern(object P)
{
  var rand = Random(17);
  if(!rand--) SetName(GetPlayerName(GetPlayerByIndex(Random(GetPlayerCount()))), P);
  if(!rand--) SetName("$Emptiness$", P);
  if(!rand--) SetName("$YourBrain$", P);
  if(!rand--) SetName("$Nothing$", P);
  if(!rand--) SetName("$Freezer$", P);
  if(!rand--) SetName("$Space$", P);
  if(!rand--) SetName("$YourGun$", P);
  if(!rand--) SetName("$YourWallet$", P);
  if(!rand--) SetName("$YourCookieJar$", P);
  if(!rand--) SetName("$ThisWindow$", P);
  if(!rand--) SetName("$YourStomach$", P);
  if(!rand--) SetName("$EverythingEmpty$", P);
  if(!rand--) SetName("$Barrel$", P);
  if(!rand--) SetName("$Beer$", P);
  if(!rand--) SetName("$YourGlance$", P);
  if(!rand--) SetName("$YourHarddrive$", P);
  if(!rand--) SetName("$YourChat$", P);
}

/* Spielzielwahl */

protected func OpenGoalChooseMenu()
{
  var pClonk = GetCursor(iChosenPlr);
  if(!pClonk || !GetLength(aGoals))
    return ScheduleCall(this, "OpenMenu", 1);

  CloseMenu(pClonk);

  //Nur ein Spielziel verfügbar: Direkt auswählen
  if(GetLength(aGoals) == 1)
    return CreateGoal(aGoals[0], aTempGoalSave[0]);

  if(GetLeague())
    return OpenGoalVoteMenu(GetID(), pClonk);

  CreateMenu(GetID(), pClonk, 0, 0, 0, 0, 1);

  var i;
  var rGoals = RecommendedGoals();
  if(!rGoals)
    rGoals = [];

  //Spielziele auflisten
  for(var goal in aGoals)
  {
    var obj = CreateObject(TIM1, 0, 0, -1);
    SetPicture(0, 0, 64, 64, obj);
    SetGraphics(0, obj, goal);
    if(GetIndexOf(goal, rGoals) != -1)
    {
      SetGraphics(0, obj, SM14, 1, GFXOV_MODE_Picture);
      SetObjDrawTransform(600, 0, 8500, 0, 600, 8500, obj, 1);
    }

    AddMenuItem("%s", Format("CreateGoal(%i, %d)", goal, aTempGoalSave[i]), goal, pClonk, 0, 0, 0, 4, obj);
    RemoveObject(obj);
    i++;
  }
  //Menüweitergabe
  AddMenuItem("$ChangeHost$", "ChangeHostMenu", MCMC, pClonk, 0,0, "$ChangeHostInfo$");
  //Zufall
  AddMenuItem("$RandomGoal$", "OpenGoalRandomMenu", GetID(), pClonk, 0, pClonk, "$RandomGoal$");
  //Abstimmung
  AddMenuItem("$VoteGoal$", "CheckGoalVoteMenu", GetID(), pClonk, 0, pClonk, "$VoteGoal$");
}

public func ChangeHostMenu()
{
  var pClonk = GetCursor(iChosenPlr);
  if(!pClonk)
    return ScheduleCall(this, "OpenMenu", 1);

  CloseMenu(pClonk);

  if(GetLength(aGoals) == 1)
    return CreateGoal(aGoals[0], aTempGoalSave[0]);

  CreateMenu(GetID(), pClonk, 0, 0, 0, 0, 1);
  
  for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
  {
    var plr = GetPlayerByIndex(i, C4PT_User);
    var clr = 0x777777, name = GetPlayerName(plr), cmd = 0;
    if(iChosenPlr != plr)
    {
      clr = GetPlrColorDw(plr);
      cmd = "ChangeHostPlayer";
    }

    AddMenuItem(Format("<c %x>%s</c>", clr, name), cmd, 0, pClonk, 0, plr);
  }

  AddMenuItem("$Back$", "OpenMenu", 0, pClonk, 0, 0, "$Back$");
  return true;
}

public func ChangeHostPlayer(id dummy, int iPlr)
{
  var pClonk = GetCursor(iChosenPlr);
  if(!pClonk)
    return ScheduleCall(this, "OpenMenu", 1);

  CloseMenu(pClonk);

  //Eventnachricht: Neuer Host
  EventInfo4K(0, Format("$NewHost$", GetPlrColorDw(iPlr), GetPlayerName(iPlr)), CHOS, 0, 0, 0, "Info_Event.ogg");

  iChosenPlr = iPlr;
  return OpenMenu();
}

/* Random */

local aGoalsChecked;

protected func OpenGoalRandomMenu(id id, object pClonk)
{
  CreateMenu(GetID(), pClonk, 0, 0, 0, 0, C4MN_Style_Context);

  var rGoals = RecommendedGoals();
  if(!rGoals)
    rGoals = [];

  if(!aGoalsChecked)
    aGoalsChecked = [];
  var fChecked = false;
  for (var i = 0; i < GetLength(aGoals); i++)
  {
    if(!aGoals[i])
      continue;

    var obj = CreateObject(TIM1, 0, 0, -1);
    SetPicture(0, 0, 64, 64, obj);
    SetGraphics(0, obj, aGoals[i]);
    if(GetIndexOf(aGoals[i], rGoals) != -1)
    {
      SetGraphics(0, obj, SM14, 1, GFXOV_MODE_Picture);
      SetObjDrawTransform(600, 0, 8500, 0, 600, 8500, obj, 1);
    }

    if(aGoalsChecked[i])
    {
      AddMenuItem("%s", "CheckRandomGoal", aGoals[i], pClonk, 0, pClonk, GetDesc(0, aGoals[i]), 4, obj);
      fChecked = true;
    }
    else
      AddMenuItem("<c 777777>%s</c>", "CheckRandomGoal", aGoals[i], pClonk, 0, pClonk, GetDesc(0, aGoals[i]), 4, obj);

    RemoveObject(obj);
  }
  if(fChecked)
    AddMenuItem("$GoalRandomChoose$", "GoalRandomChoose", GetID(), pClonk, 0, pClonk, "$GoalRandomChoose$", 2, 3);
  else
    AddMenuItem("<c 777777>$GoalRandomChoose$</c>", 0, GetID(), pClonk, 0, pClonk, "$GoalRandomChoose$", 2, 3);
  AddMenuItem("$Back$", "OpenGoalChooseMenu", 0, pClonk, 0, pClonk, "$Back$");
}

protected func CheckRandomGoal(id idGoal, object pClonk)
{
  var iIndex = GetIndexOf(idGoal, aGoals);
  aGoalsChecked[iIndex] = !aGoalsChecked[iIndex];
  OpenGoalRandomMenu(0, pClonk);
  SelectMenuItem(iIndex, pClonk);
  Sound("Grab", true, 0, 0, GetOwner(pClonk) + 1);
}

protected func GoalRandomChoose(id id, object pClonk)
{
  var array = [];
  for (var i = 0; i < GetLength(aGoals); i++)
    if(aGoalsChecked[i])
      array[GetLength(array)] = aGoals[i];
  var idGoal = array[Random(GetLength(array))];
  if(!idGoal)
    return OpenGoalRandomMenu(0, pClonk);
  return CreateGoal(idGoal, aTempGoalSave[GetIndexOf(idGoal, aGoals)]);
}

/* Voting */

local aGoalsVoted;
static const CHOS_GoalVotingTime = 15;

protected func CheckGoalVoteMenu(id id, object pClonk)
{
  CreateMenu(GetID(), pClonk, 0, 0, 0, 0, C4MN_Style_Context);
  AddMenuItem("$BeginVote$", 0, GetID(), pClonk, 0, 0, 0, C4MN_Add_ForceNoDesc);
  AddMenuItem("$BeginVoteYes$", "OpenGoalVoteMenu", GetID(), pClonk, 0, pClonk, 0, C4MN_Add_ImgIndexed | C4MN_Add_ForceNoDesc, 3);
  AddMenuItem("$BeginVoteNo$", "OpenGoalChooseMenu", MCMX, pClonk, 0, pClonk, 0, C4MN_Add_ForceNoDesc);
}

protected func OpenGoalVoteMenu(id id, object pClonk)
{
  if(!aGoalsVoted)
    aGoalsVoted = [];
  for (var i = 0; i < GetPlayerCount(); i++)
    GoalVoteMenu(0, 0, GetPlayerByIndex(i));
  AddEffect("EvaluateGoalVote", this, 1, 35, this);
  //Eventnachricht: Spielzielvoting gestartet
  EventInfo4K(0, Format("$GoalVoteBegins$", CHOS_GoalVotingTime), CHOS, 0, 0, 0, "Info_Event.ogg");
}

protected func GoalVoteMenu(id id, object pClonk, int iPlr)
{
  if(!pClonk && !(pClonk = GetCrew(iPlr)))
    return;
  if(iPlr == NO_OWNER && (iPlr = GetOwner(pClonk)) == NO_OWNER)
    return false;
  var iSelection = GetMenuSelection(pClonk);
  CloseMenu(pClonk);

  var iTime = GetEffect("EvaluateGoalVote", this, 0, 6);
  if(iTime >= CHOS_GoalVotingTime * 35)
    return false;

  CreateMenu(GetID(), pClonk, 0, 0, 0, 0, C4MN_Style_Context);
  if(!aGoalsVoted[iPlr])
    aGoalsVoted[iPlr] = [];

  var rGoals = RecommendedGoals();
  if(!rGoals)
    rGoals = [];

  AddMenuItem(Format("$VoteGoal$ (%d)", CHOS_GoalVotingTime - iTime / 35), 0, GetID(), pClonk, 0, 0, 0, C4MN_Add_ForceNoDesc);
  for (var i = 0; i < GetLength(aGoals); i++)
  {
    var obj = CreateObject(TIM1, 0, 0, -1);
    SetPicture(0, 0, 64, 64, obj);
    SetGraphics(0, obj, aGoals[i]);
    if(GetIndexOf(aGoals[i], rGoals) != -1)
    {
      SetGraphics(0, obj, SM14, 1, GFXOV_MODE_Picture);
      SetObjDrawTransform(600, 0, 8500, 0, 600, 8500, obj, 1);
    }

    if(aGoalsVoted[iPlr][i])
      AddMenuItem("%s", "CheckVoteGoal", aGoals[i], pClonk, 0, pClonk, GetDesc(0, aGoals[i]), 4, obj);
    else
      AddMenuItem("<c 777777>%s</c>", "CheckVoteGoal", aGoals[i], pClonk, 0, pClonk, GetDesc(0, aGoals[i]), 4, obj);

    RemoveObject(obj);
  }
  SelectMenuItem(iSelection, pClonk);
  return true;
}

protected func CheckVoteGoal(id idGoal, object pClonk)
{
  var iIndex = GetIndexOf(idGoal, aGoals), iPlr = GetOwner(pClonk);
  aGoalsVoted[iPlr][iIndex] = !aGoalsVoted[iPlr][iIndex];
  GoalVoteMenu(0, pClonk, iPlr);
  SelectMenuItem(iIndex + 1, pClonk);
  Sound("Grab", true, 0, 0, iPlr + 1);
}

protected func FxEvaluateGoalVoteTimer(pTarget, iEffect, iTime)
{
  if(CHOS_GoalVotingTime - (iTime / 35) < 5)
    Sound("Select.ogg", true);
  for(var i = 0; i < GetPlayerCount(); i++)
    GoalVoteMenu(0, GetCrew(GetPlayerByIndex(i)), GetPlayerByIndex(i));

  if(iTime / 35 < CHOS_GoalVotingTime)
    return;
  var aGoalsChosen = [];
  //Spieler durchgehen
  for(var array in aGoalsVoted)
    if(GetType(array) == C4V_Array)
      for(var i = 0; i < GetLength(array); i++)
        aGoalsChosen[i] += array[i];
  //Alle Ziele mit dem höchsten Wert raussuchen sowie einen String zur Auswertung bereitstellen
  var str = "";
  var highest;
  for(var i in aGoalsChosen)
    highest = Max(highest, i);
  array = [];
  var cnt;
  //Zählen, wie viele Spielziele mehr als 0 Punkte haben.
  for(var i = 0; i < GetLength(aGoals); i++)
    if(aGoalsChosen[i])
      cnt++;

  for(var i = 0; i < GetLength(aGoals); i++)
  {
    if(aGoalsChosen[i])
    {
      var clr = 0xFFFF33;
      if(aGoalsChosen[i] == highest)
        clr = 0x33CCFF;

      if(str != "")
        str = Format("%s, ", str);
      str = Format("%s%s: <c %x>%d</c>", str, GetName(0, aGoals[i]), clr, aGoalsChosen[i]);
    }
    if(aGoalsChosen[i] == highest)
      array[GetLength(array)] = aGoals[i];
  }

  //Empfohlene Spielziele heraussuchen
  var rGoals = RecommendedGoals(), array2 = [];
  for(var i = 0; i < GetLength(array); i++)
  {
    if(GetIndexOf(array[i], rGoals) >= 0)
      array2[GetLength(array2)] = array[i];
  }

  //Und zufällig eins auswählen
  var idGoal = array[Random(GetLength(array))];

  //Empfohlene Spielziele haben die Höchstpunktzahl erreicht: Darunter auswählen
  if(GetLength(array2))
    idGoal = array2[Random(GetLength(array2))];

  str = Format("$Goal$ (%s)", GetName(0, idGoal), str);
  if(idGoal)
    CreateGoal(idGoal, aTempGoalSave[GetIndexOf(idGoal, aGoals)], str);
  else
    OpenGoalChooseMenu();
  return -1;
}

protected func MenuQueryCancel()
{
  //return _inherited(...) || GetEffect("EvaluateGoalVote", this);

  //Menüs sollten immer geöffnet bleiben, sodass z.B. der Host sie nicht
  //schließen kann und erst umständlich über das Regelmenü wieder öffnen muss
  return true;
}