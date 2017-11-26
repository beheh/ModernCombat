/*--- Head-Up-Display ---*/

#strict 2


local CharsAmmo,	//Munition (Erste Zahl - Munitionswert)
      CharsSupply,	//Nachschub (Zweite Zahl - Clonk-Munitionswert)
      CharsGrenade,	//Granate (Dritte Zahl - Clonk-Handgranatenwert)
      CharSlash,	//Schrägstrich
      CharInfinity,	//Unendlichkeit
      rechargebar;	//Aktionsbalken

local pLastItem;
local iLastWeaponAmmo;
local iLastClonkAmmo;
local iLastWeaponFM;
local iLastWeaponFT;
local TextVisible;

public func ColorEmpty()	{return RGB(255, 0, 0);}
public func ColorLow()		{return RGB(255, 150, 0);}


/* Initialisierung */

protected func Initialize()
{
  //Am unteren, linken Bildschirmrand positionieren
  SetPosition(161, -80); 

  //Bestandteile erstellen
  //Munition
  CharsAmmo = [CreateObject(HCHA, -62, 7, GetOwner()), CreateObject(HCHA, -35, 7, GetOwner()), CreateObject(HCHA, -8, 7, GetOwner())];
  //Schrägstrich
  CharSlash = CreateObject(HCHA, 15, 15, GetOwner());
  CharSlash->Set(61);
  ResizeChars(CharSlash, 600);
  //Unendlichkeit
  CharInfinity = CreateObject(HCHA, 39, 14, GetOwner());
  CharInfinity->Set(120);
  SetVisibility(VIS_None, CharInfinity);
  //Nachschub
  CharsSupply = [CreateObject(HCHA, 33, 14, GetOwner()), CreateObject(HCHA, 48, 14, GetOwner()), CreateObject(HCHA, 63, 14, GetOwner())];
  ResizeChars(CharsSupply, 580);
  //Granate
  CharsGrenade = [CreateObject(HCHA, 101, 16, GetOwner()), CreateObject(HCHA, 115, 15, GetOwner())];
  CharsGrenade[0]->Set(1337);
  ResizeChars(CharsGrenade, 520);
  //Aktionsbalken
  rechargebar = CreateObject(RBAR, 0, -15, GetOwner()); 

  //Werte initialisieren
  iLastWeaponAmmo = -1;
  iLastWeaponFT = -1;
  iLastWeaponFM = -1;

  //Sichtbarkeit nur für Besitzer
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
  //Existenz des Spielers überwachen
  if(GetOwner() == NO_OWNER || !GetPlayerName(GetOwner()) || (!GetAlive(GetCrew(GetOwner())))) 
    RemoveObject(this);
}

/* Waffen-Anzeige ein- und ausblenden */

public func HideWeapons(object pClonk)
{
  pLastItem = 0;

  if(GetVisibility() == VIS_None)
    return true;

  //Munition, Nachschub, Schrägstrich, Unendlichkeit und Aktionsbalken unsichtbar machen
  for(var char in CharsAmmo)
    SetVisibility(VIS_None, char);
  for(var char in CharsSupply)
    SetVisibility(VIS_None, char);
  SetVisibility(VIS_None, CharSlash);
  SetVisibility(VIS_None, CharInfinity);
  SetVisibility(VIS_None, rechargebar);

  //Anderer Objekttyp vorhanden: Namen anzeigen
  if(Contents(0,pClonk))
  {
    CustomMessage(Format("@%s",GetName(Contents(0,pClonk))), this, NO_OWNER, 0, 70, 0, 0, 0, MSG_NoLinebreak);
    TextVisible = true;
  }
  else
    if(TextVisible)
    {
      CustomMessage("", this, NO_OWNER);
      TextVisible = false;
    }
}

public func ShowWeapons()
{
  if(GetVisibility() == VIS_Owner)
    return true;

  //Munition, Nachschub und Schrägstrich sichtbar machen
  for(var char in CharsAmmo)
    SetVisibility(VIS_Owner, char);
  for(var char in CharsSupply)
    SetVisibility(VIS_Owner, char);
  SetVisibility(VIS_Owner, CharSlash);
}

