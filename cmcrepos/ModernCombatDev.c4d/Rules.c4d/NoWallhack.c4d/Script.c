/*-- Kein Wallhack --*/

#strict 2


/* Infos */

protected func Activate(iByPlayer)
{
  MessageWindow(GetDesc(), iByPlayer);
  return 1;
}

public func IsChooseable() { return true; }
