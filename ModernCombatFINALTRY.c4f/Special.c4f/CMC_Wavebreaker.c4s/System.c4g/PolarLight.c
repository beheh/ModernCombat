/*-- Nordlicht --*/

//Verbesserte Effekte für Nordlichter.

#strict 2
#appendto NTLT


protected func Initialize()
{
  //In den Hintergrund verschieben und einblenden
  SetCategory(C4D_StaticBack);
  SetCategory(C4D_Background);
  FadeIn(this,10);

  //Effekte
  AddLightFlash(RandomX(600,800),0,0,RGBa(100,255,50,50),this);
  SetAction(Format("Glow%d",Random(4)));
  DoCon(RandomX(80,180));
  iSpeed=Random(7)-3;
}

private func Remove()
{
  FadeOut(this,10);
}