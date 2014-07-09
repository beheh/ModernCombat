/*-- Bildschirm --*/

#strict 2


/* Initialisierung */

func Initialize()
{
  //Standardanimation
  SetAction("Eurocorps");
  AddFrame();
}

/* Serialisierung */

public func Serialize(array& extra)
{
  if(GetAction() != "Eurocorps")
    extra[GetLength(extra)] = Format("SetAction(%s)",GetAction());
}