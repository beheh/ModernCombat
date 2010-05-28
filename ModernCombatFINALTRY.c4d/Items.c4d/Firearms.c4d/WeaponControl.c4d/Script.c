/*-- Waffe 2.0 --*/

#strict 
#include WEPN

local stopauto,firemode,shooting,ratecount;

local aSlot_Type; //Hält den Munitionstyp.
local aSlot_Amount; //Hält die Munitionsmenge.

local aFM_FireTec; //Hält die Feuertechnik.

local idBulletID; //Bei Projektielmunition hält dies die ProjektilID.

static const FM_Slot          = 13; //Der Slot vom Feuermodus.
static const FM_SingleReload  = 14; //Munition wird einzeln nachgeladen. (z.B. für Shotguns)
static const FM_PrepareReload = 15; //Zeit bevor das eigentliche Nachladen beginnt. (Nur interessant wenn auch FM_SingleReload benutzt wird.)
static const FM_FinishReload  = 16; //Zeit nach dem Nachladen. (Nur interessant wenn auch FM_SingleReload benutzt wird.)
static const FM_BurstAmount   = 17;
static const FM_BurstRecharge = 18;

static const FM_SpreadAdd     = 19;
static const FM_StartSpread   = 20;
static const FM_MaxSpread     = 21;
static const FM_UnSpread      = 22;

static const FT_Name          = 23; //Name der Feuertechnik.
static const FT_Icon          = 24; //Icondefinition der Feuertechnik.
static const FT_IconFacet     = 25; //Facet. Wie bei AddMenuItem.
static const FT_Condition     = 26; //Wie FM_Condition. Nur eben für Feuertechniken.

public func IsWeapon2() {return(true);} //Diese Waffe benutzt das neue Waffensystem. Sie includiert also WPN2.
public func NoWeaponChoice() { return(GetID() == WPN2); }

public func OnSelectFT(int iFireMode, int iFireTec, int iLastFireTec){}

public func OnSingleReloadStart(int iSlot){}
public func OnSingleReloadStop(int iSlot){}
public func OnPrepareReloadStop(int iSlot){}
public func OnFinishReloadStart(int iSlot){}

public func Default(int data)    // Standardeinstellungen
{
  if(data == FT_Name)      return(0);//Wichtig!!
  if(data == FT_Condition) return(true);
  if(data == FM_Slot)      return(0);
  return(inherited(data));
}

protected func Initialize()
{
  aSlot_Type = CreateArray();
  aSlot_Amount = CreateArray();
  aFM_FireTec = CreateArray();
  
  var i = 1;
  while(CheckFireTec(1,i))
  {
    aFM_FireTec[i-1] = 1;//Defaults setzen.
    i++;
  }

  return(inherited());
}

/* Menü für Schussmodi */

public func ControlDigDouble(caller)
{
  return(FMMenu(caller));
}

public func FMMenu(clonk)
{
  if(!clonk) clonk = GetUser();
  SetUser(clonk);
  
  var ring = CreateSpeedMenu(0,clonk);
  
  var overlay;
  
  overlay = ring->AddThrowItem("$Reload$","ManualReload",firemode,RICO);
  SetGraphics("1",ring,RICO,overlay,GFXOV_MODE_IngamePicture);

  overlay = ring->AddLeftItem("$FireTecBack$","CycleFT",-1,RICO);
  SetGraphics("4",ring,RICO,overlay,GFXOV_MODE_IngamePicture);

  overlay = ring->AddRightItem("$FireTecForward$","CycleFT",+1,RICO);
  SetGraphics("4",ring,RICO,overlay,GFXOV_MODE_IngamePicture);

  overlay = ring->AddUpItem("$FireModeCycle$","CycleFM",+1,RICO);
  SetGraphics("3",ring,RICO,overlay,GFXOV_MODE_IngamePicture);
  
  overlay = ring->AddDownItem("$AmmoType$","ManualEmpty",firemode,RICO);
  SetGraphics("2",ring,RICO,overlay,GFXOV_MODE_IngamePicture);
  
  var szName = "";
  for(var i = 1; i <= GetFMCount(); i++) {
  	szName = GetFMData(FM_Name, i, 1);
	  if(!szName) continue;
  	if(i == firemode) 
  		szName = Format("<c ffff00>%s</c>", szName);
  	else
  	  szName = Format("<c eeeeee>%s</c>", szName);	
		ring->AddTopInfoItem(szName);
	}
	for(var i = 1; i <= GetFTCount(firemode); i++) {
	  szName = GetFMData(FT_Name, firemode, i);
	  if(!szName) continue;
	  if(i == GetFireTec(firemode))
	  	szName = Format("<c ffff00>%s</c>", szName);
	  else
  	  szName = Format("<c eeeeee>%s</c>", szName);	
		ring->AddBottomInfoItem(szName);
	}
	
}

