/*-- Keine Materialexplosionen --*/

#strict


protected func Activate(iByPlayer)
{
  MessageWindow(GetDesc(), iByPlayer);
  return(1);
}

global func NoMatExplosions()
{
  return(ObjectCount(NMTX));
}