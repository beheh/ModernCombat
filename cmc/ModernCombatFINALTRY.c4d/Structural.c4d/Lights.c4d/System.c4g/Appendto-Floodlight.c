/*-- Scheinwerfer --*/

//Kompatibilit�t zum neuen Lichtsystem.

#strict 2
#appendto FLHH


/* Keine Zerst�rung */

public func IsBulletTarget(id def)
{
  return false;
}

func Damage()
{
  return;
}