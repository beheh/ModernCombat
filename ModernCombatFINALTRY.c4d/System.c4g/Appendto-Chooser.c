/*-- Spielregelwahl --*/

//Erweitert die Spielregelwahl und erlaubt Voreinstellungen der Regeln in einem Szenario.

#strict
#appendto CHOS


/* Nur eine Mitteilung für Neugierige */

protected func Activate(iPlr)
{
  if(!iPlr) return(OpenMenu());
  MessageWindow(Format("$Choosing$", GetPlayerName()),iPlr);
}

/* Initalisierung */

protected func Initialize()
{
  _inherited();
  LoadRuleCfg();
}

protected func OpenGoalMenu(id dummy, int iSelection)
{
  if(!pGoal) return(OpenMenu());
  var pClonk = GetCursor();
  if(pGoal->~ConfigMenu(pClonk))
	return(1);//OpenMenu()
  return(_inherited(dummy, iSelection,...));
}

protected func LoadRuleCfg()
{
  var a = GameCall("ChooserRuleConfig");
  if(!GetLength(a)) return();

  for(var i=0, idR, def, j, check ; idR = GetDefinition(i, Chooser_Cat) ; i++)
   if(DefinitionCall(idR, "IsChooseable") && !GetLength(FindObjects(Find_ID(idR))))
   {
    if(FindInArray4K(a,idR) > -1)
     aRules[i] = true;
   }
}

protected func InitializePlayer(int iPlr, int iX, int iY, object pBase, int iTeam, id idExtra)
{
  if(GetPlayerType(iPlr) == C4PT_Script)
    for(var i=1 ; i < aAI[iTeam] ; i++)
      CreateClonk(iPlr);
  aAI[iTeam] = 0;
  if(Death) return();
  // Alle Clonks des Spielers verstauen
  for(var i=0, pCrew, tmp ; pCrew = GetCrew(iPlr, i) ; i++)
    {
    tmp = CreateObject(TIM1, GetX(pCrew), GetY(pCrew), -1);
    SetCategory(GetCategory(tmp) | C4D_Foreground,tmp);
    SetGraphics(0, tmp, GetID(pCrew), 1, 5, 0, 1, pCrew);
    Enter(tmp, pCrew);
    Eastern(tmp);
    }
  // Spieler 1? Dann Menü öffnen
  if(!iPlr)
  {
    EventInfo4K(0,Format("$ChoosingPlayer$", GetTaggedPlayerName(iPlr)), CHOS);
    return(OpenMenu());
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
  EventInfo4K(0,Format("$Goal$", GetName(0, idGoal)),idGoal);
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
  var i = 0, j = 0, pCrew, tmp, log = "$Rules$";
  for(var check in aRules)
    {
    if(check)
      {
      CreateObject(GetDefinition(i, Chooser_Cat), 10,10, -1);
      if(j != 0) {
        log = Format("%s, %s", log, GetName(0, GetDefinition(i, Chooser_Cat)));
      }
      else {
        log = Format("%s%s", log, GetName(0, GetDefinition(i, Chooser_Cat)));
      }
      j++;
      }
    i++;
    }
  // Dunkelheit erzeugen
  log = Format("%s, %s x%d", log, GetName(0, DARK), iDarkCount);
  EventInfo4K(0,Format("<c ffcc00>%s</c>",log),CHOS);
  // ein schneller GameCall für Einstellungen
  GameCallEx("ChooserFinished");

  // Spieler freilassen
  for(i = 0 ; i < GetPlayerCount() ; i++)
    {
    for(j = 0 ; pCrew = GetCrew(GetPlayerByIndex(i), j) ; j++)
      {
      tmp = Contained(pCrew);
      RemoveObject(tmp, 1);
      
      pCrew->~Recruitment(pCrew->GetOwner());
      }
    for(var rule in FindObjects(Find_Category(Chooser_Cat), Find_Exclude(this())))
      rule->~InitializePlayer(GetPlayerByIndex(i));
    }
  // Überschüssiges TIM1-Objekte entfernen (falls Spieler ziwschenzeitlich geflogen sind)
  for(tmp in FindObjects(Find_ID(TIM1)))
    if(!(tmp->Contents()))
      RemoveObject(tmp, 1);
  // Selber entfernen
  RemoveObject();
}
