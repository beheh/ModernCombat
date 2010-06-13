/*-- Athletik - I --*/

#strict 2

static sect1told;

/* 1 - Athletik I */

/*global func SetupSection3()
{
  for(var clonk in FindObjects(Find_ID(PCMK)))
  {
    SetPosition(RandomX(30,120),-50,clonk);
    CreateObject(PARA,0,0,GetOwner(clonk))->Set(clonk);
  }
  
  TutorialMessage("$Mobility_I_1$");
  ScriptGo(1);
  goto(4);
  sect1told = false;
  

  //Zeug
  CreateObject(_JW1,88,117,-1)>SetR(40);
  CreateObject(BSH2,97,123,-1);
  CreateObject(TRGT,265,151,-1);
  CreateObject(AMCT,561,118,-1)->Set(PGRN);
  CreateObject(TRGT,340,207,-1);
  CreateObject(MSGN,205,250,-1);
  CreateObject(AMCT,128,308,-1)->Set(FGRN);
  CreateObject(LADR,101,414,-1)->Set(15);
  CreateObject(ETLT,139,278,-1);
var obj10 = CreateObject(C4PA,186,257,0);
obj10->Enter(Object(0));
var obj11 = CreateObject(C4PA,186,257,0);
obj11->Enter(Object(0));
var obj12 = CreateObject(C4PA,186,257,0);
obj12->Enter(Object(0));
var obj13 = CreateObject(C4PA,186,257,0);
obj13->Enter(Object(0));
CreateObject(AMCT,186,268,0);
var obj14 = CreateObject(HA4K,161,281,0)->SetMaxDmg(100);
var obj15 = CreateObject(RAI1,152,260,0);
obj15->SetRail([0, 1, 4],1);
var obj16 = CreateObject(RAI1,177,260,0);
obj16->SetRail([0, 1, 4],1);
CreateObject(TRGT,363,295,-1);
CreateObject(EXSN,566,290,0);
var obj17 = CreateObject(HA4K,101,321,0);
obj17->SetMaxDmg(100);
var obj18 = CreateObject(SEGU,218,332,0);
obj18->SetR(-90);
obj18->SetTeam(0);
obj18->TurnOn();
obj18->Arm(MISA);
obj18->LocalN("aim_angle")=180;
obj18->LocalN("iPat_Dir")=1;
var obj19 = CreateObject(STAM,218,322,0);
obj19->Enter(Object(0));
CreateObject(MSGN,519,320,0);
CreateObject(BSH2,447,377,0);
var obj20 = CreateObject(LADR,171,491,0);
obj20->Set(8);
var obj21 = CreateObject(HA4K,171,451,0);
obj21->SetMaxDmg(100);
CreateObject(SHRK,669,451,0);
CreateObject(SHRK,701,460,0);
CreateObject(SHRK,729,453,0);
var obj22 = CreateObject(FRAG,134,498,0);
obj22->Enter(Object(0));
var obj23 = CreateObject(FRAG,134,498,0);
obj23->Enter(Object(0));
var obj24 = CreateObject(FRAG,134,498,0);
obj24->Enter(Object(0));
var obj25 = CreateObject(FRAG,134,498,0);
obj25->Enter(Object(0));
CreateObject(AMCT,134,508,0);
var obj26 = CreateObject(SLDR,199,500,0);
obj26->SetMaxDmg(300);
var obj27 = CreateObject(SLDR,151,500,0);
obj27->SetMaxDmg(300);
var obj28 = CreateObject(BSH2,89,551,0);
obj28->SetR(40);
CreateObject(BSH2,400,544,0);
var obj29 = CreateObject(FGRN,468,528,0);
obj29->Enter(Object(0));
var obj30 = CreateObject(FGRN,468,528,0);
obj30->Enter(Object(0));
var obj31 = CreateObject(FGRN,468,528,0);
obj31->Enter(Object(0));
var obj32 = CreateObject(FGRN,468,528,0);
obj32->Enter(Object(0));
CreateObject(AMCT,468,538,0);
var obj33 = CreateObject(BSH2,523,539,0);
obj33->SetR(-40);
CreateObject(TRGT,36,596,-1);
CreateObject(BSH2,89,565,0);
CreateObject(TRGT,152,585,-1);
CreateObject(TRGT,281,600,-1);
CreateObject(TRGT,35,655,-1);
CreateObject(TRGT,162,654,-1);
CreateObject(TRGT,475,643,-1);
CreateObject(TRGT,326,711,-1);
CreateObject(TRGT,387,710,-1);
}

func Script5()
{
  if(FindObject2(Find_ID(PCMK),Find_InRect(640,240,160,100)))
  {
    LoadSection(0);
    Sound("Cheer.ogg");
    TutorialMessage("$Ace_WellDone2$");
    unlocked[1] = 2;
    if(unlocked[2] == 0)
      unlocked[2] = 1;
    ScriptGo(0);
  }
  if(!FindObject(BBTP) && !sect1told)
  {
    TutorialMessage("$Mobility_I_2$");
    sect1told = true;
    wait(3*6);
  }
  if(FindObject2(Find_ID(PCMK),Find_InRect(290,270,140,80)))
    TutorialMessage("$Mobility_I_3$");
  if(FindObject2(Find_ID(PCMK),Find_InRect(460,40,80,30)))
    TutorialMessage("$Mobility_I_4$");
    
  goto(4);
}
