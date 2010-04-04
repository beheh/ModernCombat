/*-- Grenze --*/

#strict 2

local x,y,xh,yh;


/* Einstellung */

public func Set(iDir)
{
  if(iDir == 0)
  {
   x  = -GetX();
   y  = -GetY();
   xh = GetX();
   yh = LandscapeHeight();
  }
  if(iDir == 1)
  {
   x  = 0;
   y  = -GetY();
   xh = LandscapeWidth()-GetX();
   yh = LandscapeHeight();
  }
  if(iDir == 2)
  {
   x  = -GetX();
   y  = -GetY();
   xh = LandscapeWidth();
   yh = GetY();
  }
  if(iDir == 3)
  {
   x  = -GetX();
   y  = 0;
   xh = LandscapeWidth();
   yh = LandscapeHeight()-GetY();
  }
  for(var obj in FindObjects(Find_InRect(x,y,xh,yh),Find_Func("IsSpawnpoint")))
   RemoveObject(obj);
}

/* Grenzüberschreiter suchen */

private func Check()
{
  for(var clonk in FindObjects(Find_InRect(x,y,xh,yh),Find_OCF(OCF_CrewMember),Find_OCF(OCF_Alive)))
   if(!GetEffect("Border", clonk))
    AddEffect("Border", clonk, 50, 35, this);
  for(var flag in FindObjects(Find_InRect(x,y,xh,yh),Find_ID(FLA2),Find_NoContainer()))
   RemoveObject(FLA2);
}

private func DoShadows()
{
  for(var i = 0; i < Abs(xh)/150; i++)
   for(var j = 0; j < Abs(yh)/150; j++)
   {
    if(i % 2)
     CreateObject(BDSH, x+i*150+50, y+j*150+50+75, -1)->~Set(x,y,xh,yh);
    else
     CreateObject(BDSH, x+i*150+50, y+j*150+50, -1)->~Set(x,y,xh,yh);
   }
}

/* Effekt */

public func FxBorderStart(pTarget, iNo, iTemp)
{
  if(iTemp)
   return -1;

  //Countdown
  EffectVar(0, pTarget, iNo) = 10;
  Sound("Ding",0,pTarget,0,GetOwner(pTarget));
  //Hinweisnachricht
  PlayerMessage(GetOwner(pTarget),"@$Warning$",pTarget, EffectVar(0, pTarget, iNo));
}
    
public func FxBorderTimer(pTarget, iNo, iTime)
{
  var danger = 0;
  for(var obj in FindObjects(Find_InRect(x,y,xh,yh),Find_OCF(OCF_CrewMember),Find_OCF(OCF_Alive)))
   if(obj == pTarget)
    danger = 1;

  //Ziel wieder im sicheren?
  if(!danger)
  {
   PlayerMessage(GetOwner(pTarget),"@",pTarget);
   return -1;
  }

  if(EffectVar(0, pTarget, iNo) == 0)
  {
   //Opfer töten
   DoDmg(999, DMG_Projectile, pTarget);
   Sound("BRDR_Fire.ogg",pTarget);
   PlayerMessage(GetOwner(pTarget),"@",pTarget);
   return -1;
  }
  EffectVar(0, pTarget, iNo)--;
  PlayerMessage(GetOwner(pTarget),"@$Warning$",pTarget, EffectVar(0, pTarget, iNo));

  if(Random(2))
   Sound("BulletHit*.ogg",pTarget);
}