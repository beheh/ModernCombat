/*-- Aimbot --*/

#strict

static const BONUS_Duration = 882;

public func IsBonus() { return(true); }

public func Color() { return(RGB(255,0,0)); }

public func Activate(object obj)
{
  if(!AddEffect("AimbotBonus",obj,190,1,obj))
    return(0);
  Sound("Applause", 1, 0,0, GetOwner(obj)+1);
  return(1);
}

global func FxAimbotBonusTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  if(!pTarget->~IsArmed()) return();

  var angle = pTarget->~GetCrosshair()->GetAngle();
  var r =  EffectVar(0,pTarget,iEffectNumber);
  
  var v = 90 < r && r < 270;
  var off = Sin(r,5);
  var d = CH_Distance+Sin(iEffectTime*3/2,20);
  
  CreateParticle("PSpark",Sin(angle,d)+Sin(angle+90,off),-Cos(angle,d)-Cos(angle+90,off),0,0,35,RGBa(255,0,0,40),pTarget,v);

  EffectVar(0,pTarget,iEffectNumber) += 25;
  if(EffectVar(0,pTarget,iEffectNumber) > 360)
    EffectVar(0,pTarget,iEffectNumber) = 0;
  
  if(iEffectTime > BONUS_Duration)
    return(-1);
}

global func FxAimbotBonusEffect(string szNewEffectName, object pTarget, int iEffectNumber, int iNewEffectNumber)
{
  if(szNewEffectName S= "AimbotBonus")
  {
    return(-1);
  }
}

global func FxAimbotBonusFMData(object pTarget, int iEffectNumber, int iData, value)
{
  if(iData == FM_SpreadAdd) return(Max(1,value/4));
  return(value);
}
