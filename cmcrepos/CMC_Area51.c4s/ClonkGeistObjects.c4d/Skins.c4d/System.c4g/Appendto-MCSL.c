/*-- Skinimplementation --*/

#strict 2

#appendto MCSL

static const CData_Skins = 12;

public func SetupClass(int iClass, int iPlr)
{
  //Geht nicht
  if(!GetCData(iClass, CData_Condition, iPlr))
    return;

  var oldCrew = crew[iPlr];

  //Neuer Clonk
  var pCrew = crew[iPlr] = CreateObject(GetCData(iClass, CData_Clonk, iPlr), 0, 0, iPlr);

  if(Contained(oldCrew))
  {
    var tmp = Contained(oldCrew);
    SetGraphics(0, tmp, GetID(pCrew), 1, GFXOV_MODE_Object, 0, 1, pCrew);
    Enter(tmp, pCrew);
  }

  //Infosektion holen
  if(GetID(oldCrew) == GetID(pCrew))
    GrabObjectInfo(oldCrew, pCrew);
  else
    MakeCrewMember(pCrew, iPlr);
  SilentKill4K(oldCrew);

  //Auswählen
  SelectCrew(iPlr, pCrew, 1);
  SetPlrView(iPlr, pCrew);
  SetCursor(iPlr, pCrew, true, false); 

  //Ausrüsten...
  //Munition
  if (!FindObject(NOAM))
  {
    var aAmmo = GetCData(iClass, CData_Ammo);
    for (var aEntry in aAmmo)
      if (GetType(aEntry) == C4V_Array && GetType(aEntry[0]) == C4V_C4ID && aEntry[0]->~IsAmmo())
        DoAmmo(aEntry[0], aEntry[1], pCrew);
  }

  //Gegenstände
  var aItems = GetCData(iClass, CData_Items);
  for (var aEntry in aItems)
    if (GetType(aEntry) == C4V_Array && GetType(aEntry[0]) == C4V_C4ID)
      CreateContents(aEntry[0], pCrew, aEntry[1]);

  //Granaten
  var aGrenades = GetCData(iClass, CData_Grenades);
  for (var aEntry in aGrenades)
    if (GetType(aEntry) == C4V_Array && GetType(aEntry[0]) == C4V_C4ID && aEntry[0]->~IsGrenade())
      while (aEntry[1]--)
        CreateObject(aEntry[0], 0, 0, iPlr)->~Activate(pCrew);

  //Ausrüstung
  var aGear = GetCData(iClass, CData_Gear);
  var aAdditionalGear = GameCall("SpecificEquipment");

  if(GetType(aAdditionalGear) == C4V_Array)
    AddArray4K(aAdditionalGear, aGear);

  if(GetDarkness() >= 3)
    aGear[GetLength(aGear)] = [FLSH, 1];

  //Bei Fallschaden Fallschirme als Zusatzausrüstung
  if(FindObject(FDMG))
    aGear[GetLength(aGear)] = [PPAR, 1];

  var aGearTypes = [];

  for(var aEntry in aGear)
  {
    if(!aEntry[0])
      continue;

    if(GetIndexOf(aEntry[0]->~GetGearType(), aGearTypes) > -1)
      continue;

    aGearTypes[GetLength(aGearTypes)] = aEntry[0]->~GetGearType();

    if(GetType(aEntry) == C4V_Array && GetType(aEntry[0]) == C4V_C4ID && aEntry[0]->~IsHazardGear())
      while(aEntry[1]--)
        CreateObject(aEntry[0], 0, 0, iPlr)->~Activate(pCrew);
  }
  
  var aSkins = GetCData(iClass, CData_Skins);
  if(GetLength(aSkins))
  {
  	var skin = aSkins[Random(GetLength(aSkins))];
  	SetGraphics(skin[1], pCrew, skin[0]);
  	
  	if(skin[2])
  		SetPortrait(Format("Ptr%s%d", skin[1], Random(skin[2])+1), pCrew, skin[0]);
  }

  //Nachricht
  var szAction = Format("%d", GetCData(iClass, CData_Facet));
  if (!GetActMapVal("Name", szAction, GetCData(iClass, CData_Icon)))
    szAction = 0;
  for(var i = 0; i < GetPlayerCount(); i++)
    if(GetPlayerTeam(i) == GetPlayerTeam(iPlr))
      EventInfo4K(i+1, Format("$PlayerChoosedClass$", GetTaggedPlayerName(iPlr), GetCData(iClass, CData_Name)), GetCData(iClass, CData_Icon), RGB(220, 220, 220), 0, 0, 0, szAction);

  //Speichern
  lastclass[iPlr] = iClass;

  Finish(pCrew, iClass);

  return true;
}

private func Default(int iData)
{
	if(iData == CData_Skins)  return [];
	return _inherited(iData);
}
