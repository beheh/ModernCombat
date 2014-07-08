/*-- Lichtkegel --*/

#strict 2
#include LGHC


/* Erstellung */

global func AddLightCone2(int iSize, int iColor, object pTarget)
{
  if(!pTarget && !(pTarget = this))
    return;

  return CreateLight(LGH2, iSize, iColor, pTarget);
}