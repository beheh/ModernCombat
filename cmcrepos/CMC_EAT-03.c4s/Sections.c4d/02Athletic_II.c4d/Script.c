/* 3 - Athletik II */

#strict 2

global func SetupSection2()
{
  var tmp;

  //Räume
  tmp = CreateObject(ROOM, 475, 950, -1);
  tmp -> FadeOut();
  CreateObject(ROOM, 1760, 310, -1);
  
  for(var clonk in FindObjects(Find_ID(PCMK)))
  {
    Enter(tmp,clonk);
    SetCommand(clonk,"Exit");
  }

  //Leiter
  CreateObject(LADR, 940, 530, -1)->Set(10);

  //Sprungschanzen
  CreateObject (JMPD, 725, 1050, -1)->Set(90, 10);
  CreateObject (JMPD, 1230,505, -1)->Set(60, 65);

  //Geländer
  CreateObject(RAI1, 403, 1030, -1)->SetRail([1,3,1,3,1,3,1,3,1,3,1,3,1]);
  CreateObject(RAI1, 803, 870, -1)->SetRail([1,3,1,3,1,3,1]);
  CreateObject(RAI1, 1503, 670, -1)->SetRail([1,3,1,3,1,3,1]);
  CreateObject(RAI1, 1693, 310, -1)->SetRail([1,3,1,3,1,3,1,3,1,3,1]);

  //Stahlbrücken
  CreateObject(_HBR, 1015, 702, -1);
  CreateObject(_HBR, 1015, 792, -1);
  CreateObject(_HBR, 1175, 702, -1);

  ScriptGo(true);
  goto(10);
}

func Script10()
{
  if(FindObject2(Find_ID(PCMK), Find_InRect(1500,250,300,100), Find_Container(FindObject(ROOM))))
  {
    LoadSection(0);
    Sound("Cheer.ogg");
    TutorialMessage("$Ace_WellDone1$");
    unlocked[2] = 2;
    ScriptGo(0);
  }
  else
    goto(9);
}