private func ManualReload(fm)
{
  Reload(fm);
  if(GetSpeedMenu())
    GetSpeedMenu()->NoClose();
}

private func ManualEmpty(unused,fm)
{
  Sound("WPN2_Unload.ogg");
  Empty2(GetSlot(fm));
}

//Slots
public func GetSlot(int iFM)
{
  return(GetFMData(FM_Slot,iFM));
}

public func GetSlotCount()
{
  return(GetLength(aSlot_Type));
}

public func GetFMCount()
{
  var i = 1;
  while(GetFMData(FM_Name,i))
    i++;
  return(i-1);
}

public func GetFTCount(int iFM)
{
  if(!iFM) iFM = firemode;

  var i = 1;
  while(GetFMData(FT_Name,iFM,i))
    i++;
  return(i);
}

//Munityp
public func GetCurrentAmmoType()
{
  return(GetAmmoCount(GetFMData(FM_Slot)));
}

public func GetAmmoType(iSlot)
{
  return(aSlot_Type[iSlot-1]);
}

public func SetAmmoType(int iSlot, id idType)
{
  aSlot_Type[iSlot-1] = idType;
}

//Munimenge
public func GetCurrentAmmoCount()
{
  return(GetAmmoType(GetFMData(FM_Slot)));
}

public func GetAmmoCount(iSlot)
{
  return(aSlot_Amount[iSlot-1]);
}

public func SetAmmoCount(int iSlot, int iAmount)
{
  aSlot_Amount[iSlot-1] = iAmount;
}

/* Munition */

global func GetAmmo(id ammoid, object target)
{
  if(!target) target=this();
  
  if(target->~IsWeapon2())
    return(GetAmmo2(target->GetSlot(),target));
  return(_inherited(ammoid,target));
}

global func GetAmmo2(int slot, object target)
{
  if(!target) target=this();
  
  // Entsprechendes Munitionslagerobjekt suchen
  var obj = target ->~ AmmoStoring();
  if(!obj) obj = target;
  
  // no ammo rule
  if(ObjectCount(NOAM))
    if(target ->~ IsAmmoStorage())
      return(0);
  
  return(target->GetAmmoCount(slot));
}

global func DoAmmo(id ammoid, int change, object target)
{
  if(!target) target=this();

  if(target->~IsWeapon2())
    return(DoAmmo2(target->GetSlot(),ammoid,change,target));
  return(_inherited(ammoid,change,target));
}

global func DoAmmo2(int slot, id ammoid, int change, object target)
{
  // gar keine Munition
  if(!(ammoid->~IsAmmo())) return(0);
  
  // Kann 0 sein bei Objektlokalen Aufrufen.
  if(!target) target=this();
  
  // Entsprechendes Munitionslagerobjekt suchen
  var obj = target ->~ AmmoStoring();
  if(!obj) obj = target;
  
  // no ammo rule
  if(ObjectCount(NOAM))
    if(obj ->~ IsAmmoStorage())
      return(0);
    
  // Alten Munitionsstand speichern
  var oldammoamount = obj->GetAmmo(ammoid);
  var truechange;
  var maxamount = ammoid->~MaxAmount();
  // Der neue Wert wird ausgerechnet, darf aber nicht grÃ¶ÃŸer als eventuelles MaxAmount() und nicht kleiner als 0 sein
  if(maxamount == 0)
    truechange= Max(oldammoamount+change, 0);
  else
    truechange= Max(Min(maxamount, oldammoamount+change), 0);
    
  // Neuer Wert dem Objekt geben
  target->SetAmmoCount(slot,truechange);
  target->SetAmmoType(slot,ammoid);
  
  // Wenn Muni alle, Typ nullen
  if(truechange==0) target->SetAmmoType(slot,0);
  
  // Differenz zurÃ¼ckgeben: TatsÃ¤chliche Ã„nderung.
  return(truechange-oldammoamount);
}

public func Empty()    // Waffe ausleeren
{
   Empty2(GetSlot());
}

public func Empty2(int iSlot)    // Waffe ausleeren
{
  // Laden wir nach? Abbrechen.
  if(IsReloading(iSlot)) CancelReload(iSlot);
  if(IsRecharging()) RemoveEffect("Recharge", this());

  // Munitionsart wo raus muss
  var ammoid = GetFMData(FM_AmmoID);
  // Zu schiebende Munitionsmenge
  var ammoamount = GetAmmo(ammoid, this());
  // Clonk wiedergeben 
  DoAmmo(ammoid, ammoamount, GetUser());
  // Hier entfernen
  DoAmmo2(iSlot, ammoid, -ammoamount, this());
}

