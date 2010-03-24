/*-- Standardklassen des Klassenwählers --*/

#strict
#appendto MCSL


/* Die Sturmeinheit */

private func Class1Info(int iData)
{
  if(iData == CData_Name)  return("<c ffff33>$assault$</c>");
  if(iData == CData_Desc)  return("$assault_desc$");
  if(iData == CData_Clonk) return(PCMK);
  if(iData == CData_Items) return("1x {{ASTR}}|1x {{PSTL}}||--|--|--||2x {{FGRN}}|1x {{FRAG}}");
  if(iData == CData_Facet) return(1);
  return(Default(iData));
}

private func Class1Setup(int iPlr)
{
  var new = CreateObject(PCMK,0,0,iPlr);
  DoAmmo(STAM,100,new);
  DoAmmo(GRAM,12,new);
  new->CreateContents(ASTR);
  new->CreateContents(PSTL);
  CreateObject(FGRN,0,0,iPlr)->Activate(new);
  CreateObject(FGRN,0,0,iPlr)->Activate(new);
  CreateObject(FRAG,0,0,iPlr)->Activate(new);
  return(new);
}

/* Der Sanitäter */

private func Class2Info(int iData)
{

  if(iData == CData_Name)  return("<c ffff33>$medic$</c>");
  if(iData == CData_Desc)  return("$medic_desc$");
  if(iData == CData_Clonk) return(MDIC);
  if(iData == CData_Items) return("1x {{SMGN}}|1x {{PSTL}}||1x {{CDBT}}|1x {{FAPK}}|--||2x {{STUN}}|--");
  if(iData == CData_Facet) return(2);
  return(Default(iData));
}

private func Class2Setup(int iPlr)
{
  var new = CreateObject(MDIC,0,0,iPlr);
  DoAmmo(STAM,50,new);
  new->CreateContents(SMGN);
  new->CreateContents(PSTL);
  new->CreateContents(CDBT);
  new->CreateContents(FAPK);
  CreateObject(STUN,0,0,iPlr)->Activate(new);
  CreateObject(STUN,0,0,iPlr)->Activate(new);
  return(new);
}

/* Der Unterstützer */

private func Class3Info(int iData)
{
  if(iData == CData_Name)  return("<c ffff33>$support$</c>");
  if(iData == CData_Desc)  return("$support_desc$");
  if(iData == CData_Clonk) return(PCMK);
  if(iData == CData_Items) return("1x {{MNGN}}|1x {{PSTL}}||1x {{BBTP}}|--|--||2x {{SGRN}}|--");
  if(iData == CData_Facet) return(3);
  return(Default(iData));
}

private func Class3Setup(int iPlr)
{
  var new = CreateObject(PCMK,0,0,iPlr);
  DoAmmo(STAM,200,new);
  new->CreateContents(MNGN);
  new->CreateContents(PSTL);
  new->CreateContents(BBTP);
  CreateObject(SGRN,0,0,iPlr)->Activate(new);
  CreateObject(SGRN,0,0,iPlr)->Activate(new);
  return(new);
}

/* Der Antiskill */

private func Class4Info(int iData)
{

  if(iData == CData_Name)  return("<c ffff33>$antiskill$</c>");
  if(iData == CData_Desc)  return("$antiskill_desc$");
  if(iData == CData_Clonk) return(PCMK);
  if(iData == CData_Items) return("1x {{PPGN}}|1x {{PSTL}}||1x {{RSHL}}|--|--||2x {{FRAG}}|--");
  if(iData == CData_Facet) return(4);
  return(Default(iData));
}

private func Class4Setup(int iPlr)
{
  var new = CreateObject(PCMK,0,0,iPlr);
  DoAmmo(STAM,100,new);
  new->CreateContents(PPGN);
  new->CreateContents(PSTL);
  new->CreateContents(RSHL);
  CreateObject(FRAG,0,0,iPlr)->Activate(new);
  CreateObject(FRAG,0,0,iPlr)->Activate(new);
  return(new);
}

/* Der Artillerist */

private func Class5Info(int iData)
{
  if(iData == CData_Name)  return("<c ffff33>$artillerist$</c>");
  if(iData == CData_Desc)  return("$artillerist_desc$");
  if(iData == CData_Clonk) return(PCMK);
  if(iData == CData_Items) return("1x {{SGST}}|1x {{SMGN}}||--|--|--||1x {{STUN}}|1x {{SGRN}}");
  if(iData == CData_Facet) return(5);
  return(Default(iData));
}

private func Class5Setup(int iPlr)
{
  var new = CreateObject(PCMK,0,0,iPlr);
  DoAmmo(STAM,50,new);
  DoAmmo(GRAM,40,new);
  new->CreateContents(SGST);
  new->CreateContents(SMGN);
  CreateObject(STUN,0,0,iPlr)->Activate(new);
  CreateObject(SGRN,0,0,iPlr)->Activate(new);
  return(new);
}