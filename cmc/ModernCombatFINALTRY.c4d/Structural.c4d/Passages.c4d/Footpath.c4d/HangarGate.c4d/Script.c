/*--- Hangartor ---*/

#strict 2

local direction;

public func IsMachine()	{return true;}


/* Initialisierung */

protected func Initialize()
{
  SetAction("Stand");
  direction = 0;
}

/* Bewegung nach links oder rechts */

protected func ControlLeft()
{
  SetAction("Travel");
  SetComDir(COMD_Left);
}

public func ControlRight()
{
  SetAction("Travel");
  SetComDir(COMD_Right);
}

public func ControlStop()
{
  Sound("HangarClose.ogg");
  SetAction("Stand");
  SetComDir(COMD_Stop);
  SetXDir();
  SetYDir();
}

/* Konsolensteuerung */

public func ConsoleControl(int i)
{
  if(GetAction() == "Stand")
  {
    if(i == 1)
      if(direction == 0)
        return "$Right$";
      else
        return "$Left$";

    if(i == 2)
      if(direction == 1)
        return "$Right$";
      else
        return "$Left$";
  }
  else
  {
    if(i == 1)
      return "$Stop$";

    if(i == 2)
      if(direction == 1)
        return "$Right$";
      else
        return "$Left$";
  }
}

public func ConsoleControlled(int i)
{
  if(GetAction() == "Stand")
  {
    if(i == 1)
      if(direction == 0)
        ControlRight();
      else
        ControlLeft();

    if(i == 2)
      if(direction == 1)
      {
        ControlRight();
        direction = 0;
      }
      else
      {
        ControlLeft();
        direction = 1;
      }
  }
  else
  {
    if(i == 1)
      ControlStop();

    if(i == 2)
      if(direction == 1)
      {
        ControlRight();
        direction = 0;
      }
      else
      {
        ControlLeft();
        direction = 1;
      }
  }
}

/* Aufprallkontakt */

protected func ContactLeft()
{
  direction = 0;
  return(ControlStop());
}

protected func ContactRight()
{
  direction = 1;
  return(ControlStop());
}