/*-- Vogel --*/

//V�geln wird zuf�llig eine andere Grafik zugewiesen.

#strict 2

#appendto BIRD


/* Initialisierung */

public func Initialize()
{
  if(!Random(20))
    SetGraphics(0, this, BRSN);

  return inherited();
}
