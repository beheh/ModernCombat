/*-- Neues Script --*/

#strict 2

#appendto SEGU

public func ConsoleControl(int i)
{
  if(i == 1)
  {
    if(fActive) return "$TurnOff$";
    else
    return "$TurnOn$";
  }
  /*if(i == 2)
    if(GetAction() == "Destroyed")
      return "$Repair$";*/
}

public func ConsoleControlled(int i)
{
  if(i == 1)
  {
    if(fActive) TurnOff();
    else
    {
      TurnOn();
    }
  }
  /*if(i == 2)
  {
    if(GetAction() == "Destroyed")
      StartRepair();
  }*/
}
