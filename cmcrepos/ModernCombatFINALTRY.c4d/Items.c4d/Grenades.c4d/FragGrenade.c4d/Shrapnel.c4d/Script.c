/*-- Splitter --*/

#strict 2
#include SHTX

local hitcnt,size,trail_len;

public func IsSpecialAmmo()	{return false;}


public func Fast()
{
  return false;
}

public func Launch(int iAngle, int iSpeed, int iDist, int iSize, int iTrail, int iDmg)
{
  if(!iDmg)     iDamage = 3;
  else iDamage = iDmg;

  SetPosition(GetX(),GetY()+GetDefWidth()/2);

  if(!iSize)    iSize = 8;
  if(!iTrail)   iTrail = 300;

  speed = iSpeed;

  iSize = Min(iSize+2,GetDefWidth());

  DoCon(100*iSize/GetDefWidth()-100);

  lx = GetX();
  ly = GetY();

  iTime = 10*iDist/iSpeed;

  if(!iTime)
    return RemoveObject();

  var self = this;
  SetAction("Travel");
  if(!self) return ; //Kleiner Sicherheitscheck, ob die Kugel nicht sofort verschwindet

  SetXDir(+Sin(iAngle,iSpeed));
  SetYDir(-Cos(iAngle,iSpeed));
  SetR(+iAngle);

  // Trail erzeugen
  size = iSize;
  trail_len = iTrail;
  CreateTrail(iSize, iTrail);

  AddEffect("HitCheck", this, 1,1, 0, GetID(), shooter);
}

private func CreateTrail(int iSize, int iTrail)
{
  pTrail = CreateObject(TRAI,0,0,-1);
  if(pTrail)
  {
    pTrail->Set(iSize-2,iTrail,this);
    SetGraphics("Trail",pTrail,GetID());

    SetObjectBlitMode(GetObjectBlitMode(),pTrail);
  }
}

private func Traveling()
{
  var iATime = GetActTime();

  //Ausfaden
  SetClrModulation(Color(iATime));
  //Löschen
  if(iATime >= iTime) return Remove();

  //Außerhalb der Landschaft: löschen
  if(GetY()<0) return Remove();
}

private func Color(int iATime)
{
  var iPrg = 100*iATime/iTime;
  return RGBa(255-iPrg*2,255-iPrg*2,255-iPrg*2,iPrg*2);
}

public func TrailColor(int iATime)
{
  var iPrg = 100*iATime/iTime;
  return RGBa(255-iPrg*2,255-iPrg*2,255-iPrg*2,iPrg*2);
}

public func Hit(int iXDir, int iYDir)
{
  if(!iXDir && !iYDir)
  {
    iXDir = GetXDir(0, 100);
    iYDir = GetYDir(0, 100);
  }

  if(!Random(GetActTime()/2))
  {
    //Hazard-Script
    if(iXDir != 0)
    {
      if(iXDir > 0)
      {
        if(GBackSolid(1,0))
          iXDir *= -1;
      }
      else
        if(GBackSolid(-1,0))
          iXDir *= -1;
    }
    if(iYDir != 0)
    {
      if(iYDir > 0)
      {
        if(GBackSolid(0,1))
          iYDir *= -1;
      }
      else
        if(GBackSolid(0,-1))
          iYDir *= -1;
    }

    SetXDir(iXDir, 0, 100);
    SetYDir(iYDir, 0, 100);
    SetR(Angle(0, 0, iXDir, iYDir));

    SetPosition(GetX(),GetY(), pTrail);
    SetSpeed(0,0,pTrail);
    pTrail->Remove();

    CreateTrail(size,trail_len);

    return ;
  }
  return inherited(...);
}

private func HitObject(object pObject)
{
  if(shooter && pObject)
    if(pObject == shooter)
      return false;//>:O

  if(BulletStrike(pObject))
  {
    hitcnt++;

    var x,y;
    OnShrapnelHit(pObject,x,y);

    if((hitcnt > 1) || !pObject)
      Remove();
    return true;
  }
}

public func BulletStrike(object pObj)
{
  if(pObj)
  {
    if(pObj->~IsBullet()) return false;
    if(GetID(pObj) == TRAI) return false;
    if(GetEffect("IntShrapnelHit",pObj))
    {
      return false;
    }
    
    if(GetID(pObj) != RSLH)
      AddEffect("IntShrapnelHit",pObj,1,10,0,GetID());

    if(GetOCF(pObj) & OCF_Alive)
    {
      //Fling(pObj,GetXDir()/20 + GetXDir(pObj),GetYDir()/20 + GetYDir(pObj));
      ObjectSetAction(pObj, "Tumble");
    }

    DoDmg(iDamage,DMG_Projectile,pObj,0,0,wpnid);

    return true;
  }
  return true;
}

public func FxIntShrapnelHitTimer()	{return -1;}

public func OnShrapnelHit(object pObject, int iX, int iY)
{
  if(pObject)
  {
    if(Distance(lx,ly) > 20)
      Sound("SharpnelImpact*.ogg");
  }
}
