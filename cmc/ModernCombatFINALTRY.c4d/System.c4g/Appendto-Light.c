/*-- Licht --*/

//Lichter werden nur sichtbar wenn die Effektstufe es zul�sst.

#strict 2
#appendto LIGH


public func TurnOn()
{
  if(GetEffectLevel() == 3)
    SetVisibility(VIS_All);
}