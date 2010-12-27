/*-- Spielregelwahl --*/

//Erweitert die Spielregelwahl und erlaubt Voreinstellungen der Regeln in einem Szenario.

#strict 2

#appendto CHOS

local iEffectCount;
local iChoosePlr;


/* Initialisierung */

protected func Initialize()
{
  _inherited();
  LoadRuleCfg();
  iEffectCount = 3;
}

/* Nur eine Mitteilung für Neugierige */

protected func Activate(iPlr)
{
  if(!iPlr)
    return OpenMenu();
  MessageWindow(Format("$Choosing$", GetPlayerName(iChoosePlr)), iPlr);
}

protected func OpenMenu()
{
  if(GetLength(aGoals))
    return OpenGoalChooseMenu();

  var pClonk = GetCursor(iChoosePlr);
  if(!pClonk)
    return ScheduleCall(this, "OpenMenu", 1);

  if(GetMenu(pClonk))
    CloseMenu(pClonk);

  Message("", pClonk);

  CreateMenu(GetID(), pClonk, 0, 0, 0, 0, 1);
  // Spielregeln
  if (!GetLeague())
    AddMenuItem("$CreateRules$", "OpenRuleMenu", CTFL, pClonk, 0,0, "$RuleInfo$");
  // Dunkelheit
  if(IsDark())
    AddMenuItem("%s", "OpenDarknessMenu", DARK, pClonk,0,0,"$DarknessChose$");
  // Spielziel
  if(GoalIsCompatible() && !GetLeague())
    AddMenuItem("%s", "OpenGoalMenu", GetID(pGoal), pClonk,0,0,"$GoalChose$");
  // KI
  if(ObjectCount(WAYP) && !GetLeague())
    AddMenuItem("$AIMenu$", "OpenAIMenu", HZCK, pClonk, 0,0, "$AIInfo$");
  // Effekte
  AddMenuItem("$Effects$", "OpenEffectMenu", EFMN, pClonk, 0,0, "$EffectInfo$");
  // Fertig
  AddMenuItem("$Finished$", "ConfigurationFinished", CHOS, pClonk,0,0,"$Finished$",2,3);
}

protected func OpenEffectMenu(id dummy, int iSelection)
{
  var pClonk = GetCursor(iChoosePlr);
  // Menü aufmachen
  CreateMenu(GetID(), pClonk, 0,0,0,0, 1);
  // Anzeige
  AddMenuItem(" ", "OpenEffectMenu", EFMN, pClonk, iEffectCount, 0, " ");
  // Zähler erhöhen
  AddMenuItem("$MoreEffects$", "ChangeEffectConf", CHOS, pClonk, 0, +1, "$MoreEffects$",2,1);
  // Zähler senken
  AddMenuItem("$LessEffects$", "ChangeEffectConf", CHOS, pClonk, 0, -1, "$LessEffects$",2,2);
  // Fertig
  AddMenuItem("$Finished$", "OpenMenu", CHOS, pClonk,0,0, "$Finished$",2,3);
  // Letzten Eintrag auswählen
  SelectMenuItem(iSelection, pClonk);
}

protected func ChangeEffectConf(id dummy, int iChange)
{
  // Stand verändern
  iEffectCount = BoundBy(iEffectCount+iChange, 1, 3);
  EFSM_SetEffects(iEffectCount);
  // Geräusch
  Sound("Grab", 1,0,0,1);
  // Menü wieder öffnen
  var iSel = 1;
  if(iChange == -1) iSel = 2;
  OpenEffectMenu(0, iSel);
}

protected func OpenGoalMenu(id dummy, int iSelection)
{
  if(!pGoal)
    return OpenMenu();
  var pClonk = GetCursor();
  if(pGoal->~ConfigMenu(pClonk))
	return 1;
  return _inherited(dummy, iSelection, ...);
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
  // Alle Clonks des Spielers verstauen
  for(var i=0, pCrew, tmp ; pCrew = GetCrew(iPlr, i) ; i++)
  {
    tmp = CreateObject(TIM1, GetX(pCrew), GetY(pCrew), -1);
    SetCategory(GetCategory(tmp) | C4D_Foreground,tmp);
    SetGraphics(0, tmp, GetID(pCrew), 1, 5, 0, 1, pCrew);
    Enter(tmp, pCrew);
    Eastern(tmp);
  }
}

/* Spielziel setzen */

