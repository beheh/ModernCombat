/*-- Standardklassen des Klassenwählers --*/

#strict 2
#appendto MCSL


/* Die Sturmeinheit */

private func Class1Info(int iData, int iPlr)
{
  if(iData == CData_Name)       return "$assault$";
  if(iData == CData_Desc)       return "$assault_desc$";
  if(iData == CData_Clonk)      return PCMK;
  if(iData == CData_Ammo)       return [[STAM, 90], [GRAM, 8]];
  if(iData == CData_Items)      return [[ASTR, 1], [PSTL, 1], [C4PA, 1], [DGNN, 1]];
  if(iData == CData_Grenades)   return [[STUN, 2], [SGRN, 1]];
  if(iData == CData_Facet)      return 1;
  return Default(iData);
}

/* Der Sanitäter */

private func Class2Info(int iData, int iPlr)
{

  if(iData == CData_Name)       return "$medic$";
  if(iData == CData_Desc)       return "$medic_desc$";
  if(iData == CData_Clonk)      return MDIC;
  if(iData == CData_Ammo)       return [[STAM, 80]];
  if(iData == CData_Items)      return [[SMGN, 1], [PSTL, 1], [CDBT, 1], [FAPK, 1]];
  if(iData == CData_Grenades)   return [[FGRN, 1], [FRAG, 2]];
  if(iData == CData_Facet)      return 2 ;
  return Default(iData);
}

/* Der Unterstützer */

private func Class3Info(int iData, int iPlr)
{
  if(iData == CData_Name)       return "$support$";
  if(iData == CData_Desc)       return "$support_desc$";
  if(iData == CData_Clonk)      return PCMK;
  if(iData == CData_Ammo)       return [[STAM, 200], [MIAM, 4]];
  if(iData == CData_Items)
    if(FindObject(NOAM))
      return [[MNGN, 1], [RTLR, 1], [DGNN, 1]];
    else
      return [[MNGN, 1], [RTLR, 1], [AMPK, 1], [DGNN, 1]];
  if(iData == CData_Grenades)   return [[STUN, 2], [SGRN, 2]];
  if(iData == CData_Facet)      return 3;
  return Default(iData);
}

/* Der Antiskill */

private func Class4Info(int iData, int iPlr)
{

  if(iData == CData_Name)       return "$antiskill$";
  if(iData == CData_Desc)       return "$antiskill_desc$";
  if(iData == CData_Clonk)      return PCMK;
  if(iData == CData_Ammo)       return [[STAM, 40]];
  if(iData == CData_Items)      return [[PPGN, 1], [PSTL, 1], [RSHL, 1], [DGNN, 1]];
  if(iData == CData_Grenades)   return [[PGRN, 1], [STUN, 2]];
  if(iData == CData_Facet)      return 4;
  return Default(iData);
}

/* Der Artillerist */

private func Class5Info(int iData, int iPlr)
{
  if(iData == CData_Name)       return "$artillerist$";
  if(iData == CData_Desc)       return "$artillerist_desc$";
  if(iData == CData_Clonk)      return PCMK;
  if(iData == CData_Ammo)       return [[STAM, 50], [GRAM, 30]];
  if(iData == CData_Items)      return [[SGST, 1], [PSTL, 1], [BBTP, 2], [DGNN, 1]];
  if(iData == CData_Grenades)   return [[PGRN, 2], [STUN, 2]];
  if(iData == CData_Facet)      return 5;
  return Default(iData);
}