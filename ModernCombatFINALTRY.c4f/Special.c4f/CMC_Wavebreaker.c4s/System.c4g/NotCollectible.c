/*-- Einsammeln verbieten --*/

//Bestimmte Objekte k�nnen nicht mehr eingesammelt werden.

#strict 2
#appendto SHEL
#appendto DPIR


public func RejectEntrance()
{
  return true;
}