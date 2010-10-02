/*-- HUD --*/

#strict 2

local ammoobjs,allAmmo,
      alpha,
      charge, recharge, ftName,
      ammoID, ammoCount, ammoLoad, masterAmmoCount,
      info;

static const HUD_Grenade_Icon = GRNS;

public func GraphicsID()	{return 1HUD;}
public func ColorTrue()		{return RGB(255,255,0);}
public func ColorFalse()	{return RGB(255,0,0);}
public func ColorAmmoID()	{return RGB(255,255,255);}
public func IdleAlpha()		{return 150;}


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

/* Initialisierung */
      
protected func Initialize()
{
  //Einstellungen
  SetPosition(85, -55);
  SetGraphics(0, this, GraphicsID());
  SetGraphics("Row", this, GraphicsID(), 1, 1);
  SetVisibility(VIS_None);

  //Infos
  info = CreateObject(2HUD, 0,0, GetOwner());
  SetPosition(168, -23, info);//150, -55
  
  //Ammobag
  InitAmmoBagHUD();
}

private func InitAmmoBagHUD()
{
  //Munitionsanzeige entfernen
  var def = 0;
  allAmmo = CreateArray();
  var arrI = 0;

  //Granaten
  allAmmo[arrI++] = HUD_Grenade_Icon;

  //Munition erfassen
  for(var i=0; def = GetDefinition(i,C4D_StaticBack); ++i)
  {
    if(DefinitionCall(def,"IsHUDAmmo"))
      allAmmo[arrI++] = def;
  }

  ammoobjs = CreateArray();

  //Munitionsobjekte plazieren
  for(var i=0; i < GetLength(allAmmo); ++i)
  {
    ammoobjs[i] = CreateObject(2HUD,0,0,GetOwner());
    SetPosition(70,-100-i*28, ammoobjs[i]);
    SetGraphics(0, ammoobjs[i], allAmmo[i],1,4);
    SetObjDrawTransform(750,0,-50000,0,750,-62000,ammoobjs[i],1);
    SetVisibility(VIS_None, ammoobjs[i]);
  }
}

protected func Destruction()
{
  if(info) RemoveObject(info);
}

/* Timer */

protected func Timer()
{
  //Spieler weg?
  if(GetOwner() == NO_OWNER || !GetPlayerName(GetOwner())) return RemoveHUD();
  //Clonk falsch?
  if(!GetCursor(GetOwner()))
  {
    SetVisibility(VIS_None);
    SetVisibility(VIS_None,info);
    return;
  }
  else
  {
   SetVisibility(VIS_Owner);
   SetVisibility(VIS_Owner,info);
  }

  //Anzeige
  SetObjDrawTransform(charge, 0, -1000 * (1000-charge)/20,0, 1000, 0, this, 1);
  
  //Recharge: Roter Balken wandert von links nach rechts
  if(recharge)
  {
    SetGraphics("Red", this, GetID(), 2, 2, "Red");
    SetObjDrawTransform(1000, 0, 50000 - 100 * (1000-recharge),0, 1000, 0, this, 2);
  }
  else
  {
    SetGraphics(0,0,0, 2);
  }
}

/* Aufrufe */

public func Update(object weapon, object ammobag, object who)
{
  //Nur den Cursor updaten
  if(who)
  {
    var pCursor = GetCursor(GetOwner(who));
    if(pCursor != who) pCursor = pCursor->~GetRealCursor();
    if(pCursor == who)
    {
      UpdateWeapon(weapon);
      UpdateAmmoBag(ammobag, weapon, who);
    }
  }
}

private func UpdateWeapon(object weapon)
{
  //Munitionsstand anzeigen
  if(weapon)
  {
    if(weapon->~CustomHUD())
    {
      recharge = 0;
      SetVisibility(VIS_Owner);
      Message("",info);
      Message("",this);
      weapon->UpdateHUD(this);
      ShowSelectProcess(weapon);
      return;
    }
    else if(weapon->~IsWeapon())
    {
      //Recharge updaten
      if(weapon->IsRecharging())
        recharge = weapon->GetRecharge()*10;
      else
        recharge = 0;

      //Charge updaten
      charge = weapon->GetCharge();

      //Nachricht ausgeben
      SetVisibility(VIS_Owner);

      var ammoid = weapon->GetFMData(FM_AmmoID);
      var ammoload = weapon->GetFMData(FM_AmmoLoad);
      var modusname = weapon->GetFMData(FM_Name);
      var ammocount = weapon->GetAmmo(ammoid);

      if(weapon->~IsWeapon2() && weapon->GetFMData(FT_Name))
        Message("@%s|<c %x>%s</c>", this, modusname, RGB(128,128,128), weapon->GetFMData(FT_Name));
      else
        Message("@%s", this, modusname);

      var color = ColorTrue();
      if(!ammocount) color = ColorFalse();
        Message("@<c %x>%d/%d</c>", info, color, ammocount, ammoload);

      ShowSelectProcess(weapon);
      return;
    }
    else
    if(weapon->~SelectionTime())
    {
      SetVisibility(VIS_Owner);
      Message("",info);
      Message("",this);
      ShowSelectProcess(weapon);
      return;
    }
  }
  //Keine Waffe: Ausblenden
  SetVisibility(VIS_None);
  Message("",info);
  Message("",this);
  charge = 0;
  recharge = 0;
}

