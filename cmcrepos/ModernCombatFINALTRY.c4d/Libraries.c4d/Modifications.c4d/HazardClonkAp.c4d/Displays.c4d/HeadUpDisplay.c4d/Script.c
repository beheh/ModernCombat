/*--- Head-Up-Display ---*/

#strict 2

local CharsWAmmo,
      CharsMaxAmmo,
      CharsClonkAmmo,
      CharsGrenade,
      CharEqS,
      rechargebar;

local lItem;

public func ColorEmpty()	{return RGB(255, 0, 0);}


/* Initialisierung */

protected func Initialize()
{
  SetPosition(155, -80); 

  CharsWAmmo = [CreateObject(CHAR, -105, 0, GetOwner()), CreateObject(CHAR, -85, 0, GetOwner()), CreateObject(CHAR, -65, 0, GetOwner())];

  CharsMaxAmmo = [CreateObject(CHAR, -35, 3, GetOwner()), CreateObject(CHAR, -20, 3, GetOwner()), CreateObject(CHAR, -5, 3, GetOwner())];
  ResizeChars(CharsMaxAmmo, 750);

  CharEqS = CreateObject(CHAR, 11, 3, GetOwner());
  CharEqS->Set(61);

  CharsClonkAmmo = [CreateObject(CHAR, 23, 4, GetOwner()), CreateObject(CHAR, 37, 4, GetOwner()), CreateObject(CHAR, 51, 4, GetOwner())];
  for(var char in CharsClonkAmmo)
    char->SetClrModulation(RGB(0, 255, 255));

  ResizeChars(CharsClonkAmmo, 700);

  CharsGrenade = [CreateObject(CHAR, -105, -25, GetOwner()), CreateObject(CHAR, -95, -27, GetOwner()), CreateObject(CHAR, -115, -28, GetOwner())];
  CharsGrenade[0]->Set(120, RGB(255, 255, 0));
  CharsGrenade[2]->Set(1337);
  ResizeChars(CharsGrenade, 650);

  rechargebar = CreateObject(RBAR, 0, -15, GetOwner()); 

  SetVisibility(VIS_Owner);
}

public func ResizeChars(chars, int size)
{
  if(!size)
    size = 1000;

  if(GetType(chars) == C4V_Array)
    for(var char in chars)
      SetObjDrawTransform(size, 0, 0, 0, size, 0, char);
  else
    SetObjDrawTransform(size, 0, 0, 0, size, 0, chars);

  return true;
}

protected func Timer()
{
  if(GetOwner() == NO_OWNER || !GetPlayerName(GetOwner()) || (!GetAlive(GetCrew(GetOwner())))) 
  {
    RemoveObject(this);
  }
}

public func HideWeapons()
{
  if(GetVisibility() == VIS_None)
    return true;

  for(var char in CharsWAmmo)
    SetVisibility(VIS_None, char);

  for(var char in CharsMaxAmmo)
    SetVisibility(VIS_None, char);

  if(!NoAmmo())
  {
    for(var char in CharsClonkAmmo)
      SetVisibility(VIS_None, char);

    SetVisibility(VIS_None, CharEqS); 
  }
  SetVisibility(VIS_None, rechargebar);
  CustomMessage("", this, GetOwner());
  SetVisibility(VIS_None);
}

public func ShowWeapons()
{
  if(GetVisibility() == VIS_Owner)
    return true;

  for(var char in CharsWAmmo)
    SetVisibility(VIS_Owner, char);

  for(var char in CharsMaxAmmo)
    SetVisibility(VIS_Owner, char);

  if(!NoAmmo())
  {
    for(var char in CharsClonkAmmo)
      SetVisibility(VIS_Owner, char);

    SetVisibility(VIS_Owner, CharEqS);
  }
  SetVisibility(VIS_Owner);
}

