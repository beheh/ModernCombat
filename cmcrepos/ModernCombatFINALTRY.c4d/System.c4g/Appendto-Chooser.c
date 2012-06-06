/*-- Spielregelwahl --*/

//Erweitert die Spielregelwahl und erlaubt unter anderem Voreinstellungen der Regeln innerhalb eines Szenarios.

#strict 2

#appendto CHOS

local iEffectCount;
local iChoosedPlr;
local iTeamCount, arTeams;
local iTeamMode, iUsedTeamSort;


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

  //Spielziele verz�gert entfernen
  ScheduleCall(this(), "RemoveGoals", 1);

  //Host identifizieren
  ChoosePlayer();

  //Dunkelheit ermitteln
  iDarkCount = ObjectCount(DARK);
  
  //Anz. der Teams angeben
  iTeamCount = GetTeamCount();
  arTeams = [];
  for(var i = 0; i < iTeamCount; i++)
    arTeams[i+1] = true;

  //Regeln voreinstellen
  LoadRuleCfg();

  //Effektstufe setzen
  iEffectCount = 3;
  EFSM_SetEffects(3);

  //Spielerr�nge ermitteln
  iAchievementCount = 0;
  while(GetName(0, C4Id(Format("AC%02d", iAchievementCount+1))))
  {
    iAchievementCount++;
  }
  
  //Falls in der Lobby Zuf�llig & unsichtbar gew�hlt wurde, nutzen wir unsere Variante.
  if(!GetTeamConfig(TEAM_AutoGenerateTeams) && GetTeamConfig(TEAM_Dist) == 3)
  	ScheduleCall(this, "CreateTeams", 4, 0, 2-GetTeamConfig(TEAM_AutoGenerateTeams), CHOS_TeamRandomInvisible);
}

/* Statusanzeige im Scoreboard */

static const CHOS_SBRD_Chooser	= SBRD_Caption;
static const CHOS_SBRD_Goal	= 0;
static const CHOS_SBRD_Rules	= 1;
static const CHOS_SBRD_Effect	= 2;
static const CHOS_SBRD_Darkness	= 3;
static const CHOS_SBRD_Teams	= 4;

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
  SortScoreboard(0);
  AddEffect("ChooserScoreboard", this, 21, 10, this);
}

global func SetScoreboardData(int iRowID, int iColID, string sText, int iData, bool fChooser)	//Bearbeitung des Scoreboard w�hrend des Choosers ben�tigt fChooser
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
  SetScoreboardData(CHOS_SBRD_Chooser, 1, GetPlayerName(iChoosedPlr), 0, true);

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
    if(pGoal->~GoalExtraValue()) // Extrawerte f�r die Spielziele (Tickets bei GOCC und so)
    {
      str_extra = Format("(%v)", pGoal->~GoalExtraValue());
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

  //Teamverteilung und Spielerr�nge
  for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
  {
    var row_id = CHOS_SBRD_Teams + 1 + i;
    var plr = GetPlayerByIndex(i, C4PT_User);
    var team_name;
    if(iTeamMode != CHOS_TeamRandomInvisible)
      team_name = GetTeamName(GetPlayerTeam(plr));
    else
      team_name = "$Random$";

    SetScoreboardData(row_id, 0, GetTaggedPlayerName(plr, true), CHOS_SBRD_Teams+plr, true);
    SetScoreboardData(row_id, 1, team_name, 0, true);
  }

  SortScoreboard(0);
}

public func RemovePlayer()
{
  ClearScoreboard(CHOS_SBRD_Teams + GetPlayerCount()+2, 2);
  InitScoreboard();
  ScheduleCall(this, "UpdateScoreboard", 1);
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
      //Eintrag �berschreiben und somit l�schen
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

/* Nur eine Mitteilung f�r Neugierige */

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
  var pClonk = GetCursor(iChoosedPlr);
  if(pGoal->~ConfigMenu(pClonk))
    return 1;
  return _inherited(dummy, iSelection, ...);
}

protected func OpenTeamMenu(id dummy, int iSelection)
{
  var pClonk = GetCursor(iChoosedPlr);
  //Men� erstellen
  CreateMenu(GetID(), pClonk, 0, 0, 0, 0, 1);
  //Teams auflisten
  for(var j = 0; j < GetPlayerCount(); j++)
  {
    var plr = GetPlayerByIndex(j);
    var team_name = GetTeamName(GetPlayerTeam(plr));
    if(iTeamMode == CHOS_TeamRandomInvisible)
      team_name = "$Random$";

    AddMenuItem(Format("%s (%s)", GetTaggedPlayerName(GetPlayerByIndex(plr), true), team_name), "SwitchTeam", PCMK, pClonk, 0, plr);
  }
  
  //Zus�tzliche Teameinstellungen
  AddMenuItem("$RandomTeams$", "ChoosePossibleTeams(CHOS_TeamRandom)", MCMC, pClonk);
  AddMenuItem("$AutoBalanceTeams$", "ChoosePossibleTeams(CHOS_TeamAutobalance)", MCMC, pClonk);

  //Fertig
  AddMenuItem("$Finished$", "OpenMenu", CHOS, pClonk, 0, 0, "$Finished$", 2, 3);
  //Letzten Eintrag ausw�hlen
  SelectMenuItem(iSelection, pClonk);
}

