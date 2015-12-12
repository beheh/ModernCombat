/*-- Klassenwähler - Ingenieur-Addon --*/

#strict 2
#appendto MCSL


/* Ingenieur */

private func Class6Info(int iData, int iPlr)
{
  if(iData == CData_Name)       return "Engineer";
  if(iData == CData_Desc)       return "The engineer is very powerful!";
  if(iData == CData_Clonk)      return PCMK;
  if(iData == CData_Ammo)       return [[STAM, 50], [GRAM, 100]];
  if(iData == CData_Items)      return [[ABOW, 1], [FMTR, 1], [TLKT, 1], [DGNN, 1]];
  if(iData == CData_Grenades)   return [[STUN, 2], [SGRN, 1]];
  if(iData == CData_Facet)      return 1;
  if(iData == CData_Skins)	return [[SKIN, "Engineer",1]];
  return Default(iData);
}
