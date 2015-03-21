/*-- Zünder --*/

#strict 2
#include PACK

local thrown;

static const C4PA_Cooldown = 30;

public func IsDrawable()	{return true;}		//Wird sichtbar getragen
public func HandSize()		{return 1000;}
public func HandX()		{return 3500;}

public func IsEquipment()	{return true;}
public func MaxPoints()		{return 6;}
public func StartPoints()	{return 1;}
public func TeamSupportTime()	{return 0;}
public func PackLight()		{return false;}
public func DestroyEmptyPack()	{return !GetC4();}


public func GetC4()
{
  return FindObjects(Find_ID(C4EX), Find_Func("GetPacket", this));
}

/* Benutzung */

public func ControlThrow(object pByObj)
{
  if(GetPlrDownDouble(GetOwner(pByObj)))
    return _inherited(...);

  //Kein C4 übrig oder Wurfverzögerung?
  if(!GetPackPoints() || GetEffect("IntC4Cooldown", this))
    return true;

  var x = (GetDir(pByObj) * 8) - 8, y = -9;

  //C4 erstellen und abziehen  
  var c4 = CreateObject(C4EX, x, y, GetOwner(pByObj));
  DoPackPoints(-1);
  AddEffect("IntC4Cooldown", this, 1, C4PA_Cooldown, this);

  //Beim Klettern
  if(WildcardMatch(GetAction(pByObj), "Scale*") && GetAction(pByObj) != "ScaleLadder")
  {
    SetR((GetDir(pByObj) * -180) + 90, c4);
    SetPosition(GetX(pByObj) + (GetDir(pByObj) * 12) - 6, GetY(pByObj), c4);
    c4->SetActive(this);
    return true;
  }

  //Beim Hangeln
  if(GetAction(pByObj) == "Hangle")
  {
    SetR(180, c4);
    SetPosition(GetX(pByObj), GetY(pByObj) - 8, c4);
    c4->SetActive(this);
    return true;
  }

  //Beim Laufen, Springen oder Klettern an Leitern
  if(WildcardMatch(GetAction(pByObj), "Walk*") || WildcardMatch(GetAction(pByObj), "Jump*") || GetAction(pByObj) == "ScaleLadder")
  {
    SetRDir(RandomX(-20, 20), c4);
    SetXDir((GetDir(pByObj) * 2 - 1) * 20, c4);
    SetYDir(-15, c4);
    c4->SetActive(this);
    Sound("GrenadeThrow*.ogg");
    return true;
  }

  //Beim Kriechen
  if(GetAction(pByObj) == "Crawl")
  {
    SetPosition(GetX(pByObj), GetY(pByObj) + 5, c4);
    c4->SetActive(this);
    return true;
  }

  //Beim Schwimmen
  if(GetProcedure(pByObj) == "SWIM")
  {
    SetPosition(GetX(pByObj), GetY(pByObj) + 5, c4);
    SetXDir(GetXDir(pByObj) + (GetDir(pByObj) * 2 - 1) * 20, c4);
    SetYDir(GetYDir(pByObj) + 10, c4);
    c4->SetActive(this);
    Sound("GrenadeThrow*.ogg");
    return true;
  }

  //Keine Aktion bei unpassender Situation
  RemoveObject(c4);
  DoPackPoints(1);
  RemoveEffect("IntC4Cooldown", this);
}

/* Zündung */

public func Activate(object pActivator)
{
  for (var c4 in GetC4())
  {
    SetController(GetOwner(pActivator), c4);
    ScheduleCall(c4, "Trigger", Max(ObjectDistance(c4) / 10, 1));
  }

  //Effekte
  CreateParticle("FlashLight", 0, 0, 0, 0, 60, RGBa(255), this);
  Sound("C4PA_Activate.ogg");

  //Bild ändern wenn C4 übrig
  if (GetPackPoints())
  {
    SetPicture(0, 4, 64, 64);
    Schedule("SetPicture(64, 4, 64, 64)", 25);
  }
}

/* Zusammenlegen */

public func JoinPack(object pInto, object pMsgObj)
{
  var i = _inherited(pInto, pMsgObj);
  if (i)
  {
    //C4 übertragen
    for (var obj in GetC4())
      obj->~SetPacket(pInto);
  }
  pInto->~Check();
  Check();
  return i;
}

/* HUD */

public func MinValue()		{return 0;}
public func MinValue2()		{return 1;}

public func UpdateHUD(object pHUD)
{
  _inherited(pHUD, ...);
  pHUD->Recharge(GetEffect("IntC4Cooldown", this, 0, 6), C4PA_Cooldown);
}

/* Sonstiges */

public func OnRefill()
{
  if (!GetPackPoints())
    SetPicture(0, 4, 64, 64);
  else
    SetPicture(64, 4, 64, 64);
}

protected func Check()
{
  if(thrown) return;

  //Grund zum Existieren?
  if (!GetPackPoints() && !GetLength(GetC4()))
  {
    thrown = true;

    //Kategorie wechseln
    SetCategory(C4D_Vehicle);

    //Schützen verlassen
    if(Contained())
    {
      var dir = +1;
      if(GetDir(Contained()) == DIR_Right)
        dir = -1;

      Exit(0, 0,0, Random((360)+1), dir,-3, Random(11)-5);

      Sound("AT4R_ThrowAway.ogg");
    }

    //Verschwinden
    FadeOut();
  }
}

protected func CalcValue()
{
  return Interpolate2(0, Value(GetID()), GetPackPoints(), 3);
}

/* Aufnahme */

public func RejectEntrance()
{
  if(thrown)
    return true;

  return(_inherited(...));
}

/* Allgemein */

protected func Hit()
{
  Sound("WPN2_Hit*.ogg");
}

protected func Selection()
{
  Sound("C4PA_Deploy.ogg");

  //Ladungen kurzzeitig markieren
  var c4 = GetC4();
  for(var obj in c4)
  {
    var marker = CreateObject(SM08, 0, 0, GetOwner(Contained()));
    marker->Set(obj, this, 0, 30, "ObjectAllied");
    marker->SetActionData(4);
    marker->SetVisibility(VIS_Owner);
  }
}
