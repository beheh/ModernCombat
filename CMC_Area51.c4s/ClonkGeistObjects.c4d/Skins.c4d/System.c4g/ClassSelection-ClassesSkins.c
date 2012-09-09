/*-- Standardklassen des Klassenwählers --*/

//Stellt die Standardklassen für CMC bereit.

#strict 2
#appendto MCSL

/* Der Artillerist */

private func Class5Info(int iData, int iPlr)
{
  if(iData == CData_Skins)	return [[SKIN, "GhilliDirty"], [SKIN, "GhilliGreen"], [SKIN, "Swap"]];
  return _inherited(iData, iPlr);
}
