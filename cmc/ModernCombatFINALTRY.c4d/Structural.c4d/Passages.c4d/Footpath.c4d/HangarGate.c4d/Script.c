/*--- Hangartor ---*/

#strict 2
#include CSTR

public func IsMachine()		{return true;}


/* Initialisierung */

protected func Initialize()
{
  SetAction("Stand");
}

/* ÷ffnen und Schlieﬂen */

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
  if(i == 1) return("$Left$");
  if(i == 2) return("$Right$");
  if(i == 3) return("$Stop$");
}

public func ConsoleControlled(int i)
{
  if(i == 1 && GetAction() != "ControlLeft") ControlLeft();
  if(i == 2 && GetAction() != "ControlRight") ControlRight();
  if(i == 3 && GetAction() != "ControlStop") ControlStop();
}

/* Aufprallkontakt */

protected func ContactLeft()
{
  return(ControlStop());
}

protected func ContactRight()
{
  return(ControlStop());
}