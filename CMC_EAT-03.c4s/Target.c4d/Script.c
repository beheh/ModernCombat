/* Zielscheibe */

#strict

local triggered,plr,target;

public func IsBulletTarget(id idBullet, object pBullet, object pShooter)
{
  if(Triggered())
    return(false);
  
  if(plr != NO_OWNER)
    if(plr != GetController(pBullet))
      return(false);
      
  if(target)
    if(target != pShooter)
      return(false);
      
  return(true);
}	

public func MinDmg(){return(1);}
public func Triggered(){return(triggered);}
public func ObjectHit(){}

public func Initialize()
{
  plr = GetController();
  SetOwner(NO_OWNER);//*lol* Wegen FriendlyFire.
  SetEntrance(1);
  SetColorDw(RGB(255));
  SetAction("Exist");
  SetPhase(Random(16));
  return(1);
}

protected func Activate(int iPlayer)
{
  if(Triggered()) return(false);
  
  /*if(target)
    if(GetController(target) != iPlayer)
      return(false);*/
      
  if(plr != NO_OWNER)
    if(plr != iPlayer)
      return(false);
      
  if(!this())
    return(false);
    
  triggered = true;
  Sound("Ding");
  DoWealth(iPlayer,GetValue (this(), 0, 0, iPlayer));
  AddEffect("IntRemoving", this(), 101, 2,0,GetID());
  return(true);
}

//Kann nur von _diesem_ Objekt (und seinen Schüssen) getroffen werden.
public func SetTarget(object pTarget)
{
  target = pTarget;
  plr = GetOwner(pTarget);
  SetColorDw(GetColorDw(pTarget));
}

public func SetPlr(int iPlr)
{
  plr = iPlr;
  SetColorDw(GetPlrColorDw(plr));
}

public func GetTarget(){return(target);}

public func GetPlr(){return(plr);}

public func OnHit(int iDamage, int iType, object pFrom)
{
  if(iDamage >= MinDmg())
    Activate(GetController(pFrom));
  else
    ObjectHit();
}

public func Damage(int iChange,int byPlr)
{
  if(GetDamage() >= MinDmg())
    Activate(byPlr);
  else
    ObjectHit();
    
  return(1);
}

protected func RejectCollect(id unused, object pObj)
{
  if(Activate(GetController(pObj)))
    pObj->~Hit(GetXDir(pObj),GetYDir(pObj));
    
  return(true);
}

protected func Check()
{
  var pObj;
  if(pObj = FindObject(0,1,0,0,0, OCF_CrewMember(),0,0,NoContainer()))
    return(Activate(GetController(pObj)));
}

func MoveLine(int iX, int iY, int iSpeed)
{
  return(AddEffect("IntMoveLine", this(), 1, 1, 0, GetID(),iX,iY,iSpeed));
}

public func FxIntMoveLineStart(object target, int effect, int temp,x, y, speed)
{
  EffectVar(0,target,effect) = GetX(target); 
  EffectVar(1,target,effect) = GetY(target); 
  EffectVar(2,target,effect) = GetX(target)+x; 
  EffectVar(3,target,effect) = GetY(target)+y; 
  EffectVar(4,target,effect) = Max(1,speed);
}

public func FxIntMoveLineTimer(object target, int effect, int time)
{
  var x1 = EffectVar(0,target,effect); 
  var y1 = EffectVar(1,target,effect); 
  var x2 = EffectVar(2,target,effect); 
  var y2 = EffectVar(3,target,effect);
  
  var speed = EffectVar(4,target,effect);
  
  var l = EffectVar(5,target,effect);
  
  var x = ((x2-x1)*l/100)+x1;
  var y = ((y2-y1)*l/100)+y1;
  
  target->SetPosition(x,y);

  if(!EffectVar(6,target,effect))
  {
    l+=speed;
    
    if(l > 100)
      EffectVar(6,target,effect) = true;
  }
  else
  {
    l-=speed;
    
    if(l < 0)
      EffectVar(6,target,effect) = false;
  }
  
  EffectVar(5,target,effect) = l;
	
  return(0);
}

func FxIntRemovingTimer(target, no)
{
  var alpha = EffectVar(0,target,no)+=20;
  SetClrModulation(RGBa(255,255,255, BoundBy(alpha,0,255)), target);
  //target->DoCon(+10);
  
  if(alpha >= 255)
  {
    RemoveObject(target);
    return(-1);
  }
}