private func UpdateAmmoBag(object ammobag, object weapon, object who)
{
  var pCursor = GetCursor(GetOwner(who));
  if(pCursor != who) pCursor = pCursor->~GetRealCursor();

  var ammoid = 0;
  var handle_grenade, is_grenade = false;
  if(weapon)
  {
    if(weapon->~IsWeapon())
      ammoid = weapon->GetFMData(FM_AmmoID);
    if(weapon->~IsGrenade())
      is_grenade = true;
  }
  for(var i=0; i < GetLength(ammoobjs); ++i)
  {
    if(allAmmo[i] == HUD_Grenade_Icon)
      handle_grenade = true;
    else
      handle_grenade = false;
    var aobj = ammoobjs[i];

   //Unsichtbar bei "Keine Munition"-Regel
   if((NoAmmo() && !handle_grenade) || !ammobag)
   {
     SetVisibility(VIS_None, aobj);
     Message("",aobj);
     continue;
   }
   //Ansonsten Munitionsstand anzeigen
   SetVisibility(VIS_Owner, aobj);

   var ammo = 0;
   if(ammobag && !handle_grenade)
     ammo = ammobag->GetAmmo(allAmmo[i]);

   if(handle_grenade && pCursor->~GetGrenadeStoring())
     ammo = pCursor->~GrenadeCount();

   var color = "eeeeee";
     if(!ammo) color = "777777";
      if(allAmmo[i] == ammoid || (is_grenade && handle_grenade))
      {
        color = "ffff00";
        if(!ammo) color = "ff0000";
      }
   Message("@<c %s>%d</c>", aobj, color, ammo);
  }
}

private func ShowSelectProcess(object item)
{
  if(GetEffect("SelectItem",item))
  {
    Recharge(GetEffect("SelectItem",item,0,6),item->~SelectionTime());
  }
}


public func Charge(int part, int max)
{
  charge = part*1000/max;
  alpha = 80;
  if(!charge) alpha = 200;
  return 1;
}

public func Recharge(int part, int max)
{
  recharge = part*1000/max;
  return 1;
}

public func Ammo(int iAmmoCount, int iAmmoLoad, string sftName, bool fShow)
{
  //if(iAmmoCount == ammoCount && iAmmoLoad == ammoLoad && sftName == ftName) return;
  ammoCount = iAmmoCount;
  ammoLoad = iAmmoLoad;
  ftName = sftName;

  if(fShow) SetClrModulation(RGBa(255,255,255,0));
  else      SetClrModulation(RGBa(255,255,255,255));

  var color = ColorTrue();
  if(!ammoCount) color = ColorFalse();
  if((ammoCount && ammoLoad) || fShow)
    Message("@%s|<c %x>%d/%d</c>", this, ftName, color, ammoCount, ammoLoad);
  else
    Message("", this);
  return 1;
}

public func MasterAmmo(id idAmmoID, int iMasterAmmoCount)
{
  if(!info) return; // Panique!
  if(idAmmoID == ammoID && iMasterAmmoCount == masterAmmoCount) return;
  ammoID = idAmmoID;
  masterAmmoCount = iMasterAmmoCount;
  
  SetGraphics(0, info, ammoID, 1, 4);
  if(ammoID)
    SetClrModulation(ColorAmmoID(), info, 1);
  
  if(masterAmmoCount > -1)
  {
    var color = ColorTrue();
    if(!masterAmmoCount) color = ColorFalse();
    Message("@<c %x>%d</c>", info, color, masterAmmoCount);
  }
  else
    Message("", info);
  return 1;
}

public func Clear()
{
  Charge();
  Recharge();
  Ammo();
  MasterAmmo(0, -1);
}

public func RemoveHUD()
{
  for (var obj in ammoobjs)
    if (obj)
      RemoveObject(obj);
  RemoveObject(info);
  RemoveObject();
  return true;
}