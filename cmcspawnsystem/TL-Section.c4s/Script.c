/*-- Neues Szenario --*/

#strict 2
#include STSY

static const SECT_Amount = 2;
static unlocked;

func Initialize() {

  //Stuff
  unlocked = [0,1,0]; //Hier wird die erste Section freigeschaltet, die zweite bleibt locked.
  wait(3);
  
  //Lobby initialisieren
  SetupSection0();
  
  return(1);
}

public func InitializePlayer(iPlr)
{
  for(var clonk in FindObjects(Find_ID(PCMK)))
    SetPosition(RandomX(120,180),280,clonk);
  //Nachrichtensystem aktivieren
  SetTutorialMessagePos(MSG_Top | MSG_Left | MSG_WidthRel | MSG_XRel, 15, 50, 50);
}

func Script1()
{
  TutorialMessage("$TxtWelcome$");
  Sound("EI4K_NewMessage.ogg");
  ScriptGo(0);
  wait(12*3);
}

func Script2()
{
  var clonk;
  if(clonk = FindObject2(Find_ID(PCMK)))
    if(Contained(clonk))
    {
      if(GetID(Contained(clonk)) == SPRM)
        TutorialMessage("$TxtChoose$");
      else
        CustomMessage("", 0, GetPlayerByIndex(0), g_msgoffx, g_msgoffy, 0xffffff, 0, 0, g_msgpos | MSG_DropSpeech, g_msgwdt);
    }
  goto(2);
}
  

/* Sektion 0 - Lobby */
global func SetupSection0()
{
  //Clonk platzieren
  for(var clonk in FindObjects(Find_OCF(OCF_CrewMember)))
    SetPosition(RandomX(120,180),280,clonk);
    
  //Zeug
  AutoRespawn(1500,0,CreateObject(PBRL,705,53,-1));
  AutoRespawn(1500,0,CreateObject(TRGT,728,51,-1));
  CreateObject(LBDR,96,90,-1);
  CreateObject(LADR,241,121,-1)->Set(4);
  CreateObject(HA4K,241,91,-1);
  CreateObject(LBDR,296,90,-1);
  CreateObject(SBBA,349,89,-1)->Right();
  CreateObject(RAI3,302,90,-1);
  CreateObject(RAI3,331,90,-1);
  AutoRespawn(1500,0,CreateObject(PBRL,731,61,-1));
  CreateObject(LLGH,66,119,-1);
  CreateObject(FAUD,180,172,-1);
  CreateObject(_HBR,285,142,-1);
  AutoRespawn(1500,0,CreateObject(TRGT,719,144,-1));
  CreateObject(LBDR,96,170,-1);
  AutoRespawn(1500,0,CreateObject(XTNK,193,169,-1));
  AutoRespawn(1500,0,CreateObject(PTNK,153,169,-1));
  CreateObject(LBDR,326,170,-1);
  CreateObject(RAI3,332,171,-1);
  CreateObject(RAI3,390,171,-1);
  CreateObject(RAI3,361,171,-1);
  CreateObject(SBBA,407,169,-1)->Right();
  AutoRespawn(1500,0,CreateObject(XBRL,626,185,-1));
  AutoRespawn(1500,0,CreateObject(XBRL,637,185,-1));
  AutoRespawn(1500,0,CreateObject(TRGT,642,185,-1));
  CreateObject(LLGH,65,208,-1);
  CreateObject(LBDR,96,290,-1);
  CreateObject(LLGH,65,287,-1);
  CreateObject(CLVM,135,290,-1);
  CreateObject(PLT2,188,292,-1);
  CreateObject(TABL,169,288,-1);
  CreateObject(SCA2,185,286,-1);
  CreateObject(TABL,207,288,-1);
  CreateObject(CCP1,232,290,-1);
  CreateObject(ETLT,260,264,-1);
  CreateObject(SPRM,260,291,-1);
  AutoRespawn(1500,0,CreateObject(WCR2,549,293,-1));
  AutoRespawn(1500,0,CreateObject(TRGT,724,267,-1));
  CreateObject(LBDR,96,350,-1);
  AutoRespawn(1500,0,CreateObject(WCR2,135,352,-1));
  AutoRespawn(1500,0,CreateObject(WCR2,152,352,-1));
  CreateObject(ROM2,173,349,-1)->Connect(CreateObject(ROM2,291,170,-1));
  CreateObject(ROM2,191,349,-1)->Connect(CreateObject(ROM2,271,90,-1));
  AutoRespawn(1500,0,CreateObject(PBRL,204,349,-1));
  AutoRespawn(1500,0,CreateObject(PBRL,213,349,-1));
  CreateObject(CCP1,240,350,-1);
  AutoRespawn(1500,0,CreateObject(PBRL,274,349,-1));
  AutoRespawn(1500,0,CreateObject(XTNK,270,349,-1));
  CreateObject(ETLT,335,329,-1);
  CreateObject(SLDR,361,350,-1);
  CreateObject(RAI3,368,352,-1);
  CreateObject(RAI3,397,352,-1);
  CreateObject(ATBY,493,330,-1);
  AutoRespawn(1500,0,CreateObject(TRGT,599,332,-1));
  AutoRespawn(1500,0,CreateObject(HBRL,631,351,-1));
  AutoRespawn(1500,0,CreateObject(HBRL,644,346,-1));
  AutoRespawn(1500,0,CreateObject(HBRL,658,344,-1));
  AutoRespawn(1500,0,CreateObject(TRGT,690,345,-1));
  
  //Ausrüstung
  PlaceSpawnpoint(PSTL,220,280);
  
  PlaceSpawnpoint(SMGN,120,160);
  PlaceSpawnpoint(SGST,150,160);
  PlaceSpawnpoint(PPGN,180,160);
  PlaceSpawnpoint(ASTR,210,160);
  
  PlaceSpawnpoint(MNGN,130,80);
  PlaceSpawnpoint(RTLR,180,80);
  PlaceSpawnpoint(MIAP,260,80);
  
  CreateObject(AMCT, 260, 130, -1)->Set(ABOX);
  CreateObject(AMCT, 300, 130, -1)->Set(GBOX);
}

