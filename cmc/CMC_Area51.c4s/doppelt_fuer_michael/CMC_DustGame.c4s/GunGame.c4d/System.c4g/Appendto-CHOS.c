/*-- Neues Script --*/

#strict 2

#appendto CHOS

public func CustomClonkEquip() { return true; }

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
  
  //Erzwungene Regelwahl auswaehlen/abwaehlen
  for(var i = 0, idR, def; idR = GetDefinition(i, Chooser_Cat) ; i++) {
  	//Erzwungene Regelauswahlen/-abwahlen nicht auflisten
  	if(pGoal && (GetIndexOf(idR, pGoal->~ForceRuleActivation())+1 || GetIndexOf(idR, pGoal->~ForceRuleDeactivation())+1)) {
  		if(GetIndexOf(idR, pGoal->~ForceRuleActivation()))
  			aRules[i] = true;
  		else
  			aRules[i] = false;
  	}
  }

  return true;
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
  	//Erzwungene Regelauswahlen/-abwahlen nicht auflisten
  	if(pGoal && (GetIndexOf(idR, pGoal->~ForceRuleActivation())+1 || GetIndexOf(idR, pGoal->~ForceRuleDeactivation())+1))
  		continue;

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
  AddMenuItem("$Finished$", "OpenMenu", CHOS, pClonk,0,0,0,2,3);
  //Menüeintrag auswählen
  SelectMenuItem(j, pClonk);
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
    
    //Ggf. check anpassen
    if(GetIndexOf(def, pGoal->~ForceRuleActivation()) != -1)
    	check = true;
    else if(GetIndexOf(def, pGoal->~ForceRuleDeactivation()) != -1)
    	check = false;
    
    if(check)
      CreateObject(def, 10, 10, -1);
    //ggf. existierende Regeln entfernen, wenn Deaktivierung erzwungen ist
    else if(FindObject(def) && GetIndexOf(def, pGoal->~ForceRuleDeactivation()) != -1)
    	RemoveAll(def);

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
