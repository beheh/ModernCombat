/*-- Einsammeln verbieten --*/

//Bestimmte Objekte können nicht mehr eingesammelt werden.

#strict 2
#appendto DPIR
#appendto SHEL


public func RejectEntrance()
{
  return true;
}