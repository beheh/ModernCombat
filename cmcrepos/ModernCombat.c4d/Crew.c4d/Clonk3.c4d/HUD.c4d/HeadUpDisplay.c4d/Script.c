
/*-- HUD --*/

#strict

local alpha,
      charge, recharge, ftName,
      ammoID, ammoCount, ammoLoad, masterAmmoCount,
      info;

global func GetHUD(object obj, id theID)
{
  if(!obj)
    if(!(obj = this()))
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
      
protected func Initialize()
{
  // Einstellungen
  SetPosition(85, -55);//75, -55
  SetGraphics(0, this(), GraphicsID());
  SetGraphics("Row", this(), GraphicsID(), 1, 1);
  SetVisibility(VIS_None);
  // Infos
  info = CreateObject(2HUD, 0,0, GetOwner());
  info->SetPosition(168, -23);//150, -55
  
  // Ammobag
  //InitAmmoBagHUD();
}

private func InitAmmoBagHUD(){}

protected func Destruction()
{
  if(info) info->RemoveObject();
}

/* Timer */

protected func Timer()
{
  // Spieler weg?
  if(GetOwner() == NO_OWNER || !GetPlayerName(GetOwner())) return(RemoveObject());
  // Clonk falsch?
  if(!GetCursor(GetOwner()))
  {
    SetVisibility(VIS_None);
    SetVisibility(VIS_None,info);
    return();
  }
  else
  {
    SetVisibility(VIS_Owner);
    SetVisibility(VIS_Owner,info);
  }

  // Faden
  /*if(alpha < IdleAlpha()) alpha++;
  if(alpha > IdleAlpha() && charge != 0) alpha = IdleAlpha();
  SetClrModulation(RGBa(255,255,255,alpha));
  SetClrModulation(RGBa(255,255,255,alpha), 0, 1);*/

  // Anzeige
  SetObjDrawTransform(charge, 0, -1000 * (1000-charge)/20,0, 1000, 0, this(), 1);
  
  // Recharge: roter Balken wandert von links nach rechts
  if(recharge)
  {
    SetGraphics("Red", this, GetID(), 2, 2, "Red");
    SetObjDrawTransform(1000, 0, 50000 -1000 * (100-recharge),0, 1000, 0, this(), 2);
  }
  else
  {
    SetGraphics(0,0,0, 2);
  }
}

/* Aufrufe */

public func Update(object weapon, object ammobag, object who)
{
	// Nur den Cursor updaten
	if(who)
		if(GetCursor(GetOwner(who)) == who)
    {
			UpdateWeapon(weapon);
    }
}

private func UpdateWeapon(object weapon)
{
	// Munitionsstand anzeigen
	if(weapon)
  {
		if(weapon->~IsWeapon())
    {
			// Recharge updaten
			if(weapon->IsRecharging())
				recharge = weapon->GetRecharge();
			else
				recharge = 0;
				
			// charge updaten
			charge = weapon->GetCharge();
		
			// Nachricht ausgeben
			SetVisibility(VIS_Owner);
			
			var ammoid = weapon->GetFMData(FM_AmmoID);
			var ammoload = weapon->GetFMData(FM_AmmoLoad);
			var modusname = weapon->GetFMData(FM_Name);
			var ammocount = weapon->GetAmmo(ammoid);
		  
			var color = "ffff00";
			if(!ammocount) color = "ff0000";
			Message("@%s", this, modusname);
			Message("@<c %s>%d/%d</c>", info, color, ammocount, ammoload);
			
			return;
		}
	}
	// keine Waffe: ausblenden
	SetVisibility(VIS_None);
	Message("",info);
	Message("",this);
	charge = 0;
	recharge = 0;
}

private func UpdateAmmoBag()
{
	//...
}

//Altes Zeugs...

public func Charge(int iCharge)
{
  if(iCharge == charge) return();
  charge = iCharge;
  alpha = 80;
  if(!charge) alpha = 200;
  return(1);
}

public func Recharge(int iRecharge)
{
  if(iRecharge == recharge) return();
  recharge = iRecharge;
  return(1);
}

public func Ammo(int iAmmoCount, int iAmmoLoad, string sftName, bool fShow)
{
  if(iAmmoCount == ammoCount && iAmmoLoad == ammoLoad && sftName == ftName) return();
  ammoCount = iAmmoCount;
  ammoLoad = iAmmoLoad;
  ftName = sftName;
  
  if(fShow) SetClrModulation(RGBa(255,255,255,0));
  else      SetClrModulation(RGBa(255,255,255,255));
  
  var color = ColorTrue();
  if(!ammoCount) color = ColorFalse();
  if((ammoCount != 0 && ammoLoad != 0) || fShow)
    Message("@%s|<c %x>%d/%d</c>", this(), ftName, color, ammoCount, ammoLoad);
  else
    Message("", this());
  return(1);
}

public func MasterAmmo(id idAmmoID, int iMasterAmmoCount)
{
  if(!info) return(); // Panique!
  if(idAmmoID == ammoID && iMasterAmmoCount == masterAmmoCount) return();
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
  return(1);
}

public func Clear()
{
  Charge();
  Recharge();
  Ammo();
  MasterAmmo(0, -1);
}

//Modding!
public func GraphicsID(){return(1HUD);}
public func ColorTrue(){return(RGB(255,255,0));}
public func ColorFalse(){return(RGB(255,0,0));}
public func ColorAmmoID(){return(RGB(255,255,255));}
public func IdleAlpha(){return(150);}