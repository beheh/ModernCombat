/*-- Spielregelwahl --*/

//Erweitert die Spielregelwahl und erlaubt Voreinstellungen der Regeln in einem Szenario.

#strict 2

#appendto CHOS

local iEffectCount;


/* Initialisierung */

protected func Initialize()
{
  SetPosition();
  aRules = CreateArray();
  aGoals = CreateArray();
  aTempGoalSave = CreateArray();
  aAI = CreateArray();

  //Spielziele verz�gert entfernen
  ScheduleCall(this(), "RemoveGoals", 1);

  //Dunkelheit ermitteln
  iDarkCount = ObjectCount(DARK);

  //Regeln voreinstellen
  LoadRuleCfg();

  //Effektstufe setzen
  iEffectCount = 3;
  EFSM_SetEffects(3);
}

/* Nur eine Mitteilung f�r Neugierige */

protected func Activate(iPlr)
{
  if(iPlr == GetPlayerByIndex(0, C4PT_User))
    return OpenMenu();
  MessageWindow(Format("$Choosing$", GetPlayerName(GetPlayerByIndex(0, C4PT_User))), iPlr);
}

protected func OpenMenu()
{
  if(GetLength(aGoals))
    return OpenGoalChooseMenu();

  var pClonk = GetCursor(GetPlayerByIndex(0, C4PT_User));
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
  if(GoalIsCompatible() && !GetLeague())
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
  var pClonk = GetCursor(GetPlayerByIndex(0, C4PT_User));
  //Men� aufmachen
  CreateMenu(GetID(), pClonk, 0,0,0,0, 1);
  //Anzeige
  AddMenuItem(" ", "OpenEffectMenu", EFMN, pClonk, iEffectCount, 0, " ");
  //Z�hler erh�hen
  AddMenuItem("$MoreEffects$", "ChangeEffectConf", CHOS, pClonk, 0, +1, "$MoreEffects$",2,1);
  //Z�hler senken
  AddMenuItem("$LessEffects$", "ChangeEffectConf", CHOS, pClonk, 0, -1, "$LessEffects$",2,2);
  //Fertig
  AddMenuItem("$Finished$", "OpenMenu", CHOS, pClonk,0,0, "$Finished$",2,3);
  //Letzten Eintrag ausw�hlen
  SelectMenuItem(iSelection, pClonk);
}

protected func ChangeEffectConf(id dummy, int iChange)
{
  //Stand ver�ndern
  iEffectCount = BoundBy(iEffectCount+iChange, 1, 3);
  EFSM_SetEffects(iEffectCount);
  //Ger�usch
  Sound("Grab", 1,0,0,1);
  //Men� wieder �ffnen
  var iSel = 1;
  if(iChange == -1) iSel = 2;
  OpenEffectMenu(0, iSel);
}

protected func OpenGoalMenu(id dummy, int iSelection)
{
  if(!pGoal)
    return OpenMenu();
  var pClonk = GetCursor(GetPlayerByIndex(0, C4PT_User));
  if(pGoal->~ConfigMenu(pClonk))
	return 1;
  return _inherited(dummy, iSelection, ...);
}

protected func OpenTeamMenu(id dummy, int iSelection)
{
  var pClonk = GetCursor(GetPlayerByIndex(0, C4PT_User));
  //Men� erstellen
  CreateMenu(GetID(), pClonk, 0,0,0,0, 1);
  //Teams auflisten
  for(var j = 0; j < GetPlayerCount(); j++)
    AddMenuItem(Format("%s (%s)", GetTaggedPlayerName(GetPlayerByIndex(j)), GetTeamName(GetPlayerTeam(j))), "SwitchTeam", PCMK, pClonk, 0, GetPlayerByIndex(j));

  //Fertig
  AddMenuItem("$Finished$", "OpenMenu", CHOS, pClonk,0,0, "$Finished$",2,3);
  //Letzten Eintrag ausw�hlen
  SelectMenuItem(iSelection, pClonk);

  for(var j = 0; j < GetPlayerCount(); j++)
  {
    if(j == 0) continue;
    //F�r jeden Spieler das selbe Men� auch nochmal erstellen
    var clnk = GetCursor(GetPlayerByIndex(j));
    CreateMenu(GetID(), clnk, 0, 0, 0, 0, 1);
    for(var k = 0; k < GetPlayerCount(); k++)
    {
      AddMenuItem(Format("%s (%s)", GetTaggedPlayerName(GetPlayerByIndex(k)), GetTeamName(GetPlayerTeam(k))), 0, PCMK, clnk, 0, GetPlayerByIndex(k));
    }
  }
}

