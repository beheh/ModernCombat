/*-- Raffinerie --*/

#strict 2
#include CCBS
#include BT02

local aWaypointX, aWaypointY, aMAVProgress, aMAVs, controlling, fReturning;

public func TechLevel()			{return TECHLEVEL_2;}	//Ben�tigte Techstufe
public func ProvideTechLevel()		{return TECHLEVEL_3;}	//Vorhandene Techstufe
public func BuildingRadius()		{return 300;}		//Bauradius
public func RequiredEnergy()		{return 50;}		//Energieverbraucher
public func MaxDamage()			{return 150;}		//Maximaler Schadenswert bis zur Zerst�rung
public func PossibleUpgrades()		{return [U_SB];}	//M�gliche Upgrades

public func ProcessingInterval()	{return 2;}		//Intervall der Verarbeitung


/* Initalisierung */

protected func Initialize()
{
  //Effekte
  AddEffect("SmokeEffects", this, 100, 26, this);
  if(HasEnergy())
    Sound("Building_PowerOn.ogg");
  else
    Sound("Building_PowerOff.ogg");

  //Ressourcenverarbeitung
  AddEffect("ProcessingResource", this, 1, ProcessingInterval(), this);

  //Sammler-MAV
  aWaypointX = CreateArray();
  aWaypointY = CreateArray();
  aMAVProgress = CreateArray();
  aMAVs = CreateArray();

  aWaypointX[0] = GetX() + GetDefCoreVal("Collection", "DefCore", 0, 0) + GetDefCoreVal("Collection", "DefCore", 0, 2)/2;
  aWaypointY[0] = GetY() + GetDefCoreVal("Collection", "DefCore", 0, 1) + GetDefCoreVal("Collection", "DefCore", 0, 3)/2 + GetDefCoreVal("Offset", "DefCore", CMAV, 1) - 20;

  aMAVs[0] = CreateObject(CMAV, aWaypointX[0] - GetX(), aWaypointY[0] - GetY(), GetOwner(this));
  aMAVs[0]->Start(this);
  controlling = -1;

  return _inherited(...);
}

/* Effekte */

public func FxSmokeEffectsTimer()
{
  Smoke();
  return true;
}

private func Smoke()
{
  //Effekte
  CastSmoke("Smoke",3,10,40,-35,100,500);
}

/* Men�f�hrung */

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

