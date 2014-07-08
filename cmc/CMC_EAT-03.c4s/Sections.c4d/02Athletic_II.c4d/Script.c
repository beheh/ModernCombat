/*-- Athletik II --*/

#strict 2
#include SSY2

/* Erstellung */

public func SetupSection()
{
  var tmp;

  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Showtime.ogg");
  Music("CMC_Showtime.ogg",1);

  //Respawnpunkt
  SetRespawnPosition(485,1020);

  //Räume
  tmp = CreateObject(ROOM, 475, 950, -1);
  tmp ->FadeOut();
  CreateObject(ROOM, 1760, 310, -1);

  //Spieler plazieren
  for(var clonk in FindObjects(Find_ID(PCMK)))
  {
    Enter(tmp,clonk);
    SetCommand(clonk,"Exit");
  }

  //Selbstschussanlage und Konsole
  tmp = CreateObject (SEGU, 1760, 338, -1);
  tmp->Arm(MISA);
  tmp->TurnOn();
  CreateObject(CONS, 1715, 585, -1)->Set(tmp);

  //Sprungschanzen
  CreateObject(JMPD, 725, 1050, -1)->Set(90, 10);
  CreateObject(JMPD, 1230,505, -1)->Set(60, 65);

  //Orientierungslichter
  CreateObject(OLGH, 485, 1030, -1)->Set(3, 15, 1, 1, 40);
  CreateObject(OLGH, 625, 1050, -1)->Set(2, 15, 1, 1, 40);
  CreateObject(OLGH, 725, 1050, -1)->Set(2, 15, 1, 1, 40);

  CreateObject(OLGH, 930, 690, -1)->Set(3, 15, 1, 1, 40);
  CreateObject(OLGH, 1095, 690, -1)->Set(3, 15, 1, 1, 40);
  CreateObject(OLGH, 1260, 690, -1)->Set(3, 15, 1, 1, 40);

  CreateObject(OLGH, 1565, 310, -1)->Set(2, 15, 1, 1, 40);
  CreateObject(OLGH, 1755, 310, -1)->Set(3, 15, 1, 1, 40);

  //Glasscheibe
  CreateObject(_WIN, 1210, 880, -1);

  //Metallkiste
  CreateObject(MWCR, 920, 690, -1);
  CreateObject(MWCR, 940, 690, -1);

  //Geländer
  CreateObject(RAI1, 403, 1030, -1)->SetRail([1,3,1,3,1,3,1,3,1,3,1,3,1]);
  CreateObject(RAI1, 803, 870, -1)->SetRail([1,3,1,3,1,3,1]);
  CreateObject(RAI1, 1503, 670, -1)->SetRail([1,3,1,3,1,3,1]);
  CreateObject(RAI1, 1693, 310, -1)->SetRail([1,3,1,3,1,3,1,3,1,3,1]);

  //Regale
  CreateObject(FRAM, 1620, 510, -1);
  CreateObject(FRAM, 1640, 510, -1);

  //Stahlbrücken
  CreateObject(_HBR, 1015, 702, -1);
  CreateObject(_HBR, 1015, 792, -1);
  CreateObject(_HBR, 1175, 702, -1);

  //Terminals
  CreateObject(TMNL, 625, 1050, -1)->SetNewInfo("$Info01$");
  CreateObject(TMNL, 820, 870, -1)->SetNewInfo("$Info02$");
  CreateObject(TMNL, 1095, 690, -1)->SetNewInfo("$Info03$");
  CreateObject(TMNL, 1790, 590, -1)->SetNewInfo("$Info04$");
  CreateObject(TMNL, 1790, 510, -1)->SetNewInfo("$Info05$");

  //Sounds

  //Hallen
  CreateObject(SE4K, 590, 960, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 1175, 740, -1)->Set("Interior*.ogg",670,105);
  CreateObject(SE4K, 1265, 450, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 1685, 550, -1)->Set("Interior*.ogg",670,105);
  CreateObject(SE4K, 1665, 330, -1)->Set("Interior*.ogg",665,105);

  SectionGoto(10);
}

/* Ablauf */

func Script10()
{
  Sound("RadioConfirm*.ogg");
  TutorialMessage("$Txt01$");

  SetArrow(850, 855);
}

func Script15()
{
  if(FindObject2(Find_ID(PCMK), Find_InRect(810,830,40,80)))
  {
    Sound("RadioConfirm*.ogg");
    TutorialMessage("$Txt02$");

    SetRespawnPosition(835,860);

    RemoveArrow();
  }
  else
    SectionGoto(14);
}

func Script20()
{
  if(FindObject2(Find_ID(PCMK), Find_InRect(800,840,100,30), Find_Action("Crawl")))
  {
    Sound("RadioConfirm*.ogg");
    TutorialMessage("$Txt03$");

    SetArrow(1155, 855);
  }
  else
    SectionGoto(19);
}

func Script25()
{
  if(FindObject2(Find_ID(PCMK), Find_InRect(1140,790,80,80)))
  {
    Sound("RadioConfirm*.ogg");
    TutorialMessage("$Txt04$");

    SetRespawnPosition(1170,860);

    SetArrow(960, 770);
  }
  else
    SectionGoto(24);
}

func Script30()
{
  if(FindObject2(Find_ID(PCMK), Find_InRect(950,760,30,20)))
  {
    Sound("RadioConfirm*.ogg");
    TutorialMessage("$Txt05$");

    SetRespawnPosition(965,770);

    SetArrow(870, 770);
  }
  else
    SectionGoto(29);
}

func Script35()
{
  if(FindObject2(Find_ID(PCMK), Find_Action("Scale"), Find_InRect(830,740,50,50)))
  {
    Sound("RadioConfirm*.ogg");
    TutorialMessage("$Txt06$");

    SetArrow(1250, 665);
  }
  else
    SectionGoto(34);
}

func Script40()
{
  if(FindObject2(Find_ID(PCMK), Find_InRect(1220,600,80,90)))
  {
    Sound("RadioConfirm*.ogg");
    TutorialMessage("$Txt07$");

    SetRespawnPosition(1250,680);

    SetArrow(1240, 480);
  }
  else
    SectionGoto(39);
}

func Script45()
{
  if(FindObject2(Find_ID(PCMK), Find_InRect(1500,640,100,30)))
  {
    Sound("RadioConfirm*.ogg");
    TutorialMessage("$Txt08$");

    SetRespawnPosition(1550,660);

    SetArrow(1630, 485);
  }
  else
    SectionGoto(44);
}

func Script50()
{
  if(FindObject2(Find_ID(PCMK), Find_InRect(1610,450,40,60)))
  {
    Sound("RadioConfirm*.ogg");
    TutorialMessage("$Txt09$");

    SetRespawnPosition(1595,580);

    SetArrow(1790, 485);
  }
  else
    SectionGoto(49);
}

func Script55()
{
  if(FindObject2(Find_ID(PCMK), Find_InRect(1760,470,40,60)))
  {
    Sound("RadioConfirm*.ogg");
    TutorialMessage("$Txt10$");

    SetRespawnPosition(1815,500);

    SetArrow(1770, 280);
  }
  else
    SectionGoto(54);
}


func Script60()
{
  if(FindObject2(Find_ID(PCMK), Find_InRect(1500,250,300,100), Find_Container(FindObject(ROOM))))
  {
    Sound("Cheer.ogg");
    TutorialMessage("$WellDone1$");
    unlocked[2] = 2;
    if(unlocked[4] == 0)
      unlocked[4] = 1;
    LoadSection(0);
    SectionStop();
  }
  else
    SectionGoto(59);
}
