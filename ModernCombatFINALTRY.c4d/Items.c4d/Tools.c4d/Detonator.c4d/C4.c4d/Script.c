/*-- C4 Ladung --*/

#strict 2

local fuse, active, thrown, pStickTo, iStickXOffset, iStickYOffset, iStickROffset, iPreviousCategory, iBulletsTrigger;

public func LimitationCount()	{return 8;}
public func AttractTracer()	{return false;}
public func RejectC4Attach()	{return true;}
public func IsAttached()	{return pStickTo != false;}
public func IsC4Explosive()	{return true;}


/* Initialisierung */

public func Initialize()
{
  active = false;
  thrown = false;
  iBulletsTrigger = 0;
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
  iBulletsTrigger = 40;

  SetController(GetOwner(pCaller));
  
  if(!GetXDir() && !GetYDir())
    SetClrModulation(RGBa(255,255,255,100));

  //Effekte
  Sound("C4PA_Ignition.ogg");
  CreateParticle("PSpark",0,0,0,0,60,RGBa(255,122,0,0),this);
  AddEffect("Blink", this, 50, iBulletsTrigger, this);
}

/* Blinkeffekt */

public func FxBlinkTimer(object pTarget, int iEffectNumber)
{
  CreateParticle("PSpark",0,0,0,0,60,RGBa(255,0,0,0),this);
  return -1;
}

/* Pr�fungseffekt */

private func CheckFuse()
{
  if(!fuse)
    FadeOut();
}

protected func Timer()
{
  CheckFuse();
  if(pStickTo && !Contained(pStickTo) && pStickTo->~IsBulletTarget(GetID(), this))
  {
    SetPosition(GetX(pStickTo)+iStickXOffset, GetY(pStickTo)+iStickYOffset, this, false);
    SetXDir();
    SetYDir();
    SetR(GetR(pStickTo)+iStickROffset);
  }
  else
  {
    pStickTo = FindObject2(Find_AtPoint(), Find_Func("IsBulletTarget", GetID(), this), Find_Not(Find_Func("RejectC4Attach", this)), Find_NoContainer(), Find_Not(Find_OCF(OCF_Living)));
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
  if(iBulletsTrigger) iBulletsTrigger--;
}

/* Z�ndung */

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
  //Sound
  if(GBackLiquid())
    Sound("C4EX_WaterDetonation.ogg");
  else
    Sound("C4EX_Detonation*.ogg");
  Sound("Debris*.ogg");

  //Effekte
  var helper = CreateObject(TIM1,0,0,-1);
  AddEffect("IntShockWave",helper,10,1,0,GetID()); 
  CreateParticle("Blast",0,0,0,0,10*BlastRadius(),RGB(255,255,128));

  //Extraschaden f�r Strukturen
  for(var obj in FindObjects(Find_Distance(50), Find_Category(C4D_Structure | C4D_Vehicle), Find_Exclude()))
    DoDmg(BoundBy(InvertA1(ObjectDistance(obj), 100),0,60), DMG_Explosion, obj, 0, GetOwner()+1, GetID());

  //Explosion
  Explode(BlastRadius());
}

/* Au�eneinwirkung */

public func IsBulletTarget(id idBullet)
{
  if(idBullet == GetID()) return;
  if((idBullet == SHT1 || idBullet == SHTX) && !iBulletsTrigger) return;
  return true;
}

public func OnHit(int iDamage, int iType, object pFrom)
{
  if(((iType == DMG_Fire || iType == DMG_Explosion) && iDamage > 10))
  {
    SetController(GetController(pFrom));
    Trigger();
  }
  if(iType == DMG_Projectile && iBulletsTrigger > 0)
  {
    SetController(GetController(pFrom));
    BlowUp();
  }
}

func Incineration()
{
  BlowUp();
}

/* Schaden */

public func OnDmg(int iDmg, int iType)
{
  if(iType == DMG_Bio)		return 100;	//S�ure und biologische Schadstoffe
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

/* Aufschlag */

protected func Hit()
{
  Sound("AmmoBoxHit*.ogg", 0, 0, 50);
  return 1;
}