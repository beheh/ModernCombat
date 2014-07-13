/*-- Raffinerie --*/

#strict 2
#include CCBS

public func TechLevel()			{return 2;}	//Techstufe
public func BuildingRadius()		{return 200;}	//Bauradius
public func RequiredEnergy() 		{return 50;}	//Energieverbraucher
public func BasementID()		{return BT02;}	//Fundament

public func ProcessingInterval()	{return 2;}	//Intervall der Verarbeitung


/* Initalisierung */

protected func Initialize()
{
  //Techlevel anheben
  if(GetTeamTechLevel(GetPlayerTeam(GetOwner())) == 2)
    SetTeamTechLevel(GetPlayerTeam(GetOwner()), 3);

  //Effekte
  if(HasEnergy())
    Sound("Building_PowerOn.ogg");
  else
    Sound("Building_PowerOff.ogg");

  //Ressourcenverarbeitung
  AddEffect("ProcessingResource", this, 1, ProcessingInterval(), this);

  return _inherited(...);
}

/* Menüführung */

public func AdditionalStatusMenu(object pMenuObj)
{
  var process_rate;
  for(var refinery in FindObjects(Find_Func("IsProcessing")))
  {
    process_rate += 36*1000/(refinery->ProcessingInterval()*(4*!HasEnergy()+1));
  }

  //Bei Ressourcenverarbeitung entsprechende Informationen einblenden
  if(IsProcessing())
  {
    AddMenuItem(Format("$ResourceProcessInfo$", GetTeamResources(GetPlayerTeam(GetOwner()))), 0, 0, pMenuObj);
    AddMenuItem(Format("$ResourceProcessRate$", process_rate/1000, process_rate%1000), 0, 0, pMenuObj);    
  }
  else
    AddMenuItem("$ResourceProcessInactive$", 0, GetID(), pMenuObj);

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
  //Nicht fertiggestellte Raffinerien können noch nichts verarbeiten
  if(GetCon() < 100)
    return false;
  //Keine Lebewesen oder Fahrzeuge verarbeiten
  if(!(GetCategory(pObj) & C4D_Object))
    return false;
  //Als Ausnahmen markierte Objekte auslassen
  if(pObj->~RejectProcessing())
    return false;

  return true;
}

/* Material in Ressourcen umwandeln */

//data darf object/int/id sein (bzw. array aus object/int/id)
public func Process(data, bool fNoRemove)
{
  //Debug: Falsche Datentypen melden
  var type = GetType(data);
  if(type && type != C4V_C4Object && type != C4V_C4ID && type != C4V_Int && type != C4V_Array)
    return FatalError(Format("Got wrong datatype! (%d) Only object, id, int or array are supported!"));

  //Wert des Materials ermitteln
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

  //Ressourcen gutschreiben
  DoTeamResources(GetPlayerTeam(GetOwner()), value);

  return value;
}

/* Ressourcenverarbeitung zu Credits */

public func IsProcessing()	{return EffectVar(0, this, GetEffect("ProcessingResource", this));}

public func FxProcessingResourceTimer(object pTarget, int iNr)
{
  //Bei fehlender Stromversorgung verlangsamte Produktion
  if(!HasEnergy())
  {
    EffectVar(1, pTarget, iNr)++;
    if(EffectVar(1, pTarget, iNr) % 5)
      return true;
  }

  //Besitzer und Ressourcen ermitteln
  var team = GetPlayerTeam(GetOwner(pTarget));
  var amount = GetTeamResources(team);

  //Ressourcen vorhanden: In Credits umwandeln
  if(!amount)
  {
    EffectVar(0, pTarget, iNr) = false;
    return true;
  }
  else
  {
    EffectVar(0, pTarget, iNr) = true;
    //Ressourcen abziehen
    DoTeamResources(team, -1);
    //Credits gutschreiben
    SetWealth(GetOwner(pTarget), GetWealth(GetOwner(pTarget))+1);
    //Effekt
    Sound("CRFY_Credits.ogg",true,0,20,GetOwner()+1);
  }

  return true;
}