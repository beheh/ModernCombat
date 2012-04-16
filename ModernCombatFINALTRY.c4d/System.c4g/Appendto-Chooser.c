/*-- Spielregelwahl --*/

//Erweitert die Spielregelwahl und erlaubt unter anderem Voreinstellungen der Regeln innerhalb eines Szenarios.

#strict 2

#appendto CHOS

local iEffectCount;
local iChoosedPlr;


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
  ScheduleCall(this(), "RemoveGoals", 1);

  //Host identifizieren
  ChoosePlayer();

  //Dunkelheit ermitteln
  iDarkCount = ObjectCount(DARK);

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
}

/* Statusanzeige im Scoreboard */

static const CHOS_SBRD_Chooser	= 0;
static const CHOS_SBRD_Goal	= 1;
static const CHOS_SBRD_Rules	= 2;
static const CHOS_SBRD_Effect	= 3;
static const CHOS_SBRD_Darkness	= 4;
static const CHOS_SBRD_Teams	= 5;

public func InitScoreboard()
{
  //Scoreboard einblenden
  Schedule("DoScoreboardShow(1)", 1);

  //Zeilen setzen
  SetScoreboardData(SBRD_Caption, SBRD_Caption, "", 0, true);
  SetScoreboardData(CHOS_SBRD_Chooser, SBRD_Caption, "$ScoreboardChooser$", CHOS_SBRD_Chooser, true);
  SetScoreboardData(CHOS_SBRD_Goal, SBRD_Caption, "$ScoreboardGoals$", CHOS_SBRD_Goal, true);
  SetScoreboardData(CHOS_SBRD_Rules, SBRD_Caption, "$ScoreboardRules$", CHOS_SBRD_Rules, true);
  SetScoreboardData(CHOS_SBRD_Effect, SBRD_Caption, "$ScoreboardEffects$", CHOS_SBRD_Effect, true);
  SetScoreboardData(CHOS_SBRD_Darkness, SBRD_Caption, "$ScoreboardDarkness$", CHOS_SBRD_Darkness, true);
  SetScoreboardData(CHOS_SBRD_Teams, SBRD_Caption, "$ScoreboardTeams$", CHOS_SBRD_Teams, true);
  AddEffect("ChooserScoreboard", this, 21, 10, this);
}

global func SetScoreboardData(int iRowID, int iColID, string sText, int iData, bool fChooser) // Bearbeitung des Scoreboard während des Choosers benötigt fChooser.
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
  SetScoreboardData(CHOS_SBRD_Chooser, 0, GetPlayerName(iChoosedPlr), 0, true);
  
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
    if(pGoal->~GoalExtraValue()) // Extrawerte für die Spielziele (Tickets bei GOCC und so)
    {
      str_extra = Format("(%v)", pGoal->~GoalExtraValue());
    }
  }
  SetScoreboardData(CHOS_SBRD_Goal, 0, Format("%s%s", str_goals, str_extra), 0, true);

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
  SetScoreboardData(CHOS_SBRD_Rules, 0, str_rules, 0, true);

  //Effektstufe
  SetScoreboardData(CHOS_SBRD_Effect, 0, Format("%dx", iEffectCount), 0, true);

  //Dunkelheitsstufe
  if(FindObject(DARK))
  {
    SetScoreboardData(CHOS_SBRD_Darkness, SBRD_Caption, "$ScoreboardDarkness$", CHOS_SBRD_Darkness, true);
    SetScoreboardData(CHOS_SBRD_Darkness, 0, Format("%dx", iDarkCount), 0, true);
  }
  else
  {
    SetScoreboardData(CHOS_SBRD_Darkness, SBRD_Caption, "<c 777777>$ScoreboardDarkness$</c>", CHOS_SBRD_Darkness, true);
    SetScoreboardData(CHOS_SBRD_Darkness, 0, Format("<c 777777>%dx</c>", iDarkCount), 0, true);
  }
  
  //Teamverteilung und Spielerränge
  for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
  {
    var row_id = CHOS_SBRD_Teams + 1 + i;
    var plr = GetPlayerByIndex(i, C4PT_User);

    SetScoreboardData(row_id, SBRD_Caption, GetTaggedPlayerName(plr, true), 0, true);
    SetScoreboardData(row_id, 0, GetTeamName(GetPlayerTeam(plr)), 0, true);
  }
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
  for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
    if(GetPlayerID(GetPlayerByIndex(i, C4PT_User)) == 1)
      return iChoosedPlr = GetPlayerByIndex(i, C4PT_User);
}

