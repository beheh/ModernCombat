/*-- Wüste --*/

#strict 2


/* Initialisierung */

public func Initialize()
{
  SetCategory(GetCategory() | C4D_Parallax | C4D_Background | C4D_MouseIgnore);
  Local()=40; Local(1)=100;
}