/*-- C4 Paket --*/

#strict 2

local amount;

public func HandSize()   	{return 1000;}
public func HandX()     	{return 3500;}
public func HandY()     	{return 0;}
public func IsDrawable()	{return true;}
public func IsEquipment()	{return true;}

protected func Initialize()
{
  amount = 4;
}

/* Benutzung */
public func ControlThrow(object pByObj)
{
  Log("Controlled1");
  if(GetPlrDownDouble(GetOwner(pByObj)))
    return _inherited(...);
  Log("Controlled2");
  
  if(amount <= 0)
  {
    Sound("GrenadeThrow*.ogg"); //Sound für Päckchen alle?
    return true;
  }
  
  var x,y;
  x = (GetDir(pByObj)*8)-8;
  y = -9;
  
  var c4 = CreateObject(C4__, x, y, GetOwner(pByObj));
  amount--;
  
  if(WildcardMatch(GetAction(pByObj), "Scale*") && GetAction(pByObj) != "ScaleLadder")
  {
    c4->SetR((GetDir(pByObj)*180)-90);
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
    return true;
  }
  
  if(GetAction(pByObj) == "Crawl")
  {
    c4->SetPosition(GetX(pByObj),GetY(pByObj)+5);
    c4->SetActive(this());
    return true;
  }
}

/* Aktivierung */
public func Activate()
{
  var packets = 0;
  for(var c4 in FindObjects(Find_ID(C4__), Find_Func("GetPacket", this)))
  {
    ScheduleCall(c4, "Trigger", ObjectDistance(c4)/10);
    packets++;
  }
   
  //Effekte
  SetPicture(10,5,39,64);
  Schedule("SetPicture(64,6,64,64)", 25);
  if(packets == 0 && amount <= 0)
    Schedule("RemoveObject()", 25);
  //Sound(...)  Drücken des Knopfes oder so
}

/* HUD */
func CustomHUD() {return true;}
func UpdateHUD(object pHUD)
{
  pHUD->Charge(amount,4);
  pHUD->Ammo(amount, 4, GetName(), true);
}

/* Methoden */
public func DoPackAmount(int iAmount) { return amount += iAmount; }
public func GetPackAmount() { return amount; }
