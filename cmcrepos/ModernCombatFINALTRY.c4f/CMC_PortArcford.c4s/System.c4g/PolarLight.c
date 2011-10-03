/*-- Nordlicht --*/

//Verbesserte Effekte für Nordlichter.

#strict 2
#appendto NTLT

protected func Initialize()
{
  AddLight(200,RGB(100,255,50));
  SetClrModulation(RGBa(255, 255, 255, 80));
  SetAction(Format("Glow%d",Random(4)));
  DoCon(Random(80)*(-Random(3)));
  iSpeed=Random(7)-3;
}