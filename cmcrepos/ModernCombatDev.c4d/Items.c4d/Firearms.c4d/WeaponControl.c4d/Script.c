/*-- Waffe 2.0 --*/

#strict 2

/* Konstanten */

static const WeaponDrawLayer = 1; //Layer in dem die Waffen gezeichnet werden
//Sollte vllt. mal per Callback in Zielobjekt gemacht werden. Wenn das 0 ergibt wird der Defaultwert benutzt.

//Feuermodus
static const FM_Name      = 1;  // Name des Feuermodus'
static const FM_AmmoID    = 2;  // ID der benutzten Munition
static const FM_AmmoLoad  = 3;  // Menge der eingeladenen Munition
static const FM_AmmoUsage = 4;  // Munitionsverbrauch pro AmmoRate Schüsse
static const FM_AmmoRate  = 5;  // s.o.
static const FM_Reload    = 6;  // Nachladezeit der Waffe in Frames.
static const FM_Recharge  = 7;  // Zeitabstand zwischen zwei Schüssen in Frames.
static const FM_Auto      = 8;  // Automatische Waffe.
static const FM_Aim       = 9;  // Waffe zielt
static const FM_Condition =10;  // Vorraussetzung für den Schussmodus
static const FM_Icon      =11;  // Icon das im Schussmodimenü angezeigt wird
static const FM_Damage    =12;  // Schaden, den der Schussmodus anrichtet

static const FM_Slot          = 13; // Der Slot vom Feuermodus.
static const FM_SingleReload  = 14; // Munition wird einzeln nachgeladen. (z.B. für Shotguns)
static const FM_PrepareReload = 15; // Zeit bevor das eigentliche Nachladen beginnt. (Nur interessant wenn auch FM_SingleReload benutzt wird.)
static const FM_FinishReload  = 16; // Zeit nach dem Nachladen. (Nur interessant wenn auch FM_SingleReload benutzt wird.)
static const FM_BurstAmount   = 17;
static const FM_BurstRecharge = 18;
static const FM_SpreadAdd     = 19;
static const FM_StartSpread   = 20;
static const FM_MaxSpread     = 21;
static const FM_UnSpread      = 22;

// Feuertechnik
static const FT_Name          = 23; // Name der Feuertechnik.
static const FT_Icon          = 24; // Icondefinition der Feuertechnik.
static const FT_IconFacet     = 25; // Facet. Wie bei AddMenuItem.
static const FT_Condition     = 26; // Wie FM_Condition. Nur eben für Feuertechniken.


// Informationen für KIs
static const BOT_Range    = 100;  // Reichweite des Modus
static const BOT_DmgType  = 101;  // Schadenstyp, den der Modus hauptsächlich anrichtet

static const BOT_Ballistic= 102;  // Der Modus schießt ballistische Geschosse

static const BOT_Power    = 103;  // Stärke der Waffe
static const BOT_Power_1  = 1;    // geringe Stärke (Pistole, Pistolen-Granatwerfer, Pistole-Laser, Pistole-Schleim, EMP)
static const BOT_Power_2  = 2;    // mittlere Stärke (Pumpgun, Energiegewehr, Flammenwerfer, Napalmgranate, Lasergranate)
static const BOT_Power_3  = 3;    // hohe Stärke (Granatwerfer: Normal, Cluster & Schleim, Motorsäge, Energiegewehr mit Laser, Biowerfer, Clusterpumpgun, Feuerball)
static const BOT_Power_LongLoad = 4;
                                  // Waffen, mit einer langen Nachladezeit (Partikelk., Bazooka, Minigun)
                                  // Diese Waffen werden nur leer geschossen und dann erstmal nicht benutzt
static const BOT_EMP      = 104;  // EMP-Waffe. Wird nur gegen Maschinen eingesetzt

public func IsWeapon() {return true;}
public func IsWeapon2() {return true;}//Diese Waffe benutzt das neue Waffensystem. Sie includiert also WPN2.
public func NoWeaponChoice() { return GetID() == WPN2; }

public func Default(int data)    // Standardeinstellungen
{
  if(data == FM_Name)      return "Standard";
  if(data == FM_AmmoID)    return STAM;
  if(data == FM_AmmoUsage) return 1;
  if(data == FM_AmmoRate)  return 1;
  if(data == FM_Auto)      return false;
  if(data == FM_Aim)       return 0;
  if(data == FM_Condition) return true;
  if(data == FM_AmmoLoad)  return 5;
  if(data == FM_Reload)    return 100;
  if(data == FM_Recharge)  return 10;
  if(data == FM_Icon)      return GetID();
  if(data == FM_Damage)    return 0;
  if(data == FM_Slot)      return 0;
  
  if(data == FT_Name)      return 0;//Wichtig!
  if(data == FT_Icon)      return FICO;
  if(data == FT_IconFacet) return 0;
  if(data == FT_Condition) return true;
  
  if(data == BOT_Range)    return 100;
  if(data == BOT_DmgType)  return DMG_Projectile;
  if(data == BOT_Ballistic)return 0;
  if(data == BOT_Power)    return BOT_Power_1;
  if(data == BOT_EMP)      return false;
}

public func GetFMData(int data, int i)
{
  if(!i) i = firemode;
  var value = ObjectCall(this, Format("FMData%d",i), data);

  var effect,user = GetUser(),j;
  while(effect = GetEffect("*Bonus*",this,j) || j == 0)
  {
    j++;
    if(!GetEffect("*Bonus*",this,effect,1))
      continue;
 
    var tval = EffectCall(this,effect,"FMData",data,value);
    if(tval)
      value = tval;
  }
  
  if(user)
  {
    for(var i; i <= GetEffectCount("*Bonus*",user); i++)
    {
      var tval = EffectCall(user,GetEffect("*Bonus*",user,i-1),"FMData",data,value,i);
      if(tval)
        value = tval;
    }
  }

  return value;
}

