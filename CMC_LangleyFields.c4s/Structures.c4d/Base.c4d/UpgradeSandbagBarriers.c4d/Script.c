/*-- Sandsackbarriere --*/

#strict 2

#include CCUS

public func ResearchCost()			{return 80;}	//Entwicklungskosten
public func ResearchDuration()			{return 35*8;}	//Entwicklungsdauer in Frames

public func ResearchBase()			{return [];}	//Upgrades, die bereits erforscht sein müssen
public func BarrierDamage()     {return 200;} //Abgefangener Schaden durch Sandsack-Upgrade

public func FxSandbagBarrierStart(object pTarget, int iNr, int iTemp)
{
  if(iTemp)
    return;

  EffectVar(0, pTarget, iNr) = BarrierDamage();
  
  return true;
}

public func FxSandbagBarrierDamage(object pTarget, int iNr, int iDmg, int iCause)
{
  //Sandsackupgrade nicht reparieren
  if(iDmg < 0)
    return iDmg;

  EffectVar(0, pTarget, iNr) -= iDmg;
  
  //Schaden zu hoch
  if(EffectVar(0, pTarget, iNr) <= 0)
    RemoveEffect("SandbagBarrier", pTarget);
  
  //Effekte
  
  return iDmg;
}

public func FxSandbagBarrierStop(object pTarget, int iNr, int iReason, bool fTemp)
{
  if(fTemp)
    return;

  pTarget->RemoveUpgrade(U_SB);

  //Effekte
}
