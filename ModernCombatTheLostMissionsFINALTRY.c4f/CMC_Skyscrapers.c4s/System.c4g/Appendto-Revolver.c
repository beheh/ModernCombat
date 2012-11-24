/*-- Revolver --*/

//Revolver können keine Peilsender abfeuern.

#strict 2
#appendto RVLR


/* Peilsender-Feuermodus deaktivieren */

public func FMData2(int data)
{
  return false;
}