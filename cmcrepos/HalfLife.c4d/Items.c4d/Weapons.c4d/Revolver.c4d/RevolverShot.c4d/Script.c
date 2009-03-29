#strict
#include SHTX

public func IsSpecialAmmo(){return(true);}

public func BulletStrike(object pObj)
{
  if(pObj)
  {
    DoDmg(iDamage,DMG_Projectile,pObj,0,0,0,wpnid);
    if(pObj)
      if(!ObjectSetAction(pObj,"Tumble"))
        SetYDir(-2,pObj);
  }
  return(true);
}