/* Fade-Effekte */

//Fade-Befehle von Hazard an CMC-Gegenstück weiterleiten.

#strict 2


global func FadeIn(object pObject, int level)
{
  return FadeIn4K(level, pObject);
}

global func FadeOut(object pObject, int level)
{
  return FadeOut4K(level, pObject);
}