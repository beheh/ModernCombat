/*-- Einsammeln verbieten --*/

//Bestimmte Objekte k�nnen nicht mehr eingesammelt werden.

#strict 2
#appendto DBRD


public func RejectEntrance()
{
  return true;
}