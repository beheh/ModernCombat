/*-- Rasen im Vordergrund --*/

#strict 2
#appendto GRAS

func Construction()
{
  _inherited();
  SetCategory(C4D_Foreground|GetCategory());
}

protected func Incineration() {Extinguish();}