global func CheckAmmo(id ammoid, int count)
{
  // gar keine Munition
  if(!(ammoid->~IsAmmo())) return(false);

  if(!Par(2)) Par(2) = this();
  var iSum = 0;

  // search
  for(var i=2; i<10 && Par(i); ++i)
  {
    // no ammo rule
    var obj = Par(i)->~AmmoStoring();
    if(!obj) obj = Par(i);
    if(ObjectCount(NOAM))
      if(obj->~IsAmmoStorage())
        return(true);
    iSum += GetAmmo(ammoid,obj);
  }

  if(iSum >= count)
    return(true);
  return(false);
}

global func CheckAmmo2(int slot, id ammoid, int count)
{
  // gar keine Munition
  if(!(ammoid->~IsAmmo())) return(false);

  if(!Par(3)) Par(3) = this();
  var iSum = 0;

  // search
  for(var i=3; i<10 && Par(i); ++i)
  {
    // no ammo rule
    var obj = Par(i) ->~ AmmoStoring();
    if(!obj) obj = Par(i);
    if(ObjectCount(NOAM))
      if(obj ->~ IsAmmoStorage())
        return(true);
    iSum += GetAmmo2(slot,obj);
  }

  if(iSum >= count)
    return(true);
  return(false);
}

public func GetCharge()
{
  var charge;
  var ammoid = GetFMData(FM_AmmoID);
  // lädt nach: Nachladestatus anzeigen
  if(IsReloading())
  {
      charge = 1000*
               (GetReloadTime()+(GetFMData(FM_Reload)*GetAmmo(ammoid)/GetFMData(FM_AmmoLoad)))/
               GetFMData(FM_Reload)*MaxReloadAmount(GetUser())/GetFMData(FM_AmmoLoad);
  }
  // ansonsten: Ladestand anzeigen
  else
  {
    charge = 1000*GetAmmo(ammoid)/GetFMData(FM_AmmoLoad);
  }

  return(charge);
}

public func GetRecharge()
{
  if(IsCanceling())
  {
    return(100*EffectVar(5,this(),IsReloading())/GetFMData(FM_FinishReload));
  }

  if(!IsRecharging())
    return(100);
  var time = GetEffect("Recharge",this(),0,6);
  return((100*time)/GetFMData(FM_Recharge));
}

public func FxNoFinishReloadStart(object pTarget, int iNumber)
{
	EffectVar(0,pTarget,iNumber) = 30;
}

public func FxNoFinishReloadTimer(object pTarget, int iNumber, int iTime)
{
	//Schauen ob der Delay vorbei ist
	if(EffectVar(0,pTarget,iNumber)-- <= 0)
		return -1;
	
	//Falls nicht mehr nachgeladen wird
	if(!GetEffect("Reload", pTarget))
		return -1;
}

//Nachladen
/*
  0: insgesammte Nachladezeit
  1: Nachladen aktiv?
  2: Slot
  3: bereits nachgeladene Munition (Nur FM_SingleReload.)
  4: abgelaufene Nachladezeit
  5: Status -1 Vorbereiten | 0 Nachladen | +1 Beenden
*/
public func FxReloadStart(object pTarget, int iNumber, int iTemp, int iTime,iSlot)
{
  if(iTemp) return();
  
  EffectVar(0,pTarget,iNumber) = GetFMData(FM_Reload)*(GetFMData(FM_AmmoLoad)-GetAmmo2(iSlot))/GetFMData(FM_AmmoLoad);
  EffectVar(4,pTarget,iNumber) = EffectVar(0,pTarget,iNumber);
  EffectVar(2,pTarget,iNumber) = iSlot;
  
  //Vorbereiten.
  EffectVar(1,pTarget,iNumber) = 0;
  
  //Log("Vorbereiten - Start");
  EffectVar(5,pTarget,iNumber) = -1;
  
  //Reload-End-Spam verhindern
  if(GetFMData(FM_SingleReload))
  	AddEffect("NoFinishReload", pTarget, 20, 1, pTarget);
}

public func FxReloadTimer(object pTarget, int iNumber, int iTime)
{
  //Normales Nachladen...
  if(EffectVar(1,pTarget,iNumber))
  {
    //Log("  Nachladen");
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
    if(EffectVar(4,pTarget,iNumber)-- <= 0) //Fertig?
    {
      EffectVar(1,pTarget,iNumber) = 0;
      EffectVar(5,pTarget,iNumber) = +1; //Jetzt wird beendet.
      OnFinishReloadStart(EffectVar(2,pTarget,iNumber));
      
      if(GetFMData(FM_SingleReload))
        OnSingleReloadStop(EffectVar(2,pTarget,iNumber));
    }
  }
  else //Ansonsten bereitet er vor oder beendet?
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
          
        return();
      }
    }
    
    //Beenden...
    if(EffectVar(5,pTarget,iNumber) >= +1)
    {
      EffectVar(5,pTarget,iNumber)++;
      if(EffectVar(5,pTarget,iNumber) >= GetFMData(FM_FinishReload))
      {
        return(-1);
      }
    }
  }
}

