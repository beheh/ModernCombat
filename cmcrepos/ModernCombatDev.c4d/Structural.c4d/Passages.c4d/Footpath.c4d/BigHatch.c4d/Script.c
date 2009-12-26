/*-- Groﬂe Bodenlucke --*/

#strict

#include GBDR
#include HA4K


/* Initalisierung */

public func Initialize() 
{
   SetAction("Closed");
   _inherited();
   SetMaxDamage(200);
}

/* Schlieﬂung */

public func OnClose()
{
  if(GetAction() eq "Closed" || GetAction() eq "Close") return();
  SetAction("Close");
  Sound("Airlock2");
  SetSolidMask(0, 30, 30, 30);
}