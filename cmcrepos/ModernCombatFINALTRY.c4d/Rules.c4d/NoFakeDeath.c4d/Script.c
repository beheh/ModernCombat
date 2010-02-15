/*-- Kein FakeDeath --*/

#strict


/* Infos */

protected func Activate(iByPlayer)
{
  MessageWindow(GetDesc(), iByPlayer);
  return(1);
}

public func IsChooseable() { return(1); }