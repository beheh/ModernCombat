/*-- EAT-03 --*/

#strict 2
#include STSY

static const SECT_Amount = 4;
static unlocked;


/* Initialisierung */

public func Initialize()
{
  //Sektion 1 freischalten
  unlocked = [0,1,1,1,1];
  wait(3);

  //Nachrichtensystem aktivieren
  SetTutorialMessagePos(MSG_Top | MSG_Left | MSG_WidthRel | MSG_XRel, 15, 50, 50);

  return(1);
}

public func InitializePlayer()
{
  //Lobby initialisieren
  LoadSection(0);
}

public func RelaunchPlayer(int iPlr, object pCrew)
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

/* Sektionen */

global func SectionName0()	{return ["$SectionName0$",STSY,PCMK];}
global func SectionName1()	{return ["$SectionName1$",PCMK,PCMK];}
global func SectionName2()	{return ["$SectionName2$",PCMK,PCMK];}
global func SectionName3()	{return ["$SectionName3$",PCMK,PCMK];}
global func SectionName4()	{return ["$SectionName4$",PCMK,PCMK];}
