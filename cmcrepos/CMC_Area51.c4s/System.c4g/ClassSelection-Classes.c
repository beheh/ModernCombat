/*-- Standardklassen des Klassenwählers --*/

//Stellt die Standardklassen für CMC bereit.

#strict 2
#appendto MCSL

/* Der Artillerist */

private func Class5Info(int iData, int iPlr)
{
  if(iData == CData_Name)       return "$artillerist$";
  if(iData == CData_Desc)       return "$artillerist_desc$";
  if(iData == CData_Clonk)      return PCMK;
  if(iData == CData_Ammo)       return [[STAM, 50], [GRAM, 25]];
  if(iData == CData_Items)      return [[SGST, 1], [PSTL, 1], [DGNN, 1], [BTBG, 2]];
  if(iData == CData_Grenades)   return [[PGRN, 2], [FGRN, 2]];
  if(iData == CData_Facet)      return 5;
  return Default(iData);
}
