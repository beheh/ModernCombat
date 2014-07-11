/*-- Raffinerie --*/

#strict 2
#include CCBS

local basement;	//Fundament

public func TechLevel()		{return 2;}	//Techstufe
public func BuildingRadius()	{return 200;}	//Bauradius
public func RequiredEnergy() 	{return 50;}	//Energieverbraucher
public func ProcessingInterval() {return 3;} //Intervall der Verarbeitung

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
  
  AddEffect("ProcessingResource", this, 1, ProcessingInterval(), this);

  return _inherited(...);
}

/* Menüführung */

public func AdditionalBuildingMenu(object pMenuObj)
{
  /*if(IsProcessing())
    AddMenuItem(Format("$ResourceProcessInfo$", GetProcessProgress(), GetProcessDuration()), 0, GetID(), pMenuObj);
  else
    AddMenuItem("$ResourceProcessInactive$", 0, GetID(), pMenuObj);*/
  return true;
}

/* Aufnahme von Objekten */

public func Collection2(object pObj)
{
  //Verarbeitung von Ressourcen beginnen
  if(CanProcess(pObj))
  	Process(pObj);

  return _inherited(pObj, ...);
}

public func CanProcess(object pObj)
{
  //Keine Lebewesen, Fahrzeuge und Co. verarbeiten
	if(!(GetCategory(pObj) & C4D_Object))
		return false;
	//Verarbeitung Objektseitig verboten
	if(pObj->~RejectProcessing())
		return false;

	return true;
}

//data darf object/int/id sein (bzw. array aus object/int/id)
public func Process(data, bool fNoRemove)
{
  //Datentyp past?
	var type = GetType(data);
	if(type != C4V_C4Object && type != C4V_C4ID && type != C4V_Int && type != C4V_Array)
		return FatalError(Format("Got wrong datatype! (%d) Only object, id, int or array are supported!"));

  var value = 0;
  if(type == C4V_C4Object)
  {
  	value = GetValue(data);
  	if(!fNoRemove)
  	  RemoveObject(data);
  }
  else if(type == C4V_C4ID)
  	value = GetValue(0, data);
  else if(type == C4V_Int)
  	value = data;
  else if(type == C4V_Array)
  {
  	for(var entry in data)
  	  value += Process(entry);
  }

  DoTeamResources(GetPlayerTeam(GetOwner()), value);

  return value;
}

/* Raffination */

public func IsProcessing() { return EffectVar(0, this, GetEffect("ProcessingResource", this)); }

public func FxProcessingResourceTimer(object pTarget, int iNr)
{
	var team = GetPlayerTeam(GetOwner(pTarget));
	var amount = GetTeamResources(team);
	
	if(!amount)
	{
		EffectVar(0, pTarget, iNr) = false;
		return true;
	}
	else
	{
		EffectVar(0, pTarget, iNr) = true;
	  DoTeamResources(team, -1);
	  SetWealth(GetOwner(pTarget), GetWealth(GetOwner(pTarget))+1);
	}

  return true;
}

/*public func IsProcessing()		{ return GetEffect("ProcessResource", this); }
public func GetProcessProgress()	{ return GetEffect("ProcessingDuration", this, 0, 6); }
public func GetProcessDuration()	{ return GetEffect("ProcessingDuration", this, 0, 3); } 

public func ProcessingResources()
{
  //Keine Ressourcen gefunden
  if(!FindContents(RSCE, this))
  {
    if(IsProcessing())
      RemoveEffect("ProcessResource", this);

    return;
  }

  //Höherer Wert: Höhere Priorität
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
}*/
