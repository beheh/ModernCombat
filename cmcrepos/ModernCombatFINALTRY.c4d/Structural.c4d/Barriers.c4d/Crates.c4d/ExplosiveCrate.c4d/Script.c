/*-- Explosivkiste --*/

#strict 2
#include WCR2


/* Entzündung */

func Incineration(int iPlr)
{
  ClearScheduleCall(this, "InstaExplode");
  ScheduleCall(this, "InstaExplode", 100+Random(80),0,iPlr);
}

func IncinerationEx(int iPlr)
{
  ClearScheduleCall(this, "InstaExplode");
}

/* Zerstörung */

func Damage(int iChange, int iPlr)
{
  if(GetDamage() > 0)
      Incinerate();
      
  if(GetDamage() < 70) return ;
  
  InstaExplode(iPlr);
}

public func InstaExplode(int iPlr)
{
  //Effektgehasche
  CastParticles("WoodenCrateSplinter", 4, 50, 0,0, 141, RGBa(80,0,0,0), RGBa(30,0,0,0));
  CastParticles("WoodSplinter", 10, 200, 0,0, 50, 75, RGBa(80,0,0,0), RGBa(30,0,0,0));
  Sound("CrateCrack");
  
  //Umliegende Objekte anzünden
  for(var obj in FindObjects(Find_Distance(40+Random(20)),Find_Exclude(this),Find_Not(Find_Category(C4D_StaticBack))))
  {
    var inc = GetDefCoreVal("ContactIncinerate",0,GetID(obj));
    if(!inc) continue;
    
    if(inc <= 2) {
      obj->Incinerate();
      continue; 
    }
    else {
      if(!Random(inc-2)) {
        obj->Incinerate();
        continue;
      }
    }
  }
  
  AddLightFlash(50, 0,0, RGBa(255,255,200,100));
  
  //Explosion
  SetController(iPlr);
  RemoveObject(this, true);
  CreateObject(ROCK,0,0,iPlr)->Explode(45);
  CreateObject(ROCK,0,0,iPlr)->Explode(30);
}
