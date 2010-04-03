/*-- Standardklassen des Klassenwählers --*/

#strict
#appendto MCSL


/* Die Sturmeinheit */

private func Class1Info(int iData)
{
  //Hat zu viel Explosives gehabt
  if(iData == CData_Name)  return("<c ffff33>$assault$</c>");
  if(iData == CData_Desc)  return("$assault_desc$");
  if(iData == CData_Clonk) return(PCMK);
  if(iData == CData_Ammo)  return("90x {{STAM}} 8x {{GRAM}}");
  if(iData == CData_Items) return("1x {{ASTR}}     1x {{PSTL}}|1x {{DGNN}}     1x {{C4PA}}|2x {{STUN}}");
  if(iData == CData_Facet) return(1);
  return(Default(iData));
}

private func Class1Setup(int iPlr)
{
  var new = CreateObject(PCMK,0,0,iPlr);
  DoAmmo(STAM,90,new);
  DoAmmo(GRAM,8,new);
  new->CreateContents(ASTR);
  new->CreateContents(PSTL);
  new->CreateContents(DGNN);
  new->CreateContents(C4PA);
  CreateObject(STUN,0,0,iPlr)->Activate(new);
  CreateObject(STUN,0,0,iPlr)->Activate(new);
  CreateObject(PGRN,0,0,iPlr)->Activate(new);
  return(new);
}

/* Der Sanitäter */

private func Class2Info(int iData)
{
  //Hat explosives benötigt
  if(iData == CData_Name)  return("<c ffff33>$medic$</c>");
  if(iData == CData_Desc)  return("$medic_desc$");
  if(iData == CData_Clonk) return(MDIC);
  if(iData == CData_Ammo)  return("60x {{STAM}}");
  if(iData == CData_Items) return("1x {{SMGN}}     1x {{PSTL}}|1x {{CDBT}}     1x {{FAPK}}|2x {{FGRN}}     1x {{FRAG}}");
  if(iData == CData_Facet) return(2);
  return(Default(iData));
}

private func Class2Setup(int iPlr)
{
  var new = CreateObject(MDIC,0,0,iPlr);
  DoAmmo(STAM,60,new);
  new->CreateContents(SMGN);
  new->CreateContents(PSTL);
  new->CreateContents(CDBT);
  new->CreateContents(FAPK);
  CreateObject(FGRN,0,0,iPlr)->Activate(new);
  CreateObject(FGRN,0,0,iPlr)->Activate(new);
  CreateObject(FRAG,0,0,iPlr)->Activate(new);
  return(new);
}

/* Der Unterstützer */

private func Class3Info(int iData)
{
  //Wenn er schon so viel trägt, kann er einen Raketenwerfer für den explosiven Faktor tragen Oo
  if(iData == CData_Name)  return("<c ffff33>$support$</c>");
  if(iData == CData_Desc)  return("$support_desc$");
  if(iData == CData_Clonk) return(PCMK);
  if(iData == CData_Ammo)  return("500x {{STAM}} 5x{{MISS}}");
  if(iData == CData_Items) return("1x {{MNGN}}     1x {{PSTL}}|1x {{RTLR}}     --|2x {{SGRN}}     1x{{PGRN}}");
  if(iData == CData_Facet) return(3);
  return(Default(iData));
}

private func Class3Setup(int iPlr)
{
  var new = CreateObject(PCMK,0,0,iPlr);
  DoAmmo(STAM,500,new);
  DoAmmo(MIAM,10,new);
  new->CreateContents(MNGN);
  new->CreateContents(PSTL);
  new->CreateContents(RTLR);
  CreateObject(SGRN,0,0,iPlr)->Activate(new);
  CreateObject(SGRN,0,0,iPlr)->Activate(new);
  CreateObject(PGRN,0,0,iPlr)->Activate(new);
  return(new);
}

/* Der Antiskill */

private func Class4Info(int iData)
{
  //Etwas wenig Equip, da sein Schild so stark ist... ich hab ihm mal ne Clay gegeben
  if(iData == CData_Name)  return("<c ffff33>$antiskill$</c>");
  if(iData == CData_Desc)  return("$antiskill_desc$");
  if(iData == CData_Clonk) return(PCMK);
  if(iData == CData_Ammo)  return("70x {{STAM}}");
  if(iData == CData_Items) return("1x {{PPGN}}     1x {{PSTL}}|1x {{RSHL}}     1x {{BBTP}}|2x {{FRAG}}     1x{{STUN}}");
  if(iData == CData_Facet) return(4);
  return(Default(iData));
}

private func Class4Setup(int iPlr)
{
  var new = CreateObject(PCMK,0,0,iPlr);
  DoAmmo(STAM,70,new);
  new->CreateContents(PPGN);
  new->CreateContents(PSTL);
  new->CreateContents(RSHL);
  new->CreateContents(BBTP);
  CreateObject(FRAG,0,0,iPlr)->Activate(new);
  CreateObject(FRAG,0,0,iPlr)->Activate(new);
  CreateObject(STUN,0,0,iPlr)->Activate(new);
  return(new);
}

/* Der Artillerist */

private func Class5Info(int iData)
{
  //Auch noch eine Clay zur Selbstverteidigung
  if(iData == CData_Name)  return("<c ffff33>$artillerist$</c>");
  if(iData == CData_Desc)  return("$artillerist_desc$");
  if(iData == CData_Clonk) return(PCMK);
  if(iData == CData_Ammo)  return("50x {{STAM}}30x {{GRAM}}");
  if(iData == CData_Items) return("1x {{SGST}}     1x {{PSTL}}|1x {{DGNN}}     1x {{BBTP}}|2x {{SGRN}}     2x {{PGRN}}");
  if(iData == CData_Facet) return(5);
  return(Default(iData));
}

private func Class5Setup(int iPlr)
{
  var new = CreateObject(PCMK,0,0,iPlr);
  DoAmmo(STAM,50,new);
  DoAmmo(GRAM,30,new);
  new->CreateContents(SGST);
  new->CreateContents(PSTL);
  new->CreateContents(DGNN);
  new->CreateContents(BBTP);
  CreateObject(SGRN,0,0,iPlr)->Activate(new);
  CreateObject(SGRN,0,0,iPlr)->Activate(new);
  CreateObject(PGRN,0,0,iPlr)->Activate(new);
  CreateObject(PGRN,0,0,iPlr)->Activate(new);
  return(new);
}