protected func UpdateHUD(object weapon, object pClonk)
{
  if(!pClonk) return true;  
  if(!weapon) return HideWeapons();

  //Granaten updaten
  var grenades = Format("%d", pClonk->~GrenadeCount());
  CharsGrenade[1]->Set(GetChar(grenades));

  if(lItem != weapon)
  {  
    SetVisibility(VIS_None, rechargebar);
    lItem = weapon;
  }

  if(NoAmmo() && GetVisibility(CharEqS) == VIS_Owner)
  {
    for(var char in CharsClonkAmmo)
      SetVisibility(VIS_None, char);

    SetVisibility(VIS_None, CharEqS); 
  }

  if(weapon && !(weapon->~IsWeapon()) && !(weapon->~CustomHUD()))
    HideWeapons(); 
  else if(GetVisibility() == VIS_None)
    ShowWeapons();

  if(weapon->~CustomHUD())
  {
    ShowSelectProcess(weapon);
    return weapon->~UpdateHUD(this);
  }

  if(weapon->~IsWeapon())
  {
    //Munition in der Waffe
    var weaponAmmo = GetAmmo(weapon->~GetFMData(FM_AmmoID), weapon);
    var wAmmo = Format("%03d", weaponAmmo);
    var i = 0;
    for(var char in CharsWAmmo)
    {
      char->Set(GetChar(wAmmo, i), ColorEmpty()*(!weaponAmmo));
      i++;
    }

    i = 0;

    //Maximal ladbare Munition
    var maxAmmo = weapon->~GetFMData(FM_AmmoLoad);
    var mAmmo = Format("%03d", maxAmmo);
    for(var char in CharsMaxAmmo)
    {
      char->Set(GetChar(mAmmo, i));
      i++;
    }

    i = 0;
    //Bei aktiver Keine Munition-Regel überspringen
    if(!NoAmmo())
    {
      //Munition des Munitionsgürtels ermitteln
      var clonkAmmo = GetAmmo(weapon->~GetFMData(FM_AmmoID), pClonk);
      var cAmmo = Format("%03d", clonkAmmo);
      if(clonkAmmo > 0)
      {
        for(var char in CharsClonkAmmo)
        {
          char->Set(GetChar(cAmmo, i), RGB(0, 255, 255));
          i++;
        }
      }
      else
        for(var char in CharsClonkAmmo)
          char->Set(48, ColorEmpty());

      if(GetVisibility(CharEqS) == VIS_None)
        SetVisibility(VIS_Owner, CharEqS);
    }

    /*-- Schrift: Munitionsart - Feuertechnik --*/
    var ammoName = weapon->~GetFMData(FM_Name);
    if(weapon->~IsWeapon2() && weapon->GetFMData(FT_Name))
      var firemode = weapon->~GetFMData(FT_Name);
    else
      var firemode = weapon->~GetFMData(FM_Name);

    var str = Format("@<c ffff00>%s</c> - %s", ammoName, firemode);
    LimitString(str, 29 + 15);
    CustomMessage(str, this, GetOwner(), 0, 60, 0, 0, 0, MSG_NoLinebreak);
    if(weapon->IsRecharging())
    {
      SetVisibility(VIS_Owner, rechargebar);
      var recharge = weapon->GetRecharge();
      var x = -122+(176*recharge/100);
      SetPosition(GetX()+x, GetY()-15, rechargebar);
    }
    else if(weapon->IsReloading())
    {
      SetVisibility(VIS_Owner, rechargebar);
      var charge = (weapon->GetCharge()/10);
      var x = -122+(176*charge/100);
      SetPosition(GetX()+x, GetY()-15, rechargebar);
    }
    else if(ShowSelectProcess(weapon))
    {
      // !
    }
    else if(GetVisibility(rechargebar) != VIS_None) 
    {
      SetVisibility(VIS_None, rechargebar);
    }
  }
  return true;
}

public func Update(object weapon, object ammobag, object who)
{
  UpdateHUD(weapon, who);
}

private func ShowSelectProcess(object item)
{
  var effect = GetEffect("SelectItem",item,0,6);
  if(effect)
  {
    Recharge(effect,item->~SelectionTime());
  }
  return effect;
} 

global func GetHUD(object obj, id theID)
{
  if(!obj)
    if(!(obj = this))
      return;
  if(!theID)
    theID = 1HUD;
  var owner = GetOwner(obj);
  if(owner <= -1) return;
  var bar = FindObjectOwner(theID,owner);
  if(!bar)
    bar = CreateObject(theID, 0,0, owner);
  return bar;
}

public func Charge(int part, int max)
{
  //Recharge(part, max);
  return 1;
}

public func Recharge(int part, int max)
{
  if(part != max && part != 0)
    SetVisibility(VIS_Owner, rechargebar);
  else
    SetVisibility(VIS_None, rechargebar);

  var x = -122+(176*part/max);
  SetPosition(GetX()+x, GetY()-15, rechargebar);
  return 1;
}

public func Ammo(int iAmmoCount, int iAmmoLoad, string szName, bool fShow, int dwColorW, int dwColorM)
{
  var wAmmo = Format("%03d", iAmmoCount);
  var i = 0;
  //dwColorW zur Färbung der ersten Zahl (Momentan geladene Munition)
  if(!dwColorW) dwColorW = ColorEmpty()*(!iAmmoCount);
  
  for(var char in CharsWAmmo)
  {
    char->Set(GetChar(wAmmo, i), dwColorW);
    i++;
  }

  var mAmmo = Format("%03d", iAmmoLoad);
  var i = 0;
  for(var char in CharsMaxAmmo)
  {
    //dwColorM zur Färbung der zweiten Zahl (Maximale Munition)
    char->Set(GetChar(mAmmo, i), dwColorM);
    i++;
  }

  if(GetVisibility(CharsClonkAmmo[0]) == VIS_Owner)
  {
    for(var char in CharsClonkAmmo)
      SetVisibility(VIS_None, char);

    SetVisibility(VIS_None, CharEqS);
  }

  if(szName)
    CustomMessage(Format("@%s",szName), this, GetOwner(), 0, 60, 0, 0, 0, MSG_Left);

  return 1;
}

/* String-Längenlimitierung */

global func LimitString(string &szString, int iLimit)
{
  if(!iLimit) return false;
  if(GetLength(szString) <= iLimit) return false;

  var str = "";
  for(var i = 0; i < iLimit; i++)
  {
    str = Format("%s%c", str, GetChar(szString, i));
  }
  szString = Format("%s...", str);

  return true;
}