/* Fade-Effekte */

//Verbesserter Einblendeffekt.

#strict 2


/* Jetzt wirklich keinen Frame sichtbar */

global func FadeIn(object pObject, int level) {
  return FadeIn4K(level, pObject);
}

global func FadeOut(object pObject, int level) {
  return FadeOut4K(level, pObject);
}
