/*-- Klassen für den Klassenwähler --*/

#strict
#appendto MCSL


//Rifleman
private func Class1Info(int iData)
{
  if(iData == CData_Name)  return("$rifleman$");
  if(iData == CData_Desc)  return("$rifleman_desc$");
  if(iData == CData_Clonk) return(PCMK);
  if(iData == CData_Items) return("1x {{ASTR}}|1x {{92FS}}|2x {{FGRN}}");
  if(iData == CData_Facet) return(2);
  return(Default(iData));
}

private func Class1Setup(int iPlr)
{
  var new = CreateObject(PCMK,0,0,iPlr);
  DoAmmo(STAM,100,new);
  DoAmmo(GRAM,12,new);
  new->CreateContents(ASTR);
  new->CreateContents(92FS);
  CreateObject(FGRN,0,0,iPlr)->Activate(new);
  new->CreateContents(FGRN);
  return(new);
}

//Assault
private func Class2Info(int iData)
{
  if(iData == CData_Name)  return("$assault$");
  if(iData == CData_Desc)  return("$assault_desc$");
  if(iData == CData_Clonk) return(PCMK);
  if(iData == CData_Items) return("1x {{P29W}}|1x {{92FS}}|1x {{STUN}}");
  if(iData == CData_Facet) return(2);
  return(Default(iData));
}

private func Class2Setup(int iPlr)
{
  var new = CreateObject(PCMK,0,0,iPlr);
  DoAmmo(STAM,100,new);
  new->CreateContents(P29W);
  new->CreateContents(92FS);
  new->CreateContents(STUN);
  return(new);
}

//Peacemaker
private func Class3Info(int iData)
{

  if(iData == CData_Name)  return("$peacemaker$");
  if(iData == CData_Desc)  return("$peacemaker_desc$");
  if(iData == CData_Clonk) return(PCMK);
  if(iData == CData_Items) return("1x {{PPGN}}|1x {{RSHL}}|2x {{FRAG}}");
  if(iData == CData_Facet) return(10);
  return(Default(iData));
}

private func Class3Setup(int iPlr)
{
  var new = CreateObject(PCMK,0,0,iPlr);
  DoAmmo(STAM,100,new);
  new->CreateContents(PPGN);
  new->CreateContents(RSHL);
  CreateObject(FRAG,0,0,iPlr)->Activate(new);
  new->CreateContents(FRAG);
  return(new);
}

//Riotcrusher
private func Class4Info(int iData)
{
  if(iData == CData_Name)  return("$riotcrusher$");
  if(iData == CData_Desc)  return("$riotcrusher_desc$");
  if(iData == CData_Clonk) return(PCMK);
  if(iData == CData_Items) return("1x {{MP7R}}|1x {{RSHL}}|2x {{SGRN}}");
  if(iData == CData_Facet) return(5);
  return(Default(iData));
}

private func Class4Setup(int iPlr)
{
  var new = CreateObject(PCMK,0,0,iPlr);
  DoAmmo(STAM,100,new);
  new->CreateContents(MP7R);
  new->CreateContents(RSHL);
  CreateObject(SGRN,0,0,iPlr)->Activate(new);
  new->CreateContents(SGRN);
  return(new);
}

//Support
private func Class5Info(int iData)
{

  if(iData == CData_Name)  return("$support$");
  if(iData == CData_Desc)  return("$support_desc$");
  if(iData == CData_Clonk) return(PCMK);
  if(iData == CData_Items) return("1x {{M249}}|1x {{PDTW}}|2x {{SGRN}}");
  if(iData == CData_Facet) return(8);
  return(Default(iData));
}

private func Class5Setup(int iPlr)
{
  var new = CreateObject(PCMK,0,0,iPlr);
  DoAmmo(STAM,200,new);
  new->CreateContents(M249);
  new->CreateContents(PDTW);
  CreateObject(SGRN,0,0,iPlr)->Activate(new);
  new->CreateContents(SGRN);
  return(new);
}

//Medic
private func Class6Info(int iData)
{

  if(iData == CData_Name)  return("$medic$");
  if(iData == CData_Desc)  return("$medic_desc$");
  if(iData == CData_Clonk) return(MDIC);
  if(iData == CData_Items) return("1x {{P29W}}|1x {{PDTW}}|1x {{FAPK}}|2x {{STUN}}");
  if(iData == CData_Facet) return(1);
  return(Default(iData));
}

private func Class6Setup(int iPlr)
{
  var new = CreateObject(MDIC,0,0,iPlr);
  DoAmmo(STAM,50,new);
  new->CreateContents(P29W);
  new->CreateContents(PDTW);
  new->CreateContents(FAPK);
  CreateObject(STUN,0,0,iPlr)->Activate(new);
  new->CreateContents(STUN);
  return(new);
}

//Infantryman
private func Class7Info(int iData)
{
  if(iData == CData_Name)  return("$infantryman$");
  if(iData == CData_Desc)  return("$infantryman_desc$");
  if(iData == CData_Clonk) return(PCMK);
  if(iData == CData_Items) return("1x {{DEGL}}|1x {{FAPK}}|4x {{FGRN}}");
  if(iData == CData_Facet) return(3);
  return(Default(iData));
}

private func Class7Setup(int iPlr)
{
  var new = CreateObject(PCMK,0,0,iPlr);
  DoAmmo(STAM,100,new);
  new->CreateContents(DEGL);
  new->CreateContents(FAPK);
  CreateObject(FGRN,0,0,iPlr)->Activate(new);
  CreateObject(FGRN,0,0,iPlr)->Activate(new);
  CreateObject(FGRN,0,0,iPlr)->Activate(new);
  new->CreateContents(FGRN);
  return(new);
}

//Artillerist
private func Class8Info(int iData)
{
  if(iData == CData_Name)  return("$artillerist$");
  if(iData == CData_Desc)  return("$artillerist_desc$");
  if(iData == CData_Clonk) return(PCMK);
  if(iData == CData_Items) return("1x {{SGST}}|1x {{92FS}}");
  if(iData == CData_Facet) return(4);
  return(Default(iData));
}

private func Class8Setup(int iPlr)
{
  var new = CreateObject(PCMK,0,0,iPlr);
  DoAmmo(STAM,30,new);
  DoAmmo(GRAM,40,new);
  new->CreateContents(SGST);
  new->CreateContents(92FS);
  return(new);
}