public func FxReloadStop(object pTarget, int iNumber, int iReason, bool fTemp)
{
  // nicht nachladen
  if(!GetAlive(GetUser()) && GetCategory(GetUser())&C4D_Living) return(0);
  
  // temporäre Aufrufe ignorieren
  if(fTemp) return();
  Message(" ",pTarget);
  
  if(iReason == 0)
  {
    if(!GetFMData(FM_SingleReload))
      if(EffectVar(4,pTarget,iNumber) <= 0)
        EffectVar(3,pTarget,iNumber) = GetFMData(FM_AmmoLoad)-GetAmmo2(EffectVar(2,pTarget,iNumber));
      
    pTarget->Reloaded(GetUser(),EffectVar(2,pTarget,iNumber),EffectVar(3,pTarget,iNumber));
  }
}

public func IsCanceling(int iSlot)
{
  var effect = IsReloading(iSlot);
  if(effect)
  {
    if(EffectVar(5,this(),effect) == +1)
      return(true);
  }
}

public func IsPreparing(int iSlot)
{
  var effect = IsReloading(iSlot);
  if(effect)
  {
    if(EffectVar(5,this(),effect) == -1)
      return(true);
  }
}

public func GetReloadTime(int iSlot)
{
  var effect = IsReloading(iSlot);
  if(effect)
  {
    return(EffectVar(0,this(),effect)-EffectVar(4,this(),effect));
  }
}

public func IsReloading(int iSlot)
{
  if(!iSlot)
    return(GetEffect("Reload",this()));
  
  for(var i;i<=GetEffectCount("Reload",this());i++)
  {
    if(EffectVar(2,this(),GetEffect("Reload",this(),i))==iSlot)
      return(GetEffect("Reload",this(),i));
  }
}

public func Reload(int iFM)// Waffe nachladen
{
  if(!iFM) iFM = firemode;
  var iSlot = GetSlot(iFM);
  if(IsReloading(iSlot)) return(false);

  // Verzögerung? Abbrechen
  if(IsRecharging()) RemoveEffect("Recharge", this());
  
  //Hat schon genug?!
  if(GetFMData(FM_SingleReload,iFM))
    if(GetAmmo2(iSlot,ammoid) >= GetFMData(FM_AmmoLoad))
      return(false);

  // nicht genug Ammo um nachzuladen
  var ammoid = GetFMData(FM_AmmoID,iFM);
  var ammousage = GetFMData(FM_AmmoUsage,iFM);
  if(!CheckAmmo(ammoid,ammousage,GetUser()))
    return(false);

  // entleeren falls noch voll
  if(!GetFMData(FM_SingleReload,iFM) || !CheckAmmo(ammoid,GetFMData(FM_AmmoLoad,iFM),GetUser()))
    if(CheckAmmo2(iSlot,ammoid,ammousage,this()))
      Empty2(iSlot);

  if(GetFMData(FM_Auto,iFM))
    OnAutoStop();
  OnReload(iFM);
    
  AddEffect("Reload", this(), 1, 1, this(), 0, 0,iSlot);

  return(true);
}

public func ControlThrow(caller)
{
  SetUser(caller);
  
  // autom. Schuss beenden, wenn erneut Werfen gedrückt (klassisch)
  if(IsRecharging())
  {
    if(!GetPlrCoreJumpAndRunControl(caller->GetController()))
      StopAutoFire();

    return(1);
  }

  // Unterstützt der Schussmodus Zielen, aber wir tuns nicht?
  if(GetFMData(FM_Aim) == 1 && !(GetUser()->~IsAiming()) && !(GetUser()->~AimOverride()))
  {
    // Können wir?
    if(GetUser()->~ReadyToAim())
      // Auf gehts:
      GetUser()->StartAiming();
  
    // Nachladen?
    var ammoid = GetFMData(FM_AmmoID);
    var ammousage = GetFMData(FM_AmmoUsage);
    // weil keine Muni mehr da war? -> nachladen wenn geht
    if(!CheckAmmo(ammoid,ammousage,this()))
      if(CheckAmmo(ammoid,ammousage,GetUser()))
      {
        Reload();
      }
      // nicht genügend Munition
      else {
        PlayerMessage(GetOwner(caller), "$NotEnoughAmmo$", caller, ammoid);
      }
    // Sonst nix.
    return(1);
  }

  // Feuern! Fehlgeschlagen?
  if(!Fire())
  {

    var ammoid = GetFMData(FM_AmmoID);
    var ammousage = GetFMData(FM_AmmoUsage);
    // weil keine Muni mehr da war? -> nachladen wenn geht
    if(!CheckAmmo(ammoid,ammousage,this()))
      if(CheckAmmo(ammoid,ammousage,GetUser())) {
        Reload();
      }
      // nicht genügend Munition
      else {
        PlayerMessage(GetOwner(caller), "$NotEnoughAmmo$", caller, ammoid);
      }
  }
  else
  {
    if(GetFMData(FM_BurstAmount) > 1)
    {
      var rechargetime = GetFMData(FM_BurstRecharge);
      if(rechargetime)
        AddEffect("BurstFire", this(), 1, rechargetime, this(), 0, GetFMData(FM_BurstAmount));
      else
      {
        for(var i = GetFMData(FM_BurstAmount); i > 0; i--)
          if(!Fire())
            return(1);
      }
    }
  }

  return(1);
}

