/*-- Hazardclonk --*/

//Clonks k�nnen keine B�ume f�llen.

#strict 2
#appendto HZCK


func Initialize()
{
  SetPhysical("CanChop",0,2);
  return(_inherited());
}