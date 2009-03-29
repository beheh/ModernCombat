/*Chooser-Append*/
#appendto CHOS
#strict

protected func ConfigurationFinished()
{
  Death = true;
  // Regeln erzeugen
  var i = 0, j, pCrew, tmp, log = "$Rules$";
  for(var check in aRules)
    {
    if(check)
      {
      CreateObject(GetDefinition(i, Chooser_Cat), 10,10, -1);
      log = Format("%s, %s", log, GetName(0, GetDefinition(i, Chooser_Cat)));
      }
    i++;
    }
  // Dunkelheit erzeugen
  log = Format("%s, %s x%d", log, GetName(0, DARK), iDarkCount);
  Log(log);
  // ein schneller GameCall für Einstellungen
  GameCallEx("ChooserFinished");

  // Spieler freilassen
  for(i = 0 ; i < GetPlayerCount() ; i++)
    {
    for(j = 0 ; pCrew = GetCrew(GetPlayerByIndex(i), j) ; j++)
      {
      pCrew->~Recruitment();
      }
    for(var rule in FindObjects(Find_Category(Chooser_Cat), Find_Exclude(this())))
      rule->~InitializePlayer(GetPlayerByIndex(i));
    }
  // Überschüssiges TIM1-Objekte entfernen (falls Spieler ziwschenzeitlich geflogen sind)
  var tmp = 0;
  for(tmp in FindObjects(Find_ID(TIM1)))
    if(!(tmp->Contents()))
      RemoveObject(tmp, 1);
  // Selber entfernen
  RemoveObject();
}