public func FxBurstFireStart(object pTarget, int iEffectNumber, int iTemp,iAmount)
{
  EffectVar(0,pTarget,iEffectNumber) = iAmount;
}

public func FxBurstFireTimer(object pTarget, int iNumber, int iTime)
{
  if(!GetUser()) return(0);
  if(!GetAlive(GetUser()) && GetCategory(GetUser())&C4D_Living) return(0);
  
  EffectVar(0,pTarget,iNumber)--;
  if(EffectVar(0,pTarget,iNumber) > 0)
    pTarget->Fire();
  else
  	return -1;
}

public func Fire()
{
  if(GetSpread()+GetFMData(FM_SpreadAdd) >= CH_MaxSpread)
    return(false);

  // bereit zum Schießen
  if(!(GetUser()->~ReadyToFire())) return(false); 

  // lädt grad nach... bitte nicht stören..oder doch?
  if(IsReloading())
  {
    if(GetFMData(FM_SingleReload))
      if(!GetEffect("NoFinishReload", this))
      	if(!IsPreparing() || !IsCanceling() || EffectVar(3,this(),GetEffect("Reload",this())))
        	FinishReload();
    return(false);
  }

  var ammoid = GetFMData(FM_AmmoID);
  var ammousage = GetFMData(FM_AmmoUsage);

  // leer?
  if(!CheckAmmo(ammoid,ammousage,this()))
  {
    OnEmpty(firemode);
    return(false);
  }

  // Callback
  if(GetFMData(FM_Auto))
    OnAutoStart(firemode);

  // FEUAAA!!!1 OMGOMG
  Shoot();

  return(true);
}

public func CancelReload(int iSlot)  // Nachladen abbrechen
{
  if(!IsReloading(iSlot)) return(false);
  
  if(!iSlot)
  {
    RemoveEffect("Reload", this(),0);
    return(true);
  }
  
  for(var i;i<=GetEffectCount("Reload",this());i++)
  {
    if(EffectVar(2,this(),GetEffect("Reload",this(),i))==iSlot)
    {
      RemoveEffect("Reload", this(),i);
      return(true);
    }
  }
}

public func FinishReload(int iSlot)  // Nachladen abbrechen (Aber nicht sofort.)
{
  if(!IsReloading(iSlot)) return(false);
  
  if(!iSlot)
  {
    var effect = GetEffect("Reload",this());
    EffectVar(4,this(),effect) = 0;//Färtig!11
    return(true);
  }
  
  for(var i;i<=GetEffectCount("Reload",this());i++)
  {
    if(EffectVar(2,this(),GetEffect("Reload",this(),i))==iSlot)
    {
      var effect = GetEffect("Reload",this(),i);
      EffectVar(4,this(),effect) = 0;//Färtig!11
      return(true);
    }
  }
}

public func PauseReload(iSlot) // Nachladen pausieren
{
  if(!IsReloading(iSlot)) return(false);

  if(!iSlot)
  {
    if(EffectVar(1,this(),GetEffect("Reload",this())) == 0) return(false);
    EffectVar(1,this(),GetEffect("Reload",this())) = 0;
    return(true);
  }
  
  for(var i;i<=GetEffectCount("Reload",this());i++)
  {
    if(EffectVar(2,this(),GetEffect("Reload",this(),i))==iSlot)
    {
      if(EffectVar(1,this(),GetEffect("Reload",this(),i)) == 0) return(false);
      EffectVar(1,this(),GetEffect("Reload",this(),i)) = 0;
      return(true);
    }
  }
}