protected func CreateGoal(id idGoal, int iScore)
{
  // Spielziel erstellen
  var goal = CreateObject(idGoal, 0,0, -1);
  // Alten Wert setzen
  SetWinScore(iScore, goal);
  // Alle benachrichtigen
  EventInfo4K(0,Format("$Goal$", GetName(0, idGoal)),idGoal, 0, 0, 0, "Info.ogg");
  // Array leeren, damit das Menü nicht nochmal kommt
  aGoals = CreateArray();
  // Normales Menü öffnen
  OpenMenu();
}

/* Konfiguration abschließen */

protected func ConfigurationFinished2()
{
  Death = true;
  // Regeln erzeugen
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
  // Dunkelheit erzeugen
  log = Format("%s, %s x%d", log, GetName(0, DARK), iDarkCount);
  EventInfo4K(0,log,CHOS, 0, 0, 0, "Info.ogg");
  // ein schneller GameCall für Einstellungen
  GameCallEx("ChooserFinished");

  // Spieler freilassen
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
  // Überschüssiges TIM1-Objekte entfernen (falls Spieler ziwschenzeitlich geflogen sind)
  for(tmp in FindObjects(Find_ID(TIM1)))
    if(!Contents(0, tmp))
      RemoveObject(tmp, 1);
  // Effekte
  EFSM_SetEffects(iEffectCount);
  // Selber entfernen
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
  var pClonk = GetCursor();
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
  AddMenuItem("$VoteGoal$", "OpenGoalVoteMenu", GetID(), pClonk, 0, pClonk, "$VoteGoal$");
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

protected func OpenGoalVoteMenu(id id, object pClonk)
{
  if (!aGoalsVoted)
    aGoalsVoted = [];
  for (var i = 0; i < GetPlayerCount(); i++)
    GoalVoteMenu(0, 0, GetPlayerByIndex(i));
  AddEffect("EvaluateGoalVote", this, 1, 35 * CHOS_GoalVotingTime, this);
  EventInfo4K(0, Format("$GoalVoteBegins$", CHOS_GoalVotingTime), GetID(), 0, 0, 0, "Info.ogg");
}

protected func GoalVoteMenu(id id, object pClonk, int iPlr)
{
  if (!pClonk)
    pClonk = GetCursor(iPlr);
  iPlr = GetOwner(pClonk);
  CloseMenu(pClonk);
  CreateMenu(GetID(), pClonk, 0, 0, 0, 0, C4MN_Style_Context);
  if (!aGoalsVoted[iPlr])
    aGoalsVoted[iPlr] = [];
  for (var i = 0; i < GetLength(aGoals); i++)
    if (aGoalsVoted[iPlr][i])
      AddMenuItem("%s", "CheckVoteGoal", aGoals[i], pClonk, 0, pClonk, GetDesc(0, aGoals[i]));
    else
      AddMenuItem("<c 777777>%s</c>", "CheckVoteGoal", aGoals[i], pClonk, 0, pClonk, GetDesc(0, aGoals[i]));
}

protected func CheckVoteGoal(id idGoal, object pClonk)
{
  var iIndex = GetIndexOf(idGoal, aGoals), iPlr = GetOwner(pClonk);
  aGoalsVoted[iPlr][iIndex] = !aGoalsVoted[iPlr][iIndex];
  GoalVoteMenu(0, pClonk, iPlr);
  SelectMenuItem(iIndex, pClonk);
  Sound("Grab", true, 0, 0, iPlr + 1);
}

protected func FxEvaluateGoalVoteTimer()
{
  for (var i = 0; i < GetPlayerCount(); i++)
    CloseMenu(GetCursor(GetPlayerByIndex(i)));
  var aGoalsChosen = [];
  //Spieler durchgehen
  for (var array in aGoalsVoted)
  {
    if (GetType(array) != C4V_Array)
      continue;
    for (var i = 0; i < GetLength(array); i++)
      aGoalsChosen[i] += array[i];
  }
  //Alle Ziele mit dem höchsten Wert raussuchen
  var highest;
  for (var i in aGoalsChosen)
    highest = Max(highest, i);
  array = [];
  for (var i = 0; i < GetLength(aGoals); i++)
    if (aGoalsChosen[i] == highest)
      array[GetLength(array)] = aGoals[i];
/*  Log("Goals: %v", aGoals);
  Log("Chosen: %v", aGoalsChosen);
  Log("Voting: %v", aGoalsVoted);
  Log("Highest: %v (%d)", array, highest);*/
  //Und zufällig eins auswählen
  var idGoal = array[Random(GetLength(array))];
  if (idGoal)
    CreateGoal(idGoal, aTempGoalSave[GetIndexOf(idGoal, aGoals)]);
  else
    OpenGoalChooseMenu();
  return -1;
}

protected func MenuQueryCancel()
{
  return _inherited(...) || GetEffect("EvaluateGoalVote", this);
}