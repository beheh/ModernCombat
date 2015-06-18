/*-- Bildschirm --*/

#strict 2

#include SCA1

/* Initialisierung */

func Initialize()
{
  //Standardanimation
  SetAction("Scan");
  AddFrame();
}

/* Serialisierung */

public func Serialize(array& extra)
{
if(GetAction() != "Scan")
    extra[GetLength(extra)] = Format("SetAction(%s)",GetAction());
}
