/*-- Fading --*/

//Globaler Ausblendeffekt.

#strict


global func FxFadeOutStart(target, no, temp, level)
{
  if(!level)
    EffectVar(1, target, no) = 255;
  else
    EffectVar(1, target, no) = level;
}

global func FxFadeOutTimer(target, no)
{
  EffectVar(0, target, no)++;
  SetClrModulation(RGBa(255,255,255, EffectVar(0, target, no)), target);
  if(EffectVar(0, target, no) >= EffectVar(1, target, no)) { if(EffectVar(1, target, no) >= 255) RemoveObject(target); return(-1); }
}

global func FadeOut(object pObject, int level)
{
  if(!pObject)
    pObject = this();
    
  if(GetEffect("*FadeOut*", pObject)) return();
  return(AddEffect("FadeOut", pObject, 101, 2,0,0,level));
}

global func FxFadeInStart(target, no, temp, level)
{
  EffectVar(0, target, no) = 255;
  EffectVar(1, target, no) = level;
}

global func FxFadeInTimer(target, no)
{
  EffectVar(0, target, no)--;
  SetClrModulation(RGBa(255,255,255, EffectVar(0, target, no)), target);
  if(EffectVar(0, target, no) <= EffectVar(1, target, no)) { return(-1); }
}

global func FadeIn(object pObject, int level)
{
  if(!pObject)
    pObject = this();
  
  if(GetEffect("*FadeIn*", pObject)) return();
  return(AddEffect("FadeIn", pObject, 101, 2,0,0,level));
}

global func StopFading(int color, object pObject)
{
  if(!pObject)
    pObject = this();
  
  var eff = GetEffectCount("*Fade*",pObject);
  for(eff; eff; eff--)
    RemoveEffect("*Fade*",pObject);
  
  SetClrModulation(color,pObject);
}
