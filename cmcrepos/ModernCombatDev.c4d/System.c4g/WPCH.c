/*-- Waffenwahl --*/

#strict 2
#appendto WPCH

public func IsChooseable()
{
  if(!ObjectCount(MCSL))
    return 1;
}
