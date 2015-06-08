/*-- Keine Materialexplosionen --*/

#strict 2


protected func Activate(iByPlayer)
{
  MessageWindow(GetDesc(), iByPlayer);
  return 1;
}

global func NoMatExplosions()
{
  return ObjectCount(NMTX);
}