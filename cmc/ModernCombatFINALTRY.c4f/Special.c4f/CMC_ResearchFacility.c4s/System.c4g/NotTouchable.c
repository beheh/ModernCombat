/*-- Anfassen verbieten --*/

//Bestimmte Objekte können nicht mehr angefasst werden.

#strict 2
#appendto LCKR
#appendto GTBL
#appendto FLGH


/* Initialisierung */

public func Initialize()
{
  SetObjectLayer(this());
  return(_inherited());
}