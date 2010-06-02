/*-- LMS-Bugs mit Hazard --*/

#strict 2
#appendto GLMS

protected func InitializePlayer(int iPlr, int iX, int iY, object pBase, int iTeam)
{
  if(FindObject(CHOS)) return;
  _inherited(iPlr, iX, iY, pBase, iTeam);
}
