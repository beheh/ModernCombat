/*-- Klassen des Klassenwählers --*/

//Stellt modifizierte Klassen bereit.

#strict 2
#appendto MCSL


/* Der Pilot */

private func Class1Info(int iData, int iPlr)
{
  if(iData == CData_Name)       return "$pilot$";
  if(iData == CData_Desc)       return "$pilot_desc$";
  if(iData == CData_Clonk)      return MDIC;
  if(iData == CData_Ammo)       return [[STAM, 120]];
  if(iData == CData_Items)      return [[SMGN, 1], [RVLR, 1], [BWTH, 1], [DGNN, 1]];
  if(iData == CData_Grenades)   return [[STUN, 2], [SRBL, 2]];
  if(iData == CData_Facet)      return 6;
  if(iData == CData_Skins)	return [[SKIN, "Medic",2]];
  return Default(iData);
}

/* Der Unterstützer */

private func Class2Info(int iData, int iPlr)
{
  if(iData == CData_Name)       return "$support$";
  if(iData == CData_Desc)       return "$support_desc$";
  if(iData == CData_Clonk)      return PCMK;
  if(iData == CData_Ammo)       return [[STAM, 90], [MIAM, 3]];
  if(iData == CData_Items)
    if(FindObject(NOAM))
      return [[MNGN, 1], [RTLR, 1], [DGNN, 1]];
    else
      return [[MNGN, 1], [RTLR, 1], [AMPK, 1], [DGNN, 1]];
  if(iData == CData_Grenades)   return [[FGRN, 1], [STUN, 2]];
  if(iData == CData_Facet)      return 3;
  if(iData == CData_Skins)	return [[SKIN, "Supporter",2]];
  return Default(iData);
}

/* Der Artillerist */

private func Class3Info(int iData, int iPlr)
{
  if(iData == CData_Name)       return "$artillerist$";
  if(iData == CData_Desc)       return "$artillerist_desc$";
  if(iData == CData_Clonk)      return PCMK;
  if(iData == CData_Ammo)       return [[STAM, 50], [GRAM, 25]];
  if(iData == CData_Items)      return [[SGST, 1], [PSTL, 1], [BTBG, 2], [DGNN, 1]];
  if(iData == CData_Grenades)   return [[FGRN, 2], [SRBL, 1]];
  if(iData == CData_Facet)      return 5;
  if(iData == CData_Skins)	return [[SKIN, "Artillerist",2]];
  return Default(iData);
}

private func Class4Info(int iData, int iPlr)	{return;}
private func Class5Info(int iData, int iPlr)	{return;}