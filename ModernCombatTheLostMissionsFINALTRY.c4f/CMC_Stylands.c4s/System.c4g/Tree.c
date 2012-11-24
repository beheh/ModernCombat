/*-- Rasen im Vordergrund --*/

#strict
#appendto GRAS

func Construction()
{
  _inherited();
  SetCategory(C4D_Foreground|GetCategory());
}