/* Nur eine Mitteilung für Neugierige */

protected func Activate(iPlr)
{
  if(iPlr == iChoosedPlr)
    return OpenMenu();
  MessageWindow(Format("$Choosing$", GetPlayerName(iChoosedPlr)), iPlr);
}

protected func OpenMenu()
{
  if(GetLength(aGoals))
    return OpenGoalChooseMenu();

  var pClonk = GetCursor(iChoosedPlr);
  if(!pClonk)
    return ScheduleCall(this, "OpenMenu", 1);

  for(var i = 0; i < GetPlayerCount(); i++)
  {
    var clnk = GetCursor(GetPlayerByIndex(i));
    if(GetMenu(clnk))
      CloseMenu(clnk);
  }

  Message("", pClonk);

  CreateMenu(GetID(), pClonk, 0, 0, 0, 0, 1);
  //Spielregeln
  if(!GetLeague())
    AddMenuItem("$CreateRules$", "OpenRuleMenu", CTFL, pClonk, 0,0, "$RuleInfo$");
  //Dunkelheit
  if(IsDark())
    AddMenuItem("%s", "OpenDarknessMenu", DARK, pClonk,0,0,"$DarknessChose$");
  //Spielziel
  if(pGoal && pGoal->~IsConfigurable() && !GetLeague())
    AddMenuItem("%s", "OpenGoalMenu", GetID(pGoal), pClonk,0,0,"$GoalChose$");
  //Teameinteilung
  if(GetTeamCount() && !GetLeague())
    AddMenuItem("$TeamArrangement$", "OpenTeamMenu", TEAM, pClonk, 0, 0, "$TeamInfo$");
  //KI
  if(ObjectCount(WAYP) && !GetLeague())
    AddMenuItem("$AIMenu$", "OpenAIMenu", HZCK, pClonk, 0,0, "$AIInfo$");
  //Effekte
  AddMenuItem("$Effects$", "OpenEffectMenu", EFMN, pClonk, 0,0, "$EffectInfo$");
  //Fertig
  AddMenuItem("$Finished$", "ConfigurationFinished", CHOS, pClonk,0,0,"$Finished$",2,3);
}

protected func OpenEffectMenu(id dummy, int iSelection)
{
  var pClonk = GetCursor(iChoosedPlr);
  //Menü aufmachen
  CreateMenu(GetID(), pClonk, 0,0,0,0, 1);
  //Anzeige
  AddMenuItem(" ", "OpenEffectMenu", EFMN, pClonk, iEffectCount, 0, " ");
  //Zähler erhöhen
  AddMenuItem("$MoreEffects$", "ChangeEffectConf", CHOS, pClonk, 0, +1, "$MoreEffects$",2,1);
  //Zähler senken
  AddMenuItem("$LessEffects$", "ChangeEffectConf", CHOS, pClonk, 0, -1, "$LessEffects$",2,2);
  //Fertig
  AddMenuItem("$Finished$", "OpenMenu", CHOS, pClonk,0,0, "$Finished$",2,3);
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
  var pClonk = GetCursor(iChoosedPlr);
  if(pGoal->~ConfigMenu(pClonk))
    return 1;
  return _inherited(dummy, iSelection, ...);
}

protected func OpenTeamMenu(id dummy, int iSelection)
{
  var pClonk = GetCursor(iChoosedPlr);
  //Menü erstellen
  CreateMenu(GetID(), pClonk, 0, 0, 0, 0, 1);
  //Teams auflisten
  for(var j = 0; j < GetPlayerCount(); j++)
  {
    var plr = GetPlayerByIndex(j);
    AddMenuItem(Format("%s (%s)", GetTaggedPlayerName(GetPlayerByIndex(plr), true), GetTeamName(GetPlayerTeam(plr))), "SwitchTeam", PCMK, pClonk, 0, plr);
  }

  //Fertig
  AddMenuItem("$Finished$", "OpenMenu", CHOS, pClonk, 0, 0, "$Finished$", 2, 3);
  //Letzten Eintrag auswählen
  SelectMenuItem(iSelection, pClonk);
}

