/*-- Fahrzeugfabrik --*/

#strict 2
#include CCBS

local basement;	//Fundament

public func TechLevel()		{return 3;}	//Techstufe
public func BuildingRadius()	{return 200;}	//Bauradius
public func RequiredEnergy() 	{return 100;}	//Energieverbraucher


/* Konstruktion */

protected func Construction()
{
  basement = CreateObject(BT04,0,8,GetOwner());
}

/* Initalisierung */

protected func Initialize()
{
  Sound("CCBS_PowerUp.ogg");
  return _inherited(...);
}

public func AdditionalBuildingMenu(object pMenuObj)
{
	AddMenuItem("$CreateVehicles$", "OpenVehicleMenu", CVHC, pMenuObj, 0, pMenuObj, "$VehicleMenuDesc$");
	return true;
}

public func OpenVehicleMenu(id dummy, object pMenuObj)
{
	MenuHeader(pMenuObj, "$VehicleMenu$");

	var def, i, plr = GetOwner(pMenuObj);
	while(def = GetPlrKnowledge(plr, 0, i++, C4D_Vehicle))
		if(def)
			AddMenuItem(GetName(0, def), "StartVehicleCreatingProcess", def, pMenuObj);
	
	return true;
}

public func StartVehicleCreatingProcess(id idDef, object pMenuObj)
{
	//Checks ob bauen soll etc. fehlen
	
	
	AddEffect("CreateVehicle", this, 1, 36, this, 0, GetOwner(pMenuObj), idDef);
}

public func FxCreateVehicleStart(object pTarget, int iNr, int iTemp, int iOwner, id idDef)
{
	EffectVar(0, pTarget, iNr) = iOwner;
	EffectVar(1, pTarget, iNr) = idDef;
	
	Sound("SF_Build", 0, pTarget, 0, 0, +1);
}

public func FxCreateVehicleTimer(object pTarget, int iNr, int iTime)
{
	//Entrancewerte	
  for(var i = 10; i > 0; i--)
    pTarget->CreateParticle("Frazzle", -43+Random(48), 10+Random(35), RandomX(-20, 20), RandomX(-20, 20), 30, HSL(60, 150, RandomX(150, 250)), pTarget);

	if(iTime > 360)
		return -1;
}

public func FxCreateVehicleStop(object pTarget, int iNr)
{
	Sound("SF_Build", 0, pTarget, 0, 0, -1);
	
	CreateObject(EffectVar(1, pTarget, iNr), -22, 35, EffectVar(0, pTarget, iNr));
	return true;
}