public func GetFMData(int data, int i, int t)
{
  if(!i) i = firemode;
  if(!t) t = GetFireTec(i);
  
  var value,ammoid;
  value = ObjectCall(this, Format("FMData%dT%d",i,t), data);
  ammoid = ObjectCall(this, Format("FMData%dT%d",i,t), FM_AmmoID);
    
  // Modifikationen
  /*var ammoid;
  if(CheckFireTec(t,i))
    ammoid = ObjectCall(this, Format("FMData%dT%d",i,t), FM_AmmoID);
  else
    ammoid = ObjectCall(this, Format("FMData%d",i), FM_AmmoID);*/
  
  if(ammoid == STAP)
    value = ammoid->~FMMod(data,value);

  var effect,user = GetUser(),j;
  while(effect = GetEffect("*Bonus*",this,j) || j == 0)
  {
    j++;
    if(!GetEffect("*Bonus*",this,effect,1))
      continue;

    var tval = EffectCall(this,effect,"FMData",data,value);
    if(tval)
      value = tval;
  }
  
  if(user)
  {
    for(var i; i <= GetEffectCount("*Bonus*",user); i++)
    {
      var tval = EffectCall(user,GetEffect("*Bonus*",user,i-1),"FMData",data,value,i);
      if(tval)
        value = tval;
    }
  }
  
  return value;
}

public func GetSpecialAmmo(int iSlot)
{
	return aSlot_SA[iSlot];
}

public func SetSpecialAmmo(id idAmmo, int iSlot)
{
	aSlot_SA[iSlot] = idAmmo;
}

public func GetFireTec(int iFM)
{
  if(!iFM) iFM = firemode;
  return aFM_FireTec[iFM-1];
}

public func SetFireTec(int iFT,int iFM, bool bNoCalls)
{
  if(!iFM) iFM = firemode;
  if(!GetFMData(FT_Condition,iFM,iFT)) return false;

  var last = aFM_FireTec[iFM-1];
  aFM_FireTec[iFM-1] = iFT;
  
  ratecount = GetFMData(FM_AmmoRate, iFM);
  
  if(!bNoCalls && (last != iFT))
  {
    Sound("WPN2_Switch.ogg");
    OnSelectFT(iFM,iFT,last);
  }
  
  return true;
}

public func CheckFireTec(int iFT, int iFM)
{
  if(!iFM) iFM = firemode;
  if(!iFT) iFT = GetFireTec(iFM);

  if(ObjectCall(this, Format("FMData%dT%d",iFM,iFT),FT_Name))
    return true;
  return false;
}

public func GetBotData(int data, int i)
{
  if(!i) i = firemode;
  return ObjectCall(this, Format("BotData%d",i), data);
}

public func HasAmmoSlots() { return true; }

/* Callbacks */

public func OnEmpty(int iFireMode)     { } // wird aufgerufen wenn Waffe leer (keine Munition)
public func OnSelect(int iFireMode)    { } // wird bei Anwahl im Inventar aufgerufen
public func OnDeselect(int iFireMode)  { } // wird bei Abwahl im Inventar aufgerufen

public func OnReload(int iFireMode)    { } // wird bei Start des Nachladens aufgerufen
public func OnReloaded(int iFireMode)  { } // wird nach erfolgreichem Nachladen aufgerufen

public func OnAutoStart(int iFireMode) { } // wird beim Starten einer autom. Waffe aufgerufen (erster Schuss)
public func OnAutoStop(int iFireMode)  { } // wird beim Stoppen einer atuom. Waffe aufgerufen

public func OnAimStart(int iFireMode)  { } // Anfangen zu zielen
public func OnAimStop(int iFireMode)   { } // Stoppen zu zielen

public func IsUpgradeable(id iFireMode){ } // Upgradebar?
public func OnUpgrade(id iFireMode)    { } // Beim Upgraden
public func OnDowngrade(id iFireMode)  { } // Beim Downgraden

public func OnSelectFT(int iFireMode, int iFireTec, int iLastFireTec) { } // Auswahl von Feuertechnik.

public func OnSingleReloadStart(int iSlot) { }
public func OnSingleReloadStop(int iSlot) { }
public func OnPrepareReloadStop(int iSlot) { }
public func OnFinishReloadStart(int iSlot) { }


/* Allgemein */

public func HandX()    { return 0; }    // X-Position in der Hand
public func HandY()    { return 0; }    // Y-Position in der Hand
public func HandSize() { return 1000; } // Größe in der Hand, Standard: 1000
public func HandBarrel(){return 0; }    // Y-Offset des Laufs

public func CanAim() { return true; }   // mit diesem Gegenstand kann man zielen


local firemode; // aktueller Feuermodus
local ratecount, stopauto, shooting; // intern...
local controller; // the object which controls the weapon (the hazard clonk)
local upgrades; // Welche Upgrades wir haben
local aSlot_Type; // Hält den Munitionstyp.
local aSlot_Amount; // Hält die Munitionsmenge.
local aSlot_SA; // Bei Spezialmuniton hält dies die ProjektilID.
local aFM_FireTec; // Hält die Feuertechnik.