protected func SwitchTeam(id dummy, int iPlr)
{
  var team = GetPlayerTeam(iPlr);
  if(GetTeamName(GetTeamByIndex(team)))
    team = GetTeamByIndex(team);
  else
    team = GetTeamByIndex(0);

  SetPlayerTeam(iPlr, team);

  var sel = GetMenuSelection(GetCursor(iChoosedPlr));

  //Geräusch
  Sound("Grab", 1,0,0,1);

  for(var j = 0; j < GetPlayerCount(); j++)
  {
    if(j == 0) continue;
    CloseMenu(GetCursor(GetPlayerByIndex(j)));
  }

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
  //Spielziel erstellen
  var goal = CreateObject(idGoal, 0,0, -1);
  //Spielziel als lokale Variable setzen
  pGoal = goal;
  //Alten Wert setzen
  SetWinScore(iScore, goal);
  //Alle benachrichtigen
  if(!szMessage)
     EventInfo4K(0, Format("$Goal$", GetName(0, idGoal)), idGoal, 0, 0, 0, "Info.ogg");
  else
    EventInfo4K(0, szMessage, idGoal, 0, 0, 0, "Info.ogg");
  //Array leeren um erneuten Menüaufruf zu verhindern
  aGoals = CreateArray();
  //Normales Menü öffnen
  OpenMenu();
}

/* Konfiguration abschließen */

protected func ConfigurationFinished2()
{
  Death = true;
  //Regeln erzeugen
  var i = 0, j = 0, pCrew, tmp, log, def;
  if (IsStandardSetting())
    log = "$StdRules$";
  else
    log = "$Rules$";
  for(var check in aRules)
  {
    if(check)
    {
      def = GetDefinition(i, Chooser_Cat);
      CreateObject(def, 10,10, -1);
      if(j)
        log = Format("%s, <c %x>%s</c>", log, GetRuleColor(def), GetName(0, def));
      else
        log = Format("%s<c %x>%s</c>", log, GetRuleColor(def), GetName(0, def));
      j++;
    }
    i++;
  }
  //Dunkelheit erzeugen
  log = Format("%s, %s x%d", log, GetName(0, DARK), iDarkCount);
  EventInfo4K(0,log,CHOS, 0, 0, 0, "Info.ogg");
  //Schneller GameCall für Einstellungen
  GameCallEx("ChooserFinished");

  //Spieler freilassen
  for(i = 0 ; i < GetPlayerCount() ; i++)
  {
    for(j = 0 ; pCrew = GetCrew(GetPlayerByIndex(i), j) ; j++)
    {
      tmp = Contained(pCrew);
      RemoveObject(tmp, 1);
      pCrew->~Recruitment(GetOwner(pCrew));
    }
    for(var rule in FindObjects(Find_Category(Chooser_Cat), Find_Exclude(this)))
      rule->~InitializePlayer(GetPlayerByIndex(i));
  }

  //Scoreboard leeren
  ClearScoreboard(CHOS_SBRD_Teams + GetPlayerCount(), 0);
  //Nach einem Frame die Auswahlsperre auflösen
  ScheduleCall(pGoal, "InitScoreboard", 1);
  //Selber entfernen
  RemoveObject();
}

private func IsStandardSetting()
{
  var a = GameCall("ChooserRuleConfig"), i;
  for (var i = 0; i < GetLength(aRules); i++) {
    if (GetIndexOf(GetDefinition(i, Chooser_Cat), a) != -1)
    {
      if (!aRules[i]) //Regel im Standardsatz, aber nicht ausgewählt
        return false;
    }
    else if (aRules[i]) //Regel ausgewählt, aber nicht Standard
      return false;
  }
  return true;
}

private func GetRuleColor(id idDef)
{
  if (GetIndexOf(idDef, GameCall("ChooserRuleConfig")) == -1)
    return RGB(255);
  return RGB(255,255,255);
}

/* Easteregg Textnachrichten */

private func Eastern(object P)
{
  var rand = Random(17);
  if(!rand) SetName(GetPlayerName(GetPlayerByIndex(Random(GetPlayerCount()))), P);
  if(rand == 1) SetName("$Emptiness$", P);
  if(rand == 2) SetName("$YourBrain$", P);
  if(rand == 3) SetName("$Nothing$", P);
  if(rand == 4) SetName("$Freezer$", P);
  if(rand == 5) SetName("$Space$", P);
  if(rand == 6) SetName("$YourGun$", P);
  if(rand == 7) SetName("$YourWallet$", P);
  if(rand == 8) SetName("$YourCookieJar$", P);
  if(rand == 9) SetName("$ThisWindow$", P);
  if(rand == 10) SetName("$YourStomach$", P);
  if(rand == 11) SetName("$EverythingEmpty$", P);
  if(rand == 12) SetName("$Barrel$", P);
  if(rand == 13) SetName("$Beer$", P);
  if(rand == 14) SetName("$YourGlance$", P);
  if(rand == 15) SetName("$YourHarddrive$", P);
  if(rand == 16) SetName("$YourChat$", P);
}

