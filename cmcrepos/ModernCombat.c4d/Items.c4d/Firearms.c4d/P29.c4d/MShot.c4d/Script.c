#strict
#include SHTX

private func CreateTrail(int iSize, int iTrail)
{
  pTrail = CreateObject(TRAI,0,0,-1);
  if(pTrail)
  {
    pTrail->Set(iSize-2,iTrail,this());
    SetGraphics("Trail",pTrail,GetID());
    SetObjectBlitMode(GetObjectBlitMode(),pTrail);
  }
}

private func CreateTrail2(int iSize, int iTrail, int iR)
{
  pTrail = CreateObject(TRAI,lx,ly,-1);
  if(pTrail)
  {
    iTrail = Distance(lx,ly);
    pTrail->Set(iSize-2,iTrail,0,iR);
    SetGraphics("Trail",pTrail,GetID());
    SetObjectBlitMode(GetObjectBlitMode(),pTrail);
  }
}

public func RicochetAngle(){return(30);}

private func Color(int iATime) {
  var iPrg = 100*iATime/iTime;
  return(RGBa(255-iPrg*2,255-iPrg*2,255,iPrg*2));
}

public func TrailColor(int iATime) {
  var iPrg = 100*iATime/iTime;
  return(RGBa(255-iPrg*2,255-iPrg*2,255-iPrg*2,iPrg*2));
}

public func GlowColor(int iATime) {
  return(RGBa(0,128,255,50));
}

public func IsSpecialAmmo(){return(true);}

public func CustomBulletCasing(int iX, int iY, int iXDir, int iYDir, int iSize, int iColor)
{
  return(BulletCasing(iX,iY,iXDir,iYDir,iSize,RGB(200,200,200)));
}

public func CustomMuzzleFlash(int iSize, object pTarget, int iX, int iY, int iAngle, int iColor)
{
  return(MuzzleFlash(iSize*3/2,pTarget,iX,iY,iAngle,RGBa(0,127,255,85)));
}

public func FMMod(int iType,Data)
{
  if(iType == FM_BurstAmount) return(Max(2,Data*2));
  if(iType == FM_BurstRecharge) return(Data/2);
  return(Data);
}


///Für Moncocrom-Waffen:
/*public func FxIntMonochromStart(object pTarget, int iEffectNumber, int iTemp,size,fm)
{
  EffectVar(0,pTarget,iEffectNumber) = size;
  if(!fm) fm = 1;
  EffectVar(1,pTarget,iEffectNumber) = fm;
  return(1);
}

public func FxIntMonochromTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  var user = pTarget->~GetUser();
  if(!user) return(-1);
  
  var xoff, yoff, r;
  if(!(user->WeaponAt(xoff, yoff, r)))
  //if(user->Contents(0) != pTarget) return(0);
  if(pTarget->GetFireMode() != EffectVar(1,pTarget,iEffectNumber)) return(0);
  if(!pTarget->GetCharge()) return(0);
  if(pTarget->IsReloading()) return(0);
  if(pTarget->IsRecharging()) return(0);
  
  var angle = user->AimAngle();
  var x,y;
  user->WeaponEnd(x,y);
  var size = EffectVar(0,pTarget,iEffectNumber);
  
  pTarget->CreateParticle("MonoGlow",x,y,+Sin(angle,1000),-Cos(angle,1000),5*(size + Random(size/3)),RGBa(0,128,255,32),pTarget,false); 
  
  return(0);
}

public func FxIntMonochromStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  return(-1);
}

global func AddMonochromGlow(int iSize, int iFM, object pTarget)
{
  if(!pTarget) pTarget = this();
  if(!pTarget) return(0);
  if(!pTarget->IsWeapon()) return(0);
  
  var effect = GetEffect("IntMonochrom",pTarget);
  if(effect) return(effect);
  
  if(!iSize) iSize = 30;
  return(AddEffect ("IntMonochrom",pTarget,20,3,0,MSHT,iSize,iFM));
}

global func RemoveMonochromGlow(object pTarget)
{
  if(!pTarget) pTarget = this();
  if(!pTarget) return(0);
  RemoveEffect ("IntMonochrom",pTarget); 
}*/