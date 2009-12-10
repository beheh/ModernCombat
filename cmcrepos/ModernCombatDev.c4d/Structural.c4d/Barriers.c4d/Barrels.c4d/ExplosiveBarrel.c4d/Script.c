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

  //Effektgehasche
  Sound("BarrelImpact*.ogg");
  CastParticles("Wave", 7+Random(5), RandomX(35,55), 0,0, 30,60, RGBa(1,1,1,50), RGBa(1,1,1,100));
  AddLightFlash(50, 0,0, RGBa(255,255,200,100));
  Sparks(7+Random(5), RGBa(255,255,150,100));
  //CreateParticle("Blast",0,0,0,0,40*10,RGB(255,64),this(),true); 

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