/*-- Generator --*/

//Der Generator kann per Konsole gesteuert werden.

#strict 2
#appendto GNRT


/* Konsolensteuerung */

public func ConsoleControl(int i)
{
  if(i == 1)
  {
    if(IsWorking())
      return "$TurnOff$";
    else
      return "$TurnOn$";
  }
}

public func ConsoleControlled(int i)
{
  if(i == 1)
  {
    if(IsWorking())
      TurnOff();
    else
      TurnOn();
  }
}