//Infos über die Sektion
global func SectionName0() { return ["Lobby",STSY,PCMK]; }

/* Sektion 1 - Anfängerparcours */
//Initialisierung der Sektion
global func SetupSection1()
{
  for(var clonk in FindObjects(Find_ID(PCMK)))
  {
    SetPosition(RandomX(30,120),-50,clonk);
    CreateObject(PARA,0,0,GetOwner(clonk))->Set(clonk);
  }
  
  //Zeug
  CreateObject(BSH2,434,43,-1);
  CreateObject(LADR,543,206,-1)->Set(18);
  CreateObject(STNE,37,223,-1);
  CreateObject(STNE,14,231,-1);
  CreateObject(RAI3,247,230,-1);
  CreateObject(XTNK,278,229,-1);
  CreateObject(RAI3,277,230,-1);
  CreateObject(RAI3,306,230,-1);
  CreateObject(BBTP,426,228,-1)->Set(-70,-1);
  CreateObject(BSH2,429,243,-1);
  CreateObject(WCR2,479,229,-1);
  CreateObject(LFTP,476,241,-1);
  CreateObject(WCR2,533,214,-1);
  CreateObject(WCR2,549,232,-1);
  CreateObject(WCR2,533,232,-1);
  CreateObject(STNE,577,284,-1);
  CreateObject(STNE,730,313,-1);
  CreateObject(STNE,757,305,-1);
  CreateObject(STNE,83,322,-1);
  CreateObject(BSH2,321,321,-1);
  CreateObject(STNE,425,349,-1);
  CreateObject(STNE,508,332,-1);
  
  ScriptGo(false);
  goto(5);
}

//Infos über die Sektion
global func SectionName1() { return [" Mobilität I ",PCMK,PCMK]; }

/* Sektion 2 - Schildtraining */
//Initialisierung der Sektion
global func SetupSection2()
{
  for(var clonk in FindObjects(Find_ID(PCMK)))
  {
    SetPosition(RandomX(130,180),-50,clonk);
    CreateObject(PARA,0,0,GetOwner(clonk))->Set(clonk);
    CreateContents(RSHL,clonk);
  }
  
  var SelfDefense=CreateObject(SEGU,100,75,-1);
  SelfDefense->Arm(MISA);
  SelfDefense->TurnOn();
  SelfDefense=CreateObject(SEGU,58,120,-1);
  SelfDefense->Arm(MISA);
  SelfDefense->SetR(-90);
  SelfDefense->TurnOn();
  
  Message("<c 4444ff>Michael: Überlebe 10 Sekunden! Nutze das Schild um die Kugeln abzuwehren!</c>");
  Sound("Ding");
  
  ScriptGo(true);
  goto(10);
}

//Infos über die Sektion
global func SectionName2() { return [" Schild I ",RSHL,PCMK]; }

func Script45()
{
  if(GetAlive(FindObject2(Find_ID(PCMK),Find_Owner(0))))
  {
    LoadSection(0);
    Sound("Cheer.ogg");
    Message("<c 4444ff>Michael: Yeah, assness.</c>");
    for(var sentry in FindObjects(Find_ID(SEGU)))
      sentry->TurnOff();
    unlocked[2] = 2;
    ScriptGo(false);
  }
  else
  {
    Sound("Lol.ogg");
    Message("<c 4444ff>Michael: You fail'd, soldier.</c>");
  }
}


