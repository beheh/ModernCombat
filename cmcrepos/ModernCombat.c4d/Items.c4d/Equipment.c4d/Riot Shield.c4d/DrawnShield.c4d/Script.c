/*-- Gezogenes Schild --*/

#strict

local target;
public func NoWarp() {return(true);}

public func Set(object pTarget)
{
  target = pTarget;
  SetVertex(0,1,4,0,2);
  SetAction("Attach",pTarget);
  SetOwner(GetOwner(pTarget));
  SetController(GetController(pTarget));
}

public func UpdateR()
{
  if(!target) RemoveObject();

  var iAngle = Normalize(target->AimAngle());
  SetR(iAngle);
  SetDir(target->GetDir());
}

public func Damage() {return(1);}//TODO: Wirklich unzerstörbar?
public func IsBulletTarget() {return(true);}
public func OnHit(int iDamage, int iType, object pFrom)
{
  if(pFrom) if(pFrom->~IsBullet())
    Sound("ArmorImpact*");
}

//Script teilweise aus dem Ritterpack.
public func QueryCatchBlow(pObj) 
{
  var iPower = BoundBy(GetMass(pObj),0,50) * Distance(GetXDir(pObj),GetYDir(pObj)) * Distance(GetXDir(pObj),GetYDir(pObj));
  if(iPower/3 < 2500+Random(500))
  {
    if(GetXDir(pObj) < 0) if(GetX(pObj) < GetX() + 6) SetPosition(GetX() + 9, GetY(pObj) - GetYDir(pObj) / 3, pObj); 
    if(GetXDir(pObj) > 0) if(GetX(pObj) > GetX() - 6) SetPosition(GetX() - 9, GetY(pObj) - GetYDir(pObj) / 3, pObj); 
    SetXDir(BoundBy(-GetXDir(pObj) / 3, -10, 10), pObj);
    SetYDir(BoundBy(-GetYDir(pObj) / 4, -10, 10), pObj); 

    if(GetMass(pObj) >= 10) Sound("ClonkHit*"); 
    if(GetMass(pObj) < 10)  Sound("ArmorImpact*",false,0,50);
    ProtectedCall(pObj,"Hit");       
  }
  return(true);
}