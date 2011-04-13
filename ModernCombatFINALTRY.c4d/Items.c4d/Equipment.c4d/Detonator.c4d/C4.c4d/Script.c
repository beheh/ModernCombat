/*-- C4 Ladung --*/

#strict 2

local fuse, active, thrown, pStickTo, iStickXOffset, iStickYOffset, iStickROffset, iPreviousCategory;


public func LimitationCount()   {return 8;}

/* Initialisierung */

public func Initialize()
{
  active = false;
  thrown = false;
  iPreviousCategory = GetCategory();
  fuse = 0;
  CheckLimitation();
}

public func GetPacket(pObj)
{
  return pObj == fuse;
}

public func SetPacket(pObj)
{
  if (pObj)
    return fuse = pObj;
}

public func SetActive(object pCaller)
{
  if(!pCaller)
    return;

  fuse = pCaller;
  active = true;

  SetController(GetOwner(pCaller));
  
  if(!GetXDir() && !GetYDir())
    SetClrModulation(RGBa(255,255,255,100));
  //else
  //AddEffect("Check", this, 200, 1, this, C4EX);

  //Effekte
  Sound("C4PA_Ignition.ogg");
  CreateParticle("PSpark",0,0,0,0,60,RGBa(255,0,0,0),this);
}

/* Prüfungseffekt */

private func CheckFuse()
{
  if(!fuse)
    FadeOut();
}

protected func Timer()
{
  CheckFuse();
  if(pStickTo && !Contained(pStickTo) && pStickTo->~IsBulletTarget(GetID()))
  {
    SetPosition(GetX(pStickTo)+iStickXOffset, GetY(pStickTo)+iStickYOffset, this, false);
    SetXDir();
    SetYDir();
    SetR(GetR(pStickTo)+iStickROffset);
  }
  else
  {
    pStickTo = FindObject2(Find_AtPoint(), Find_Func("IsBulletTarget", GetID()), Find_Not(Find_Func("RejectC4Attach", this)), Find_NoContainer(), Find_Not(Find_OCF(OCF_Living)));
    if(pStickTo)
    {
      Sound("C4EX_Attach.ogg");
      iStickXOffset = GetX()-GetX(pStickTo);
      iStickYOffset = GetY()-GetY(pStickTo);
      iStickROffset = GetR()-GetR(pStickTo);
      iPreviousCategory = GetCategory();
      SetCategory(C4D_Vehicle);
      SetObjectOrder(pStickTo, this);
      SetRDir();
    }
    else
    {
      SetCategory(iPreviousCategory); 
    }
  }
}

/* Zündung */

public func Trigger()
{
  Sound("C4EX_Ignition.ogg");
  CreateParticle("PSpark",0,0,0,0,60,RGBa(255,0,0,0),this);
  ScheduleCall(0, "BlowUp", 10);
}

/* Explosion */

public func BlastRadius()	{return 50;}

public func BlowUp()
{
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",15,20,0,0,220,500, RGBa(255,255,255,0));
  
  if(GBackLiquid())
    Sound("C4EX_WaterDetonation.ogg");
  else
    Sound("C4EX_Detonation*.ogg");

  //Effekte
  var helper = CreateObject(TIM1,0,0,-1);
  AddEffect("IntShockWave",helper,10,1,0,GetID()); 
  CreateParticle("Blast",0,0,0,0,10*BlastRadius(),RGB(255,255,128));

  //Extraschaden für Strukturen
  for(var obj in FindObjects(Find_Distance(50), Find_Category(C4D_Structure | C4D_Vehicle), Find_Exclude()))
    DoDmg(BoundBy(InvertA1(ObjectDistance(obj), 100),0,60), DMG_Explosion, obj, 0, GetOwner()+1, GetID());

  Explode(BlastRadius());
}

/* Außeneinwirkung */

public func IsBulletTarget(ID)
{
  return ID != SHT1 && ID != SHTX && ID != C4EX;
}

public func OnHit(int iDamage, int iType, object pFrom)
{
  if((iType == DMG_Fire || iType == DMG_Explosion) && iDamage > 10)
  {
    SetController(GetController(pFrom));
    SetOwner(GetOwner(pFrom));
    Trigger();
  }
}

func Incineration()
{
  BlowUp();
}

/* Schaden */

public func OnDmg(int iDmg, int iType)
{
  if(iType == DMG_Bio)		return 100;	//Säure und biologische Schadstoffe
}

/* Schockwelle */

public func FxIntShockWaveStart(object pTarget, int iEffectNumber, int iTemp)
{}

public func FxIntShockWaveTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  pTarget->CreateParticle("ShockWave",0,0,0,0,iEffectTime*(10*(C4EX->BlastRadius()*3/2))/5,RGB(255,255,128));
  if(iEffectTime >= 5) return -1;
}

public func FxIntShockWaveStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  RemoveObject(pTarget);
}