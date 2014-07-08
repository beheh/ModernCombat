/*-- Neues Script --*

#strict 2

#appendto CLNK

local parachute;

public func ControlRight()
{
  if(parachute && GetActionTarget(0, parachute) == this)
  	parachute->~ControlRight();
  
  return _inherited(...);
}

public func ControlLeft()
{
  if(parachute && GetActionTarget(0, parachute) == this)
  	parachute->~ControlLeft();
  
  return _inherited(...);
}
*/
