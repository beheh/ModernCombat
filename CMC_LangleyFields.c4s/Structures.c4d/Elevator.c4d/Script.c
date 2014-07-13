/*-- Fahrstuhl --*/

#strict 2
#include CCBS

local basement, steelcable, case;

public func TechLevel()		{return 1;}	//Techstufe
public func RequiredEnergy() 	{return 50;}	//Energieverbraucher


/* Konstruktion */

protected func Construction()
{
  //Fundament erstellen
  //basement = CreateObject(BT02,0,8,GetOwner()); Benötigt spezielles Fundament

  steelcable = [];
}

/* Initalisierung */

protected func Initialize()
{
  //Fahrstuhlplattform erstellen
  CreateCase();

  //Effekte
  Sound("CCBS_PowerUp.ogg");
  Sound("CFRT_Fuse.ogg",50,0,0,0,+1);

  return _inherited(...);
}

/* Plattform */

public func CreateCase()
{
  //Plattform erstellen
  case = CreateObject(CHLP, 0, GetObjHeight()/2, GetOwner());
  case->Set(this);

  //Dekorative Seile erstellen
  steelcable[0] = CreateObject(SLCB, -24, 0, GetOwner());
  steelcable[1] = CreateObject(SLCB, -17, 0, GetOwner());
  steelcable[2] = CreateObject(SLCB, +17, 0, GetOwner());
  steelcable[3] = CreateObject(SLCB, +24, 0, GetOwner());

  //Seile konfigurieren
  var fBack = true;
  for(var obj in steelcable)
    obj->Set(CreateObject(FXTR, 0, 0, NO_OWNER)->Set(case, 0, 25), (fBack = !fBack));

  return true;
}

public func OnRemoveCase()
{
  //Seile entfernen
  for(var obj in steelcable)
    if(obj)
      RemoveObject(obj);

  //Neue Plattform erstellen
  CreateCase();

  return true;
}