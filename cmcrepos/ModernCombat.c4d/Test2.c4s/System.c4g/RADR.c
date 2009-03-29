/*-- Radare geben nun Funksignale aus --*/

#strict 2
#appendto RADR

func Initialize()
{
  Sound("RADR_Chat",0,0,0,0,+1); 
  return _inherited();
}