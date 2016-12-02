/*-- Anfassen verbieten --*/

//Bestimmte Objekte können nicht mehr angefasst werden.

#strict 2
#appendto LCKR
#appendto LTBL
#appendto BANR
#appendto THRN
#appendto BWRC
#appendto SPRC
#appendto SWRC
#appendto WODC
#appendto CNDL
#appendto SHRC


/* Initialisierung */

public func Initialize() 
{
  SetObjectLayer(this());
  return(_inherited());
}