public func ResumeReload(iSlot)  // Nachladen wiederaufnehmen (wenn pausiert)
{
  if(!IsReloading(iSlot)) return(false);

  if(!iSlot)
  {
    if(EffectVar(1,this(),GetEffect("Reload",this())) == 1) return(false);
    EffectVar(1,this(),GetEffect("Reload",this())) = 1;
    return(true);
  }
  
  for(var i;i<=GetEffectCount("Reload",this());i++)
  {
    if(EffectVar(2,this(),GetEffect("Reload",this(),i))==iSlot)
    {
      if(EffectVar(1,this(),GetEffect("Reload",this(),i)) == 1) return(false);
      EffectVar(1,this(),GetEffect("Reload",this(),i)) = 1;
      return(true);
    }
  }
}

private func Reloaded(caller,slot,amount) //Waffe nachgeladen
{
  //Munitionsart wo rein muss
  var ammoid = GetFMData(FM_AmmoID);
  //Wie viel?
  amount = Min(amount,MaxReloadAmount(caller)-GetAmmo2(slot));
  //Hier einfügen
  DoAmmo2(slot, ammoid, amount, this());
  //Dem Clonk abziehen
  DoAmmo(ammoid, -amount, caller);

  HelpMessage(caller->GetOwner(),"$Reloaded$",caller,amount,ammoid);

  //Callback
  OnReloaded(firemode,slot);
}

/* Firemode */

private func CycleFM(int iDir)
{
  var fm = firemode;
  fm += iDir;
  if(iDir < 0) {
  	if(!fm) fm = GetFMCount();
  }
  if(iDir > 0) {
  	if(!GetFMData(FM_Name, fm)) fm = 1;
  }
  SetFireMode(fm);
  if(GetSpeedMenu())
    GetSpeedMenu()->NoClose();
  return 1;
}

public func SetFireMode(int i)
{
  if((i > GetFMCount()) || i < 1) {	Message("Feuermodus nicht vorhanden:|{{%i}} FM: %d",this,GetID(),i); return();	}

  // Gleicher Modus: Nur nachladen wenn nicht mehr voll und lädt nicht nach
  if(i == firemode)
  {
    if(CheckAmmo(GetFMData(FM_AmmoID, i),GetFMData(FM_AmmoLoad),this())) return(); 
    if(IsReloading(i)) return();
  }
  
  CancelReload(firemode);
   
  // Schussmodus umstellen
  firemode = i;
  stopauto = false;
  ratecount = GetFMData(FM_AmmoRate, i);
  
  ResumeReload(i);
  
  //Sound
  Sound("WPN2_Switch*.ogg");
  
  //Helpmessage
  HelpMessage(GetUser()->GetOwner(),"$FireModeChanged$",GetUser(),GetFMData(FM_Name),GetFMData(FM_AmmoID));
  
  return(1);
}

public func GetFMData(int data, int i, int t)
{
  // Vom Feuermodus i Information holen
  // i nicht angegeben? Muss nicht, ist aktueller dann
  if(!i) i=firemode;
  if(!t) t=GetFireTec(i);
  
  var value,ammoid;
  if(CheckFireTec(t,i))
  {
    value = ObjectCall(this(), Format("FMData%dT%d",i,t), data);
    ammoid = ObjectCall(this(), Format("FMData%dT%d",i,t), FM_AmmoID);
  }
  else
  {
    value = ObjectCall(this(), Format("FMData%d",i), data);//Abwärtskomplatiblität
    ammoid = ObjectCall(this(), Format("FMData%d",i), FM_AmmoID);
  }
    
  if(ammoid == STAP)
    value = ammoid->~FMMod(data,value);

  var effect,user = GetUser(),j;
  while(effect = GetEffect("*Bonus*",this(),j) || j == 0)
  {
    j++;
    if(!GetEffect("*Bonus*",this(),effect,1))
      continue;
    var tval = EffectCall(this(),effect,"FMData",data,value);
    if(tval)
      value = tval;
  }
  if(user)
  {
    for(var i; i <= GetEffectCount("*Bonus*",user); i++) {
      var tval = EffectCall(user,GetEffect("*Bonus*",user,i-1),"FMData",data,value,i);
      if(tval)
        value = tval;
    }
  }
  
  return(value);
}

/* Feuertechniken (Burst,etc.) */

public func GetFireTec(int iFM)
{
  if(!iFM) iFM = firemode;
  return(aFM_FireTec[iFM-1]);
}

