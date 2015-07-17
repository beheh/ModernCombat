/*--- Head-Up-Display ---*/

#strict 2

local CharsWAmmo,	//Zähler-Munitionswert
      CharsMaxAmmo,	//Nenner-Munitionswert
      CharsClonkAmmo,	//Clonk-Munitionswert
      CharsGrenade,	//Clonk-Granatenwert
      CharEqS,		//Gleichheitszeichen
      rechargebar;	//Balken

local pLastItem;
local iLastWeaponAmmo;
local iLastClonkAmmo;
local iLastWeaponFM;
local iLastWeaponFT;

public func ColorEmpty()	{return RGB(255, 0, 0);}
public func ColorLow()		{return RGB(255, 150, 0);}


/* Initialisierung */

protected func Initialize()
{
  //Am unteren, linken Bildschirmrand positionieren
  SetPosition(161, -80); 

  //Zähler-Munitionswert setzen
  CharsWAmmo = [CreateObject(HCHA, -106, 3, GetOwner()), CreateObject(HCHA, -86, 3, GetOwner()), CreateObject(HCHA, -66, 3, GetOwner())];

  //Nenner-Munitionswert setzen
  CharsMaxAmmo = [CreateObject(HCHA, -33, 5, GetOwner()), CreateObject(HCHA, -17, 5, GetOwner()), CreateObject(HCHA, -1, 5, GetOwner())];
  ResizeChars(CharsMaxAmmo, 750);

  //Gleichheitszeichen setzen
  CharEqS = CreateObject(HCHA, 17, 4, GetOwner());
  CharEqS->Set(61);

  //Clonk-Munitionswert setzen
  CharsClonkAmmo = [CreateObject(HCHA, 32, 5, GetOwner()), CreateObject(HCHA, 46, 5, GetOwner()), CreateObject(HCHA, 60, 5, GetOwner())];
  for(var char in CharsClonkAmmo)
    char->SetClrModulation(RGB(0, 255, 255));
  ResizeChars(CharsClonkAmmo, 750);

  //Clonk-Granatenwert setzen
  CharsGrenade = [CreateObject(HCHA, 98, 6, GetOwner()), CreateObject(HCHA, 112, 5, GetOwner()), CreateObject(HCHA, 84, 3, GetOwner())];
  CharsGrenade[0]->Set(120, RGB(255, 255, 0));
  CharsGrenade[2]->Set(1337);
  ResizeChars(CharsGrenade, 750);

  //Balken setzen
  rechargebar = CreateObject(RBAR, 0, -15, GetOwner()); 

  //Alle Werte initialisieren
  iLastWeaponAmmo = -1;
  iLastWeaponFT = -1;
  iLastWeaponFM = -1;

  //Sichtbarkeit nur für den Besitzer
  SetVisibility(VIS_Owner);
}

/* Symbolgrößen verändern */

public func ResizeChars(chars, int size)
{
  //Keine Angabe: Normalgröße annehmen
  if(!size)
    size = 1000;

  //Symbol transformieren
  if(GetType(chars) == C4V_Array)
    for(var char in chars)
      SetObjDrawTransform(size, 0, 0, 0, size, 0, char);
  else
    SetObjDrawTransform(size, 0, 0, 0, size, 0, chars);

  return true;
}

/* Timer */

protected func Timer()
{
  if(GetOwner() == NO_OWNER || !GetPlayerName(GetOwner()) || (!GetAlive(GetCrew(GetOwner())))) 
  {
    RemoveObject(this);
  }
}

/* Sichtbarkeit und Aktualisierung von HUD-Elementen */

public func HideWeapons()
{
  pLastItem = 0;

  if(GetVisibility() == VIS_None)
    return true;

  //Alle Munitionsanzeigen unsichtbar machen
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

  //Balken unsichtbar machen
  SetVisibility(VIS_None, rechargebar);

  //Textanzeige neutralisieren
  CustomMessage("", this, NO_OWNER);

  //Granatenwert an Seite verschieben
  CharsGrenade[0]->SetPosition(GetX(CharsGrenade[0])-192, GetY(CharsGrenade[0]), CharsGrenade[0]);
  CharsGrenade[1]->SetPosition(GetX(CharsGrenade[1])-192, GetY(CharsGrenade[1]), CharsGrenade[1]);
  CharsGrenade[2]->SetPosition(GetX(CharsGrenade[2])-192, GetY(CharsGrenade[2]), CharsGrenade[2]);

  //HUD unsichtbar machen
  SetVisibility(VIS_None);
}

