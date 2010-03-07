/*-- gezogenes Schild --*/

#strict 2

local target, item, angle;
public func NoWarp() {return(true);}
public func ShoveTime() {return 13*3;}

public func Incineration()
{
  Extinguish();
}

public func Set(object pTarget, object pItem)
{
  target = pTarget;
  item = pItem;

  SetOwner(GetOwner(pTarget));
  SetController(GetController(pTarget));

  Show();
}

public func ExecShove()
{
  if(GetEffect("Shove",this)) return false;

  AddEffect("Shove",this,10,ShoveTime(),this);
  
  var px,py,dx,dy;
  if(target)
  {
    px = (GetX()-GetX(target))*5/4;
    py = (GetY()-GetY(target))*5/4;
    
    dx = GetX(target)-GetX();
    dy = GetY(target)-GetY();
  }
  
  var victim = FindObject2(Find_AtPoint(px,py),
                 Find_Exclude(this()),
                 Find_NoContainer(),
                 Find_Or
                 (
                   Find_Func("IsBulletTarget",GetID(),this,target),
                   Find_OCF(OCF_Alive)
                 ),
                 Find_Func("CheckEnemy",this),
                 Sort_Distance(dx,dy));

  if(victim)
  {
    Fling(victim, (target->GetDir()*2-1)*2, -1);
    Sound("RSHL_Shove.ogg");
  }
  else
  {
    Sound("GrenadeThrow*.ogg");
  }
}

public func FxShoveTimer() { return -1; }

public func Update()
{
  if(!target)
  {
    item->RemoveShield();
    return;
  }

  var x,y,r;
  if(!target->WeaponAt(x,y,r))
  {
    Hide();
    return;
  }
  else
  {
    Show();
  }

  angle = Normalize(target->AimAngle());
  var dir = target->GetDir()*2-1;
  
  if(GetEffect("Shove",this))
  {
    var t = GetEffect("Shove",this,0,6);
    
    if(t > ShoveTime()/3)
      t = 35+((t-35)*2/3);
    
    angle -= Max(Sin(t*90/(ShoveTime()/3), 40),0) * dir;
  }
  
  SetVertexXY(0,-Sin(angle,7),+Cos(angle,7));

  //Nur optisch drehen, damit die Shape sich nicht verändert. ;)
  var fsin = -Sin(angle,1000), fcos = +Cos(angle,1000);
  SetObjDrawTransform
  (
    dir*-fcos, +fsin, 0,
    dir*+fsin, +fcos, 0
  );
}

/*public func Damage(int iChange, int iByPlayer)
{
  if(item)
  {
    SetKiller(iByPlayer,item);
    DoDamage(iChange,item);
  }
  return(1);
}*/

public func IsBulletTarget(id idBullet, object pBullet, object pShooter)
{
  if(!pBullet && !pShooter)
    return true;
    
  if(pBullet)
    if(pBullet->~IsGrenade())
      return true;
 
  var r;
  if(pBullet)
    r = Angle(GetX(),GetY(),GetX(pBullet),GetY(pBullet));
  else
    r = Angle(GetX(),GetY(),GetX(pShooter),GetY(pShooter));
  
  r = AngleOffset4K(GetR(),r);
  
  if(Abs(r) <= 45)
    return false;
  else
    return true;
}

public func OnHit(int iDamage, int iType, object pFrom)
{
  Sound("BlockOff*.ogg");
  Sparks(5,RGB(255,255,255));
  AddLightFlash(40,0,0,RGB(255,255,255),this);
  
  if(pFrom && !Random(16 - Min(iDamage, 16)))
  {
    var a = Angle(GetX(pFrom),GetY(pFrom),GetX(),GetY());
    CreateParticle("MetalSplinter",0,0,RandomX(-10,+10)+Sin(a,30),RandomX(-10,+10)-Cos(a,30),30+Random(40));
  }
  
  if(item)
  {
    var by = NO_OWNER;
    if(pFrom) by = GetOwner(pFrom);

    if(item->UpdateDmg(iDamage,by))
    {
      if(pFrom)
      {
        var a = Angle(GetX(pFrom),GetY(pFrom),GetX(),GetY());
     
        for(var i = 7; i > 0; i--)
          CreateParticle("MetalSplinter",0,0,+Sin(a+RandomX(-20,+20),25+Random(10)),-Cos(a+RandomX(-20,+20),25+Random(10)),30+Random(40));
          
        if(target)
          Fling(target, +Sin(a,6), -Cos(a,6));
      }
      else
      {
        for(var i = 10; i > 0; i--)
          CreateParticle("MetalSplinter",0,0,RandomX(-30,+30),RandomX(-30,+30),30+Random(40));

        if(target)
          Fling(target,0,-1);
      }
    }
  }
}

//Script teilweise aus dem Ritterpack.
public func QueryCatchBlow(object pObj) 
{
  if(GetEffect("CatchBlow",pObj))
    return;

  /*if(Abs(AngleOffset4K(Angle(GetX(),GetY(),GetX(pObj),GetY(pObj)),
                       Angle(0,0,GetXDir(pObj),GetYDir(pObj))))  <=  80)
    return;*/

  var iPower = BoundBy(GetMass(pObj),0,50) * Distance(GetXDir(pObj),GetYDir(pObj)) * Distance(GetXDir(pObj),GetYDir(pObj));
  if(iPower/3 < 15000)
  {
    //if(GetXDir(pObj) < 0) if(GetX(pObj) < GetX() + 6) SetPosition(GetX() + 9, GetY(pObj) - GetYDir(pObj) / 3, pObj); 
    //if(GetXDir(pObj) > 0) if(GetX(pObj) > GetX() - 6) SetPosition(GetX() - 9, GetY(pObj) - GetYDir(pObj) / 3, pObj); 
    SetXDir(BoundBy(-GetXDir(pObj) / 3, -10, 10), pObj);
    SetYDir(BoundBy(-GetYDir(pObj) / 4, -10, 10), pObj); 

    if(GetMass(pObj) >= 10) Sound("ClonkHit*"); 
    if(GetMass(pObj) < 10)  Sound("ArmorImpact*",false,0,50);
    ProtectedCall(pObj,"Hit");
    if(pObj)
      AddEffect("CatchBlow",pObj,1,35,0,GetID());
  }
  return(true);
}

public func FxCatchBlowTimer() { return -1; }


/* Hidez */

private func Hide()
{
  if(ActIdle()) return;

  SetAction("Idle");
  SetPosition(10,10);
  SetR(0);
  SetRDir(0);
  SetXDir(0);
  SetYDir(0);
  SetCategory(C4D_StaticBack);
  SetVisibility(VIS_None);
}

private func Show()
{
  if(!ActIdle()) return;

  SetAction("Attach",target);
  SetPosition(GetX(target),GetY(target));
  SetCategory(GetCategory(0,GetID()));
  SetVisibility(VIS_All);
  Update();
}