/*-- Fahrstuhl --*/

#strict 2
#include CCBS

local basement;	//Fundament

public func TechLevel()		{return 1;}	//Techstufe
public func RequiredEnergy() 	{return 50;}	//Energieverbraucher


/* Konstruktion */

protected func Construction()
{
  //basement = CreateObject(BT02,0,8,GetOwner()); Benötigt spezielles Fundament
}

/* Initalisierung */

protected func Initialize()
{
  Sound("CCBS_PowerUp.ogg");
  Sound("CFRT_Fuse.ogg",50,0,0,0,+1);
  CreateCase();

  return _inherited(...);
}

public func CreateCase()
{
	var case = CreateObject(CHLP, 0, GetObjHeight()/2, GetOwner());
	case->Set(this);
	return true;
}

public func OnRemoveCase()
{
	CreateCase();
	return true;
}
