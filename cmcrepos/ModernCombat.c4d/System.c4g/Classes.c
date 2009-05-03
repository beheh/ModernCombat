/*-- Klassen für den Klassenwähler --*/
//Temporäre Version.

#strict
#appendto MCSL


//Rifleman
private func Class1Info(int iData)
{
  if(iData == CData_Name)  return("$rifleman$");
  if(iData == CData_Desc)  return("$rifleman_desc$");
  if(iData == CData_Clonk) return(PCMK);
  if(iData == CData_Items) return("1x {{M16A}}|1x {{92FS}}|2x {{FGRN}}");
  if(iData == CData_Facet) return(2);
  return(Default(iData));
}

private func Class1Setup(int iPlr)
{
  var new = CreateObject(PCMK,0,0,iPlr);
  DoAmmo(STAM,100,new);
  new->CreateContents(M16A);
  new->CreateContents(92FS);
  CreateObject(FGRN,0,0,iPlr)->Activate(new);
  CreateObject(FGRN,0,0,iPlr)->Activate(new);
  return(new);
}

//Assault
private func Class2Info(int iData)
{
  if(iData == CData_Name)  return("$assault$");
  if(iData == CData_Desc)  return("$assault_desc$");
  if(iData == CData_Clonk) return(PCMK);
  if(iData == CData_Items) return("1x {{FP90}}|1x {{92FS}}|1x {{STUN}}");
  if(iData == CData_Facet) return(2);
  return(Default(iData));
}

private func Class2Setup(int iPlr)
{
  var new = CreateObject(PCMK,0,0,iPlr);
  DoAmmo(STAM,100,new);
  new->CreateContents(FP90);
  new->CreateContents(92FS);
  CreateObject(STUN,0,0,iPlr)->Activate(new);
  return(new);
}

//CloseCombat
private func Class3Info(int iData)
{
  if(iData == CData_Name)  return("$closecombat$");
  if(iData == CData_Desc)  return("$closecombat_desc$");
  if(iData == CData_Clonk) return(PCMK);
  if(iData == CData_Items) return("1x {{SPAS}}|1x {{92FS}}|1x {{FRAG}}");
  if(iData == CData_Facet) return(2);
  return(Default(iData));
}

private func Class3Setup(int iPlr)
{
  var new = CreateObject(PCMK,0,0,iPlr);
  DoAmmo(STAM,100,new);
  new->CreateContents(SPAS);
  new->CreateContents(92FS);
  CreateObject(FRAG,0,0,iPlr)->Activate(new);
  return(new);
}

//Riotcrusher
private func Class4Info(int iData)
{
  if(iData == CData_Name)  return("$Riotcrusher$");
  if(iData == CData_Desc)  return("$Riotcrusher_desc$");
  if(iData == CData_Clonk) return(PCMK);
  if(iData == CData_Items) return("1x {{MP7R}}|1x {{RSHL}}|2x {{SGRN}}");
  if(iData == CData_Facet) return(2);
  return(Default(iData));
}

private func Class4Setup(int iPlr)
{
  var new = CreateObject(PCMK,0,0,iPlr);
  DoAmmo(STAM,100,new);
  new->CreateContents(MP7R);
  new->CreateContents(RSHL);
  CreateObject(SGRN,0,0,iPlr)->Activate(new);
  CreateObject(SGRN,0,0,iPlr)->Activate(new);
  return(new);
}

//Support
private func Class5Info(int iData)
{

  /*if(iData == CData_Name)  return("$support$");
  if(iData == CData_Desc)  return("$support_desc$");
  if(iData == CData_Clonk) return(PCMK);
  if(iData == CData_Items) return("1x {{M249}}|1x {{92FS}}");
  if(iData == CData_Facet) return(2);
  return(Default(iData));*/
}

private func Class5Setup(int iPlr)
{
  /*var new = CreateObject(PCMK,0,0,iPlr);
  DoAmmo(STAM,200,new);
  new->CreateContents(M249);
  new->CreateContents(92FS);
  return(new);*/
}