public func SetFireTec(int iFT,int iFM, bool bNoCalls)
{
  //Nicht mehr weiterfeuern
  var iBurstEffect = GetEffect("BurstFire", this());
	if(iBurstEffect)
		EffectVar(0,this,iBurstEffect) = 0;
	while(GetEffect("Recharge", this)) {
		RemoveEffect("Recharge", this);
	}
	StopAutoFire();

  if(!iFM) iFM = firemode;
  if(!GetFMData(FT_Condition,iFM,iFT)) return(false);

  var last = aFM_FireTec[iFM-1];
  aFM_FireTec[iFM-1] = iFT;
  
  //diverse sachen aktualisieren
  ratecount = GetFMData(FM_AmmoRate, iFM);
  
  if(!bNoCalls && (last != iFT))
  {
    Sound("WPN2_Switch*.ogg");
    OnSelectFT(iFM,iFT,last);
  }
  
  return(true);
}

public func CheckFireTec(int iFT, int iFM)
{
  if(!iFM) iFM=firemode;
  if(!iFT) iFT=GetFireTec(iFM);

  if(ObjectCall(this(), Format("FMData%dT%d",iFM,iFT),FT_Name))
    return(true);
  return(false);
}

private func CycleFT(int iDir)
{
  var fm = firemode;
  var ft = GetFireTec(fm);
  ft += iDir;
  if(iDir < 0) {
  	if(!ft) ft = GetFTCount(fm)-1;
  }
  if(iDir > 0) {
  	if(ft > GetFTCount(fm)-1) ft = 1;
  }
  SetFireTec(ft, fm);
  if(GetSpeedMenu())
    GetSpeedMenu()->NoClose();
  return 1;
}

/* Schuss */

private func Shoot(object caller)// Feuern mit Feuermodus
{
  var user = GetUser();
  if(user)
  {
    var x,y;
    user->WeaponEnd(x,y);
    
    if(!PathFree(GetX(),GetY(),GetX()+x,GetY()+y) && !GBackSolid(x, y)) {
      PlayerMessage(GetOwner(user), "$NotAbleToShoot$", user);
      return(false);
    }
  }

  var ammoid = GetFMData(FM_AmmoID);
  //soll er zielen, zielt aber nicht?
  if(!(GetUser()->~IsAiming()) && GetFMData(FM_Aim) == 1)
    stopauto=true;//abbrechen
  // Feuern...
  if(CheckFireTec())
    Call(Format("Fire%dT%d",firemode,GetFireTec()));
  else
    Call(Format("Fire%d",firemode));
  DoSpread(+GetFMData(FM_SpreadAdd));

  // Munition abziehen
  if(ratecount == 1)
  {
    var muni = GetFMData(FM_AmmoUsage);
    DoAmmo(ammoid,-muni);
    ratecount = GetFMData(FM_AmmoRate);
  }
  else
    --ratecount;

  // Pause nach einem Schuss
  if(GetAmmo(ammoid)) Recharge();
  else
  {
    OnEmpty();
    if(GetFMData(FM_Auto)) OnAutoStop(firemode);

    // Automatisch nachladen, wenn die Feuertaste nach 5 Frames noch gedrückt ist
    if(GetPlrCoreJumpAndRunControl(GetUser()->GetController()))
      ScheduleCall(this(), "Reload", 5);
  }
  // HZCK soll Munition doch bitte neu anschauen
  GetUser()->~UpdateCharge();
  return(true);
}

/* Neues Zielsystem */

public func DoSpread(int iChange)
{
  var user = this()->~GetUser();
  if(!user) return();
  if(!user->~IsClonk()) return();
  
  user->DoSpread(iChange,GetFMData(FM_MaxSpread));
}

public func GetSpread()
{
  var user = this()->~GetUser();
  if(!user) return();
  if(!user->~IsClonk()) return();
  
  return(user->GetSpread());
}

public func Selection(object pContainer)
{
  //Startstreuung hinzufügen
  var diff = GetFMData(FM_StartSpread) - GetSpread();
  if(diff > 0)
    DoSpread(diff);
    
  // Callback
  this()->~OnSelect(firemode);
  // Laden wiederaufnehmen
  this()->~ResumeReload();
}


/* Spezialzeugs wegen Silencer! */

