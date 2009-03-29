/*-- Granatensteuerung --*/

#strict

///Allgemein
public func IsDrawable(){return(true);}
public func IsGrenade(){return(true);}
public func IsEquipment(){return(true);}
public func NoWeaponChoice() {return(GetID() == NADE);}
public func FuseTime(){return(3*35);}
public func ContainedDamage(){return(60);}
public func CanAim() { return(true); }//°.° Na klar!
public func IsRecharging() {return(false);}
public func IsDrobotMaterial(){return(true);}

public func Color(){return(0);}//Farbcode der Granate. ;)

public func IsDangerous4AI() { return(IsFusing()); }

public func ThrowSpeed() {return(60);}

public func HandX()    { return(0); }    // X-Position in der Hand
public func HandY()    { return(0); }    // Y-Position in der Hand
public func HandSize() { return(1000); } // Größe in der Hand, Standard: 1000
public func HandBarrel(){return(0); }    // Y-Offset des Laufs
public func BarrelXOffset(){return(0);}
public func BarrelYOffset(){return(0);}

local controller,activated;

func NoArenaRemove(){IsFusing();}

func HitSound()
{
  Sound("GrenadeHit*");
}

func HitObject(object pObj)
{
  if(!pObj) return();
  if(pObj->~IsClonk())
    Sound("BodyFall*");
    
  pObj->SetAction("Tumble");
  SetXDir(GetXDir(pObj)+GetXDir()/3,pObj);
  SetYDir(GetYDir(pObj)+GetYDir()/3,pObj);

  SetXDir(-GetXDir(0,100)/4,0,100);
  SetYDir(-GetYDir(0,100)/4,0,100);
}

func Hit(int iXDir, int iYDir)
{
  HitSound();
  
  if(GBackSolid(0,+5)) return(SetYDir(-iYDir/26));
  if(GBackSolid(0,-5))  return(SetYDir(-iYDir/26));
  if(GBackSolid(-5,0))  return(SetXDir(-iXDir/16));
  if(GBackSolid(+5,0))   return(SetXDir(-iXDir/16));
}

///Waffe
public func ControlThrow(object caller)
{
  SetUser(caller);
  
  if(!IsFusing())
  {
    Fuse();
    return(true);
  }
  
  if(!Contained(GetUser()))
  {
    GetUser()->~CheckArmed();//Noch einmal schnell prüfen.
    if(GetUser()->~ReadyToFire())
    {
      Throw();
      return(true);
    }
  }
  
  return(_inherited(...));
}

public func Throw()
{
  if(!IsFusing()) return();
  
  var user = GetUser();
  
  var dir = GetDir(user)*2-1;
  var angle = user->AimAngle();
  var x,y;
  GetUser()->WeaponEnd(x,y);

  SetCategory(C4D_Vehicle);
  Exit();
  SetController(GetOwner(user));
  SetPosition(GetX(user),GetY(user));
  
  SetR(angle);
  SetXDir(+Sin(angle,ThrowSpeed()));
  SetYDir(-Cos(angle,ThrowSpeed()));
  SetRDir(RandomX(-6,6));
  
  AddEffect("HitCheck",this(),1,1,0,GetID(),user);
  
  if(user->~IsClonk())
    if(!user->~IsAiming())
      if((user->GetProcedure() eq "WALK")||(user->GetProcedure() eq "THROW"))
        user->SetAction("Throw");
  
  Sound("GrenadeThrow");
  
  var nade = user->GrabGrenade(GetID());
  user->~ResetShowWeapon(0);//Seht ihrs? ... DA! ... Eine NULL!
  if(user->~IsAiming())
  {
    if(!nade) user->StopAiming();
    else user->SetAiming(angle);
  }
}

public func Activate(pCaller)
{
  pCaller->~StoreGrenade(this());
  HelpMessage(GetOwner(pCaller),"$Collected$",pCaller,GetID());
}

public func RejectEntrance()
{
  return(activated);
}

public func Collection(object pObj)
{
  if(GetCategory(pObj) & C4D_Living)
    Sound("GrenadeCharge");
}

public func Departure(object pObj)
{
  PlayerMessage(GetController(pObj)," ",pObj);
}

///Fuse-Effekt
public func FxIntFuseStart()
{
  activated = true;
  return(1);
}

public func FxIntFuseTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  if(!Contained())
  {
    var vel=Abs(GetXDir())+Abs(GetYDir());
    var alpha=Max(0,60-vel);
    var rgb = Color();
    if(!rgb) rgb = RGB(100,100,100);
    
    CreateParticle("Smoke2", -GetXDir()/6, -GetYDir()/6, RandomX(-10, 10), -5,
                         vel/3+RandomX(10, 20), SetRGBaValue(rgb,alpha)); 
  }
  else
  {
    if(Contained()->Contents() == this())
      PlayerMessage(GetController(Contained()),"<c %x>•</c>",Contained(),InterpolateRGBa2(RGB(0,255),RGB(255,255),RGB(255,0),0,FuseTime(),iEffectTime));    
  }

  if(iEffectTime < FuseTime()) return();
  
  var c = Contained();
  if(Contained())
  {
    if(GetID(Contained()) == GRNS)
    {
      var obj = Contained();
      var user = obj->GetUser();
      if(!user)
        return(false);
        
      Exit();
      SetPosition(GetX(user),GetY(user));
    }
    else
      Exit();
  }
  pTarget->Fused2(c);
  return(-1);
}

