/*-- Zünder --*/

#strict 2

local amount, max, init;

public func HandSize()   	{return 1000;}
public func HandX()     	{return 3500;}
public func HandY()     	{return 0;}
public func IsDrawable()	{return true;}
public func IsEquipment()	{return true;}
public func GetPackAmount()	{return amount;}
public func NoArenaRemove()	{return true;}

/* Initalisierung */

protected func Initialize()
{
  amount = 3;
  max = 3;
  init = true;
}

/* Benutzung */

public func ControlThrow(object pByObj)
{
  if(GetPlrDownDouble(GetOwner(pByObj)))
   return _inherited(...);

  //Kein C4 übrig oder Wurfverzögerung?
  if(amount <= 0 || GetEffect("IntC4Cooldown", this))
  {
   return true;
  }

  var x,y;
  x = (GetDir(pByObj)*8)-8;
  y = -9;

  //C4 erstellen und abziehen  
  var c4 = CreateObject(C4EX, x, y, GetOwner(pByObj));
  amount--;
  AddEffect("IntC4Cooldown", this, 1, 15, this);

  //Beim Klettern (aber nicht an Leitern) 
  if(WildcardMatch(GetAction(pByObj), "Scale*") && GetAction(pByObj) != "ScaleLadder")
  {
   SetR((GetDir(pByObj)*-180)+90, c4);
   SetPosition(GetX(pByObj)+(GetDir(pByObj)*12)-6,GetY(pByObj), c4);
   c4->SetActive(this);
   return true;
  }

  //Beim Hangeln
  if(GetAction(pByObj) == "Hangle")
  {
   SetR(180, c4);
   SetPosition(GetX(pByObj), GetY(pByObj)-8, c4);
   c4->SetActive(this);
   return true;
  }

  //Beim Laufen, Springen oder Klettern an Leitern
  if(WildcardMatch(GetAction(pByObj), "Walk*") || WildcardMatch(GetAction(pByObj), "Jump*") || GetAction(pByObj) == "ScaleLadder")
  {
   SetRDir(RandomX(-20,20), c4);
   SetXDir((GetDir(pByObj)*2-1)*20, c4);
   SetYDir(-15, c4);
   c4->SetActive(this);
   Sound("GrenadeThrow*.ogg");
   return true;
  }

  //Beim Kriechen
  if(GetAction(pByObj) == "Crawl")
  {
   SetPosition(GetX(pByObj),GetY(pByObj)+5, c4);
   c4->SetActive(this);
   return true;
  }

  //Beim Schwimmen
  if(GetAction(pByObj) == "Swim")
  {
   SetPosition(GetX(pByObj),GetY(pByObj)+5, c4);
   SetXDir(GetXDir(pByObj)+(GetDir(pByObj)*2-1)*20, c4);
   SetYDir(GetYDir(pByObj)+10, c4);
   c4->SetActive(this);
   Sound("GrenadeThrow*.ogg");
   return true;
  }

  //Keine Aktion bei unpassender Situation
  RemoveObject(c4);
  amount++;
  RemoveEffect("IntC4Cooldown", this);
}

/* Zündung */

public func Activate(object pActivator)
{
  for(var c4 in FindObjects(Find_ID(C4EX), Find_Func("GetPacket", this)))
  {
   SetController(GetOwner(pActivator), c4);
   ScheduleCall(c4, "Trigger", ObjectDistance(c4)/10, 0);
  }

  //Effekte
  SetPicture(10,5,39,64);
  Schedule("SetPicture(64,6,64,64)", 25);
  Sound("C4PA_Ignition.ogg");
  Check();
}

/* HUD */

func CustomHUD()	{return true;}

func UpdateHUD(object pHUD)
{
  pHUD->Charge(amount, max);
  pHUD->Ammo(amount, max, GetName(), true);
}

/* Methoden */

public func DoPackAmount(int iAmount)
{ 
  if((amount = BoundBy(amount + iAmount,0,8)) > max)
   max = amount;
  return amount;
}

/* Aufnahme */

public func Entrance(object pContainer)
{
	if(pContainer->~IsSpawnpoint()) return;
	for(var c4 in FindObjects(Find_ID(C4EX), Find_Func("GetPacket", this)))
    SetOwner(GetOwner(pContainer),c4);
  for(var obj in FindObjects(Find_Container(pContainer),Find_ID(GetID()),Find_Exclude(this)))
    if(obj->DoPackAmount())
    {
      if(DoPackAmount() >= 8) return;
      var amount = DoPackAmount();
      obj->DoPackAmount(amount);
      RemoveObject();
    }
}

public func RejectEntrance(object pObj)
{
  var pack;
  if(pack = FindObject2(Find_Container(pObj), Find_ID(C4PA)))
   if(pack->DoPackAmount() >= 8)
    return 1;
}

/* TimerCall */

protected func Check()
{
	if(!init) return;
  //Grund zum existieren?
  if(amount <= 0 && !FindObject2(Find_ID(C4EX), Find_Func("GetPacket", this)))
    RemoveObject();
}

/* Sounds */

protected func Hit()
{
  Sound("WPN2_Hit*.ogg");
}

protected func Selection()
{
  Sound("C4PA_Deploy.ogg");
}