protected func ChoosePossibleTeams(int iMode, bool fInvisible, int iSelection)
{
  var pClonk = GetCursor(iChoosedPlr);

  //Men� erstellen
  CreateMenu(GetID(), pClonk, 0, 0, 0, 0, 1);
  
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
        AddMenuItem("$SwitchInvisible$", Format("ChoosePossibleTeams(CHOS_TeamRandom, false, %d)", 3), CHOS, pClonk, 0, 0, 0, 2, 3);
      else
        AddMenuItem("$SwitchInvisible$", Format("ChoosePossibleTeams(CHOS_TeamRandom, true, %d)", 3), SM06, pClonk);
    }

    AddMenuItem("$CreateTeams$", Format("CreateTeams(1, %d)", iMode+fInvisible), CHOS, pClonk, 0, 0, 0, 2, 3);
    AddMenuItem("$Back$", "OpenTeamMenu", 0, pClonk, 0, 0, "$Back$");
  }
  else
  {
    for(var i = 0; i < GetTeamCount(); i++)
    {
      var team = GetTeamByIndex(i);
      var clr = GetTeamColor(team);
      if(!arTeams[team])
        clr = 0x777777;

      AddMenuItem(Format("<c %x>%s</c>", clr, GetTeamName(team)), Format("SwitchTeam2(%d, %d, %d)", team, iMode, fInvisible), PCMK, pClonk);
    }
    if(iMode == CHOS_TeamRandom)
    {
      if(fInvisible) 
        AddMenuItem("$SwitchInvisible$", Format("ChoosePossibleTeams(CHOS_TeamRandom, false, %d)", GetTeamCount()), CHOS, pClonk, 0, 0, 0, 2, 3);
      else
        AddMenuItem("$SwitchInvisible$", Format("ChoosePossibleTeams(CHOS_TeamRandom, true, %d)", GetTeamCount()), SM06, pClonk);
    }

    AddMenuItem("$CreateTeams$", Format("CreateTeams(2, %d)", iMode+fInvisible), CHOS, pClonk, 0, 0, 0, 2, 3);
    AddMenuItem("$Back$", "OpenTeamMenu", 0, pClonk, 0, 0, "$Back$");
  }

  if(iSelection)
    SelectMenuItem(iSelection, pClonk);

  return true;
}

protected func SwitchTeam2(int iTeam, int iMode, bool fInvisible)
{
  arTeams[iTeam] = !arTeams[iTeam];
  if(GetIndexOf(true, arTeams) == -1) //Deaktivierung aller Teams verhindern
    arTeams[iTeam] = true;

  ChoosePossibleTeams(iMode, fInvisible, iTeam-1);
  return true;
}

protected func ChangeTeamCount(int iChange, int iMode, bool fInvisible)
{
  iTeamCount = BoundBy(iTeamCount + iChange, 1, GetPlayerCount(C4PT_User));
  ChoosePossibleTeams(iMode, fInvisible, !!(iChange-1)+1);
  return true;
}

static const CHOS_TeamManual		= 0; // Manuell
static const CHOS_TeamRandom		= 1; // Zuf�llig
static const CHOS_TeamRandomInvisible	= 2; // Zuf�llig & unsichtbar
static const CHOS_TeamAutobalance	= 3; // Autobalanced

