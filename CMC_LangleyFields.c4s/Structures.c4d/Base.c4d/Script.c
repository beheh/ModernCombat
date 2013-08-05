/*-- Stützpunkt --*/

#strict 2
#include CCBS

local basement;	//Fundament


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