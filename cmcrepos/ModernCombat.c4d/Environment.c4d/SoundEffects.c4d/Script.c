/*-- Soundkulisse --*/

#strict

local soundset,interval,variation;


public func Set(string szSoundSet, int iInterval, int iVariation)
{
  soundset = szSoundSet;
  if(!soundset) soundset = "Waves*";
  
  interval = iInterval;
  //if(!interval) interval = 100;
  
  variation = iVariation;
  //if(!variation) variation = 40;
  
  if(iInterval)
    StartSoundEffect();
  else
    StartDuroSound();
}

protected func StartDuroSound()
{
  Sound (soundset,false,this(),0,0,+1); 
}

protected func StartSoundEffect()
{
  AddEffect ("IntDoSound",this(),50,
            interval+RandomX(-variation,+variation),
            this(),0,soundset); 
}

public func FxIntDoSoundStart(object pTarget, int iEffectNumber, int iTemp,szsound)
{
  EffectVar (0,pTarget,iEffectNumber) = szsound;
  return(1);
}

public func FxIntDoSoundTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  Sound (EffectVar(0,pTarget,iEffectNumber),false, pTarget);
  pTarget->StartSoundEffect();
  return(-1);
}