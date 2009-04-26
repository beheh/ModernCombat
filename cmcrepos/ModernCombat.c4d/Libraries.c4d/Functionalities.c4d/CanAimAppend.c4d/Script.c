/*-- Zielen --*/

#strict 2
#appendto L_CA

//Callback: CanAim
//Callback: ControlAim

/*protected func ControlAim(string command)
{
  if(Contained())
  {
    if(Contained()->~CanAim())
      return Contained()->ControlAim(command,this,...);
  }
  
  if(GetProcedure() == "PUSH")
  {
    if(GetActionTarget())
      if(GetActionTarget()->~CanAim())
        return GetActionTarget()->~ControlAim(command,this,...);
  }
  
  return _inherited(command,...);
}*/

public func IsAiming()
{ 
  if(GetProcedure() == "PUSH")
  {
    if(GetActionTarget())
      //if(GetActionTarget()->~CanAim())
        return GetActionTarget()->~IsAiming();
  }

	return _inherited();
}