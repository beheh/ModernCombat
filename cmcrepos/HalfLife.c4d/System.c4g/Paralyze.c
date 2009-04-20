/* Lähm-Effekt */
#strict

global func Paralyze(int iFrames, object pTarget)
{
  if(!pTarget)
  {
    pTarget = this();
    if(!pTarget)
      return(0);
  }

  return(AddEffect("Paralysis", pTarget, 125, 5, 0, 0, iFrames));
}


global func FxParalysisStart(object pTarget, int iEffectNumber, int iTemp, iFrames)
{
  if(iTemp)
    return();

  //Lähmung
  EffectVar(0, pTarget, iEffectNumber) = GetPhysical("Walk", 1, 0, GetID(pTarget))*6/10;
  EffectVar(1, pTarget, iEffectNumber) = GetPhysical("Jump", 1, 0, GetID(pTarget))*6/10;
  EffectVar(2, pTarget, iEffectNumber) = GetPhysical("Scale", 1, 0, GetID(pTarget))*6/10;
  EffectVar(3, pTarget, iEffectNumber) = GetPhysical("Hangle", 1, 0, GetID(pTarget))*6/10;

  SetPhysical("Walk", GetPhysical("Walk", 0, pTarget)-EffectVar(0, pTarget, iEffectNumber), 2, pTarget);
  SetPhysical("Jump", GetPhysical("Jump", 0, pTarget)-EffectVar(1, pTarget, iEffectNumber), 2, pTarget);
  SetPhysical("Scale", GetPhysical("Scale", 0, pTarget)-EffectVar(2, pTarget, iEffectNumber), 2, pTarget);
  SetPhysical("Hangle", GetPhysical("Hangle", 0, pTarget)-EffectVar(3, pTarget, iEffectNumber), 2, pTarget);
  
  EffectVar(4, pTarget, iEffectNumber) = iFrames;
  
  return(1);
}

global func FxParalysisEffect(string szNewEffect, object pTarget, int iEffectNumber)
{
  if(szNewEffect eq "Paralysis") return (-3);
  return(0);
}

global func FxParalysisAdd(object pTarget, int iEffectNumber, string szNewEffectName, int iNewEffectTimer, iFrames)
{
  EffectVar(4, pTarget, iEffectNumber) = Max(EffectVar(4, pTarget, iEffectNumber),iFrames);
  return(1);
}

global func FxParalysisTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  EffectVar(4, pTarget, iEffectNumber) -= GetEffect(0,pTarget,iEffectNumber,3);
  if(EffectVar(4, pTarget, iEffectNumber) <= 0)
    return(-1);
}

global func FxParalysisStop(target, no, reason, temp)
{
  if(temp)
    return();

  SetPhysical("Walk", GetPhysical("Walk", 0, target)+EffectVar(0, target, no), 2, target);
  SetPhysical("Jump", GetPhysical("Jump", 0, target)+EffectVar(1, target, no), 2, target);
  SetPhysical("Scale", GetPhysical("Scale", 0, target)+EffectVar(2, target, no), 2, target);
  SetPhysical("Hangle", GetPhysical("Hangle", 0, target)+EffectVar(3, target, no), 2, target);
  
  return(1);
}