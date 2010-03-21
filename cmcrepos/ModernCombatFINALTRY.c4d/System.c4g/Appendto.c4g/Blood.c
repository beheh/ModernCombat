/*-- Blut --*/

//Das Appendto lässt bestimmte Objekte bei Schaden bluten.

#strict 2

#appendto ANIM //Tiere
#appendto CLNK //Clonks
#appendto HZCK //Hazardclonks


public func OnHit(int iDmg, int iType, object pFrom)
{
  Splatter(iDmg,iType,pFrom);
  return _inherited(iDmg,iType,pFrom);
}