public func Initialize()
{
  firemode = 1;
  stopauto = false;
  upgrades = CreateArray();
  aSlot_Type = CreateArray();
  aSlot_Amount = CreateArray();
  aSlot_SA = CreateArray();
  aFM_FireTec = CreateArray();
  
  var i = 1;
  while(CheckFireTec(1,i))
  {
    aFM_FireTec[i-1] = 1;//Defaults setzen.
    i++;
  }
  
	SetFireMode(1);
  SetUser();
}

public func SetUser(object pObj) { controller = pObj; }
public func GetUser() { return controller; }

public func GetSlot(int iFM)
{
  return GetFMData(FM_Slot,iFM);
}

public func GetSlotCount()
{
  return GetLength(aSlot_Type);
}

public func GetAmmoID(iSlot)
{
  if(!iSlot) iSlot = GetSlot();
  return aSlot_Type[iSlot-1];
}

public func SetAmmoID(id idType, int iSlot)
{
  if(!iSlot) iSlot = GetSlot();
  aSlot_Type[iSlot-1] = idType;
}

public func GetAmmoCount(iSlot)
{
  if(!iSlot) iSlot = GetSlot();
  return aSlot_Amount[iSlot-1];
}

public func SetAmmoCount(int iAmount, int iSlot)
{
  if(!iSlot) iSlot = GetSlot();
  aSlot_Amount[iSlot-1] = iAmount;
}

public func GetFMCount()
{
  var i = 1;
  while(GetFMData(FM_Name,i))
    i++;
  return i;
}

public func GetFTCount(int iFM)
{
  if(!iFM) iFM = firemode;

  var i = 1;
  while(GetFMData(FT_Name,iFM,i))
    i++;
  return i;
}


/* Feuer-System */

public func StopAutoFire()
{
  stopauto = true;
}

public func ControlUpdate(object caller, int comdir, bool dig, bool throw)
{
  SetUser(caller);

  if(IsRecharging() && !throw)
    StopAutoFire();
  else
  if(!throw)
    ClearScheduleCall(this,"Reload");
}

public func ControlThrow(object caller)
{
  SetUser(caller);
  
  if(IsRecharging())
  {
    if(!GetPlrCoreJumpAndRunControl(caller->GetController()))
      StopAutoFire();

    return 1;
  }

  if(GetFMData(FM_Aim) && !(GetUser()->~IsAiming()) && !(GetUser()->~AimOverride()))
  {
    if(GetUser()->~ReadyToAim())
      GetUser()->StartAiming();
  
    var ammoid = GetFMData(FM_AmmoID);
    var ammousage = GetFMData(FM_AmmoUsage);

    if(!CheckAmmo(ammoid,ammousage,this))
      if(CheckAmmo(ammoid,ammousage,GetUser()))
      {
        Reload();
      }
      else
      {
        PlayerMessage(GetOwner(caller), "$NotEnoughAmmo$", caller, ammoid);
      }

    return 1;
  }

  if(!Fire())
  {
    var ammoid = GetFMData(FM_AmmoID);
    var ammousage = GetFMData(FM_AmmoUsage);

    if(!CheckAmmo(ammoid,ammousage,this))
      if(CheckAmmo(ammoid,ammousage,GetUser()))
      {
        Reload();
      }
      else
      {
        PlayerMessage(GetOwner(caller), "$NotEnoughAmmo$", caller, ammoid);
      }
  }
  else
  {
    if(GetFMData(FM_BurstAmount) > 1)
    {
      var rechargetime = GetFMData(FM_BurstRecharge);
      if(rechargetime)
        AddEffect("BurstFire", this, 1, rechargetime, this,0,GetFMData(FM_BurstAmount));
      else
      {
        for(var i = GetFMData(FM_BurstAmount); i > 0; i--)
          if(!Fire())
            return 1;
      }
    }
  }

  return 1;
}

public func ControlDigDouble(object caller)
{
  SetUser(caller);
  return FMMenu(caller);
}

/// MENU START

public func FTMenu(int iFM)
{
  if(!GetFMData(FT_Name, iFM,1))
    AddMenuItem("$NA$",Format("WeaponMenu(%d)",iFM),FICO,GetUser());
  else
  {
    var i = aFM_FireTec[iFM-1];
    AddMenuItem(GetFMData(FT_Name, iFM,i),"FTCycle", GetFMData(FT_Icon, iFM,i), GetUser(), 0, iFM, GetFMData(FT_Name, iFM,i),2,GetFMData(FT_IconFacet, iFM,i));
  }
}

//public func FTCycle(unused,fm)
public func FTCycle(fm)
{
  var safe = 10;
  var ft = aFM_FireTec[fm-1];
  while(safe--)
  {
    if(GetFMData(FT_Name,fm,ft+1))
      ft++;
    else
      ft = 1;
      
    if(GetFMData(FT_Condition,fm,ft))
      break;
  }
    
  SetFireTec(ft,firemode);
  if(GetSpeedMenu())
    GetSpeedMenu()->NoClose();
}

public func FMMenu(object clonk)
{
  if(!clonk) clonk = GetUser();
  SetUser(clonk);
  
  var ring = CreateSpeedMenu(0,clonk);
  
  var overlay;
  overlay = ring->AddThrowItem("$Reload$","Reload",firemode,RICO);
  SetGraphics("1",ring,RICO,overlay,GFXOV_MODE_IngamePicture);
  
  var overlay;
  overlay = ring->AddLeftItem("$Left$","CycleFM",-1,RICO);
  SetGraphics(0,ring,RICO,overlay,GFXOV_MODE_IngamePicture);
  
  var overlay;
  overlay = ring->AddRightItem("$Right$","CycleFM",+1,RICO);
  SetGraphics(0,ring,RICO,overlay,GFXOV_MODE_IngamePicture);
  
  var overlay;
  overlay = ring->AddUpItem("$FireTec$","FTCycle",firemode,RICO);
  SetGraphics("3",ring,RICO,overlay,GFXOV_MODE_IngamePicture);
  
  var overlay;
  overlay = ring->AddDownItem("$AmmoType$","CycleSA",GetSlot(firemode),RICO);
  SetGraphics("3",ring,RICO,overlay,GFXOV_MODE_IngamePicture);
}

