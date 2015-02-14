/*-- Fahrstuhl --*/

#strict 2
#include CCBS
#include BT03

local steelcable, case;

public func TechLevel()		{return TECHLEVEL_1;}	//Techstufe
public func RequiredEnergy()	{return 50;}		//Energieverbraucher
public func MaxDamage()		{return 150;}		//Maximaler Schadenswert bis zur Zerstörung


/* Initalisierung */

protected func Initialize()
{
  //Fahrstuhlplattform erstellen
  steelcable = [];
  CreateCase();

  //Effekte
  if(HasEnergy())
    Sound("Building_PowerOn.ogg");
  else
    Sound("Building_PowerOff.ogg");

  return _inherited(...);
}

/* Menüführung */

public func AdditionalBuildingMenu(object pMenuObj)
{
  //Fahrstuhlkorb kaufen
  if(!case)
  {
    if(GetWealth(GetOwner(pMenuObj)) < GetValue(0, CHLP))
      AddMenuItem("$BuyElevatorCaseNotEnough$", "BuyElevatorCase", CHLP, pMenuObj, GetValue(0, CHLP), pMenuObj, "$ElevatorCaseMenuDesc$");
    else
      AddMenuItem("$BuyElevatorCase$", "BuyElevatorCase", CHLP, pMenuObj, GetValue(0, CHLP), pMenuObj, "$ElevatorCaseMenuDesc$");
  }
  else
    AddMenuItem("$BuyElevatorCaseInactive$", 0, CHLP, pMenuObj);

  return true;
}

public func BuyElevatorCase(dummy, object pMenuObj)
{
  if(!pMenuObj)
    return;
  if(case)
    return;

  //Nicht genügend Geld
  if(GetWealth(GetOwner(pMenuObj)) < GetValue(0, CHLP))
  {
    Sound("Error", 0, this, 0, GetOwner(pMenuObj)+1);
    return true;
  }

  //Geld abziehen
  SetWealth(GetOwner(pMenuObj), GetWealth(GetOwner(pMenuObj))-GetValue(0, CHLP));
  CreateCase();

  //Sounds
  Sound("Cash", 0, this, 0, GetOwner(pMenuObj)+1);

  return true;
}

/* Plattform */

public func CreateCase()
{
  //Plattform erstellen
  case = CreateObject(CHLP, 0, GetObjHeight()/2+5, GetOwner());
  case->Set(this);

  //Der Objektliste hinzufügen
  AddObject(case);

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

  return true;
}