/*-- Granate --*/

#strict 2

public func IsDrawable()	{return true;}
public func HandX()		{return 3000;}
public func HandSize()		{return 1000;}

public func IsGrenade()		{return true;}
public func IsEquipment()	{return true;}
public func NoWeaponChoice()	{return GetID() == NADE;}
public func CanAim()		{return true;}
public func IsRecharging()	{return false;}
public func IsDangerous4AI()	{return IsFusing();}
public func IsBouncy()		{return true;}
public func IsReloading()	{return false;}
public func IsShooting()	{return false;}
public func NoArenaRemove()	{return IsFusing();}
public func LockInventory()	{return !IsFusing();}

public func DoSmoke()		{return true;}		//Granate erzeugt im Flug Rauch
public func Color()		{return 0;}		//Farbe
public func FuseTime()		{return 3*35;}		//Zeit bis zur Z�ndung
public func ThrowSpeed()	{return 60;}		//Wurfgeschwindigkeit
public func ThrowDelay()	{return 20;}		//Zeit, bis erneut geworfen werden kann
public func MaxDamage()		{return 10;}		//Ben�tigter Schaden, um die Granate zu zerst�ren/aktivieren
public func ContainedDamage()	{return 60;}		//Schaden bei Detonation innerhalb eines Objekts

local controller,activated;


/* Aufschlag */

func HitSound()
{
  Sound("GrenadeHit*.ogg");
}

func HitObject(object pObj)
{
  if(!pObj)
    return;

  if(pObj->~QueryCatchBlow(this))
    return;

  if(GetOCF(pObj) | OCF_Alive)
    return;

  if(pObj->~IsClonk())
    Sound("BodyFall*.ogg");

  ObjectSetAction(pObj, "Tumble");
  SetXDir(GetXDir(pObj)+GetXDir()/3,pObj);
  SetYDir(GetYDir(pObj)+GetYDir()/3,pObj);

  SetXDir(-GetXDir(0,100)/4,0,100);
  SetYDir(-GetYDir(0,100)/4,0,100);
}

func Hit(int iXDir, int iYDir)
{
  //Sound
  HitSound();

  //Entsprechende H�pfbewegung
  if(GBackSolid(0,+5)) return(SetYDir(-iYDir/26));
  if(GBackSolid(0,-5)) return(SetYDir(-iYDir/26));
  if(GBackSolid(-5,0)) return(SetXDir(-iXDir/16));
  if(GBackSolid(+5,0)) return(SetXDir(-iXDir/16));
}

/* Aimstart */

public func AimStart()
{
  if(!Contained()) return;
  Contained()->~DoAiming(-30);
}

/* Steuerung */

public func ControlThrow(object caller)
{
  SetUser(caller);
  var user = caller;
  var delay = GetThrowDelay(GetUser());

  if(user->~IsClonk())
  {
    if(!user->~IsAiming() && user->~IsCrawling())
    {
      if(user->~IsCrawling() && user->~ReadyToAim())
      {
        user->~StartAiming();
        return 1;
      }
    }
    else
    {
      if(!IsFusing() && !delay)
      {
        Fuse();
        return true;
      }
    }
  }

  if(!Contained(GetUser()))
  {
    GetUser()->~CheckArmed();
    if(GetUser()->~ReadyToFire() && !delay || GetUser()->~ReadyToAttack() && !delay)
    {
      Throw();
      return true;
   }
  }

  return delay || _inherited(...);
}

public func Throw()
{
  var user = GetUser();

  if(user->~IsClonk())
  {
    if(!user->~IsAiming())
    {
      if(user->~IsCrawling() && user->~ReadyToAim())
      {
        user->~StartAiming();
        return 1;
      }
      else
      {
        if((user->GetProcedure() == "WALK")||(user->GetProcedure() == "THROW"))
          user->SetAction("Throw");
      }
    }
  }

  if(!IsFusing()) return 1;

  var dir = GetDir(user)*2-1;
  var angle = user->AimAngle();
  var x,y;
  GetUser()->WeaponEnd(x,y);

  SetCategory(C4D_Vehicle);
  Exit();
  SetController(GetOwner(user));
  SetPosition(GetX(user)+(dir*3),GetY(user));

  SetR(angle);
  SetXDir(+Sin(angle,ThrowSpeed()));
  SetYDir(-Cos(angle,ThrowSpeed()));
  SetRDir(RandomX(-6,6));

  AddThrowDelay(user);

  AddEffect("HitCheck",this,1,1,0,SHT1,user);

  Sound("GrenadeThrow*.ogg");

  var nade = user->~GrabGrenade(GetID());
  user->~ResetShowWeapon();
  if(user->~IsAiming())
  {
    if(!nade) user->StopAiming();
    else user->SetAiming(angle);
  }
  return 1;
}

/* Granateng�rtel */

public func Activate(pCaller)
{
  pCaller->~StoreGrenade(this);
  HelpMessage(GetOwner(pCaller),"$Collected$",pCaller,GetID());
  return true;
}

public func AI_IdleInventory(pCaller)
{
  return Activate(pCaller);
}

public func RejectEntrance()
{
  return activated;
}

public func Collection(object pObj)
{
  if(GetCategory(pObj) & C4D_Living)
    Sound("GrenadeCharge.ogg");
}

public func Departure(object pObj)
{
  if(IsFusing())
    SetCategory(C4D_Vehicle);
  PlayerMessage(GetController(pObj)," ",pObj);
}

/* Effekt bei Aktivit�t */

