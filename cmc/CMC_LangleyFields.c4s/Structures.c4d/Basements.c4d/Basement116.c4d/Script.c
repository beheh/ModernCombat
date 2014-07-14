/*-- Fundament --*/

#strict 2
#include BAS7

private func BasementID()	{return(BT01);}
private func BasementWidth()	{return(116);}

/* Fertigstellung */

protected func Initialize()
{
  //Bug in CR Basements, der verhindert, dass im inkludierten Gebäude Initialize aufgerufen wird.
  if(GetID() != BasementID() ) 
    return _inherited(...);
  SetAction("Consolidate");
  MoveOutClonks();
  return _inherited(...);
}

protected func Damage()
{
  //todo!
  return; 
}