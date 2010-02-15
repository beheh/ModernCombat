/*-- langsame Projektile --*/

#strict

protected func Activate(iPlr)
{
  MessageWindow(GetDesc(),iPlr);
}

global func FastBullets()
{
  return(!ObjectCount(SBUL));
}

public func IsChooseable() { return(1); }