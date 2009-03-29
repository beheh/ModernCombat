/*-- Waffenwahl --*/

#strict
#appendto WPCH

public func IsChooseable()
{
  if(!ObjectCount(MCSL))
    return(1);
}