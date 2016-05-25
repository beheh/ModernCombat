/*-- Soundkulisse --*/

#strict 2

local name,interval,random,vollow, volhigh,global,volume;


/* Einstellung */

public func Set(string szName, int iInterval, int iRandom, int iVollow, int iVolhigh, bool fGlobal)
{
  //Soundname
  name = szName;
  if(!name) return;

  //Abspielinterval
  interval = iInterval;

  //Aufrufzufall bei jedem Intervall
  random = iRandom;

  //Lautstärke
  vollow = iVollow;
  if(vollow > 0)
    volhigh = iVolhigh;
  else
    vollow = 0;

  //Lokalität
  global = fGlobal;

  //Sound konfigurieren
  StartSoundEffect();
}

/* Soundkonfiguration */

protected func StartSoundEffect()
{
  //Lautstärke ermitteln
  if(vollow)
  {
    if(volhigh <= 0 || volhigh > 100)
      volhigh = 100;

    volume = RandomX(vollow,volhigh);
  }
  else
    volume = 0;

  //Intervall ermitteln und Sound abspielen
  if(interval == 0)
    Sound(name,global,this,volume,0,+1);
  else
  {
    Sound(name,global,this,volume);

    //Neuen Aufruf planen
    AddEffect("IntDoSound",this,50,interval+RandomX(-random,+random),this,0,name);
  }
}

protected func SoundEffect()
{
  //Sound abspielen
  Sound(name,global,this,volume);

  //Neuen Aufruf planen
  AddEffect("IntDoSound",this,50,interval+RandomX(-random,+random),this,0,name);
}

/* Aufrufeffekt */

public func FxIntDoSoundStart(object pTarget, int iEffectNumber, int iTemp,szsound)
{
  EffectVar(0,pTarget,iEffectNumber) = szsound;
  return 1;
}

public func FxIntDoSoundTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  pTarget->SoundEffect();
  return -1;
}