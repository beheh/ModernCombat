/*-- Waffenwahl --*/

//Diese Regel ist aus technischen Gründen nicht mehr wählbar, wenn bereits die Klassenwahl aktiv ist.

#strict
#appendto WPCH


public func IsChooseable()
{
  if(!ObjectCount(MCSL))
    return(1);
}