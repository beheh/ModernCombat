/*-- Standardklassen des Klassenwählers --*/

//Stellt die Standardklassen für CMC bereit.

#strict 2
#appendto MCSL


/* Die Sturmeinheit */

private func Class1Info(int iData, int iPlr)
{
  if(iData == CData_Name)       return "$assault$";
  if(iData == CData_Desc)       return "$assault_desc$";
  if(iData == CData_Clonk)      return PCMK;
  if(iData == CData_Ammo)       return [[STAM, 90], [GRAM, 8]];
  if(iData == CData_Items)      return [[ASTR, 1], [PSTL, 1], [C4PA, 3], [DGNN, 1]];
  if(iData == CData_Grenades)   return [[STUN, 2], [SGRN, 1]];
  if(iData == CData_Facet)      return 1;
  if(iData == CData_Skins)	return [[SKIN, "Assault",2]];
  if(iData == CData_ID)		return "Assault";
  return Default(iData);
}

/* Der Sanitäter */

private func Class2Info(int iData, int iPlr)
{
  if(iData == CData_Name)       return "$medic$";
  if(iData == CData_Desc)       return "$medic_desc$";
  if(iData == CData_Clonk)      return MDIC;
  if(iData == CData_Ammo)       return [[STAM, 90]];
  if(iData == CData_Items)      return [[SMGN, 1], [PSTL, 1], [CDBT, 1], [FAPK, 1]];
  if(iData == CData_Grenades)   return [[FGRN, 1], [FRAG, 2], [SRBL, 1]];
  if(iData == CData_Facet)      return 2;
  if(iData == CData_Skins)	return [[SKIN, "Medic",2]];
  if(iData == CData_ID)		return "Medic";
  return Default(iData);
}

/* Der Unterstützer */

private func Class3Info(int iData, int iPlr)
{
  if(iData == CData_Name)       return "$support$";
  if(iData == CData_Desc)       return "$support_desc$";
  if(iData == CData_Clonk)      return PCMK;
  if(iData == CData_Ammo)       return [[STAM, 90], [MIAM, 3]];
  if(iData == CData_Items)
    if(FindObject(NOAM))
      return [[MNGN, 1], [RTLR, 1], [BWTH, 1]];
    else
      return [[MNGN, 1], [RTLR, 1], [BWTH, 1], [AMPK, 1]];
  if(iData == CData_Grenades)   return [[STUN, 2], [SGRN, 1]];
  if(iData == CData_Facet)      return 3;
  if(iData == CData_Skins)	return [[SKIN, "Supporter",2]];
  if(iData == CData_ID)		return "Support";
  return Default(iData);
}

/* Der Antiskill */

private func Class4Info(int iData, int iPlr)
{
  if(iData == CData_Name)       return "$antiskill$";
  if(iData == CData_Desc)       return "$antiskill_desc$";
  if(iData == CData_Clonk)      return PCMK;
  if(iData == CData_Ammo)       return [[STAM, 80]];
  if(iData == CData_Items)      return [[PPGN, 1], [PSTL, 1], [RSHL, 1], [DGNN, 1]];
  if(iData == CData_Grenades)   return [[PGRN, 2], [STUN, 1], [SRBL, 1]];
  if(iData == CData_Facet)      return 4;
  if(iData == CData_Skins)	return [[SKIN, "Antiskill",2]];
  if(iData == CData_ID)		return "Antiskill";
  return Default(iData);
}

/* Der Artillerist */

private func Class5Info(int iData, int iPlr)
{
  if(iData == CData_Name)       return "$artillerist$";
  if(iData == CData_Desc)       return "$artillerist_desc$";
  if(iData == CData_Clonk)      return PCMK;
  if(iData == CData_Ammo)       return [[STAM, 50], [GRAM, 25]];
  if(iData == CData_Items)      return [[SGST, 1], [PSTL, 1], [BTBG, 2], [DGNN, 1]];
  if(iData == CData_Grenades)   return [[PGRN, 2], [FGRN, 2]];
  if(iData == CData_Facet)      return 5;
  if(iData == CData_Skins)	return [[SKIN, "Artillerist",2]];
  if(iData == CData_ID)		return "Artillerist";
  return Default(iData);
}