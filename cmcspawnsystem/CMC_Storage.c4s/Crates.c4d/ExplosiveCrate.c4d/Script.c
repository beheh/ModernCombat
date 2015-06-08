/*-- Explosivkiste --*/

#strict 2
#include WCR2

local blasted;

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
  SetController(iPlr);
  if(GetDamage() > 0)
    Incinerate();

  if(GetDamage() < 70) return;

  ScheduleCall(this, "InstaExplode", 1, 0, iPlr);
}

public func InstaExplode(int iPlr)
{
  if(blasted) return;
  blasted = true;

  SetController(iPlr);

  //Effektgehasche
  if(GetEffectData(EFSM_ExplosionEffects) > 1)
  {
    CastParticles("WoodenCrateSplinter", 4, 50, 0,0, 141, RGBa(80,0,0,0), RGBa(30,0,0,0));
    CastParticles("WoodSplinter", 10, 200, 0,0, 50, 75, RGBa(80,0,0,0), RGBa(30,0,0,0));
  }
  Sound("CrateCrack.ogg");

  //Umliegende Objekte anzünden
  for(var obj in FindObjects(Find_Distance(40+Random(20)),Find_Exclude(this),Find_Not(Find_Category(C4D_StaticBack))))
  {
    var inc = GetDefCoreVal("ContactIncinerate",0,GetID(obj));
    if(!inc) continue;

    if(inc <= 2)
    {
      Incinerate(obj);
      continue; 
    }
    else
    {
      if(!Random(inc-2))
      {
        Incinerate(obj);
        continue;
      }
    }
  }
  AddLightFlash(50, 0,0, RGBa(255,255,200,100));

  //Explosion
  FakeExplode(45, iPlr+1);
  FakeExplode(30, iPlr+1);
  RemoveObject(this);
  return;
}