public func FxIntFuseStop(object pTarget)
{
  return(0);
}

public func Fuse()
{
  Sound("GrenadeActivate");
  return(AddEffect ("IntFuse",this(),200,1,this()));
}

public func IsFusing()
{
  return(GetEffect("IntFuse",this()));
}

public func Fused2(object pContainer)
{
  if(pContainer && ContainedDamage())
  {
    if(GetOCF(pContainer) & OCF_Living)
    {
      pContainer->Fling(0,-1);
      pContainer->DoDmg(ContainedDamage(),DMG_Fire);//Autsch! >_<
    }
  }
  RemoveEffect("HitCheck",this());
  Fused();
}

public func Fused()
{
  //Kaboom!
  BlastObjects(GetX(),GetY(),30); 
  Explode(30);
}

///Waffensystem
func GetCharge()
{
  var user = GetUser();
  if(!user) return(0);
  
  return(user->GrenadeCount(GetID())+1);
}

func UpdateCharge(object pHUD)
{
  var user = GetUser();
  if(!user) return();
  
  pHUD->Charge(user->GrenadeCount(GetID())*100/ ((user->MaxGrenades() - user->GrenadeCount()) + user->GrenadeCount(GetID())));
  pHUD->Ammo(user->GrenadeCount(GetID()),(user->MaxGrenades() - user->GrenadeCount()) + user->GrenadeCount(GetID()), GetName(), true);
  pHUD->MasterAmmo(GetID(),user->GrenadeCount(GetID()));
}

public func ReadyToFire()
{
  return(true);//Mal sehen...
}

public func GetUser()
{
  if(!controller)
    if(Contained())
      if(GetOCF(Contained()) & OCF_Alive)//*grummel* nicht sooo toll
        controller = Contained();
  
  return(controller);
}

public func SetUser(object pUser)
{
  controller = pUser;
  SetController(GetController(pUser));
}

func IsBouncy() { return(true); }//Hach, sind wir heute sprunghaft...
func IsReloading(){return(false);}
func IsShooting(){return(false);}//Nein, keine Automatikgranaten. :D



/* Effekt für Trefferüberprüfung (Modifiert und kopiert aus Projektilscript.) */

// EffectVars:
// 0 - alte X-Position
// 1 - alte Y-Position
// 2 - Schütze (Objekt, das die Waffe abgefeuert hat, üblicherweise ein Clonk)
// 3 - ID des Schützen
// 4 - Scharf? Wenn true wird der Schütze vom Projektil getroffen 

public func FxHitCheckStart(object target, int effect, int temp, object byObj)
{
  if(temp) return();
  EffectVar(0, target, effect) = GetX(target);
  EffectVar(1, target, effect) = GetY(target);
  if(!byObj)
    byObj = target;
  if(byObj->Contained())
    byObj = (byObj->Contained());
  EffectVar(2, target, effect) = byObj;
  EffectVar(3, target, effect) = GetID(byObj);
  EffectVar(4, target, effect) = false;
  EffectVar(5, target, effect) = byObj;
}

public func FxHitCheckTimer(object target, int effect, int time)
{
  var obj;

  var oldx = EffectVar(0, target, effect);
  var oldy = EffectVar(1, target, effect);
  var newx = GetX(target);
  var newy = GetY(target);
  var dist = Distance(oldx, oldy, newx, newy);
  EffectVar(0, target, effect) = GetX(target);
  EffectVar(1, target, effect) = GetY(target);
  
  var exclude = EffectVar(5, target, effect);
  if(EffectVar(4, target, effect)) exclude = target;
  
  if(Distance(GetXDir(target),GetYDir(target)) > 25)
  {
    for(obj in FindObjects(Find_OnLine(oldx,oldy,newx,newy),Sort_Distance(oldx, oldy),
      		                 Find_Exclude(target),
      		                 Find_Exclude(exclude),
                           Find_NoContainer(),
                           Find_OCF(OCF_Alive),
                           Find_Func("CheckEnemy",target),
                           Find_Not(Find_Allied(GetOwner(target)))))
    {
      EffectVar(4, target, effect) = false;
      EffectVar(2, target, effect) = obj;
      EffectVar(3, target, effect) = GetID(obj);
    
      target->~HitObject(obj);
    }
  }

  EffectVar(0, target, effect) = GetX(target);
  EffectVar(1, target, effect) = GetY(target);
  
  if(!EffectVar(4, target, effect))
  {
    var ready = true;
    for(var foo in FindObjects(Find_AtPoint(GetX(target),GetY(target)),Find_ID(EffectVar(3, target, effect))))
      if(foo == EffectVar(2, target, effect))
        ready = false;
        
    if(ready)
      EffectVar(4, target, effect) = true;
  }
}