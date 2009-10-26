/*-- Explosive Kiste --*/

#strict
#include WCR2

func Incineration(int iPlr)
{
  ClearScheduleCall(this(), "InstaExplode");
  ScheduleCall(this(), "InstaExplode", 100+Random(80),0,iPlr);
}

func IncinerationEx(int iPlr)
{
  ClearScheduleCall(this(), "InstaExplode");
}

func Damage(int iChange, int iPlr)
{
  if(GetDamage() > 1)
      Incinerate();
      
  if(GetDamage() < 70) return();
  
  InstaExplode(iPlr);
}

func InstaExplode(int iPlr)
{
  //Effektgehasche
  Sound("CrateCrack");
  CastParticles("WoodSplinter", 10, 200, 0,0, 50, 75, RGBa(80,0,0,0), RGBa(30,0,0,0));
  
  //Umliegende Objekte anzünden
  for(var obj in FindObjects(Find_Distance(40+Random(20)),Find_Exclude(this()),Find_Not(Find_Category(C4D_StaticBack))))
  {
    var inc = GetDefCoreVal("ContactIncinerate",0,GetID(obj));
    if(!inc) continue;
    
    if(inc <= 2)
      obj->Incinerate();
    else
      if(!Random(inc-2))
        obj->Incinerate();
  }
  
  AddLightFlash(50, 0,0, RGBa(255,255,200,100));
  
  //Explosion
  SetController(iPlr);
  RemoveObject(0, 1);
  CreateObject(ROCK,0,0,iPlr)->Explode(45);
  CreateObject(ROCK,0,0,iPlr)->Explode(30);
}