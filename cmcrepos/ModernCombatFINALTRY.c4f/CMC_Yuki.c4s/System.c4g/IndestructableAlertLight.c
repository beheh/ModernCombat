/*-- Alarmlicht --*/

//Kann nicht getroffen oder beschädigt werden.

#strict 2
#appendto ALGH

public func IsBulletTarget()	{}


/* Nicht zerstörbar */

func Damage()
{
  return;
}