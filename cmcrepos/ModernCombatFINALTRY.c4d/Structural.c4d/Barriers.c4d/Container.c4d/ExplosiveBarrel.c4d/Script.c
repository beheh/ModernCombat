/*-- Explosivfass --*/

#strict 2

local damaged;

public func IsBulletTarget()		{return !damaged;}
public func IsCraneGrabable()		{return !damaged;}
public func IgnoreFriendlyFire()	{return 1;}
public func IsMeleeTarget(object obj)
{
  if(damaged) return;
  if(obj) if(GetID(obj) == RSLH || GetID(obj) == BWTH) return 1;
  return;
}


/* Steuerung */

public func ControlRightDouble(object pByObj)
{
  if(damaged)
    return;
  if(!pByObj)
    return;

  //Clonk drehen und anhalten
  pByObj->SetDir(1);
  pByObj->SetAction("Throw");
  pByObj->SetComDir(COMD_Stop);

  //Tonne anschieben
  Fling(this, 2, -1);

  Sound("RSHL_Shove.ogg");
}

public func ControlLeftDouble(object pByObj)
{
  if(damaged)
    return;
  if(!pByObj)
    return;

  //Clonk drehen und anhalten
  pByObj->SetDir();
  pByObj->SetAction("Throw");
  pByObj->SetComDir(COMD_Stop);

  //Tonne anschieben
  Fling(this, -2, -1);

  Sound("RSHL_Shove.ogg");
}

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

/* Entzündung */

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

/* Zerstörung */

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
  ScheduleCall(this, "InstaExplode", 1,0,iPlr);
}

func InstaExplode(int iPlr)
{
  if(damaged) return;
  damaged = true;

  //Effekte
  Sound("BarrelImpact*.ogg");
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter",4,100,0,0,20,70,RGB(250,0,0));

  //Umliegende Objekte anzünden
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
  //Achievement-Fortschritt (Barrel Roll)
  DoAchievementProgress(1, AC47, iPlr);

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
  if(iType == DMG_Bio)		return 100;	//Säure und biologische Schadstoffe
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