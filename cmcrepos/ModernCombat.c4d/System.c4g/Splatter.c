/*-- Splatter-Effekte --*/

#strict 2
#appendto ANIM
#appendto CLNK
#appendto HZCK

static const SplatterScale = 100;

public func OnHit(int iDmg, int iType, object pFrom)
{
  if(SplatterOn())
    Splatter(iDmg,iType,pFrom);
  else
    Sparks(iDmg/3, RGBa(255,255,255));
    
  return _inherited(iDmg,iType,pFrom);
}
