/*-- Hai --*/

//Haien wird zuf�llig eine andere Grafik zugewiesen.

#strict 2

#appendto SHRK


/* Initialisierung */

public func Initialize()
{
  if(!Random(36))
    SetGraphics(0, this, SKSN);

  return inherited();
}