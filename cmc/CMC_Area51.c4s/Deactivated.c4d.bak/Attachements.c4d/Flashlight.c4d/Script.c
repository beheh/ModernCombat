/*-- Taschenlampe --*/

#strict 2
#include GEAR

local light, user;

public func HandSize()		{return 1000;}
public func HandX()		{return 4500;}
public func HandY()		{}
public func IsDrawable()	{return true;}
public func IsEquipment()	{return true;}
public func NoArenaRemove()	{return true;}
public func GetGearType()	{return(GEAR_Light);}
public func IsLamp()		{return true;}


/* Übergeben */

public func ControlThrow(pByObject)
{
  Pass(pByObject);
  return true;
}

func Pass(caller)
{
  //Clonk suchen
  var obj;
  if(obj = FindObject2(Find_InRect(-10,-10,20,20),Find_OCF(OCF_CrewMember),Find_Exclude(caller),Find_Allied(GetOwner(caller)),Find_NoContainer()))
  {
    //Clonk kompatibel?
    if(!(obj->~CanUse(GetID()))) return;
    //Identische Ausrüstung bereits vorhanden?
    if(obj->~HasGear(this()->GetGearType()))
    {
      PlayerMessage(GetOwner(caller),"$AlreadyWears$",caller,GetName(caller));
      return;
    }
    //Anlegen
    if(caller->~EquipGear(obj))
      GearBuckle(obj);
  }
  return 1;
}

/* Callbacks */

public func GearEffect(object pClonk)
{
  user = pClonk;
  if(IsDark())
  {
    light = AddLightCone(1000,RGBa(255,255,220,90),user);
    light->ChangeSizeXY(1900,6000);
    light->ChangeOffset(0,0, true);
    SetAction("On");
  }
}

public func GearEffectReset(object pClonk)
{
  user = 0;
  if(light) RemoveObject(light);
  SetAction("Idle");
  Sound("Equip.ogg", pClonk);
}

/* Rotation */

private func CheckRotation()
{
  //Kein Nutzer mehr, gleicherer verschachtelt oder gerade unbewaffnet: Licht entfernen
  if (!user || Contained(user) || !(user->~IsArmed2()))
  {
    if (light)
      RemoveObject(light);
    return;
  }

  //Ansonsten Licht einschalten sofern noch nicht vorhanden
  if (!light)
  {
    light = AddLightCone(1000, RGBa(255, 255, 220, 90), user);
    light->ChangeSizeXY(1900, 6000);
    light->ChangeOffset(0, -2, true);
  }

  //Lichtrotation anpassen
  if (light && user)
    light->ChangeR(user->~AimAngle(20, 180));
}

/* Kontextmenü */

public func ConDesc()
{
  if(light)
    return("$SwitchOff$");
  return("$SwitchOn$");
}

public func ConUseable()
{
  return true;
}

public func ConUse(object pHazard)
{
  //Ein- oder Ausschalten
  if(light)
  {
    if(light) RemoveObject(light);
    SetAction("Idle");
  }
  else
  {
    light = AddLightCone(1000,RGBa(255,255,220,90),user);
    light->ChangeSizeXY(1900,6000);
    light->ChangeOffset(0,0, true);
    SetAction("On");
  }

  Sound("WPN2_Switch*.ogg", pHazard);
}