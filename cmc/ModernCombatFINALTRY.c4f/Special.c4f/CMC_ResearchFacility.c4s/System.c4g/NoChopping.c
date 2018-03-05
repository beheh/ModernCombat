/*-- Hazardclonk --*/

//Clonks können keine Bäume fällen.

#strict 2
#appendto HZCK


func Initialize()
{
  SetPhysical("CanChop",0,2);
  return(_inherited());
}