/*-- Fahrstuhl --*/

#strict 2
#include CCBS

local basement, steelcable, case;	//Fundament

public func TechLevel()		{return 1;}	//Techstufe
public func RequiredEnergy() 	{return 50;}	//Energieverbraucher


/* Konstruktion */

protected func Construction()
{
  //basement = CreateObject(BT02,0,8,GetOwner()); Benötigt spezielles Fundament
  steelcable = [];
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
	case = CreateObject(CHLP, 0, GetObjHeight()/2, GetOwner());
	case->Set(this);
	steelcable[0] = CreateObject(SLCB, -24, 0, GetOwner());
	steelcable[1] = CreateObject(SLCB, -17, 0, GetOwner());
	steelcable[2] = CreateObject(SLCB, +17, 0, GetOwner());
	steelcable[3] = CreateObject(SLCB, +24, 0, GetOwner());
	
	var fBack = true;
	for(var obj in steelcable)
		obj->Set(CreateObject(FXTR, 0, 0, NO_OWNER)->Set(case, 0, 25), (fBack = !fBack));

	return true;
}

public func OnRemoveCase()
{
	for(var obj in steelcable)
		if(obj)
			RemoveObject(obj);

	CreateCase();
	return true;
}
