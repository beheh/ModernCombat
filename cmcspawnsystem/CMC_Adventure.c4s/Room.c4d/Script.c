/*-- Raum --*/

#strict 2
#include DOOR


/* Sounds */

private func SoundOpenDoor()
{
  Sound("DOOR_Open.ogg");
}

private func SoundCloseDoor()
{
  Sound("DOOR_Close.ogg");
}

func Collection2(object pObj)
{
	if(!aSectionIDs)
  	g_TutorialObject->ExitArrived(pObj);
  else
  	CreateSectionMenu(pObj);
  
  return _inherited(...);
}

local aSectionIDs;

public func AddSectionID(id idSection)
{
	if(!aSectionIDs)
		aSectionIDs = [];
	
	aSectionIDs[GetLength(aSectionIDs)] = idSection;
	return true;
}

public func CreateSectionMenu(object pByObj)
{
	if(!pByObj)
		return;
	if(GetMenu(pByObj))
		CloseMenu(pByObj);

	CreateMenu(CSTR, pByObj, this, C4MN_Extra_None, "Tutorialwahl", 0, C4MN_Style_Context);

	for(var sect in aSectionIDs)
	{
		if(!sect)
			continue;
	
		if(!sect->~HideInMenu())
		{
			if(!sect->Condition(pByObj))
				AddMenuItem(Format("<c 777777>%s</c>", GetName(0, sect)), 0, sect, pByObj);
			else
				AddMenuItem(GetName(0, sect), Format("LoadTutorial(%i, Object(%d))", sect, ObjectNumber(pByObj)), sect, pByObj, 0, 0, GetDesc(0, sect));
		}
	}
	AddMenuItem("$Cancel$", Format("CloseMenu(Object(%d))", ObjectNumber(pByObj)), CHOS, pByObj);
	OnMenuSelection(0, pByObj);
}

public func Ejection(object pObj)
{
	MenuQueryCancel(0, pObj);
}

public func MenuQueryCancel(int iSelection, object pMenuObj)
{
	CustomMessage("", 0, GetOwner(pMenuObj));
}

public func OnMenuSelection(int iItemIndex, object pMenuObj)
{
	var sect = aSectionIDs[iItemIndex];
	
	if(sect)
		CustomMessage(GetDesc(0, sect), 0, GetOwner(pMenuObj), 100, 100, 0xFFFFFF, DECO, Format("%i", sect));
	else
		CustomMessage("", 0, GetOwner(pMenuObj));
}

public func LoadTutorial() { return GameCall("LoadTutorial", ...); }