public func CycleSA(int slot)
{
  var safe = 10;
  var a = [];
  var j = 0;
  
  for(var i = 0, idR; idR = GetDefinition(i, C4D_Vehicle) ; i++)
  {
    if(idR->~IsSpecialAmmo())
    {
      if(idR == GetSpecialAmmo(slot)) j = i;
      a[GetLength(a)] = idR;
    }
  }
  
  j++;
  if(j >= GetLength(a)-1)
    j = 0;

  SetSpecialAmmo(a[j], slot);
  if(GetSpeedMenu())
    GetSpeedMenu()->NoClose();
}

private func CycleFM(int iDir)
{
  var safe = 10;
  var fm = firemode;
  while(safe--)
  {
    if(GetFMData(FM_Name,fm))
      fm += iDir;
    else
      fm = 1;
      
    if(GetFMData(FM_Condition,fm))
      break;
  }
    
  SetFireMode(fm);
  if(GetSpeedMenu())
    GetSpeedMenu()->NoClose();
}

/// MENU END

public func FxBurstFireStart(object pTarget, int iEffectNumber, int iTemp,iAmount)
{
  EffectVar(0,pTarget,iEffectNumber) = iAmount;
}

public func FxBurstFireTimer(object pTarget, int iNumber, int iTime)
{
  if(!GetUser()) return 0;
  if(!GetAlive(GetUser()) && GetCategory(GetUser())&C4D_Living) return 0;
  
  EffectVar(0,pTarget,iNumber)--;
  if(EffectVar(0,pTarget,iNumber) > 0)
    pTarget->Fire();
}

public func Fire()
{
  //TODO: Hier Spread prüfen?
  if(!(GetUser()->~ReadyToFire())) return false; 

  if(IsReloading())
  {
    if(GetFMData(FM_SingleReload))
      if(!IsPreparing() || !IsCanceling() || EffectVar(3,this,GetEffect("Reload",this)))
        FinishReload();

    return false;
  }

  var ammoid = GetFMData(FM_AmmoID);
  var ammousage = GetFMData(FM_AmmoUsage);

  if(!CheckAmmo(ammoid,ammousage,this))
  {
    OnEmpty(firemode);
    return false;
  }

  if(GetFMData(FM_Auto))
    OnAutoStart(firemode);

  Shoot();

  return true;
}

private func Shoot(object caller)
{
  var user = GetUser();
  if(user)
  {
    if(ObjectCount(NOWH))
		{
			var x,y;
			user->WeaponEnd(x,y);
      if(!PathFree4K(0,0,x,y,4))
        return false;
		}
  }
	else
		return StopAutoFire();

  var ammoid = GetFMData(FM_AmmoID);
	
	// Soll zielen, tut es aber nicht?
  if(!(user->~IsAiming()) && (GetFMData(FM_Aim) > 0) && !user->~AimOverride())
    return StopAutoFire();

	// Unser user ist in einem Container.
  if(user->Contained() && (user == Contained()))
    return StopAutoFire();
 
  // Feuern...
  if(CheckFireTec())
    Call(Format("Fire%dT%d",firemode,GetFireTec()));
  else
    Call(Format("Fire%d",firemode));
  DoSpread(+GetFMData(FM_SpreadAdd));

	if(!user) return 0;

  if(ratecount == 1)
  {
    var muni = GetFMData(FM_AmmoUsage);
    DoAmmo(ammoid,-muni);
    ratecount = GetFMData(FM_AmmoRate);
  }
  else
    --ratecount;

  if(GetAmmo(ammoid))
  {
    Recharge();
  }
  else
  {
		shooting = false;
    OnEmpty();
    if(GetFMData(FM_Auto)) OnAutoStop(firemode);

    if(GetPlrCoreJumpAndRunControl(user->GetController()))
      ScheduleCall(this, "Reload", 5);
  }

  user->~UpdateCharge();

  return true;
}

public func IsShooting() { return shooting; }

public func GetFireMode() { return firemode; }

private func SetFireMode(int i)
{
  if(i == 0) return;

  if(i == firemode)
  {
    if(CheckAmmo(GetFMData(FM_AmmoID, i),GetFMData(FM_AmmoLoad),this)) return; 
    if(IsReloading()) return;
  }

  Empty();

  firemode=i;
  stopauto=false;
  ratecount = GetFMData(FM_AmmoRate, i);
  
  // Helpmessage
  if(GetUser())
    HelpMessage(GetUser()->GetOwner(),"$FireModeChanged$",GetUser(),GetFMData(FM_Name),GetFMData(FM_AmmoID));
  
  return 1;
}


/* Zielen */

public func AimStart() { OnAimStart(firemode); }

public func AimStop()
{ 
  if(IsRecharging())
    StopAutoFire();
  OnAimStop(firemode);
}


/* Munitions-System */

