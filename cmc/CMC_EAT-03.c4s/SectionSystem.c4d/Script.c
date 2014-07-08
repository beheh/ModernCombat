/*-- Tutorial-System --*/

/* Dieses System muss in ein Szenarioscript inkludiert werden wie das Standart-Szen-Script
Eine neue Sektion wird gemacht indem man:
eine "Sect*.c4g" erstellt. Ersetze den Stern dabei druch die Nummer der Section.
Eine public Funkttion "SetupSection()" erstellen.
Diese Funktion wird ausgeführt, sobald eine neue Sektion geladen wurde. Clonks müssen
nicht extra initialisiert werden, das wird vom System übernommen. Eine neue Sektion
wird mit "LoadSection(int iSection, idClonk)" geladen. idClonk bestimmt hierbei die
ID der Clonks, die die Spieler erhalten sollen. Im Script sollte eine static const
mit dem Namen "SECT_Amount" deklariert werden, welche die Menge der Sections angibt.
Außerdem sollte zu jeder Section eine Funktion namens "SectionName*()" haben, die
den Namen der Sektion, die ID des anzuzeigenden Objekts zurückgibt und den zu verwendenden
Clonktyp, im Array.
Z.B: return [Format("$Section1$"),OPFL,PCMK];
*/

#strict 2

//Sollte ersetzt werden
static const SECT_Amount = 0;

static unlocked;
//Die Nummer der Stelle im Array stellt die jeweilige Section, der Inhalt dies dar.
//Am Ende einer Section sollte dann die jeweilige Stelle im Array so markiert werden:
//2 für geschafftes Level
//1 für noch nicht geschafftes, aber spielbares Level
//0 für noch nicht freigeschaltet

protected func Initialize()
{
  unlocked = []; //Sollte ersetzt werden
}


//Lädt die angegebene Sektion und erstellt für alle Spieler einen Clonk des übergebenen Typs
global func LoadSection(Section, id idClonk)
{
  if(this)
  {
    if(Contained())
      Contained()->SetPosition(0,0);
    else
      SetPosition(0,0);
  }
  
  if(!idClonk)
    idClonk = PCMK;
  
  CloseMenu();
  
  for(var alive in FindObjects(Find_OCF(OCF_Alive)))
    SilentKill4K(alive);
  RemoveAll();
  Log("Loading Section %s...",GetName(0, C4Id(Format("SE%02d", Section))));
  LoadScenarioSection(Format("%v",Section));
  
  for(var i = 0; i < GetPlayerCount(); i++)
  {
    var clonk = CreateObject(idClonk,LandscapeWidth()/2,LandscapeHeight()/2,GetPlayerByIndex(i));
    MakeCrewMember(clonk, GetPlayerByIndex(i));
    SelectCrew(i,clonk);
  }
  CreateObject(C4Id(Format("SE%02d", Section)), AbsX(0), AbsY(0), -1);
}

//Öffnet das Sektion-Auswahlmenü für den gegebenen Spieler
global func OpenSectionMenu(int iPlayer)
{
  var helper = CreateObject(TIM1,0,0,-1);
  
  CreateMenu(STSY, GetCursor(iPlayer), helper, 0, "$ChooseSection$", 0, C4MN_Style_Context, true);
  for(var i = 1; i <= SECT_Amount; i++)
  {
    var sect = eval(Format("SectionName%d()",i));
    if(unlocked[i] == 2)
      AddMenuItem(Format("<c 44ff44>%s</c>",sect[0]), Format("LoadSection(%d,%i)",i,sect[2]), sect[1], GetCursor(iPlayer));
    if(unlocked[i] == 1)
      AddMenuItem(Format("<c ffff44>%s</c>",sect[0]), Format("LoadSection(%d,%i)",i,sect[2]), sect[1], GetCursor(iPlayer));
    if(unlocked[i] == 0)
      AddMenuItem(Format("<c 888888>%s</c>",sect[0]), "Sound(\"Error\")", sect[1], GetCursor(iPlayer));
  }
}
