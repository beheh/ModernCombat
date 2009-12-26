/*-- Explosivfass --*/

#strict

local damaged;

public func IsBulletTarget(){if(!damaged) return(1);}
public func IsCraneGrabable() { return(!damaged); }
public func IgnoreFriendlyFire() { return(1); }


/* Entzündung */

func Incineration(int iPlr)
{
  if(damaged) Extinguish();
  ClearScheduleCall(this(), "InstaExplode");
  ScheduleCall(this(), "InstaExplode", 80+Random(300),0,iPlr);
}

func IncinerationEx(int iPlr)
{
  ClearScheduleCall(this(), "InstaExplode");
}

/* Zerstörung */

func Damage(int iChange, int iPlr)
{
  if(GetDamage() > 1)
   Incinerate();
  if(GetDamage() < 20) return();
   InstaExplode(iPlr);
}

func InstaExplode(int iPlr)
{
  if(damaged) return();
  damaged = true;

  //Effekte
  Sound("BarrelImpact*.ogg");
  CastParticles("MetalSplinter",4,100,0,0,20,70,RGB(250,0,0));

  //Umliegende Objekte anzünden
  for(var obj in FindObjects(Find_Distance(30+Random(20)),Find_Exclude(this()),Find_Not(Find_Category(C4D_StaticBack))))
  {
   var inc = GetDefCoreVal("ContactIncinerate",0,GetID(obj));
   if(!inc) continue;
    if(inc == 3)
     obj->Incinerate();
   else
   if(!Random(inc-3))
    obj->Incinerate();
  }
  //Explosion
  BlowUp(iPlr);
}

func BlowUp(int iPlr)
{
  SetAction("Wreck");
  SetController(iPlr);
  CreateObject(ROCK,0,0,iPlr)->Explode(30);
  Extinguish();

  SetRDir(RandomX(-40,+40));
  AddFireEffect(this(),50,RGB(80,80,80),true,30);
  FadeOut();
}

/* Aufschlag */ 

protected func Hit3()
{
  DoDamage(20);
}

public func OnHit(int iDamage, int iType, object pFrom)
{
  Sound("BarrelDamaged*.ogg");
}
  
protected func Hit()
{
  Sound("BarrelImpact*.ogg");
  return(1);
}