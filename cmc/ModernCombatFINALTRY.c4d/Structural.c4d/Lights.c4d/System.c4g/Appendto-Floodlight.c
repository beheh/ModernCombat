/*-- Scheinwerfer --*/

//Kompatibilit�t zum Lichtsystem.

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