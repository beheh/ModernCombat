/*-- Scheinwerfer --*/

//Kompatibilität zum neuen Lichtsystem.

#strict 2
#appendto FLHH


/* Keine Zerstörung */

public func IsBulletTarget(id def)
{
  return false;
}

func Damage()
{
  return;
}