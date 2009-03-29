/*-- VehicleHUD --*/

#strict

local clonk,target,
      //alpha,
      flash,
      charge, recharge, modusName,
      fuelID, fuelCount, fuelLoad, masterFuelCount,
      infoID;

public func Set(object pTarget, object pClonk)
{
  target = pTarget;
  clonk = pClonk;
  SetOwner(GetOwner(pClonk));
  Timer();
}
      
protected func Initialize()
{
  // Einstellungen
  SetVisibility(VIS_Owner);
  SetPosition(75, -150);
  // Infos
  SetGraphics("Row", this(), GetID(), 1, 1);
  //alpha = 150;
  infoID = CreateObject(2HUD, 0,0, GetOwner());
  infoID->SetPosition(150, -150);
  // Update
  Timer();
  return(1);
}

protected func Destruction()
{
  if(infoID) infoID->RemoveObject();
}

/* Timer */

protected func Timer()
{
  // Spieler weg?
  if(GetOwner() == NO_OWNER || !GetPlayerName(GetOwner())) return(SetVisibility(VIS_None));
  
  // Clonk falsch?
  SetVisibility(VIS_Owner);
  SetVisibility(VIS_Owner,infoID);
  if(GetCursor(GetOwner())!=clonk)
  {
    SetVisibility(VIS_None);
    SetVisibility(VIS_None,infoID);
  } 
  
  if(!target) return();
  if(!clonk) return();
  
  target->~UpdateHUD(this());
  
  // Faden
  /*if(alpha < 150) alpha++;
  if(alpha > 150 && charge != 0) alpha = 150;
  SetClrModulation(RGBa(255,255,255, alpha));
  SetClrModulation(RGBa(255,255,255, alpha), 0, 1);
  SetClrModulation(RGBa(255,255,255, alpha), infoID);*/
  
  if(flash)
  {
    var mod  = Sin(flash,100);
    SetClrModulation(RGBa(255,255,255,mod));
    flash += 10;
  }
  
  // Anzeige
  SetObjDrawTransform(charge * 10, 0, -1000 * (100-charge)/2,0, 1000, 0, this(), 1);
  if(recharge)
  {
    SetGraphics("Red", this(), GetID(), 2, 2, "Red");
    SetObjDrawTransform(1000, 0, 50000 -1000 * (100-recharge),0, 1000, 0, this(), 2);
  }
  else
    SetGraphics(0,0,0, 2);
}

/* Aufrufe */

public func Charge(int iCharge)
{
  if(iCharge == charge) return();
  charge = iCharge;
  //alpha = 80;
  //if(!charge) alpha = 200;
  return(1);
}

public func Recharge(int iRecharge)
{
  if(iRecharge == recharge) return();
  recharge = iRecharge;
  return(1);
}

public func Fuel(int iFuelCount, int iFuelLoad, string sModusName)
{
  if(iFuelCount == fuelCount && iFuelLoad == fuelLoad && sModusName == modusName) return();
  fuelCount = iFuelCount;
  fuelLoad = iFuelLoad;
  modusName = sModusName;
  var color = "ffff00";
  if(!fuelCount) color = "ff0000";
    Message("@%s|<c %s>%d/%d</c>", this(), modusName, color, fuelCount, fuelLoad);
  return(1);
}

public func MasterFuel(id idFuelID, int iMasterFuelCount)
{
  if(!infoID) return(); // Panique!
  if(idFuelID == fuelID && iMasterFuelCount == masterFuelCount) return();
  fuelID = idFuelID;
  masterFuelCount = iMasterFuelCount;
  SetGraphics(0, infoID, fuelID, 1, 4);
  //if(ammoID) SetClrModulation(RGBa(255,255,255, 128), infoID, 1);
  if(masterFuelCount > -1) {
    var color = "ffff00";
    if(!masterFuelCount) color = "ff0000";
    Message("@<c %s>%d</c>", infoID, color, masterFuelCount);
  }
  else
    Message("", infoID);
  return(1);
}

public func Clear()
{
  Charge();
  Recharge();
  Fuel();
  MasterFuel(0, -1);
}

public func SetFlash(bool fFlash)
{
  if(fFlash)
  {
   if(!flash)
     flash = 1;
  }
  else
  {
    flash = 0;
    SetClrModulation(RGBa(255,255,255));
  }
}