public func GetCharge()
{
  var charge;
  var ammoid = GetFMData(FM_AmmoID);

  if(IsReloading())
  {
    //TODO: Hier gibts noch ein doofes Problem mit der Anzeige. :C (Nicht vollständiges Nachladen.)
     charge = 1000*
              (GetReloadTime()+(GetFMData(FM_Reload)*GetAmmo(ammoid)/GetFMData(FM_AmmoLoad)))/
              GetFMData(FM_Reload)*MaxReloadAmount(GetUser())/GetFMData(FM_AmmoLoad);
  }
  else
  {
    charge = 1000*GetAmmo(ammoid)/GetFMData(FM_AmmoLoad);
  }

  return charge;
}

private func MaxReloadAmount(object pAmmostore)
{
  var ammoid = GetFMData(FM_AmmoID);
  var ammoload = GetFMData(FM_AmmoLoad);
  var ammousage = GetFMData(FM_AmmoUsage);

  if(!CheckAmmo(ammoid,ammoload,pAmmostore))
    ammoload = GetAmmo(ammoid,pAmmostore);
  ammoload /= ammousage;
  ammoload *= ammousage;

  return ammoload;
}

public func IsCanceling(int iSlot)
{
  var effect = IsReloading(iSlot);
  if(effect)
  {
    if(EffectVar(5,this,effect) == +1)
      return true;
  }
}

public func IsPreparing(int iSlot)
{
  var effect = IsReloading(iSlot);
  if(effect)
  {
    if(EffectVar(5,this,effect) == -1)
      return true;
  }
}

public func GetReloadTime(int iSlot)
{
  var effect = IsReloading(iSlot);
  if(effect)
  {
    return EffectVar(0,this,effect)-EffectVar(4,this,effect);
  }
}

public func IsReloading(int iSlot)
{
  if(!iSlot)
    return GetEffect("Reload",this);
  
  for(var i; i<=GetEffectCount("Reload",this); i++)
  {
    if(EffectVar(2,this,GetEffect("Reload",this,i))==iSlot)
      return GetEffect("Reload",this,i);
  }
}

public func IsRecharging() { return GetEffect("Recharge", this); }

public func GetRecharge()
{
  if(IsCanceling())
  {
    return 100*EffectVar(5,this,IsReloading())/GetFMData(FM_FinishReload);
  }
  
  /*
  if(IsPreparing())
  {
    return 100*GetEffect("Reload", this, IsReloading(),6)/GetFMData(FM_PrepareReload);
  }*/

  if(!IsRecharging())
    return 100;

  var time = GetEffect("Recharge",this,0,6);
  return (100*time)/GetFMData(FM_Recharge);
}


//Nachladen
/*
  0: insgesammte Nachladezeit
  1: Nachladen aktiv?
  2: Slot
  3: bereits nachgelade!= Munition (Nur FM_SingleReload.)
  4: abgelaufe!= Nachladezeit
  5: Status -1 Vorbereiten | 0 Nachladen | +1 Beenden
*/
public func FxReloadStart(object pTarget, int iNumber, int iTemp, int iTime, iSlot)
{
  if(iTemp) return 0;

  EffectVar(0,pTarget,iNumber) = GetFMData(FM_Reload)*(GetFMData(FM_AmmoLoad)-GetAmmo(0,this,iSlot))/GetFMData(FM_AmmoLoad);
                                 //GetFMData(FM_Reload)*(GetFMData(FM_AmmoLoad)-GetAmmo2(iSlot))/MaxReloadAmount(GetUser());

  EffectVar(1,pTarget,iNumber) = 0;

	EffectVar(2,pTarget,iNumber) = iSlot;

  EffectVar(4,pTarget,iNumber) = EffectVar(0,pTarget,iNumber);

  EffectVar(5,pTarget,iNumber) = -1;
}

public func FxReloadTimer(object pTarget, int iNumber, int iTime)
{
  //Normales Nachladen...
  if(EffectVar(1,pTarget,iNumber))
  {
    if(GetFMData(FM_SingleReload))
    {
      if(iTime >= GetFMData(FM_Reload)*EffectVar(3,pTarget,iNumber)/GetFMData(FM_AmmoLoad))
      {
        OnSingleReloadStop(EffectVar(2,pTarget,iNumber));
        EffectVar(3,pTarget,iNumber) += GetFMData(FM_AmmoUsage);
        OnSingleReloadStart(EffectVar(2,pTarget,iNumber));
      }
    }
    
    GetUser()->~UpdateCharge();
    if(EffectVar(4,pTarget,iNumber)-- <= 0)//Fertig?
    {
      EffectVar(1,pTarget,iNumber) = 0;
      EffectVar(5,pTarget,iNumber) = +1;//Jetzt wird beendet.
      OnFinishReloadStart(EffectVar(2,pTarget,iNumber));
      
      if(GetFMData(FM_SingleReload))
        OnSingleReloadStop(EffectVar(2,pTarget,iNumber));
    }
  }
  else//Ansonsten bereitet er vor oder beendet?
  {
    //Vorbereiten...
    if(EffectVar(5,pTarget,iNumber) == -1)
    {
      if(iTime >= GetFMData(FM_PrepareReload))
      {
        EffectVar(5,pTarget,iNumber) = 0;//Jetzt wird nachgeladen.
        EffectVar(1,pTarget,iNumber) = 1;
        OnPrepareReloadStop(EffectVar(2,pTarget,iNumber));
        
        if(GetFMData(FM_SingleReload))
          OnSingleReloadStart(EffectVar(2,pTarget,iNumber));
          
        return 0;
      }
    }
    
    //Beenden...
    if(EffectVar(5,pTarget,iNumber) >= +1)
    {
      EffectVar(5,pTarget,iNumber)++;
      if(EffectVar(5,pTarget,iNumber) >= GetFMData(FM_FinishReload))
      {
        return -1;
      }
    }
  }
}

