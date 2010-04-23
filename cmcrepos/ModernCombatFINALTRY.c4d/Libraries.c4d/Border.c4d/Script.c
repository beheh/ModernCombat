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
    {
     if(Contained())
     {
      if(GetID(Contained()) == FKDT || GetID(Contained()) == TIM2 || GetID(Contained()) == GOCC)
       continue;
      else
       AddEffect("Border", clonk, 50, 35, this);
     }
     else
      AddEffect("Border", clonk, 50, 35, this);
    }
  for(var flag in FindObjects(Find_InRect(x,y,xh,yh),Find_ID(FLA2),Find_Action("Lost")))
   RemoveObject(FLA2);
}

/* Effekt */

public func FxBorderStart(pTarget, iNo, iTemp)
{
  if(iTemp)
   return -1;

  //Countdown
  EffectVar(0, pTarget, iNo) = 10;
  Sound("TicketsLow.ogg",0,pTarget,0,GetOwner(pTarget));
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
   Kill(pTarget);
   Sound("BRDR_Fire.ogg",pTarget);
   PlayerMessage(GetOwner(pTarget),"@",pTarget);
   return -1;
  }
  EffectVar(0, pTarget, iNo)--;
  PlayerMessage(GetOwner(pTarget),"@$Warning$",pTarget, EffectVar(0, pTarget, iNo));

  if(Random(2))
   Sound("BulletHit*.ogg",pTarget);
}