protected func SwitchTeam(id dummy, int iPlr)
{
  var team = GetPlayerTeam(iPlr);
  if(GetTeamName(GetTeamByIndex(team)))
    team = GetTeamByIndex(team);
  else
    team = GetTeamByIndex(0);

  SetPlayerTeam(iPlr, team);

  var sel = GetMenuSelection(GetCursor(GetPlayerByIndex(0, C4PT_User)));

  //Ger�usch
  Sound("Grab", 1,0,0,1);

  for(var j = 0; j < GetPlayerCount(); j++)
  {
    if(j == 0) continue;
    CloseMenu(GetCursor(GetPlayerByIndex(j)));
  }

  //Men� wieder er�ffnen
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
  //Falls ein Spieler w�hrend der Abstimmung beitritt
  if(GetEffect("EvaluateGoalVote", this))
    GoalVoteMenu(0, 0, iPlr);
}

/* Spielziel setzen */

protected func CreateGoal(id idGoal, int iScore)
{
  //Spielziel erstellen
  var goal = CreateObject(idGoal, 0,0, -1);
  //Alten Wert setzen
  SetWinScore(iScore, goal);
  //Alle benachrichtigen
  EventInfo4K(0,Format("$Goal$", GetName(0, idGoal)),idGoal, 0, 0, 0, "Info.ogg");
  //Array leeren, damit das Men� nicht nochmal kommt
  aGoals = CreateArray();
  //Normales Men� �ffnen
  OpenMenu();
}

/* Konfiguration abschlie�en */

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
  //ein schneller GameCall f�r Einstellungen
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
  //Selber entfernen
  RemoveObject();
}

private func IsStandardSetting()
{
  var a = GameCall("ChooserRuleConfig"), i;
  for (var i = 0; i < GetLength(aRules); i++) {
    if (GetIndexOf(GetDefinition(i, Chooser_Cat), a) != -1)
    {
      if (!aRules[i]) //Regel im Standardsatz, aber nicht ausgew�hlt
        return false;
    }
    else if (aRules[i]) //Regel ausgew�hlt, aber nicht Standard
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
  var pClonk = GetCursor(GetPlayerByIndex(0, C4PT_User));
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
  if (CHOS_GoalVotingTime - (iTime / 35) < 5)
    Sound("Select.ogg", true);
  for (var i = 0; i < GetPlayerCount(); i++)
    GoalVoteMenu(0, GetCrew(GetPlayerByIndex(i)), GetPlayerByIndex(i));

  if (iTime / 35 < CHOS_GoalVotingTime)
    return;
  var aGoalsChosen = [];
  //Spieler durchgehen
  for (var array in aGoalsVoted)
    if (GetType(array) == C4V_Array)
      for (var i = 0; i < GetLength(array); i++)
        aGoalsChosen[i] += array[i];
  //Alle Ziele mit dem h�chsten Wert raussuchen
  var highest;
  for (var i in aGoalsChosen)
    highest = Max(highest, i);
  array = [];
  for (var i = 0; i < GetLength(aGoals); i++)
    if (aGoalsChosen[i] == highest)
      array[GetLength(array)] = aGoals[i];
  //Und zuf�llig eins ausw�hlen
  var idGoal = array[Random(GetLength(array))];
  if (idGoal)
    CreateGoal(idGoal, aTempGoalSave[GetIndexOf(idGoal, aGoals)]);
  else
    OpenGoalChooseMenu();
  return -1;
}

protected func MenuQueryCancel()
{
  //return _inherited(...) || GetEffect("EvaluateGoalVote", this);

  //Men�s sollten immer ge�ffnet bleiben, sodass zB der Host sie nicht
  //wegdr�cken kann und erst umst�ndlich �bers Regelmen� �ffnen muss
  return true;
}