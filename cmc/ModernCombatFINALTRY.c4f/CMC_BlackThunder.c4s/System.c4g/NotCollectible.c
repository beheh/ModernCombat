/*-- Einsammeln verbieten --*/

//Bestimmte Objekte können nicht mehr eingesammelt werden.

#strict 2
#appendto DBRD
#appendto SNKE


public func RejectEntrance()
{
  return true;
}