protected func CreateTeams(int iTeamSort, int iMode)
{
  iTeamMode = iMode;
  iUsedTeamSort = iTeamSort;

  if(iMode == CHOS_TeamRandom) // Zuf�llige Teamzusammenstellung.
  {
    var players = []; var teams = [];
    for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
      players[i] = GetPlayerByIndex(i, C4PT_User);

    var i = 0;
    while(GetLength(players))
    {
      ++i;
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

      var pos = Random(GetLength(players));
      var plr = players[pos];
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
      SetPlayerTeam(GetPlayerByIndex(i), 1);

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
    var sum_nrs;			//Summe dieser Zahlen (-> R�nge)
    for(var rank in arNumbers)
      sum_nrs += rank;

    var max_rows = nr_cnt / team_count + (nr_cnt % team_count);	//Max. Anzahl von Zeilen
    var max_nr = sum_nrs / team_count + (sum_nrs % team_count);	//H�chste zu erreichende Zahl

    if(AB_GetMaxPlayerRank(arNumbers, true) > max_nr) //Sonderregelung: H�here Zahl in der Liste als die h�chste, zu erreichende Zahl
    {
      max_nr = AB_GetMaxPlayerRank(arNumbers, true);
      max_rows = 0x7FFFFFFF;
    }

    //Zusammenf�gen
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

    var blahar = true;
    while(blahar)
    {
      if(AB_DifferenceCheck(teams))
        break;

      for(var i = iTeamCount-1; i >= 0; i--)
      {
        if(!AB_Method1(teams[i-1], teams[i]))
          blahar = false;
        else
          blahar = true;
      }
    }
    if(!blahar)
    {
      blahar = true;
      while(blahar)
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
          blahar = false;
      }
    }

    //Aufstellung fertigstellen
    var arTempTeams = arTeams;
    var aExcept = [];
    for(var i = 0; i < GetLength(teams); i++)
    {
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

  return OpenTeamMenu();
}

/* Weitere Autobalance Methoden, zur �bersichtlichkeit ausgelagert. */

//Austausch: Es werden in beiden Listen versucht, Zahlen auszutauschen, um ein besseres Ergebnis zu erreichen.
public func AB_Method1(array &arListOne, array &arListTwo)
{
  var sumL1 = AB_ArraySum(arListOne);	//Summe beider Listen
  var sumL2 = AB_ArraySum(arListTwo);

  var savedL1 = arListOne;		//Tempor�re Speicher
  var savedL2 = arListTwo;
  var savedSums = [sumL1, sumL2];	//Tempor�re Summen beider Listen

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

//Es wird gepr�ft, ob es m�glich ist, aus einem Array kleinere Zahlen in den anderen Array zu packen, um einen Ausgleich zu erhalten.
public func AB_Method2(array &arTeams, int iMaxNr)
{
  var arraySums = [];
  for(var i = 0; i < GetLength(arTeams); i++)
    arraySums[i] = AB_ArraySum(arTeams[i]);

  var teamMin = GetMinArrayVal(arraySums, true); //Array-Positionen in arTeams
  var teamMax = GetMaxArrayVal(arraySums, true); 

  var arTempTeams = arTeams; //Zur Absicherung

  var val = GetMinArrayVal(arTempTeams[teamMax]); //Kleinste Zahl im gr��tem Array
  var pos = GetMinArrayVal(arTempTeams[teamMax], true); //und deren Position

  DelArrayItem4K(arTempTeams[teamMax], pos); // Zahl entfernen
  arTempTeams[teamMin][GetLength(arTempTeams[teamMin])] = val; // Zahl hinzuf�gen

  if(AB_ArraySum(arTempTeams[teamMin]) <= iMaxNr) //�nderung darf nicht �ber das Maximum
  {
    arTeams = arTempTeams; //�nderungen �bernehmen
    return true;
  }
  else
    return false;
}

//Hinweis: Die Funktionen mit AB_-Pr�fix sollten m�glichst nur vom Autobalancer eingesetzt werden.
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
    return -1;

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
  if(GetTeamName(GetTeamByIndex(team)))
    team = GetTeamByIndex(team);
  else
    team = GetTeamByIndex(0);

  SetPlayerTeam(iPlr, team);

  var sel = GetMenuSelection(GetCursor(iChoosedPlr));

  //Ger�usch
  Sound("Grab", 1,0,0,1);

  for(var j = 0; j < GetPlayerCount(); j++)
  {
    if(j == 0) continue;
    CloseMenu(GetCursor(GetPlayerByIndex(j)));
  }

  iTeamMode = CHOS_TeamManual;
  SetScoreboardData(CHOS_SBRD_Teams, 0, "$TeamsSortedManually$", 0, true);

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
  //Array leeren um erneuten Men�aufruf zu verhindern
  aGoals = CreateArray();
  //Normales Men� �ffnen
  OpenMenu();
}

/* Konfiguration abschlie�en */

protected func ConfigurationFinished2()
{
  //Zuf�llige und unsichtbare Teams
  if(iTeamMode == CHOS_TeamRandomInvisible)
    CreateTeams(iUsedTeamSort, CHOS_TeamRandom);

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
  //Schneller GameCall f�r Einstellungen
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
  ClearScoreboard(CHOS_SBRD_Teams + GetPlayerCount(), 1);
  //Nach einem Frame die Auswahlsperre aufl�sen
  ScheduleCall(pGoal, "InitScoreboard", 1);
  //Selber entfernen
  RemoveObject();
}

private func IsStandardSetting()
{
  var a = GameCall("ChooserRuleConfig"), i;
  for (var i = 0; i < GetLength(aRules); i++)
  {
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
    AddMenuItem("%s", Format("CreateGoal(%i, %d)", goal, aTempGoalSave[i]), goal, pClonk, 0);
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
  //Alle Ziele mit dem h�chsten Wert raussuchen sowie einen String zur Auswertung bereitstellen
  var str = "";
  var highest;
  for(var i in aGoalsChosen)
    highest = Max(highest, i);
  array = [];
  var cnt;
  //Z�hlen, wie viele Spielziele mehr als 0 Punkte haben.
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
  //Und zuf�llig eins ausw�hlen
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

  //Men�s sollten immer ge�ffnet bleiben, sodass z.B. der Host sie nicht
  //schlie�en kann und erst umst�ndlich �ber das Regelmen� wieder �ffnen muss
  return true;
}