public func FxIntFuseStart()
{
  activated = true;
  return 1;
}

public func FxIntFuseTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  var c = Contained();
  if(!c && DoSmoke())
  {
    var vel=Abs(GetXDir())+Abs(GetYDir());
    var alpha=Max(0,60-vel);
    var rgb = Color();
    if(!rgb) rgb = RGB(100,100,100);

     CreateParticle("Smoke2", -GetXDir()/6, -GetYDir()/6, RandomX(-10, 10), -5,
                          vel/3+RandomX(10, 20), SetRGBaValue(rgb,alpha)); 
  }
  else if(c)
  {
    if (c->~IsClonk() && !GetAlive(c) || c->~IsFakeDeath())
      Exit(0, 0, 8);
    else if(Contents(0, c) == this)
      PlayerMessage(GetController(c),"<c %x>�</c>",c,InterpolateRGBa2(RGB(0,255),RGB(255,255),RGB(255,0),0,FuseTime(),iEffectTime));    
  }

  if(iEffectTime < FuseTime()) return;

  //Nochmal falls Clonk gestorben ist und die Granate rausfliegt
  c = Contained();

  if(c)
  {
    if(GetID(c) == GRNS)
    {
      var user = c->GetUser();
      if(!user)
        return false;

      Exit();
      SetPosition(GetX(user),GetY(user));
    }
    else
      Exit();
  }
  pTarget->Fused2(c);
  return -1;
}

public func FxIntFuseStop(object pTarget)	{}

public func Fuse()
{
  Sound("GrenadeActivate.ogg");
  SetGraphics("Active");
  return AddEffect ("IntFuse",this,200,1,this);
}

public func IsFusing()
{
  return GetEffect("IntFuse",this);
}

/* Z�ndung */

public func Fused2(object pContainer)
{
  if(pContainer && ContainedDamage())
  {
    if(GetOCF(pContainer) & OCF_Living)
    {
      if(!Contained(pContainer)) Fling(pContainer, GetXDir(pContainer)/10, GetYDir(pContainer)/10-1);
      DoDmg(ContainedDamage(), DMG_Explosion, pContainer);
    }
  }
  RemoveEffect("HitCheck",this);
  Fused();
}

public func Fused()
{
  //Explosion
  BlastObjects(GetX(),GetY(),30); 
  Explode(30);
}

/* Waffensystem */

func GetCharge()
{
  var user = GetUser();
  if(!user) return 0;
  if(!user->~MaxGrenades()) return 0;

  return user->GrenadeCount(GetID())+1;
}

func CustomHUD()		{return true;}

func UpdateHUD(object pHUD)
{
  var user = GetUser();
  if(!user) return ;
  if(!user->~MaxGrenades()) return 0;

  pHUD->Charge(user->GrenadeCount(GetID()),(user->MaxGrenades() - user->GrenadeCount()) + user->GrenadeCount(GetID()));
  pHUD->Ammo(user->GrenadeCount(GetID()),(user->MaxGrenades() - user->GrenadeCount()) + user->GrenadeCount(GetID()), GetName(), true);
  pHUD->Recharge(GetThrowDelayTime(GetUser()), ThrowDelay());
}

public func ReadyToFire()
{
  return true;
}

public func GetUser()
{
  if(!controller)
    if(Contained())
      if(GetOCF(Contained()) & OCF_Alive)
        controller = Contained();

  return controller;
}

public func SetUser(object pUser)
{
  controller = pUser;
  SetController(GetController(pUser));
}

/* Effekt f�r Treffer�berpr�fung */

// EffectVars:
// 0 - alte X-Position
// 1 - alte Y-Position
// 2 - Sch�tze (Objekt, das die Waffe abgefeuert hat, �blicherweise ein Clonk)
// 3 - ID des Sch�tzen
// 4 - Scharf? Wenn true wird der Sch�tze vom Projektil getroffen 

public func FxHitCheckStart(object target, int effect, int temp, object byObj)
{
  if(temp) return ;
  EffectVar(0, target, effect) = GetX(target);
  EffectVar(1, target, effect) = GetY(target);
  if(!byObj)
    byObj = target;
  if(Contained(byObj))
    byObj = (Contained(byObj));
  EffectVar(2, target, effect) = byObj;
  EffectVar(3, target, effect) = GetID(byObj);
  EffectVar(4, target, effect) = false;
  EffectVar(5, target, effect) = byObj;
}

/* Wurfverz�gerung */

public func GetThrowDelay(object pObj)
{
  return GetEffect("IntGrenadeThrowDelay", pObj);
}

public func GetThrowDelayTime(object pObj)
{
  return GetEffect("IntGrenadeThrowDelay", pObj, 0, 6);
}

public func AddThrowDelay(object pObj)
{
  if(!GetThrowDelay(pObj))
    return AddEffect("IntGrenadeThrowDelay", pObj, 1, ThrowDelay(), 0, GetID());
}

/* Schaden */

protected func Damage(int iChange) 
{
  //Kein Schaden nehmen wenn gehalten und eventuelles Feuer l�schen
  if(Contained())
  {
    if(OnFire())
    {
      Extinguish();
      return;
    }
    else
    return;
  }

  //Ansonsten Z�ndung durch Schaden
  if(GetDamage() < MaxDamage() || activated) return;
    Fused();
}

public func OnDmg(int iDmg, int iType)
{
  if(iType == DMG_Bio)		return 100;	//S�ure und biologische Schadstoffe
}