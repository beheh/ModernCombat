/*-- Spielregelwahl --*/

//Erweitert die Spielregelwahl und erlaubt Voreinstellungen der Regeln in einem Szenario.

#strict 2

#appendto CHOS

protected func Initialize()
{
  CMPS_iCompassConf = 10;
  
  return _inherited(...);
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
  //Kompass
  AddMenuItem("$CompassMenu$", "OpenCompassMenu", CSTD, pClonk, 0, 0, "$CompassInfo$");
  
  //Effekte
  AddMenuItem("$Effects$", "OpenEffectMenu", EFMN, pClonk, 0,0, "$EffectInfo$");
  //Fertig
  AddMenuItem("$Finished$", "ConfigurationFinished", CHOS, pClonk,0,0,"$Finished$",2,3);
}

protected func OpenCompassMenu(id dummy, int iSelection)
{
  var pClonk = GetCursor(GetPlayerByIndex(0, C4PT_User));
  //Menü aufmachen
  CreateMenu(GetID(), pClonk, 0,0,0,0, 1);
  //Kompass (de)aktivieren
 	if(!CMPS_fNoCompass)
 		AddMenuItem("$CompassActivated$", "SwitchCompass", CMPM, pClonk, 0, 0, 0, 2, 2);
 	else	
 		AddMenuItem("$CompassDeactivated$", "SwitchCompass", CMPM, pClonk, 0, 0, 0, 2, 3);
  if(!CMPS_fNoCompass)
  {
  	AddMenuItem(Format("$CompassIntervall$", CMPS_iCompassConf), 0, FLNT, pClonk); // Jaja, iCount geht auch, ist aber hässlich.
  	//Zähler erhöhen
  	AddMenuItem("$MoreFrames$", "ChangeCompassConf", CHOS, pClonk, 0, +1, "", 2, 1);
  	//Zähler senken
  	AddMenuItem("$LessFrames$", "ChangeCompassConf", CHOS, pClonk, 0, -1, "", 2, 2);
  }
  //Fertig
  AddMenuItem("$Finished$", "OpenMenu", CHOS, pClonk,0,0, "$Finished$",2,3);
  //Letzten Eintrag auswählen
  SelectMenuItem(iSelection, pClonk);
}

public func SwitchCompass(id dummy)
{
	CMPS_fNoCompass = !CMPS_fNoCompass;
	if(!CMPS_fNoCompass)
		for(var hud in FindObjects(Find_ID(1HUD)))
			hud->CreateCompass();
	else
		for(var compass in FindObjects(Find_ID(CMPS)))
			RemoveObject(compass);
			
	if(dummy)
		return OpenCompassMenu();
}

public func ChangeCompassConf(id dummy, int iChange, int iValue)
{
	if(iValue) CMPS_iCompassConf = iValue;
	else CMPS_iCompassConf = BoundBy(CMPS_iCompassConf+iChange, 1, 60);
	
	for(var compass in FindObjects(Find_ID(CMPS)))
		compass->ChangeIntervall(CMPS_iCompassConf);
	
	if(iValue) return true;
	
	var iSel = 3;
	if(iChange == -1) iSel = 4;
	
	return OpenCompassMenu(0, iSel); 
}
/*
protected func InitializePlayer(int iPlr, int iX, int iY, object pBase, int iTeam, id idExtra)
{
  if(GetPlayerType(iPlr) == C4PT_Script)
    for(var i = 1 ; i < aAI[iTeam] ; i++)
      CreateClonk(iPlr);
  aAI[iTeam] = 0;
  if(Death) return;
  //Alle Clonks des Spielers verstauen
  Log("Schleife für %d (iPlr)", iPlr);
  for(var i=0, pCrew, tmp ; pCrew = GetCrew(iPlr, i) ; i++)
  {
    tmp = CreateObject(TIM1, GetX(pCrew), GetY(pCrew), -1);
    SetCategory(GetCategory(tmp) | C4D_Foreground,tmp);
    SetGraphics(0, tmp, GetID(pCrew), 1, 5, 0, 1, pCrew);
    Enter(tmp, pCrew);
    Eastern(tmp);
    Log("%d = i, %v = pCrew, %v = tmp, %d = iPlr", i, pCrew, tmp, iPlr);
  }
  //Falls ein Spieler während der Abstimmung beitritt
  if(GetEffect("EvaluateGoalVote", this))
    GoalVoteMenu(0, 0, iPlr);
}*/

