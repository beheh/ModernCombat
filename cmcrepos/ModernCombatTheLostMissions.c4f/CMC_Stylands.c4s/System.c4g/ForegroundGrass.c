/*-- Grass --*/

//Grass ist immer im Vordergrund.

#strict 2
#appendto GRAS


protected func Incineration() {Extinguish();}

func Construction()
{
  _inherited();
  SetCategory(C4D_Foreground|GetCategory());
}