/*-- Alarmlicht --*/

//Kann nicht getroffen oder besch�digt werden.

#strict 2
#appendto ALGH

public func IsBulletTarget()	{}


/* Nicht zerst�rbar */

func Damage()
{
  return;
}