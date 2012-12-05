/*-- Standardklassen des Klassenw�hlers --*/

//Stellt die Standardklassen f�r CMC bereit.

#strict 2
#appendto MCSL


/* Der Unterst�tzer */

private func Class3Info(int iData, int iPlr)
{
  if(iData == CData_Skins)	return [[SKIN, "Supporter"]];
  return _inherited(iData, iPlr);
}

/* Der Antiskill */

private func Class4Info(int iData, int iPlr)
{
  if(iData == CData_Skins)	return [[SKIN, "Antiskill"]];
  return _inherited(iData, iPlr);
}

/* Der Artillerist */

private func Class5Info(int iData, int iPlr)
{
  if(iData == CData_Skins)	return [[SKIN, "Artillerist"]];
  return _inherited(iData, iPlr);
}
