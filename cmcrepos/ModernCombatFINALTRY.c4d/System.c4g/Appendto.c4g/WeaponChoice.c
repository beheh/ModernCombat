/*-- Waffenwahl --*/

//Diese Regel ist aus technischen Gr�nden nicht mehr w�hlbar, wenn bereits die Klassenwahl aktiv ist.

#strict
#appendto WPCH


public func IsChooseable()
{
  if(!ObjectCount(MCSL))
    return(1);
}