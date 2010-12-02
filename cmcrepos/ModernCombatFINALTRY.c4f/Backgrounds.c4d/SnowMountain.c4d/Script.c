/*-- Schneeberg --*/

#strict


/* Initialisierung */

public func Initialize()
{
  SetCategory(GetCategory() | C4D_Parallax() | C4D_Background());
  Local()=30; Local(1)=30;
}