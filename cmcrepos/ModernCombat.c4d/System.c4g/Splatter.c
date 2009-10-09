/*-- Splatter-Effekte --*/

#strict 2
#appendto ANIM
#appendto CLNK
#appendto HZCK

public func OnHit(int iDmg, int iType, object pFrom)
{
  Splatter(iDmg,iType,pFrom);
  return _inherited(iDmg,iType,pFrom);
}
