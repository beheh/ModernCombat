/*-- C4 Paket --*/

#strict 2

local amount, max;

public func HandSize()   	{return 1000;}
public func HandX()     	{return 3500;}
public func HandY()     	{return 0;}
public func IsDrawable()	{return true;}
public func IsEquipment()	{return true;}

protected func Initialize()
{
  amount = 4;
  max = 4;
}

/* Benutzung */
public func ControlThrow(object pByObj)
{
  if(GetPlrDownDouble(GetOwner(pByObj)))
    return _inherited(...);
  
  if(amount <= 0)
  {
    //Sound für Päckchen alle?
    return true;
  }
  
  var x,y;
  x = (GetDir(pByObj)*8)-8;
  y = -9;
  
  var c4 = CreateObject(C4EX, x, y, GetOwner(pByObj));
  amount--;
  
  if(WildcardMatch(GetAction(pByObj), "Scale*") && GetAction(pByObj) != "ScaleLadder")
  {
    c4->SetR((GetDir(pByObj)*-180)+90);
    c4->SetPosition(GetX(pByObj)+(GetDir(pByObj)*12)-6,GetY(pByObj));
    c4->SetActive(this());
    return true;
  }
  
  if(GetAction(pByObj) == "Hangle")
  {
    c4->SetR(180);
    c4->SetPosition(GetX(pByObj), GetY(pByObj)-8);
    c4->SetActive(this());
    return true;
  }
  
  if(WildcardMatch(GetAction(pByObj), "Walk*"))
  {
    c4->SetRDir(RandomX(-20,20));
    c4->SetXDir((GetDir(pByObj)*80)-40);
    c4->SetYDir(-25);
    c4->SetActive(this());
    Sound("GrenadeThrow*.ogg");
    return true;
  }
  
  if(WildcardMatch(GetAction(pByObj), "Jump*"))
  {
    c4->SetRDir(RandomX(-20,20));
    c4->SetXDir((GetDir(pByObj)*80)-40);
    c4->SetYDir(-25);
    c4->SetActive(this());
    Sound("GrenadeThrow*.ogg");
    return true;
  }
  
  if(GetAction(pByObj) == "Crawl")
  {
    c4->SetPosition(GetX(pByObj),GetY(pByObj)+5);
    c4->SetActive(this());
    return true;
  }
  
  if(GetAction(pByObj) == "Swim")
  {
    c4->SetPosition(GetX(pByObj),GetY(pByObj)+5);
    c4->SetXDir((GetDir(pByObj)*60)-30);
    c4->SetYDir(10);
    c4->SetActive(this());
    Sound("GrenadeThrow*.ogg");
    return true;
  }
  
  //Falls keine passende Aktion
  RemoveObject(c4);
  amount++;
}

/* Aktivierung */
public func Activate()
{
  for(var c4 in FindObjects(Find_ID(C4EX), Find_Func("GetPacket", this)))
    ScheduleCall(c4, "Trigger", ObjectDistance(c4)/10);
   
  //Effekte
  SetPicture(10,5,39,64);
  Schedule("SetPicture(64,6,64,64)", 25);
  if(amount <= 0)
    Schedule("RemoveObject()", 25);
  //Sound(...)  Drücken des Knopfes oder so
}

/* HUD */
func CustomHUD() {return true;}
func UpdateHUD(object pHUD)
{
  pHUD->Charge(amount, max);
  pHUD->Ammo(amount, max, GetName(), true);
}

/* Methoden */
public func DoPackAmount(int iAmount)
{ 
  if((amount += iAmount) > max)
    max = amount;
  return amount;
}

public func GetPackAmount() { return amount; }
