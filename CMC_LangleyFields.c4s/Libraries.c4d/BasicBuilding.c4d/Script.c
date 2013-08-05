/*-- Grundgeb�ude --*/

#strict 2
#include DOOR	//Clonk-eigenes T�rsystem

local iEnergyAmount;

public func BuildingRadius()     	{return 0;} //Erweitert Bauradius um den angegebenen Wert
public func TechLevel() 				  {return 0;} //Ben�tigte Techstufe
public func NeedBuildingRadius()  {return true;} //Geb�ude muss innerhalb des Bauradius sein.
public func IsCMCBuilding()			 	{return true;}
public func RequiredEnergy() 			{return 0;} //Ben�tigte Energie
public func EnergyProduction()		{return 0;} //Erzeugt Energie

public func MaxDamage()						{return 500;} //Maximaler Schadenswert bis zur Zerst�rung.

public func HasEnoughEnergy()     {return !CurrentRequiredEnergy();}
public func CurrentRequiredEnergy() {return RequiredEnergy()-iEnergyAmount;}

public func BuildingConditions(int iPlr, int iX, int iY) {return true;} //Geb�udespezifische Bauanforderungen

/* Konstruktion */

protected func Construction()
{
}

/* Initialisierung */

public func Initialize()
{
}

public func EnergySupply(int iAmount, object pBy)
{
	iEnergyAmount += iAmount;
	return true;
}

public func ResetCurrentEnergy() { iEnergyAmount = 0; }

public func SendEnergy()
{
	var energy = EnergyProduction();
	if(energy <= 0)
		return;

	var plr = GetOwner();
	var objects = FindObjects(Find_Func("CurrentRequiredEnergy"), Find_Allied(plr), Sort_Distance());
	for(var obj in objects)
	{
		if(energy <= 0)
			break;
		
		var supply = Min(energy, obj->RequiredEnergy());
		obj->~EnergySupply(supply, this);
		energy -= supply;
	}
	
	return energy;
}