/* Spielzielwahl */

protected func OpenGoalChooseMenu()
{
  var pClonk = GetCursor(iChoosedPlr);
  if (!pClonk || !GetLength(aGoals))
    return ScheduleCall(this, "OpenMenu", 1);

  CloseMenu(pClonk);

  if(GetLength(aGoals) == 1)
    return CreateGoal(aGoals[0], aTempGoalSave[0]);

  CreateMenu(GetID(), pClonk, 0, 0, 0, 0, 1);

  var i;
  for(var goal in aGoals)
  {
    AddMenuItem("%s", "CreateGoal", goal, pClonk, 0, aTempGoalSave[i]);
    i++;
  }
  //Zufall
  AddMenuItem("$RandomGoal$", "OpenGoalRandomMenu", GetID(), pClonk, 0, pClonk, "$RandomGoal$");
  //Abstimmung
  AddMenuItem("$VoteGoal$", "CheckGoalVoteMenu", GetID(), pClonk, 0, pClonk, "$VoteGoal$");
}

/* Random */

local aGoalsChecked;

protected func OpenGoalRandomMenu(id id, object pClonk)
{
  CreateMenu(GetID(), pClonk, 0, 0, 0, 0, C4MN_Style_Context);
  if (!aGoalsChecked)
    aGoalsChecked = [];
  var fChecked = false;
  for (var i = 0; i < GetLength(aGoals); i++)
  {
    if (!aGoals[i])
      continue;
    if (aGoalsChecked[i])
    {
      AddMenuItem("%s", "CheckRandomGoal", aGoals[i], pClonk, 0, pClonk, GetDesc(0, aGoals[i]));
      fChecked = true;
    }
    else
      AddMenuItem("<c 777777>%s</c>", "CheckRandomGoal", aGoals[i], pClonk, 0, pClonk, GetDesc(0, aGoals[i]));
  }
  if (fChecked)
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
    if (aGoalsChecked[i])
      array[GetLength(array)] = aGoals[i];
  var idGoal = array[Random(GetLength(array))];
  if (!idGoal)
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
  if (!aGoalsVoted)
    aGoalsVoted = [];
  for (var i = 0; i < GetPlayerCount(); i++)
    GoalVoteMenu(0, 0, GetPlayerByIndex(i));
  AddEffect("EvaluateGoalVote", this, 1, 35, this);
  EventInfo4K(0, Format("$GoalVoteBegins$", CHOS_GoalVotingTime), GetID(), 0, 0, 0, "Info.ogg");
}

protected func GoalVoteMenu(id id, object pClonk, int iPlr)
{
  if (!pClonk && !(pClonk = GetCrew(iPlr)))
    return;
  if (iPlr == NO_OWNER && (iPlr = GetOwner(pClonk)) == NO_OWNER)
    return false;
  var iSelection = GetMenuSelection(pClonk);
  CloseMenu(pClonk);

  var iTime = GetEffect("EvaluateGoalVote", this, 0, 6);
  if (iTime >= CHOS_GoalVotingTime * 35)
    return false;

  CreateMenu(GetID(), pClonk, 0, 0, 0, 0, C4MN_Style_Context);
  if (!aGoalsVoted[iPlr])
    aGoalsVoted[iPlr] = [];

  AddMenuItem(Format("$VoteGoal$ (%d)", CHOS_GoalVotingTime - iTime / 35), 0, GetID(), pClonk, 0, 0, 0, C4MN_Add_ForceNoDesc);
  for (var i = 0; i < GetLength(aGoals); i++)
    if (aGoalsVoted[iPlr][i])
      AddMenuItem("%s", "CheckVoteGoal", aGoals[i], pClonk, 0, pClonk, GetDesc(0, aGoals[i]));
    else
      AddMenuItem("<c 777777>%s</c>", "CheckVoteGoal", aGoals[i], pClonk, 0, pClonk, GetDesc(0, aGoals[i]));
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
      var c = 44;
      if(!--cnt)
        c = 32;

      var clr = 0xFFFF33;
      if(aGoalsChosen[i] == highest)
        clr = 0x33CCFF;

      str = Format("%s %s: <c %x>%d</c>%c", str, GetName(0, aGoals[i]), clr, aGoalsChosen[i], c);
    }
    if(aGoalsChosen[i] == highest)
      array[GetLength(array)] = aGoals[i];
  }
  //Und zufällig eins auswählen
  var idGoal = array[Random(GetLength(array))];
  str = Format("$Goal$ (%s)", GetName(0, idGoal), str);
  if (idGoal)
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
