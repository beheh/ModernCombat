/*-- Fusionsreaktor --*/

#strict 2
#include CCBS

local basement;	//Fundament

public func TechLevel()           {return 1;}
public func EnergyProduction()		{return 40;} 

/* Konstruktion */

protected func Construction()
{
  basement = CreateObject(BT02,0,8,GetOwner());
}

/* Initalisierung */

protected func Initialize()
{
  Sound("CCBS_PowerUp.ogg");
}
