/*-- Einsammeln verbieten --*/

//Bestimmte Objekte k�nnen nicht mehr eingesammelt werden.

#strict 2
#appendto DPIR
#appendto SHEL


public func RejectEntrance()
{
  return true;
}