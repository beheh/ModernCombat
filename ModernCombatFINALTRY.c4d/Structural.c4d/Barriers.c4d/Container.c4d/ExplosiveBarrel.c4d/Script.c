/*-- Explosivfass --*/

#strict 2

local damaged;

public func IsBulletTarget()		{return !damaged;}
public func IsCraneGrabable()		{return !damaged;}
public func IgnoreFriendlyFire()	{return 1;}


/* Aufrichtung */

public func FloatUpright()
{
  if(GBackLiquid())
  {
    if(GetR() >= 0)
    {
      if(GetR() < 90) {SetR(GetR()+1);}
      if(GetR() > 90) {SetR(GetR()-1);}
    }
    else
    {
      if(GetR() < -90) {SetR(GetR()+1);}
      if(GetR() > -90) {SetR(GetR()-1);}		
    }
  }
}

/* Entz�ndung */

func Incineration(int iPlr)
{
  if(damaged) Extinguish();
  ClearScheduleCall(this, "InstaExplode");
  ScheduleCall(this, "InstaExplode", 80+Random(300),0,iPlr);
}

func IncinerationEx(int iPlr)
{
  ClearScheduleCall(this, "InstaExplode");
}

/* Zerst�rung */

func Damage(int iChange, int iPlr)
{
  if(!this)
    return;
  if(damaged)
    return;
  SetController(iPlr);
  if(GetDamage() > 1)
    Incinerate(this);

  if(!this)
    return;
  if(GetDamage() < 20) return;
  InstaExplode(iPlr);
}

func InstaExplode(int iPlr)
{
  if(damaged) return;
  damaged = true;

  //Effekte
  Sound("BarrelImpact*.ogg");
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter",4,100,0,0,20,70,RGB(250,0,0));

  //Umliegende Objekte anz�nden
  for(var obj in FindObjects(Find_Distance(30+Random(20)),Find_Exclude(this),Find_Not(Find_Category(C4D_StaticBack))))
  {
    var inc = GetDefCoreVal("ContactIncinerate",0,GetID(obj));
    if(!inc) continue;
      if(inc == 3)
        Incinerate(obj);
    else
    if(!Random(inc-3))
      Incinerate(obj);
  }

  //Explosion
  BlowUp(iPlr);
}

func BlowUp(int iPlr)
{
  SetAction("Wreck");
  FakeExplode(30, iPlr+1);
  Extinguish();

  SetRDir(RandomX(-40,+40));
  AddFireEffect(this,50,RGB(80,80,80),true,30);
  FadeOut();
}

/* Schaden */

public func OnDmg(int iDmg, int iType)
{
  if(iType == DMG_Bio)		return 100;	//S�ure und biologische Schadstoffe
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
  return 1;
}