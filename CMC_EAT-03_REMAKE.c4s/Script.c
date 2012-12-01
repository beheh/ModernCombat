/*-- Training Center --*/

#strict 2

static const TUTORIAL_Lobby = TLBY;


/* Initialisierung */

public func Initialize()
{
  wait(3);

  //Nachrichtensystem aktivieren
  SetTutorialMessagePos(MSG_Top | MSG_Left | MSG_WidthRel | MSG_XRel, 15, 50, 50);

  //Lade Sektion: Lobby
  Schedule(Format("LoadTutorial(%i)", TUTORIAL_Lobby), 15);

  return(1);
}

/* Auswahlmenü */

public func TutorialChooser(object pByObj)
{
  //Tutorial-Auswahlmenü öffnen

  CreateMenu(CSTR, pByObj, 0, C4MN_Extra_None, "Tutorialwahl", 0, C4MN_Style_Context);
  var i, def;
  while(def = GetDefinition(i++, C4D_Goal))
  {
    if(def->~IsCMCTutorialGoal() && !def->~HideInMenu())
    {
      if(!def->Condition(pByObj))
        AddMenuItem(Format("<c 777777>%s</c>", GetName(0, def)), 0, def, pByObj);
      else
        AddMenuItem(GetName(0, def), Format("LoadTutorial(%i, Object(%d))", def, ObjectNumber(pByObj)), def, pByObj, 0, 0, GetDesc(0, def));
    }
  }
  AddMenuItem("$Cancel$", Format("CloseMenu(Object(%d))", ObjectNumber(pByObj)), CHOS, pByObj);

  return true;
}

/* Sektion laden */

static g_TutorialObject;	//object - Das Tutorialobjekt, welches aktuell aktiviert ist

public func LoadTutorial(id idTutObj, object pByObj)
{
  while(GetEffect("FakeScriptCounter"))
    RemoveEffect("FakeScriptCounter");

  g_fakescriptcounter = 0;

  if(!(idTutObj->Condition(pByObj)))
    return false;

  if(!idTutObj->SectionName())
    return FatalError("No section name given!");

  SECT_ChangeSection(idTutObj->SectionName(), FindObjects(Find_ID(PCMK)));
  g_TutorialObject = CreateObject(idTutObj);

  for(var i = 0; i < GetPlayerCount(); i++)
    g_TutorialObject->InitializePlayer(i);

  for(var id in g_TutorialObject->UsedRules())
    CreateObject(id);

  g_TutorialObject->CreateInterior();
  g_TutorialObject->CreateEquipment();
  g_TutorialObject->CreateDecoration();
  g_TutorialObject->CreateOptionalFeatures();
  g_TutorialObject->CreateTriggers();

  return true;
}

//public func RelaunchPlayer(int iPlr, object pCrew, object pKiller, int iTeam, bool bFirst) { g_TutorialObject->RelaunchPlayer(iPlr, pCrew, pKiller, iTeam, bFirst); }
//protected func RemovePlayer(int iPlr) { g_TutorialObject->RemovePlayer(iPlr); }

/*public func RelaunchPlayer(int iPlr, object pCrew)
{
  //Clonkerstellung
  var pClonk = CreateObject(PCMK, 10, 10, iPlr);
  if(pCrew)
    GrabObjectInfo(pCrew, pClonk);
  else
    MakeCrewMember(pClonk, iPlr);

  DoEnergy(+150, pClonk);
  SetCursor(iPlr, pClonk);
  SetPlrView(iPlr, pClonk);
  FindObject2(Find_Func("IsSection"))->Relaunch(pClonk);
}

/* Sektionen *

global func SectionName0()	{return ["$SectionName0$",STSY,PCMK];}
global func SectionName1()	{return ["$SectionName1$",PCMK,PCMK];}
global func SectionName2()	{return ["$SectionName2$",PCMK,PCMK];}
global func SectionName3()	{return ["$SectionName3$",PCMK,PCMK];}
global func SectionName4()	{return ["$SectionName4$",PCMK,PCMK];}
*/