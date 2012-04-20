/*-- Chooser --*/

#strict 2

#appendto CHOS

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

protected func OpenTeamMenu(id dummy, int iSelection)
{
  var pClonk = GetCursor(iChoosedPlr);
  //Menü erstellen
  CreateMenu(GetID(), pClonk, 0,0,0,0, 1);
  //Teams auflisten
  for(var j = 0; j < GetPlayerCount(); j++)
  {
  	var plr = GetPlayerByIndex(j);
    AddMenuItem(Format("%s (%s)", GetTaggedPlayerName(GetPlayerByIndex(plr), true), GetTeamName(GetPlayerTeam(plr))), "SwitchTeam", PCMK, pClonk, 0, plr);
	}
	
  //Fertig
  AddMenuItem("$Finished$", "OpenMenu", CHOS, pClonk,0,0, "$Finished$",2,3);
  //Letzten Eintrag auswählen
  SelectMenuItem(iSelection, pClonk);
}
