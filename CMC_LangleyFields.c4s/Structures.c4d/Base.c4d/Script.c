/*-- Stützpunkt --*/

#strict 2
#include CCBS

local basement;	//Fundament

public func RequiredEnergy() 			{return 20;}

/* Konstruktion */

protected func Construction()
{
  basement = CreateObject(BT01,0,8,GetOwner());
}

/* Initalisierung */

protected func Initialize()
{
  Sound("CCBS_PowerUp.ogg");
}