public func AdditionalBuildingMenu(object pMenuObj)
{
  AddMenuItem("MAV steuern", "ControlMAV", CMAV, pMenuObj, 0, pMenuObj);
  if(!fReturning)
    AddMenuItem("MAVs zur�ckholen", "ReturnMAV", CMAV, pMenuObj, 0, pMenuObj);
  else
    AddMenuItem("MAVs starten", "StartMAV", CMAV, pMenuObj, 0, pMenuObj);

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
  //Nicht fertiggestellte Raffinerien k�nnen noch nichts verarbeiten
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

func StartMAV(id foo, object pByObj)
{
	fReturning = false;
	for(var i = 0; i < GetLength(aMAVs); i++)
  {
  	if(i == controlling || !aMAVs[i] || aMAVs[i]->IsDestroyed() || aMAVs[i]->HasMoveOrder())
  		continue;
  		
  	var closest = 0;
 		for(var j = 0; j < GetLength(aWaypointX); j++)
 		{
 			if(Distance(GetX(aMAVs[i]), GetY(aMAVs[i]), aWaypointX[j], aWaypointY[j]) < Distance(GetX(aMAVs[i]), GetY(aMAVs[i]), aWaypointX[closest], aWaypointY[closest])
 			&& PathFree(GetX(aMAVs[i]), GetY(aMAVs[i]), aWaypointX[j], aWaypointY[j]))
 				closest = j;
 		}
 		
 		if(aMAVs[i]->GetAction() != "Flying")
 			aMAVs[i]->Start(this);
 		
 		aMAVProgress[i] = closest;
 		aMAVs[i]->MoveTo(aWaypointX[closest], aWaypointY[closest]);
 	}
  return true;
}

func ReturnMAV(id foo, object pByObj)
{
	fReturning = true;
	for(var i = 0; i < GetLength(aMAVs); i++)
			if(aMAVs[i] && !aMAVs[i]->IsDestroyed())
			{
				aMAVProgress[i] = -Abs(aMAVProgress[i]);
				aMAVs[i]->MoveTo(aWaypointX[Abs(aMAVProgress[i])], aWaypointY[Abs(aMAVProgress[i])]);
			}
}

func ControlMAV(id foo, object pByObj)
{
	if(controlling < 0 || !aMAVs[controlling] || aMAVs[controlling]->IsDestroyed())
	{
		controlling = -1;
		for(var i = 0; i < GetLength(aMAVs); i++)
			if(aMAVs[i] && !aMAVs[i]->IsDestroyed())
			{
				controlling = i;
				aMAVProgress[i] = 0;
				aMAVs[i]->DeleteMoveOrder();
				break;
			}
		if(controlling < 0)
		{
			PlayerMessage(GetOwner(pByObj), "$NoMAVFound$", aMAVs[controlling]);
			return;
		}
	}
	
  //MAV-Kontrolle starten
  Sound("BKHK_Switch.ogg", true, this, 100, GetOwner(pByObj) + 1);
  Sound("CockpitRadio.ogg", true, 0, 100, GetOwner(pByObj)+1, +1);
  aMAVs[controlling]->Start(this);
  SetPlrView(GetController(pByObj), aMAVs[controlling]);
  AddEffect("ControllingMAV", this, 1, 1);
}

public func FxControllingMAVTimer(object pTarget, int iNr)
{Log("hallo"); //hi!
	if(controlling == -1)
		return -1;
		
	SetPlrView(GetOwner(Contents()), aMAVs[controlling]);
}

func NextWaypoint(object pMAV)
{
	for(var i = 0; i < GetLength(aMAVs); i++)
	{
		if(i == controlling || aMAVs[i] != pMAV)
			continue;
		
		if(aMAVProgress[i] == - GetLength(aWaypointX) - 1)
		{
			aMAVProgress[i]++;
			return;
		}
		
		if(aMAVProgress[i] == 0)
		{
			if(Distance(GetX(pMAV), GetY(pMAV), GetX(), GetY()) < 100)
			{
				if(pMAV->Carrying())
					pMAV->Drop();
			}
			else
			{
				pMAV->MoveTo(aWaypointX[Abs(aMAVProgress[i])], aWaypointY[Abs(aMAVProgress[i])]);
				return;
			}
			if(fReturning) return;
		}
		
		//Distance(GetX(), GetY(), GetX(FindObject2(Find_Func("IsSupplyStock"), Find_Distance(1000, GetX(0), GetY(0)))), GetY(FindObject2(Find_Func("IsSupplyStock"), Find_Distance(1000, GetX(0), GetY(0)))))
		aMAVProgress[i]++;
		if(aMAVProgress[i] == GetLength(aWaypointX))
		{
			aMAVProgress[i] = -GetLength(aWaypointX) - 1;
			var supply = FindObject2(Find_Func("IsSupplyStock"), Find_Distance(100, GetX(pMAV)-GetX(), GetY(pMAV)-GetY()), Sort_Distance(GetX(pMAV)-GetX(), GetX(pMAV)-GetX()));
			if(supply)
			{
				pMAV->MoveTo(GetX(supply), GetY(supply));
				return;
			}
			else return;
		}
		pMAV->MoveTo(aWaypointX[Abs(aMAVProgress[i])], aWaypointY[Abs(aMAVProgress[i])]);
		break;
	}
}

/* Steuerung */

public func ContainedLeft(object pByObj)
{
	if(controlling < 0)
		return _inherited(...);
		
  if(aMAVs[controlling] && !aMAVs[controlling]->IsDestroyed())
    aMAVs[controlling]->~ControlLeft(pByObj);
  return true;
}

public func ContainedLeftDouble(object pByObj)
{
	if(controlling < 0)
		return _inherited(...);
		
  if(aMAVs[controlling] && !aMAVs[controlling]->IsDestroyed())
    aMAVs[controlling]->~ControlLeftDouble(pByObj);
  return true;
}

public func ContainedLeftReleased(object pByObj)
{
	if(controlling < 0)
		return _inherited(...);
		
  if(aMAVs[controlling] && !aMAVs[controlling]->IsDestroyed())
    aMAVs[controlling]->~ControlLeftReleased(pByObj);
  return true;
}

public func ContainedRight(object pByObj)
{
	if(controlling < 0)
		return _inherited(...);
		
  if(aMAVs[controlling] && !aMAVs[controlling]->IsDestroyed())
    aMAVs[controlling]->~ControlRight(pByObj);
  return true;
}

public func ContainedRightDouble(object pByObj)
{
	if(controlling < 0)
		return _inherited(...);
		
  if(aMAVs[controlling] && !aMAVs[controlling]->IsDestroyed())
    aMAVs[controlling]->~ControlRightDouble(pByObj);
  return true;
}

public func ContainedRightReleased(object pByObj)
{
	if(controlling < 0)
		return _inherited(...);
		
  if(aMAVs[controlling] && !aMAVs[controlling]->IsDestroyed())
    aMAVs[controlling]->~ControlRightReleased(pByObj);
  return true;
}

public func ContainedDown(object pByObj)
{
	if(controlling < 0)
		return _inherited(...);
		
  if(aMAVs[controlling] && !aMAVs[controlling]->IsDestroyed())
    aMAVs[controlling]->~ControlDown(pByObj);
  return true;
}

public func ContainedDownReleased(object pByObj)
{
	if(controlling < 0)
		return _inherited(...);
		
  if(aMAVs[controlling] && !aMAVs[controlling]->IsDestroyed())
    aMAVs[controlling]->~ControlDownReleased(pByObj);
  return true;
}

public func ContainedDownDouble(object pByObj)
{
	if(controlling < 0)
		return _inherited(...);
		
  if(aMAVs[controlling] && !aMAVs[controlling]->IsDestroyed())
    aMAVs[controlling]->~ControlDownDouble(pByObj);
  return true;
}

public func ContainedUp(object pByObj)
{
	if(controlling < 0)
		return _inherited(pByObj);
		
  if(aMAVs[controlling] && !aMAVs[controlling]->IsDestroyed())
    aMAVs[controlling]->~ControlUp(pByObj);
  return true;
}

public func ContainedUpReleased(object pByObj)
{
	if(controlling < 0)
		return _inherited(...);
		
  if(aMAVs[controlling] && !aMAVs[controlling]->IsDestroyed())
    aMAVs[controlling]->~ControlUpReleased(pByObj);
  return true;
}

protected func ContainedUpDouble(object pByObj)
{
	if(controlling < 0)
		return _inherited(...);
		
  if(aMAVs[controlling] && !aMAVs[controlling]->IsDestroyed())
    aMAVs[controlling]->~ControlUpDouble(pByObj);
  return true;
}

protected func ContainedDigDouble(object pByObj)
{
	if(controlling < 0)
		return _inherited(pByObj);
		
	if(aMAVs[controlling] && !aMAVs[controlling]->IsDestroyed())
		aMAVs[controlling]->Wait();
  controlling = -1;
  
  for(var i = 0; i < GetLength(aMAVs); i++)
  {
  	if(!aMAVs[i] || aMAVs[i]->IsDestroyed() || aMAVs[i]->HasMoveOrder())
  		continue;
 		
 		if(aMAVs[i]->GetAction() != "Flying")
 			aMAVs[i]->Start(this);
 		
 		var closest = 0;
 		for(var j = 0; j < GetLength(aWaypointX); j++)
 		{
 			if(Distance(GetX(aMAVs[i]), GetY(aMAVs[i]), aWaypointX[j], aWaypointY[j]) < Distance(GetX(aMAVs[i]), GetY(aMAVs[i]), aWaypointX[closest], aWaypointY[closest])
 			&& PathFree(GetX(aMAVs[i]), GetY(aMAVs[i]), aWaypointX[j], aWaypointY[j]))
 				closest = j;
 		}
 		aMAVProgress[i] = closest;
 		aMAVs[i]->MoveTo(aWaypointX[closest], aWaypointY[closest]);
 	}
  return true;
}

protected func ContainedThrow(object pByObj)
{
	if(controlling < 0)
		return _inherited(pByObj);

 	aWaypointX[GetLength(aWaypointX)] = GetX(aMAVs[controlling]);
 	aWaypointY[GetLength(aWaypointY)] = GetY(aMAVs[controlling]);

 	var supply = FindObject2(Find_Func("IsSupplyStock"), Find_Distance(100, GetX(aMAVs[controlling])-GetX(), GetY(aMAVs[controlling])-GetY()), Sort_Distance(GetX(aMAVs[controlling])-GetX(), GetX(aMAVs[controlling])-GetX()));
	if(supply)
		PlayerMessage(GetOwner(pByObj), "$WaypointSetStock$", aMAVs[controlling]);
	else
		PlayerMessage(GetOwner(pByObj), "$WaypointSet$", aMAVs[controlling]);
 	return true;
}

/* Upgrades */

public func OnUpgrade(id idUpgrade)
{
  //Sandsackbarriere
  if(idUpgrade == U_SB)
  {
    AddEffect("SandbagBarrier", this, 100, 0, 0, U_SB);

    //Grafik�nderungen/Effekte/Sounds
  }
  
  return true;
}

public func OnUpgradeRemoved(id idUpgrade)
{
  //Sandsackbarriere
  if(idUpgrade == U_SB)
  {
    //Effekte
  }

  return true;
}

/* Sandsackbarriere */

public func OnDmg(int iDmg, int iType)
{
  if(iType == DMG_Explosion) return 50+25*GetUpgrade(U_SB);
  return _inherited(iDmg, iType, ...);
}