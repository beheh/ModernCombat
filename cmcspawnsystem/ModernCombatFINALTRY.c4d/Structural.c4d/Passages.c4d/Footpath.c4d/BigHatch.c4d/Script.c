/*-- Groﬂe Bodenluke --*/

#strict 2

#include GBDR
#include HA4K


/* Initialisierung */

public func Initialize() 
{
   SetAction("Closed");
   _inherited();
}

/* Schlieﬂung */

public func OnClose()
{
  if(GetAction() == "Closed" || GetAction() == "Close") return;
  SetAction("Close");
  Sound("Airlock2");
  SetSolidMask(0, 30, 30, 30);
}