/* HUD-Aktualisierung */

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

  //Munition und Nachschub bei Waffen einblenden, ansonsten ausblenden
  if(weapon && !(weapon->~IsWeapon()) && !(weapon->~CustomHUD()))
    HideWeapons(pClonk);
  else if(GetVisibility() == VIS_None)
    ShowWeapons();

  //Ausrüstung mit eigener Anzeige
  if(weapon->~CustomHUD())
  {
    //Unendlichkeitsicon ausblenden
    if(GetVisibility(CharInfinity) == VIS_Owner)
      SetVisibility(VIS_None, CharInfinity);

    //Schrägstrich einblenden
    if(GetVisibility(CharSlash) == VIS_None)
      SetVisibility(VIS_Owner, CharSlash);

    //Aktionsbalken bei Bedarf ausblenden
    if(GetVisibility(rechargebar) != VIS_None && weapon->~IsWeapon())
      if(!weapon->IsRecharging() && !weapon->IsReloading())
        SetVisibility(VIS_None, rechargebar);

    ShowSelectProcess(weapon);
    return weapon->~UpdateHUD(this);
  }

  //Waffe
  if(weapon->~IsWeapon())
  {
    //Munition in Waffe ermitteln
    var weaponAmmo = GetAmmo(weapon->~GetFMData(FM_AmmoID), weapon);
    var clonkAmmo = GetAmmo(weapon->~GetFMData(FM_AmmoID), pClonk);
    if(fForceUpdate || iLastWeaponAmmo != weaponAmmo || iLastClonkAmmo != clonkAmmo|| iLastWeaponFM != weapon->~GetFireMode() || iLastWeaponFT != weapon->~GetFireTec())
    {
      iLastWeaponAmmo = weaponAmmo;
      iLastClonkAmmo = clonkAmmo;
      iLastWeaponFM = weapon->~GetFireMode();
      iLastWeaponFT = weapon->~GetFireTec();
      var maxAmmo = weapon->~GetFMData(FM_AmmoLoad);
      var wAmmo = Format("%03d", weaponAmmo);

      //Textfarbe ermitteln
      var clr = ColorEmpty()*(!weaponAmmo);
      if(Inside(weaponAmmo, 1, maxAmmo/3))
        clr = ColorLow();

      var i = 0;
      if(weapon)
      for(var char in CharsAmmo)
      {
        char->Set(GetChar(wAmmo, i), clr);
        i++;
      }

      i = 0;
      //Nachschub darstellen wenn der Nutzer der Bediener ist und keine Keine Munition-Regel vorhanden ist
      if(!NoAmmo() && weapon->~GetUser() == pClonk)
      {
        //Unendlichkeitsicon ausblenden
        if(GetVisibility(CharInfinity) == VIS_Owner)
          SetVisibility(VIS_None, CharInfinity);

        //Nachschub aktualisieren
        var cAmmo = Format("%03d", clonkAmmo);
        if(clonkAmmo > 0)
        {
          for(var char in CharsSupply)
          {
            char->Set(GetChar(cAmmo, i), RGB(255, 255, 255));
            i++;
          }
        }
        else
          for(var char in CharsSupply)
            char->Set(48, ColorEmpty());

        //Schrägstrich einblenden
        if(GetVisibility(CharSlash) == VIS_None)
          SetVisibility(VIS_Owner, CharSlash);
      }
      else
      //Ansonsten Unendlichkeit und Schrägstrich einblenden und Nachschub ausblenden
      {
        //Unendlichkeitsicon einfügen
        if(GetVisibility(CharInfinity) == VIS_None)
          SetVisibility(VIS_Owner, CharInfinity);

        //Schrägstrich einblenden
        if(GetVisibility(CharSlash) == VIS_None)
          SetVisibility(VIS_Owner, CharSlash);

        //Nachschub ausblenden
        for(var char in CharsSupply)
          SetVisibility(VIS_None, char);
      }

      //Textanzeige: Feuermodus und Feuertechnik
      var ammoName = weapon->~GetFMData(FM_Name);
      if(weapon->~IsWeapon2() && weapon->GetFMData(FT_Name))
        var firemode = weapon->~GetFMData(FT_Name);
      else
        var firemode = weapon->~GetFMData(FM_Name);

      var str = Format("@<c ffbb00>%s</c> - %s", ammoName, firemode);
      LimitString(str, 29 + 15);

      CustomMessage(str, this, NO_OWNER, 0, 70, 0, 0, 0, MSG_NoLinebreak);
    }
    //Aktionsbalken-Position bei Bedarf repositionieren
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
      //!
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
    Recharge(effect,item->~SelectionTime());

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

/* Benutzerdefiniertes HUD (Ausrüstung) */

public func Ammo(int iAmmoCount, int iAmmoLoad, string szName, bool fShow, int dwColorW, int dwColorM)
{
  var wAmmo = Format("%03d", iAmmoCount);
  var i = 0;

  //Munition aktualisieren
  if(!dwColorW) dwColorW = ColorEmpty()*(!iAmmoCount);
  for(var char in CharsAmmo)
  {
    char->Set(GetChar(wAmmo, i), dwColorW);
    i++;
  }

  //Nachschub aktualisieren
  var mAmmo = Format("%03d", iAmmoLoad);
  var i = 0;
  for(var char in CharsSupply)
  {
    char->Set(GetChar(mAmmo, i), dwColorM);
    i++;
  }

  //Wenn angegeben, Nachschub und Schrägstrich ausblenden
  if(!fShow)
  {
    if(GetVisibility(CharsSupply[0]) == VIS_Owner)
    {
      for(var char in CharsSupply)
        SetVisibility(VIS_None, char);

      if(GetVisibility(CharSlash) == VIS_Owner)
        SetVisibility(VIS_None, CharSlash);
    }
  }

  //Text einblenden falls vorhanden
  if(szName)
    CustomMessage(Format("@%s",szName), this, NO_OWNER, 0, 70, 0, 0, 0, MSG_NoLinebreak);

  return 1;
}

/* HUD-Nachschubinfo (Munitionsaufnahme) */

public func ShowResupplyInfo(id SupplyType, int SupplyAmount, int iPos)
{
  //Nachschubhinweis einblenden
  var ix,iy, iz;
  if(!iPos) {ix=-49; iy=12;}
  else {ix=-12; iy=13; iz=1;}

  //Munition oder Handgranate
  if(SupplyType->~IsAmmo() && !NoAmmo())
    CustomMessage(Format("<c ffbb00>+%d</c>{{%i}}",SupplyAmount,SupplyType), CharsGrenade[iz], NO_OWNER, ix, iy, 0, 0, 0, MSG_NoLinebreak);
  else
    if(SupplyType->~IsGrenade())
      CustomMessage(Format("<c ffbb00>+</c>{{%i}}",SupplyType), CharsGrenade[iz], NO_OWNER, ix, iy, 0, 0, 0, MSG_NoLinebreak);
    else
      return;

  return 1;
}

global func ResupplyInfo(object user, id SupplyType, int SupplyAmount, int iPos)
{
  //Spieler existiert und aufrufender Clonk wird gerade gesteuert und nicht innerhalb eines Spawnobjekts
  if(user && GetCursor(GetOwner(user)) == user)
    if(!Contained(user) || !Contained(user)->~IsSpawnObject())
      GetHUD(user)->~ShowResupplyInfo(SupplyType,SupplyAmount,iPos);
}

/* Bei Entfernung alle Zeichen mitlöschen */

public func Destruction()
{
  for(var char in CharsAmmo)
  {
    RemoveObject(char);
  }
  for(var char in CharsSupply)
  {
    RemoveObject(char);
  }
  for(var char in CharsGrenade)
  {
    RemoveObject(char);
  }
  if(CharSlash)
    RemoveObject(CharSlash);
  if(CharInfinity)
    RemoveObject(CharInfinity);
  if(rechargebar)
    RemoveObject(rechargebar);
}