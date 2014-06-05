/*-- Raffinerie --*/

#strict 2
#include CCBS

local basement;	//Fundament

public func TechLevel()		{return 2;}	//Techstufe
public func BuildingRadius()	{return 200;}	//Bauradius
public func RequiredEnergy() 	{return 50;}	//Energieverbraucher


/* Konstruktion */

protected func Construction()
{
  basement = CreateObject(BT03,0,8,GetOwner());
}

/* Initalisierung */

protected func Initialize()
{
  Sound("CCBS_PowerUp.ogg");
  
  //Techlevel 3 freischalten
  if(GetTeamTechLevel(GetPlayerTeam(GetOwner())) == 2)
  	SetTeamTechLevel(GetPlayerTeam(GetOwner()), 3);

  return _inherited(...);
}

public func Collection2(object pObj)
{
	//Verarbeitung von Ressourcen beginnen
	if(GetID(pObj) == RSCE)
		if(!IsProcessing())
			ProcessingResources();
	
	return _inherited(pObj, ...);
}

public func AddWare(int iValue, int iDuration)
{
	var resource = CreateObject(RSCE, 0, 0, NO_OWNER);
	resource->Set(iValue, iDuration);
	Enter(this, resource);
}

public func AdditionalBuildingMenu(object pMenuObj)
{
	if(IsProcessing())
		AddMenuItem(Format("$ResourceProcessInfo$", GetProcessProgress(), GetProcessDuration()), 0, GetID(), pMenuObj);
	else
		AddMenuItem("$ResourceProcessInactive$", 0, GetID(), pMenuObj);
	return true;
}

public func IsProcessing() { return GetEffect("ProcessResource", this); }
public func GetProcessProgress() { return GetEffect("ProcessingDuration", this, 0, 6); }
public func GetProcessDuration() { return GetEffect("ProcessingDuration", this, 0, 3); } 

public func ProcessingResources()
{
	//Keine Ressourcen gefunden
	if(!FindContents(RSCE, this))
	{
		if(IsProcessing())
			RemoveEffect("ProcessResource", this);

		return;
	}

	//Höherer Wert -> Höhere Priorität
	var highest;
	for(var resource in FindObjects(Find_Container(this), Find_ID(RSCE)))
	{
		if(!highest)
			highest = resource;
		else if(highest->~GetVal() < resource->~GetVal())
			highest = resource;
	}
	
	if(!highest)
		return;

	if(!IsProcessing())
		AddEffect("ProcessResource", this, 1, 0, this);

	AddEffect("ProcessingDuration", this, 1, highest->GetDuration(), this, 0, highest->GetVal());
	
	if(highest)
		RemoveObject(highest);
}

public func FxProcessingDurationStart(object pTarget, int iNr, int iTemp, int iValue)
{
	if(iTemp)
		return;

	EffectVar(0, pTarget, iNr) = iValue;
}

public func FxProcessingDurationStop(object pTarget, int iNr)
{
	//Geld vergeben
	SetWealth(GetOwner(), GetWealth(GetOwner())+EffectVar(0, pTarget, iNr));

	//Weitermachen
	ScheduleCall(this, "ProcessingResources", 1);

	return;
}