public func FxReloadStop(object pTarget, int iNumber, int iReason, bool fTemp)
{
  // nicht nachladen
  if(!GetAlive(GetUser()) && GetCategory(GetUser())&C4D_Living) return 0;
  
  // temporäre Aufrufe ignorieren
  if(fTemp) return 0;
  Message(" ",pTarget);
  
  if(iReason == 0)
  {
    if(!GetFMData(FM_SingleReload))
      if(EffectVar(4,pTarget,iNumber) <= 0)
        EffectVar(3,pTarget,iNumber) = GetFMData(FM_AmmoLoad)-GetAmmo(0,this,EffectVar(2,pTarget,iNumber));

    pTarget->Reloaded(GetUser(),EffectVar(2,pTarget,iNumber),EffectVar(3,pTarget,iNumber));
  }
}


public func FxRechargeStop(object pTarget, int iNumber, int iReason, bool fTemp)
{
  if(!GetUser()) return 0;
  if(!GetAlive(GetUser()) && (GetCategory(GetUser())&C4D_Living)) return 0;

  if(GetFMData(FM_Auto) || GetPlrCoreJumpAndRunControl(pTarget->GetController()))
  {
    if(stopauto || (GetUser()->Contents() != this && GetUser() == Contained(this)) || !(GetUser()->ReadyToFire()) || !CheckAmmo(GetFMData(FM_AmmoID), GetFMData(FM_AmmoUsage)))
    {
      if(GetFMData(FM_Auto))
        OnAutoStop(firemode);

      stopauto = false;
      shooting = false;
    }
    else
    {
      Shoot(pTarget);
    }
  }
}

private func Failure(dummy, i)
{
  var id2=GetFMData(FM_AmmoID, i);
  PlayerMessage(GetOwner(GetUser()), "$NotEnoughAmmo$", GetUser(),id2);
  OnEmpty(i);
}

public func Reload(int iFM)
{
  if(!iFM) iFM = firemode;
  var iSlot = GetSlot(iFM);
  if(IsReloading(iSlot)) return false;

  //Verzögerung? Abbrechen
  if(IsRecharging()) RemoveEffect("Recharge", this);
  
  //Hat schon genug?!
  if(GetFMData(FM_SingleReload,iFM))
    if(GetAmmo(0,this,iSlot) >= GetFMData(FM_AmmoLoad))
      return false;

  //nicht genug Ammo um nachzuladen
  var ammoid = GetFMData(FM_AmmoID,iFM);
  var ammousage = GetFMData(FM_AmmoUsage,iFM);//TODO:->FireTec!!
  if(!CheckAmmo(ammoid,ammousage,GetUser()))
    return false;

  //entleeren falls noch voll
  if(!GetFMData(FM_SingleReload,iFM) || !CheckAmmo(ammoid,GetFMData(FM_AmmoLoad,iFM),GetUser()))
    if(CheckAmmo2(ammoid,ammousage,iSlot,this))
      Empty(iSlot);

  if(GetFMData(FM_Auto,iFM))
    OnAutoStop();
  OnReload(iFM);
    
  AddEffect("Reload", this, 1, 1, this, 0, /*Max(1, reloadtime)*/0, iSlot);

  return true;
}

private func Recharge()
{
  var rechargetime = GetFMData(FM_Recharge);
  AddEffect("Recharge", this, 1, 1+Max(1, rechargetime), this);
}

public func CancelReload(int iSlot)
{
  if(!IsReloading(iSlot)) return false;
  
  if(!iSlot)
  {
    RemoveEffect("Reload", this,0);
    return true;
  }
  
  for(var i; i<=GetEffectCount("Reload",this); i++)
  {
    if(EffectVar(2,this,GetEffect("Reload",this,i)) == iSlot)
    {
      RemoveEffect("Reload", this,i);
      return true;
    }
  }
}

public func FinishReload(int iSlot)
{
  if(!IsReloading(iSlot)) return false;
  
  if(!iSlot)
  {
    var effect = GetEffect("Reload",this);
    EffectVar(4,this,effect) = 0;//Färtig!11
    return true;
  }
  
  for(var i; i<=GetEffectCount("Reload",this); i++)
  {
    if(EffectVar(2,this,GetEffect("Reload",this,i)) == iSlot)
    {
      var effect = GetEffect("Reload",this,i);
      EffectVar(4,this,effect) = 0;
      return true;
    }
  }
  
  return false;
}

public func PauseReload(int iSlot)
{
  if(!IsReloading(iSlot)) return false;

  if(!iSlot)
  {
    if(EffectVar(1,this,GetEffect("Reload",this)) == 0) return false;
    EffectVar(1,this,GetEffect("Reload",this)) = 0;
    return true;
  }
  
  for(var i; i<=GetEffectCount("Reload",this); i++)
  {
    if(EffectVar(2,this,GetEffect("Reload",this,i)) == iSlot)
    {
      if(EffectVar(1,this,GetEffect("Reload",this,i)) == 0) return false;
      EffectVar(1,this,GetEffect("Reload",this,i)) = 0;
      return true;
    }
  }
}

public func ResumeReload(int iSlot)
{
  if(!IsReloading(iSlot)) return false;

  if(!iSlot)
  {
    if(EffectVar(1,this,GetEffect("Reload",this)) == 1) return false;
    EffectVar(1,this,GetEffect("Reload",this)) = 1;
    return true;
  }
  
  for(var i; i<=GetEffectCount("Reload",this); i++)
  {
    if(EffectVar(2,this,GetEffect("Reload",this,i)) == iSlot)
    {
      if(EffectVar(1,this,GetEffect("Reload",this,i)) == 1) return false;
      EffectVar(1,this,GetEffect("Reload",this,i)) = 1;
      return true;
    }
  }
}

