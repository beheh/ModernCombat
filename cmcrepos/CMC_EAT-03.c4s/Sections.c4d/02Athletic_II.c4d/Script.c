/*-- Athletik II --*/

#strict 2

/* Erstellung */

global func SetupSection2()
{
  var tmp;

  //R�ume
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
  tmp ->Arm(MISA);
  tmp ->TurnOn();
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

  //Gel�nder
  CreateObject(RAI1, 403, 1030, -1)->SetRail([1,3,1,3,1,3,1,3,1,3,1,3,1]);
  CreateObject(RAI1, 803, 870, -1)->SetRail([1,3,1,3,1,3,1]);
  CreateObject(RAI1, 1503, 670, -1)->SetRail([1,3,1,3,1,3,1]);
  CreateObject(RAI1, 1693, 310, -1)->SetRail([1,3,1,3,1,3,1,3,1,3,1]);

  //Regale
  CreateObject(FRAM, 1620, 510, -1);
  CreateObject(FRAM, 1640, 510, -1);

  //Stahlbr�cken
  CreateObject(_HBR, 1015, 702, -1);
  CreateObject(_HBR, 1015, 792, -1);
  CreateObject(_HBR, 1175, 702, -1);

  //Schilder
  CreateObject(_SGN, 625, 1050, -1)->SetNewInfo("$Info01$");
  CreateObject(_SGN, 820, 870, -1)->SetNewInfo("$Info02$");
  CreateObject(_SGN, 1095, 690, -1)->SetNewInfo("$Info03$");
  CreateObject(_SGN, 1790, 590, -1)->SetNewInfo("$Info04$");
  CreateObject(_SGN, 1790, 510, -1)->SetNewInfo("$Info05$");

  //Sounds

  //Hallen
  CreateObject(SE4K, 590, 960, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 1175, 740, -1)->Set("Interior*.ogg",670,105);
  CreateObject(SE4K, 1265, 450, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 1685, 550, -1)->Set("Interior*.ogg",670,105);
  CreateObject(SE4K, 1665, 330, -1)->Set("Interior*.ogg",665,105);

  ScriptGo(true);
  goto(10);
}

/* Ablauf */

func Script10()
{
  Sound("RadioConfirm*.ogg");
  TutorialMessage("$TxtS2_01$");

  SetArrow(850, 855);
}

func Script15()
{
  if(FindObject2(Find_ID(PCMK), Find_InRect(810,830,40,80)))
  {
    Sound("RadioConfirm*.ogg");
    TutorialMessage("$TxtS2_02$");

    RemoveArrow();
  }
  else
    goto(14);
}

func Script20()
{
  if(FindObject2(Find_ID(PCMK), Find_Action("Crawl")))
  {
    Sound("RadioConfirm*.ogg");
    TutorialMessage("$TxtS2_03$");

    SetArrow(1155, 855);
  }
  else
    goto(19);
}

func Script25()
{
  if(FindObject2(Find_ID(PCMK), Find_InRect(1140,790,80,80)))
  {
    Sound("RadioConfirm*.ogg");
    TutorialMessage("$TxtS2_04$");

    SetArrow(920, 770);
  }
  else
    goto(24);
}

func Script30()
{
  if(FindObject2(Find_ID(PCMK), Find_InRect(910,770,20,10)))
  {
    Sound("RadioConfirm*.ogg");
    TutorialMessage("$TxtS2_05$");

    RemoveArrow();
  }
  else
    goto(29);
}

func Script35()
{
  if(FindObject2(Find_ID(PCMK), Find_Action("Scale"), Find_InRect(850,760,30,30)))
  {
    Sound("RadioConfirm*.ogg");
    TutorialMessage("$TxtS2_06$");

    SetArrow(1250, 665);
  }
  else
    goto(34);
}

func Script40()
{
  if(FindObject2(Find_ID(PCMK), Find_InRect(1220,600,80,90)))
  {
    Sound("RadioConfirm*.ogg");
    TutorialMessage("$TxtS2_07$");

    SetArrow(1240, 480);
  }
  else
    goto(39);
}

func Script45()
{
  if(FindObject2(Find_ID(PCMK), Find_InRect(1500,640,100,30)))
  {
    Sound("RadioConfirm*.ogg");
    TutorialMessage("$TxtS2_08$");

    SetArrow(1630, 485);
  }
  else
    goto(44);
}

func Script50()
{
  if(FindObject2(Find_ID(PCMK), Find_InRect(1610,450,40,60)))
  {
    Sound("RadioConfirm*.ogg");
    TutorialMessage("$TxtS2_09$");

    SetArrow(1790, 485);
  }
  else
    goto(49);
}

func Script55()
{
  if(FindObject2(Find_ID(PCMK), Find_InRect(1760,470,40,60)))
  {
    Sound("RadioConfirm*.ogg");
    TutorialMessage("$TxtS2_10$");

    SetArrow(1770, 280);
  }
  else
    goto(54);
}


func Script60()
{
  if(FindObject2(Find_ID(PCMK), Find_InRect(1500,250,300,100), Find_Container(FindObject(ROOM))))
  {
    LoadSection(0);
    Sound("Cheer.ogg");
    unlocked[2] = 2;
    ScriptGo(0);
  }
  else
    goto(59);
}