global func FxShowWeaponTimer(object pTarget, int iNumber, int iTime)
{
  // Waffe aktualisieren:
  var xoff, yoff, r;  // Offset, Winkel
  // kein Inventar oder falsche Aktion
  if(!Contents(0,pTarget))
  {
    EffectVar(0, pTarget, iNumber) = 0;
    if(EffectVar(6, pTarget, iNumber))
    {
      SetObjDrawTransform(1000,0,0,0,1000,0,EffectVar(6,pTarget,iNumber));
      
      //Silencer
      SetObjDrawTransform(1000,0, EffectVar(6,pTarget,iNumber)->~BarrelXOffset()-(GetDefWidth()*1000/2),0,1000, EffectVar(6,pTarget,iNumber)->~BarrelYOffset(),EffectVar(6,pTarget,iNumber),1);

      EffectVar(6, pTarget, iNumber) = 0;
    }
    SetGraphics(0, pTarget, 0, WeaponDrawLayer);
    return(FX_OK);
  }
  //Die Waffe momentan überhaupt anzeigen?
  if(!(pTarget->~WeaponAt(xoff, yoff, r)))
  {
    EffectVar(0, pTarget, iNumber) = 0;
    if(EffectVar(6, pTarget, iNumber))
    {
      SetObjDrawTransform(1000,0,0,0,1000,0,EffectVar(6,pTarget,iNumber));
      
      //Silencer
      SetObjDrawTransform(1000,0, EffectVar(6,pTarget,iNumber)->~BarrelXOffset()-(GetDefWidth()*1000/2),0,1000, EffectVar(6,pTarget,iNumber)->~BarrelYOffset(),EffectVar(6,pTarget,iNumber),1);
      
      EffectVar(6, pTarget, iNumber) = 0;
    }
    SetGraphics(0, pTarget, 0, WeaponDrawLayer);
    return(FX_OK);
  }
  var obj = Contents(0,pTarget), id=GetID(obj);
  // Waffe nicht mehr aktuell
  if(EffectVar(0, pTarget, iNumber) != id) {
    // neues Objekt ist Waffe, oder ein Objekt, das gezeichnet werden soll
    if(obj->~IsWeapon() || obj->~IsDrawable())
    {
      EffectVar(0, pTarget, iNumber) = id;
      EffectVar(6, pTarget, iNumber) = obj;
      SetGraphics(0, pTarget,id, WeaponDrawLayer, GFXOV_MODE_Object,0,GFX_BLIT_Parent,obj);
    }
    // neues Objekt ist keine Waffe
    else
    {
      EffectVar(0, pTarget, iNumber) = 0;
      if(EffectVar(6, pTarget, iNumber))
      {
        SetObjDrawTransform(1000,0,0,0,1000,0,EffectVar(6,pTarget,iNumber));
        
        EffectVar(6, pTarget, iNumber) = 0;
      }
      SetGraphics(0, pTarget, 0, WeaponDrawLayer);
      return(FX_OK);
    }
  }

  id = EffectVar(0, pTarget, iNumber);
  obj = EffectVar(6, pTarget, iNumber);

  if(!obj) return -1;

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

  //Waffe
  SetObjDrawTransform(1000,xskew,xoff,yskew,1000,yoff, pTarget, WeaponDrawLayer); //position
  SetObjDrawTransform(width,xskew,0,yskew,height,0, obj); //Größe und Rotation
  
  //Daten
  var w = GetDefCoreVal("Width",0,id)/2;
  var brlx = DefinitionCall(id,"BarrelXOffset");
  var brly = DefinitionCall(id,"BarrelYOffset");

  // abspeichern, damit abrufbar
  r = -r-90;
  var r2 = (Angle(0,0,w-brlx/1000,brly/1000)-90)*dir;
  var dist = Distance(0,0,w*1000-brlx,brly);
  EffectVar(1, pTarget, iNumber) = r;
  EffectVar(2, pTarget, iNumber) = xoff-Sin(r,size*w);
  EffectVar(3, pTarget, iNumber) = yoff+Cos(r,size*w);
  EffectVar(4, pTarget, iNumber) = xoff+Sin(r+r2,size*(dist))/1000;
  EffectVar(5, pTarget, iNumber) = yoff-Cos(r+r2,size*(dist))/1000;
}

func Hit()
{
  Sound("WPN2_Hit*.ogg");
}

global func SALaunchBullet(int iX, int iY, int iOwner, int iAngle, int iSpeed, int iDist, int iDmg, int iRemoveTime, id idType)
{
  var ammoid = idType;
  if(!ammoid) ammoid = SHTX;
  
  var iSize = 2;
  var iTrail = iDmg*10;
  
  var ammo = CreateObject(ammoid,iX,iY,iOwner);
  ammo->CustomLaunch(iAngle,iSpeed,iDist,iSize,iTrail,iDmg,iRemoveTime);
  
  return(ammo);
}

global func SABulletCasing(int iX, int iY, int iXDir, int iYDir, int iSize, int iColor, id idType)
{
  var ammoid = idType;
  if(!ammoid) ammoid = SHTX;
  
  return(ammoid->CustomBulletCasing(GetX()+iX,GetY()+iY,iXDir,iYDir,iSize,iColor));
}

global func SAMuzzleFlash(int iSize, object pTarget, int iX, int iY, int iAngle, int iColor, id idType)
{
  var ammoid = idType;
  if(!ammoid) ammoid = SHTX;
  
  return(ammoid->CustomMuzzleFlash(iSize,pTarget,GetX()+iX,GetY()+iY,iAngle,iColor));
}