public func Empty(int iSlot)
{
  if(!iSlot) iSlot = GetSlot();
  if(IsReloading(iSlot)) CancelReload(iSlot);
  if(IsRecharging()) RemoveEffect("Recharge", this);

  var ammoid = GetFMData(FM_AmmoID);
  var ammoamount = GetAmmo(ammoid, this);
  DoAmmo(ammoid, ammoamount, GetUser());
  DoAmmo(ammoid, -ammoamount, this, iSlot);
}

private func Reloaded(object caller, int slot, int amount)
{
  var ammoid = GetFMData(FM_AmmoID);
  var amount = Min(amount,MaxReloadAmount(caller));

  if(!CheckAmmo(ammoid,amount,caller))
    return false;

  DoAmmo(ammoid, amount, this, slot);
  DoAmmo(ammoid, -amount, caller);

  HelpMessage(caller->GetOwner(),"$Reloaded$",caller,amount,ammoid);

  OnReloaded(firemode,slot);
}


/* Upgradezeug */

public func Upgrade(id uid)
{
  if(!IsUpgradeable(uid))
    return false;
  
  upgrades[GetLength(upgrades)] = uid;
  OnUpgrade(uid);
  return true;
}

public func Downgrade(id uid)
{
  if(!GetUpgrade(uid))
    return false;
  
  for(var i = 0; i < GetLength(upgrades); i++)
    if(upgrades[i] == uid)
    {
      upgrades[i] = 0;
      for(var j = i; j < GetLength(upgrades)-1; j++)
        upgrades[j] = upgrades[j+1];
      
      OnDowngrade(uid);
      if(Contained())
        CreateContents(uid,Contained());
      else
        CreateObject(uid);

      return true;
    }
  
  return false;
}

public func GetUpgrade(id uid)
{
  for(var upg in upgrades)
    if(upg == uid)
      return true;
      
  return false;
} 


/* SpecialAmmo-System */

global func SALaunchBullet(int iX, int iY, int iOwner, int iAngle, int iSpeed, int iDist, int iDmg, int iRemoveTime, id idType)
{
  var ammoid = idType;
  if(!ammoid) ammoid = this->~GetSpecialAmmo();
  if(!ammoid) ammoid = SHTX;
  
  var iSize = 2;
  var iTrail = iDmg*10;
  
  var ammo = CreateObject(ammoid,iX,iY,iOwner);
  ammo->CustomLaunch(iAngle,iSpeed,iDist,iSize,iTrail,iDmg,iRemoveTime);
  
  return ammo;
}

global func SABulletCasing(int iX, int iY, int iXDir, int iYDir, int iSize, int iColor, id idType)
{
  var ammoid = idType;
  if(!ammoid) ammoid = this->~GetSpecialAmmo();
  if(!ammoid) ammoid = SHTX;
  
  return ammoid->CustomBulletCasing(GetX()+iX,GetY()+iY,iXDir,iYDir,iSize,iColor);
}

global func SAMuzzleFlash(int iSize, object pTarget, int iX, int iY, int iAngle, int iColor, id idType)
{
  var ammoid = idType;
  if(!ammoid) ammoid = this->~GetSpecialAmmo();
  if(!ammoid) ammoid = SHTX;
  
  return ammoid->CustomMuzzleFlash(iSize,pTarget,GetX()+iX,GetY()+iY,iAngle,iColor);
}


/* Sonstiges */

protected func Deselection(object pContainer)
{
  OnDeselect(firemode);
  PauseReload();
  if(GetFMData(FM_Auto) && IsRecharging()) StopAutoFire(); 
}

protected func Selection(object pContainer)
{
  OnSelect(firemode);
  ResumeReload();
}

protected func Departure()
{
  if(GetFMData(FM_Auto))
    if(!stopauto)
      OnAutoStop(firemode);
  if(IsReloading())
    RemoveEffect("Reload", this, 0, true);

  SetUser();
}

protected func Entrance(object pContainer)
{
  SetUser(pContainer);
	pContainer->~CheckArmed(); // Sicherheitshalber nochmal. ;)
}

public func Hit()
{
  Sound("WPN2_Hit*.ogg");
}


/* Waffenanzeige */

// EffectVars:
//  0 - ID der Waffe
//  1 - Aktuelle Drehung
//  2 - X-Position des Anfangs der Waffe
//  3 - Y-Position des Anfangs der Waffe
//  4 - X-Position des Ende des Laufs
//  5 - Y-Position des Ende des Laufs
//  6 - Aktuelle Waffe

global func FxShowWeaponStart(object pTarget, int iNumber, int iTemp)
{
  if (iTemp) return FX_OK;
  // Erste Grafikaktualisierung
  FxShowWeaponTimer(pTarget, iNumber);
  return FX_OK;
}

