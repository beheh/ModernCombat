/*-- C4 Ladung --*/

#strict 2

local fuse, active, thrown, pStickTo, iStickROffset, iStickAngle, iStickDistance, iPreviousCategory, iBulletsTrigger, rt_defusecnt, fEntered;

public func LimitationCount()	{return 8;}
public func IgnoreTracer()	{return true;}			//Nicht markierbar
public func RejectC4Attach()	{return true;}
public func IsAttached()	{return pStickTo != false;}
public func IsC4Explosive()	{return true;}
public func IsSpawnTrap()	{return true;}
public func IsDefusable()	{return true;}
public func IgnoreEnemyCheck()	{return true;}			//Ignoriert Feindcheck bei Fahrzeugen


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

/* Prüfungseffekt */

private func CheckFuse()
{
  if(!fuse)
    FadeOut();
}

protected func Timer()
{
  CheckFuse();

  //Prüfen ob Ziel weiter vorhanden
  if(pStickTo && pStickTo->~IsBulletTarget(GetID(), this))
  {
    var pTargetContainer = Contained(pStickTo);
    if(pTargetContainer)
    {
      if(!Contained())
      {
        //Ziel innerhalb eines Objekts: Hineinfolgen
        Enter(pTargetContainer);
      }
      else if(pTargetContainer != Contained())
      {
        //Verschachteltem Ziel bei Transfer in ein anderes Objekt folgen
        Enter(pTargetContainer);
      }
    }
    else
    {
      fEntered = false;
      if(Contained())
      {
        //Ziel vorhanden und im Freien, C4 wiederum innerhalb eines Objekts: Objekt verlassen
        Exit();
      }
      var dir = GetDir(pStickTo)*2-1;
      if(GetActMapVal("Directions", GetAction(pStickTo), GetID(pStickTo)) < 2)
        dir = 1;

      SetPosition(GetX(pStickTo)+Sin(GetR(pStickTo)+iStickAngle, iStickDistance)*dir, GetY(pStickTo)-Cos(dir*GetR(pStickTo)+iStickAngle, iStickDistance), this, false);
      SetXDir();
      SetYDir();
      SetR(GetR(pStickTo)+iStickROffset);
    }
  }
  else
  {
    //Contained: Objekt verlassen
    if(Contained())
    {
      Exit();
    }
    if(pStickTo)
    {
      Unstick();
    }
    pStickTo = FindObject2(Find_AtPoint(), Find_Func("IsBulletTarget", GetID(), this), Find_Not(Find_Func("RejectC4Attach", this)), Find_NoContainer(), Find_Not(Find_OCF(OCF_Living)));
    if(pStickTo)
    {
      StickTo(pStickTo);
    }
  }
  if(iBulletsTrigger) iBulletsTrigger--;
}

public func StickTo(object pObj)
{
  pStickTo = pObj;
  Sound("C4EX_Attach.ogg");
  iStickROffset = GetR()-GetR(pStickTo);
  iStickDistance = Distance(GetX(pStickTo), GetY(pStickTo), GetX(), GetY());

  iStickAngle = Angle(GetX(pStickTo), GetY(pStickTo), GetX(), GetY());

  if(!GetDir(pStickTo) && GetActMapVal("Directions", GetAction(pStickTo), GetID(pStickTo)) > 1)
    iStickAngle *= -1;

  iPreviousCategory = GetCategory();
  SetCategory(C4D_Vehicle);
  SetObjectOrder(pStickTo, this);
  SetRDir();

  return true;
}

public func Unstick()
{
  pStickTo = false;
  SetCategory(iPreviousCategory);

  return true;
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
  //Effekte
  var helper = CreateObject(TIM1,0,0,-1);
  AddEffect("IntShockWave",helper,10,1,0,GetID());
  CreateParticle("Blast",0,0,0,0,10*BlastRadius(),RGB(255,255,128));
  if(GBackLiquid())
    Sound("C4EX_WaterDetonation.ogg");
  else
    Sound("C4EX_Detonation*.ogg");

  //Extraschaden für Strukturen
  for(var obj in FindObjects(Find_Distance(50), Find_Category(C4D_Structure | C4D_Vehicle), Find_Exclude()))
    DoDmg(BoundBy(InvertA1(ObjectDistance(obj), 100),0,60), DMG_Explosion, obj, 0, GetOwner()+1, GetID());

  //Explosion
  Explode(BlastRadius());
}

/* Außeneinwirkung */

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

/* Entschärfung */

public func RTDefuse()
{
  rt_defusecnt++;
  if(rt_defusecnt > 8)
  {
    Sound("MISL_ShotDown.ogg");
    DecoExplode(10);

    return true;
  }

  return false;
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

/* Aufschlag */

protected func Hit()
{
  Sound("AmmoBoxHit*.ogg", 0, 0, 50);
}