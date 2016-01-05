/*-- Overload-Fading --*/

#strict 2

//FadeOut mit anpassbarer Geschwindigkeit (in Promille)
global func FxFadeOutStart(object pTarget, int iNr, int iTemp, int iLevel, int iSpeed)
{
  if(!iLevel)
    EffectVar(1, pTarget, iNr) = 255000;
  else
    EffectVar(1, pTarget, iNr) = iLevel;
  
  if(!iSpeed)
  	iSpeed = 1000;
  EffectVar(2, pTarget, iNr) = iSpeed;
  return true;
}

global func FxFadeOutTimer(object pTarget, int iNr)
{
  EffectVar(0, pTarget, iNr) += EffectVar(2, pTarget, iNr);
  var r,g,b,a;
  SplitRGBaValue(GetClrModulation(pTarget), r, g, b, a);
  if(!GetClrModulation(pTarget))
  {
  	r = 255;
  	g = 255;
  	b = 255;
  }
  SetClrModulation(RGBa(r, g, b, EffectVar(0, pTarget, iNr)/1000), pTarget);

  if(EffectVar(0, pTarget, iNr) >= EffectVar(1, pTarget, iNr))
  {
  	if(EffectVar(1, pTarget, iNr) >= 255000)
  		RemoveObject(pTarget);
  	
  	return -1;
  }
}

global func FadeOut(object pObject, int iLevel, int iSpeed)
{
  if(!pObject)
    pObject = this;
    
  if(GetEffect("*FadeOut*", pObject))
    return;

  return AddEffect("FadeOut", pObject, 101, 2, 0, 0, iLevel*1000, iSpeed);
}
