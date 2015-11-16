/* Fade-Effekte */

//Verbesserte Ein- und Ausblendeffekte.

#strict 2


global func FadeIn(object pObject, int level)
{
  return FadeIn4K(level, pObject);
}

global func FadeOut(object pObject, int level)
{
  return FadeOut4K(level, pObject);
}