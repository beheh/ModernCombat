/*-- Soundkulisse --*/

#strict 2

local soundset,interval,variation,falloff,globalsound;


/* Einstellung */

public func Set(string szSoundSet, int iInterval, int iVariation, int iFalloff)
{
  soundset = szSoundSet;
  if(!soundset) soundset = "Wave*.ogg";

  interval = iInterval;
  variation = iVariation;

  falloff = iFalloff;

  if(iInterval)
    StartSoundEffect();
  else
    StartDuroSound();
}

/* Soundsteuerung */

protected func StartDuroSound()
{
  var f,g;
  if(falloff == -1)
  {
    f = 0;
    g = true;
  }
  else
  {
    f = falloff;
    g = false;
  }
  Sound(soundset,g,this,0,0,+1,0,f);
}

protected func StartSoundEffect()
{
  var f,g;
  if(falloff == -1)
  {
    f = 0;
    g = true;
  }
  else
  {
    f = falloff;
    g = false;
  }
  Sound(soundset,g,this,0,0,0,0,f);

  AddEffect("IntDoSound",this,50,
  		interval+RandomX(-variation,+variation),
  		this,0,soundset); 
}

public func FxIntDoSoundStart(object pTarget, int iEffectNumber, int iTemp,szsound)
{
  EffectVar(0,pTarget,iEffectNumber) = szsound;
  return 1;
}

public func FxIntDoSoundTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  pTarget->StartSoundEffect();
  return -1;
}