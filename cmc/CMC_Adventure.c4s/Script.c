/*-- EAT-03 --*/

#strict 2

static const TUTORIAL_Lobby = TLBY;

/* Initialisierung */

public func Initialize()
{
  wait(3);

  //Nachrichtensystem aktivieren
  SetTutorialMessagePos(MSG_WidthRel | MSG_XRel, 15, 50, 50);
  
 	Schedule(Format("LoadTutorial(%i)", TUTORIAL_Lobby), 15);

  return(1);
}

static g_TutorialObject; // object - Das Tutorialobjekt was momentan aktiviert ist.

public func LoadTutorial(id idTutObj, object pByObj, array aAdditionalObjects)
{
	while(GetEffect("FakeScriptCounter"))
		RemoveEffect("FakeScriptCounter");
	
	ClearScoreboard(100, 3); // Werte nur provisorisch und so.
	
	var rnd = Random(FrameCounter() % 100) + 1;
	
	g_fakescriptcounter = 0;

	if(!(idTutObj->Condition(pByObj)))
		return false;
	
	if(!idTutObj->SectionName())
		return FatalError("No section name given!");
		
	var except = FindObjects(Find_Or(Find_ID(PCMK), Find_Category(C4D_Environment)));
	if(aAdditionalObjects)
		for(var obj in aAdditionalObjects)
			if(GetIndexOf(obj, except) == -1)
				except[GetLength(except)] = obj;
	
	for(var obj in except)
		if(GetCategory(obj) & C4D_Living)
			obj->DoEnergy(150);
	
	var sect_prefix = "TU";
	if(idTutObj->~IsCMCMinigame())
		sect_prefix = "MG";

	SECT_ChangeSection(Format("%s%s", sect_prefix, idTutObj->SectionName()), except);
	
	while(--rnd) // Damit wir verschiedene Random-Werte haben beim Laden einer neuen Sektion.
		Random(100);
	
	g_TutorialObject = CreateObject(idTutObj);
	
	for(var i = 0; i < GetPlayerCount(); i++)
		g_TutorialObject->InitializePlayer(i);
	
	var hostililty = g_TutorialObject->~FreeForAll();
	
	for(var i = 0; i < GetPlayerCount(); i++)
		for(var j = 0; j < GetPlayerCount(); j++)
			if(i != j)
				SetHostility(GetPlayerByIndex(i), GetPlayerByIndex(j), hostililty, true);
	
	for(var obj in FindObjects(Find_OCF(OCF_CrewMember)))
		SetCommand(obj, "");
	
	for(var id in g_TutorialObject->UsedRules())
		CreateObject(id);
		
	g_TutorialObject->CreateInterior();
	g_TutorialObject->CreateEquipment();
	g_TutorialObject->CreateDecoration();
	g_TutorialObject->CreateOptionalFeatures();
	g_TutorialObject->CreateTriggers();
	
	g_TutorialObject->~LoadingFinished();
	
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
