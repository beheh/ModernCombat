/*-- EAT-03 --*/

#strict 2
#include STSY

static const SECT_Amount = 4;
static unlocked;


/* Initalisierung */

func Initialize()
{
  //Sektion 1 freischalten
  unlocked = [0,1,0];
  wait(3);

  //Nachrichtensystem aktivieren
  SetTutorialMessagePos(MSG_Top | MSG_Left | MSG_WidthRel | MSG_XRel, 15, 50, 50);

  return(1);
}

func InitializePlayer() {
  //Lobby initialisieren
  LoadSection(0);
}

/* Sektionen */

global func SectionName0()	{return ["$SectionName0$",STSY,PCMK];}
global func SectionName1()	{return ["$SectionName1$",PCMK,PCMK];}
global func SectionName2()	{return ["$SectionName2$",PCMK,PCMK];}
global func SectionName3()	{return ["$SectionName3$",PCMK,PCMK];}
global func SectionName4()	{return ["$SectionName4$",PCMK,PCMK];}
