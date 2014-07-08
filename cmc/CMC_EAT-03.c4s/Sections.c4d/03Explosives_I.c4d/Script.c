/*-- Explosives - I --*/

#strict 2
#include SSY2

/* 4 - Explosives I */

public func SetupSection()
{
  for(var clonk in FindObjects(Find_ID(PCMK)))
  {
    SetPosition(RandomX(520,600),-50,clonk);
    CreateObject(PARA,0,0,GetOwner(clonk))->Set(clonk);
  }
  
  TutorialMessage("$Explosives_I_1$");
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
  CreateObject(AMCT,186,268,-1)->Set(C4PA);
  CreateObject(HA4K,161,281,-1)->SetMaxDmg(100);
  CreateObject(RAI1,152,260,-1);
  CreateObject(RAI1,177,260,-1);
  CreateObject(TRGT,363,295,-1);
  CreateObject(EXSN,566,290,-1);
  CreateObject(HA4K,101,321,-1);
  var obj18 = CreateObject(SEGU,218,332,-1);
  obj18->SetR(-90);
  obj18->SetTeam(0);
  obj18->TurnOn();
  obj18->Arm(MISA);
  obj18->LocalN("aim_angle")=180;
  CreateObject(STAM,218,322,-1);
  CreateObject(MSGN,519,320,-1);
  CreateObject(BSH2,447,377,-1);
  obj20 = CreateObject(LADR,171,491,-1)->Set(8);
  CreateObject(HA4K,171,451,-1);
  CreateObject(WIPF,669,451,-1);
  CreateObject(WIPF,701,460,-1);
  CreateObject(WIPF,729,453,-1);
  CreateObject(AMCT,134,508,-1)->Set(FRAG);
  CreateObject(SLDR,199,500,-1);
  CreateObject(SLDR,151,500,-1);
  CreateObject(BSH2,89,551,-1)->SetR(40);
  CreateObject(BSH2,400,544,-1);
  CreateObject(AMCT,468,538,-1)->Set(FGRN);
  CreateObject(BSH2,523,539,-1)->SetR(-40);
  CreateObject(TRGT,36,596,-1);
  CreateObject(BSH2,89,565,-1);
  CreateObject(TRGT,152,585,-1);
  CreateObject(TRGT,281,600,-1);
  CreateObject(TRGT,35,655,-1);
  CreateObject(TRGT,162,654,-1);
  CreateObject(TRGT,475,643,-1);
  CreateObject(TRGT,326,711,-1);
  CreateObject(TRGT,387,710,-1);
}

func Script70()
{
  if(FindObject2(Find_ID(PCMK),Find_InRect(290,270,140,80)))
    TutorialMessage("$Mobility_I_3$");
  if(FindObject2(Find_ID(PCMK),Find_InRect(460,40,80,30)))
    TutorialMessage("$Mobility_I_4$");
    
  SectionGoto(4);
}