global func FxShowWeaponTimer(object pTarget, int iNumber, int iTime)
{
  // Waffe aktualisieren:
  var xoff, yoff, r;  // Offset, Winkel
  // kein Inventar oder falsche Aktion
  if(!Contents(0,pTarget)) {
    EffectVar(0, pTarget, iNumber) = 0;
    if(EffectVar(6, pTarget, iNumber)) {
      SetObjDrawTransform(1000,0,0,0,1000,0,EffectVar(6,pTarget,iNumber));
      EffectVar(6, pTarget, iNumber) = 0;
    }
    SetGraphics(0, pTarget, 0, WeaponDrawLayer);
    return FX_OK;
  }
  //Die Waffe momentan überhaupt anzeigen?
  if(!(pTarget->~WeaponAt(xoff, yoff, r))) {
    EffectVar(0, pTarget, iNumber) = 0;
    if(EffectVar(6, pTarget, iNumber)) {
      SetObjDrawTransform(1000,0,0,0,1000,0,EffectVar(6,pTarget,iNumber));
      EffectVar(6, pTarget, iNumber) = 0;
    }
    SetGraphics(0, pTarget, 0, WeaponDrawLayer);
    return FX_OK;
  }
  var obj = Contents(0,pTarget), id=GetID(obj);
  // Waffe nicht mehr aktuell
  if(EffectVar(0, pTarget, iNumber) != id) {
    // neues Objekt ist Waffe, oder ein Objekt, das gezeichnet werden soll
    if(obj->~IsWeapon() || obj->~IsDrawable()) {
      EffectVar(0, pTarget, iNumber) = id;
      EffectVar(6, pTarget, iNumber) = obj;
      SetGraphics(0, pTarget,id, WeaponDrawLayer, GFXOV_MODE_Object,0,GFX_BLIT_Parent,obj);
    }
    // neues Objekt ist keine Waffe
    else {
      EffectVar(0, pTarget, iNumber) = 0;
      if(EffectVar(6, pTarget, iNumber)) {
        SetObjDrawTransform(1000,0,0,0,1000,0,EffectVar(6,pTarget,iNumber));
        EffectVar(6, pTarget, iNumber) = 0;
      }
      SetGraphics(0, pTarget, 0, WeaponDrawLayer);
      return FX_OK;
    }
  }

  id = EffectVar(0, pTarget, iNumber);
  obj = EffectVar(6, pTarget, iNumber);

  // Ausrichtung nach Blickrichtung des Clonks
  // Variablen für die Transformation

  var width, height;  // Breiten- und Höhenverzerrung der Waffe
  var xskew, yskew;   // Zerrung der Waffe, wird zur Rotation gebraucht
  var size;           // Größe der Waffe in der Hand: 1000 = 100%
  // Variablen für die Position
  var xaim, yaim;     // Offset, dass sich durch zielen ergibt
  var dir;            // Richtung in die das Objekt schaut
  
  //schnell noch Rotation dazurechnen oder so!
  r += ObjectCall(obj,"HandR");
  
  // Variablen mit Werten versehen
  width = height = xskew = yskew = 1;
  size = id->~HandSize();
  if(!size) size = 1000;
  dir  = GetDir()*2-1;
  if(r > 180 || r < -180)
    dir *= -1;
  r *= dir;

  var xfact = size * ObjectCall(obj,"HandX");    // Attachpunkte dazurechnen
  var yfact = size * ObjectCall(obj,"HandY");

  xoff += Cos(r,xfact)/1000 + dir*Sin(r,yfact)/1000;
  yoff -= Cos(r,yfact)/1000 - dir*Sin(r,xfact)/1000;

  if(dir == 1) 
  {
    height = -1;
    xskew = -1;
    yskew = -1;
  }

  r = -90*dir-r-90;
  height *= width *= Cos(r, size);
  xskew *= Sin(r, size);
  yskew *= -xskew;
  xoff *= dir;

  SetObjDrawTransform(1000,xskew,xoff,yskew,1000,yoff, pTarget, WeaponDrawLayer); //position
  SetObjDrawTransform(width,xskew,0,yskew,height,0, obj); //Größe und Rotation

  // abspeichern, damit abrufbar
  r = -r-90;
  var w = GetDefCoreVal("Width",0,id)/2;
  var brly = DefinitionCall(id,"BarrelYOffset");
  var brlx = DefinitionCall(id,"BarrelXOffset");
  var r2 = (Angle(0,0,w-brlx/1000,brly/1000)-90)*dir;
  var dist = Distance(0,0,w*1000-brlx,brly);
  //Log("%d - %d - %d - %d",w,brl,r2,dist);
  EffectVar(1, pTarget, iNumber) = r;
  EffectVar(2, pTarget, iNumber) = xoff-Sin(r,size*w);
  EffectVar(3, pTarget, iNumber) = yoff+Cos(r,size*w);
  EffectVar(4, pTarget, iNumber) = xoff+Sin(r+r2,size*(dist))/1000;
  EffectVar(5, pTarget, iNumber) = yoff-Cos(r+r2,size*(dist))/1000;
  //EffectVar(4, pTarget, iNumber) = xoff+Sin(r,size*(w));	
  //EffectVar(5, pTarget, iNumber) = yoff-Cos(r,size*(w));
  //Log("%d / %d",EffectVar(4, pTarget, iNumber),EffectVar(5, pTarget, iNumber));
}
  
global func FxShowWeaponStop(object pTarget, int iNumber, int iReason, bool fTemp)
{
  if (fTemp) return FX_OK;
  // Grafik entfernen
  SetGraphics(0, pTarget, 0, WeaponDrawLayer);
  
  if(EffectVar(6,pTarget,iNumber))
	SetObjDrawTransform(1000,0,0,0,1000,0,EffectVar(6,pTarget,iNumber));
}  

/* neues Zielsystem */
public func DoSpread(int iChange)
{
  //...
}

public func GetSpread()
{
  return 0;
}

/*protected func Selection(object pContainer)
{
  //Startstreuung hinzufügen
  var diff = GetFMData(FM_StartSpread) - GetSpread();
  if(diff > 0)
    DoSpread(diff);
    
  // Callback
  this->~OnSelect(firemode);
  // Laden wiederaufnehmen
  this->~ResumeReload();
}*/
