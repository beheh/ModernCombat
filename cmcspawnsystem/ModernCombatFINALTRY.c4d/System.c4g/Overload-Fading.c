/* Fade-Effekte */

//Verbesserter Einblendeffekt.

#strict 2


/* Jetzt wirklich keinen Frame sichtbar */

global func FadeIn(object pObject, int level)
{
  if(!pObject)
    pObject = this();
  
  if(GetEffect("*FadeIn*", pObject)) return;
  SetClrModulation(RGBa(255,255,255,255), pObject);
  return(AddEffect("FadeIn", pObject, 101, 2,0,0,level));
}