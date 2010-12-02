/*-- Schneeberg --*/

#strict


/* Initialisierung */

public func Initialize()
{
  SetCategory(GetCategory() | C4D_Parallax() | C4D_Background());
  Local()=60; Local(1)=80;
}