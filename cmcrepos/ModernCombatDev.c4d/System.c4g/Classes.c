/*-- Klassen für den Klassenwähler --*/
//Temporäre Version.

#strict 2
#appendto MCSL


//Rifleman
private func Class1Info(int iData)
{
  if(iData == CData_Name)  return "$rifleman$";
  if(iData == CData_Desc)  return "$rifleman_desc$";
  if(iData == CData_Clonk) return PCMK;
  if(iData == CData_Items) return "1x {{M16A}}|1x {{92FS}}|2x {{FGRN}}";
  if(iData == CData_Facet) return 2;
  return Default(iData);
}

private func Class1Setup(int iPlr)
{
  var new = CreateObject(PCMK,0,0,iPlr);
  DoAmmo(STAM,100,new);
  new->CreateContents(M16A);
  new->CreateContents(92FS);
  CreateObject(FGRN,0,0,iPlr)->Activate(new);
  CreateObject(FGRN,0,0,iPlr)->Activate(new);
  return new;
}

//Assault
private func Class2Info(int iData)
{
  if(iData == CData_Name)  return "$assault$";
  if(iData == CData_Desc)  return "$assault_desc$";
  if(iData == CData_Clonk) return PCMK;
  if(iData == CData_Items) return "1x {{FP90}}|1x {{92FS}}|1x {{STUN}}";
  if(iData == CData_Facet) return 2;
  return Default(iData);
}

private func Class2Setup(int iPlr)
{
  var new = CreateObject(PCMK,0,0,iPlr);
  DoAmmo(STAM,100,new);
  new->CreateContents(FP90);
  new->CreateContents(92FS);
  CreateObject(STUN,0,0,iPlr)->Activate(new);
  return new;
}

//CloseCombat
private func Class3Info(int iData)
{
  if(iData == CData_Name)  return "$closecombat$";
  if(iData == CData_Desc)  return "$closecombat_desc$";
  if(iData == CData_Clonk) return PCMK;
  if(iData == CData_Items) return "1x {{SPAS}}|1x {{92FS}}|1x {{FRAG}}";
  if(iData == CData_Facet) return 9;
  return Default(iData);
}

private func Class3Setup(int iPlr)
{
  var new = CreateObject(PCMK,0,0,iPlr);
  DoAmmo(STAM,100,new);
  new->CreateContents(SPAS);
  new->CreateContents(92FS);
  CreateObject(FRAG,0,0,iPlr)->Activate(new);
  return new;
}

//Riotcrusher
private func Class4Info(int iData)
{
  if(iData == CData_Name)  return "$Riotcrusher$";
  if(iData == CData_Desc)  return "$Riotcrusher_desc$";
  if(iData == CData_Clonk) return PCMK;
  if(iData == CData_Items) return "1x {{MP7R}}|1x {{RSHL}}|2x {{SGRN}}";
  if(iData == CData_Facet) return 5;
  return Default(iData);
}

private func Class4Setup(int iPlr)
{
  var new = CreateObject(PCMK,0,0,iPlr);
  DoAmmo(STAM,100,new);
  new->CreateContents(MP7R);
  new->CreateContents(RSHL);
  CreateObject(SGRN,0,0,iPlr)->Activate(new);
  CreateObject(SGRN,0,0,iPlr)->Activate(new);
  return new;
}

//Support
private func Class5Info(int iData)
{

  if(iData == CData_Name)  return "$support$";
  if(iData == CData_Desc)  return "$support_desc$";
  if(iData == CData_Clonk) return PCMK;
  if(iData == CData_Items) return "1x {{M249}}|1x {{FRAG}}|1x {{SGRN}}";
  if(iData == CData_Facet) return 8;
  return Default(iData);
}

private func Class5Setup(int iPlr)
{
  var new = CreateObject(PCMK,0,0,iPlr);
  DoAmmo(STAM,200,new);
  new->CreateContents(M249);
  CreateObject(FRAG,0,0,iPlr)->Activate(new);
  CreateObject(SGRN,0,0,iPlr)->Activate(new);
  return new;
}

//Peacemaker
private func Class6Info(int iData)
{

  if(iData == CData_Name)  return "$peacemaker$";
  if(iData == CData_Desc)  return "$peacemaker_desc$";
  if(iData == CData_Clonk) return PCMK;
  if(iData == CData_Items) return "1x {{PPGN}}|1x {{RSHL}}|2x {{FRAG}}";
  if(iData == CData_Facet) return 10;
  return Default(iData);
}

private func Class6Setup(int iPlr)
{
  var new = CreateObject(PCMK,0,0,iPlr);
  DoAmmo(STAM,100,new);
  new->CreateContents(PPGN);
  new->CreateContents(RSHL);
  CreateObject(FRAG,0,0,iPlr)->Activate(new);
  CreateObject(FRAG,0,0,iPlr)->Activate(new);
  return new;
}

//Medic
private func Class7Info(int iData)
{

  if(iData == CData_Name)  return "$medic$";
  if(iData == CData_Desc)  return "$medic_desc$";
  if(iData == CData_Clonk) return MDIC;
  if(iData == CData_Items) return "1x {{P29W}}|1x {{PDTW}}|1x {{FAPK}}|2x {{STUN}}";
  if(iData == CData_Facet) return 1;
  return Default(iData);
}

private func Class7Setup(int iPlr)
{
  var new = CreateObject(PCMK,0,0,iPlr);
  DoAmmo(STAM,50,new);
  new->CreateContents(P29W);
  new->CreateContents(PDTW);
  new->CreateContents(FAPK);
  CreateObject(STUN,0,0,iPlr)->Activate(new);
  CreateObject(STUN,0,0,iPlr)->Activate(new);
  return new;
}
