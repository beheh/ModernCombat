/*-- Revolver --*/

//Revolver k�nnen keine Peilsender abfeuern.

#strict 2
#appendto RVLR


/* Peilsender-Feuermodus deaktivieren */

public func FMData2(int data)
{
  return false;
}