public func ShowWeapons()
{
  if(GetVisibility() == VIS_Owner)
    return true;

  //Alle Munitionsanzeigen sichtbar machen
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

  //Granatenwert an Ursprungsort zurückverschieben
  CharsGrenade[0]->SetPosition(GetX(CharsGrenade[0])+192, GetY(CharsGrenade[0]), CharsGrenade[0]);
  CharsGrenade[1]->SetPosition(GetX(CharsGrenade[1])+192, GetY(CharsGrenade[1]), CharsGrenade[1]);
  CharsGrenade[2]->SetPosition(GetX(CharsGrenade[2])+192, GetY(CharsGrenade[2]), CharsGrenade[2]);

  //HUD sichtbar machen
  SetVisibility(VIS_Owner);
}

protected func UpdateHUD(object weapon, object pClonk, bool fForceUpdate)
{
  if(!pClonk) return true;  

  //Granatenanzeige aktualisieren
  var grenades = Format("%d", pClonk->~GrenadeCount());
  CharsGrenade[1]->Set(GetChar(grenades));

  //Keine Waffe: HUD unsichtbar machen
  if(!weapon) return HideWeapons();

  //Waffe gewechselt: Balken unsichtbar machen
  if(pLastItem != weapon)
  {  
    SetVisibility(VIS_None, rechargebar);
    pLastItem = weapon;
    iLastWeaponAmmo = -1;
    iLastWeaponFM = -1;
    iLastWeaponFT = -1;
  }

  //Keine Munition: Clonk-Munitionswert unsichtbar machen
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
    var clonkAmmo = GetAmmo(weapon->~GetFMData(FM_AmmoID), pClonk);
    if(fForceUpdate || iLastWeaponAmmo != weaponAmmo || iLastClonkAmmo != clonkAmmo|| iLastWeaponFM != weapon->~GetFireMode() || iLastWeaponFT != weapon->~GetFireTec()) {
      iLastWeaponAmmo = weaponAmmo;
      iLastClonkAmmo = clonkAmmo;
      iLastWeaponFM = weapon->~GetFireMode();
      iLastWeaponFT = weapon->~GetFireTec();
      var maxAmmo = weapon->~GetFMData(FM_AmmoLoad);
      var wAmmo = Format("%03d", weaponAmmo);
      var clr = ColorEmpty()*(!weaponAmmo);
      if(Inside(weaponAmmo, 1, maxAmmo/3))
        clr = ColorLow();

      var i = 0;
      if(weapon)
      for(var char in CharsWAmmo)
      {
        char->Set(GetChar(wAmmo, i), clr);
        i++;
      }

      i = 0;

      //Maximal ladbare Munition
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
        //Munition des Munitionsgürtels zeichnen
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

      CustomMessage(str, this, NO_OWNER, 0, 60, 0, 0, 0, MSG_NoLinebreak);
    }
    if(weapon->IsRecharging())
    {
      SetVisibility(VIS_Owner, rechargebar);
      var recharge = weapon->GetRecharge();
      var x = -122+(244*recharge/100);
      SetPosition(GetX()+x, GetY()-15, rechargebar);
    }
    else if(weapon->IsReloading())
    {
      SetVisibility(VIS_Owner, rechargebar);
      var charge = (weapon->GetCharge()/10);
      var x = -122+(244*charge/100);
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

public func Update(object weapon, object ammobag, object who, bool fForceUpdate)
{
  UpdateHUD(weapon, who, fForceUpdate);
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

  var x = -122+(244*part/max);
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
    CustomMessage(Format("@%s",szName), this, NO_OWNER, 0, 60, 0, 0, 0, MSG_Left);

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

/* Bei Entfernung alle Zeichen mitlöschen */

public func Destruction()
{
  for(var char in CharsWAmmo)
  {
    RemoveObject(char);
  }
  for(var char in CharsMaxAmmo)
  {
    RemoveObject(char);
  }
  for(var char in CharsClonkAmmo)
  {
    RemoveObject(char);
  }
  for(var char in CharsGrenade)
  {
    RemoveObject(char);
  }
  if(CharEqS)
    RemoveObject(CharEqS);
  if(rechargebar)
    RemoveObject(rechargebar);
}