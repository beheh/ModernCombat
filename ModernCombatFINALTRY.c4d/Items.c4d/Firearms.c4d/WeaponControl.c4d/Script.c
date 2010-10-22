/*-- Waffensteuerung --*/

#strict 2
#include WEPN

local stopauto,firemode,shooting,ratecount,stopburst;

local aSlot_Type;			//Munitionstyp
local aSlot_Amount;			//Munitionsmenge

local aFM_FireTec;			//Feuertechnik

static const FM_Slot          = 13;	//Slot des Feuermodus
static const FM_SingleReload  = 14;	//Zeit des einzelnen Nachladens bei Revolversystemen (z.B. für Schrotflinten)
static const FM_PrepareReload = 15;	//Zeit bevor das eigentliche Nachladen beginnt (nur interessant wenn auch FM_SingleReload benutzt wird)
static const FM_FinishReload  = 16;	//Zeit nach dem Nachladen (nur interessant wenn auch FM_SingleReload benutzt wird)
static const FM_BurstAmount   = 17;	//Anzahl Schussabrufe pro Burst-Betätigung
static const FM_BurstRecharge = 18;	//Zeit zwischen den einzelnen Bursts

static const FM_SpreadAdd     = 19;	//Bei jedem Schuss hinzuzuaddierende Streuung
static const FM_StartSpread   = 20;	//Bei Auswahl der Waffe gesetzte Streuung
static const FM_MaxSpread     = 21;	//Maximaler Streuungswert
static const FM_UnSpread      = 22;	//Bei jedem Schuss abzuziehende Streuung

static const FT_Name          = 23;	//Name der Feuertechnik
static const FT_Icon          = 24;	//Icondefinition der Feuertechnik
static const FT_IconFacet     = 25;	//Facet, siehe AddMenuItem
static const FT_Condition     = 26;	//Wie FM_Condition, für Feuertechniken

static const MC_CanStrike     = 27;	//Waffe kann Kolbenschlag ausführen
static const MC_Damage        = 28;	//Schaden eines Kolbenschlages
static const MC_Recharge      = 29;	//Zeit nach Kolbenschlag bis erneut geschlagen oder gefeuert werden kann
static const MC_Power         = 30;	//Wie weit das Ziel durch Kolbenschläge geschleudert wird
static const MC_Angle         = 31;	//Mit welchem Winkel das Ziel durch Kolbenschläge geschleudert wird

public func IsWeapon2() {return true;}	//Nutzt/inkludiert neues Waffensystem WPN2
public func NoWeaponChoice() {return GetID() == WPN2;}

public func OnSelectFT(int iFireMode, int iFireTec, int iLastFireTec){}
public func OnSingleReloadStart(int iSlot){}
public func OnSingleReloadStop(int iSlot){}
public func OnPrepareReloadStop(int iSlot){}
public func OnFinishReloadStart(int iSlot){}
public func OnFinishReloadStop(int iSlot){}

/*----- Initalisierung -----*/

protected func Initialize()
{
  aSlot_Type = CreateArray();
  aSlot_Amount = CreateArray();
  aFM_FireTec = CreateArray();

  var i = 1;
  while(CheckFireTec(1,i))
  {
    //Standardfeuermodus setzen
    aFM_FireTec[i-1] = 1;
    i++;
  }

  return inherited();
}

/* Standardeinstellungen */

public func Default(int data)
{
  if(data == FT_Name)      return;//Wichtig
  if(data == FT_Condition) return true;
  if(data == FM_Slot)      return;
  return inherited(data);
}

/*----- Schussmodimenü -----*/

public func ControlDigDouble(caller)
{
  return FMMenu(caller);
}

public func FMMenu(clonk)
{
  if(!clonk) clonk = GetUser();
  SetUser(clonk);
  
  var ring = CreateSpeedMenu(0,clonk);
  
  var overlay;

  //Manuell nachladen
  if (GetAmmoCount(GetSlot()) < GetFMData(FM_AmmoLoad) && (clonk->~GetAmmo(GetFMData(FM_AmmoID)) >= GetFMData(FM_AmmoUsage) || FindObject(NOAM)))
  {
    overlay = ring->AddThrowItem("$Reload$", "ManualReload",firemode,RICO);
    SetGraphics("1",ring,RICO,overlay,GFXOV_MODE_IngamePicture);
  }

  //Manuell entladen
  if (GetAmmoCount(GetSlot()))
  {
  	overlay = ring->AddDownItem("$AmmoType$","ManualEmpty",firemode,RICO);
    SetGraphics("2",ring,RICO,overlay,GFXOV_MODE_IngamePicture);
  }

  //Feuertechniken durchwechseln
  if (GetFTCount() > 2)
  {
    overlay = ring->AddLeftItem("$FireTecBack$","CycleFT",-1,RICO);
    SetGraphics("5",ring,RICO,overlay,GFXOV_MODE_IngamePicture);

    overlay = ring->AddRightItem("$FireTecForward$","CycleFT",+1,RICO);
    SetGraphics("4",ring,RICO,overlay,GFXOV_MODE_IngamePicture);

	for(var i = 1; i <= GetFTCount(firemode); i++)
    {
      szName = GetFMData(FT_Name, firemode, i);
      if(!szName) continue;
      if(i == GetFireTec(firemode))
        szName = Format("<c ffff00>%s</c>", szName);
      else
        szName = Format("<c eeeeee>%s</c>", szName);	
        ring->AddBottomInfoItem(szName);
    }
  }

  //Feuermodus
  if (GetFMCount() > 1)
  {
    overlay = ring->AddUpItem("$FireModeForward$","CycleFM",+1,RICO);
    SetGraphics("3",ring,RICO,overlay,GFXOV_MODE_IngamePicture);
	
	var szName = "";
    for(var i = 1; i <= GetFMCount(); i++)
    {
      szName = GetFMData(FM_Name, i, 1);
        if(!szName) continue;
      if(i == firemode) 
        szName = Format("<c ffff00>%s</c>", szName);
      else
      szName = Format("<c eeeeee>%s</c>", szName);
      ring->AddTopInfoItem(szName);
    }
  }

  return true;
}

private func ManualReload(fm)
{
  if(IsReloading()) return false;
  return Reload(fm);
}

private func ManualEmpty(unused,fm)
{
  if(IsReloading()) return false;
  Sound("WPN2_Unload.ogg");
  return Empty2(GetSlot(fm));
}

/*----- Slots -----*/

public func GetSlot(int iFM)
{
  return GetFMData(FM_Slot,iFM);
}

public func GetSlotCount()
{
  return GetLength(aSlot_Type);
}

public func GetFMCount()
{
  var i = 1;
  while(GetFMData(FM_Name,i))
    i++;
  return i-1;
}

public func GetFTCount(int iFM)
{
  if(!iFM) iFM = firemode;

  var i = 1;
  while(GetFMData(FT_Name,iFM,i))
    i++;
  return i;
}

/*----- Munitionstyp -----*/

public func GetCurrentAmmoType()
{
  return GetAmmoCount(GetFMData(FM_Slot));
}

public func GetAmmoType(iSlot)
{
  return aSlot_Type[iSlot-1];
}

public func SetAmmoType(int iSlot, id idType)
{
  aSlot_Type[iSlot-1] = idType;
}

/*----- Munitionsmenge -----*/

public func GetCurrentAmmoCount()
{
  return GetAmmoType(GetFMData(FM_Slot));
}

public func GetAmmoCount(iSlot)
{
  return aSlot_Amount[iSlot-1];
}

public func SetAmmoCount(int iSlot, int iAmount)
{
  aSlot_Amount[iSlot-1] = iAmount;
}

/*----- Munitionsverwaltung -----*/

global func GetAmmo(id ammoid, object target)
{
  if(!target) target=this;
  
  if(target->~IsWeapon2())
    return GetAmmo2(target->GetSlot(),target);
  return _inherited(ammoid,target);
}

global func GetAmmo2(int slot, object target)
{
  if(!target) target=this;
  
  //Entsprechendes Munitionslagerobjekt suchen
  var obj = target ->~ AmmoStoring();
  if(!obj) obj = target;
  
  //Keine Munition Regel
  if(ObjectCount(NOAM))
    if(target ->~ IsAmmoStorage())
      return;
  
  return target->GetAmmoCount(slot);
}

global func DoAmmo(id ammoid, int change, object target)
{
  if(!target) target=this;

  if(target->~IsWeapon2())
    return DoAmmo2(target->GetSlot(),ammoid,change,target);
  return _inherited(ammoid,change,target);
}

global func DoAmmo2(int slot, id ammoid, int change, object target)
{
  //Gar keine Munition
  if(!(ammoid->~IsAmmo())) return;
  
  //Kann 0 sein bei Objektlokalen Aufrufen
  if(!target) target=this;
  
  //Entsprechendes Munitionslagerobjekt suchen
  var obj = target ->~ AmmoStoring();
  if(!obj) obj = target;
  
  //Keine Munition Regel
  if(ObjectCount(NOAM))
    if(obj ->~ IsAmmoStorage())
      return;
    
  //Alten Munitionsstand speichern
  var oldammoamount = obj->GetAmmo(ammoid);
  var truechange;
  var maxamount = ammoid->~MaxAmount();
  //Der neue Wert wird ausgerechnet, darf aber nicht größer als eventuelles MaxAmount() und nicht kleiner als 0 sein
  if(maxamount == 0)
    truechange= Max(oldammoamount+change, 0);
  else
    truechange= Max(Min(maxamount, oldammoamount+change), 0);
    
  //Neuer Wert dem Objekt geben
  target->SetAmmoCount(slot,truechange);
  target->SetAmmoType(slot,ammoid);
  
  //Wenn Muni alle, Typ nullen
  if(truechange==0) target->SetAmmoType(slot,0);
  
  //Differenz zurückgeben: Tatsächliche Änderung.
  return truechange-oldammoamount;
}

/*----- Entladen -----*/

public func Empty()
{
   Empty2(GetSlot());
}

public func Empty2(int iSlot)
{
  //Am nachladen? Abbrechen
  if(IsReloading(iSlot)) CancelReload(iSlot);
  if(IsRecharging()) RemoveEffect("Recharge", this);

  //Munitionsart identifizieren
  var ammoid = GetFMData(FM_AmmoID);
  //Munitionsmenge feststellen
  var ammoamount = GetAmmo(ammoid, this);
  //Munitionsmenge an Clonk übergeben
  DoAmmo(ammoid, ammoamount, GetUser());
  //Munition aus Waffe entfernen
  DoAmmo2(iSlot, ammoid, -ammoamount, this);
}

/*----- Prüfungen -----*/

global func CheckAmmo(id ammoid, int count)
{
  //Keine Munition?
  if(!(ammoid->~IsAmmo())) return false;

  if(!Par(2)) Par(2) = this;
  var iSum = 0;

  //Suche
  for(var i=2; i<10 && Par(i); ++i)
  {
    //Keine Munition Regel
    var obj = Par(i)->~AmmoStoring();
    if(!obj) obj = Par(i);
    if(ObjectCount(NOAM))
      if(obj->~IsAmmoStorage())
        return true;
    iSum += GetAmmo(ammoid,obj);
  }

  if(iSum >= count)
    return true;
  return false;
}

global func CheckAmmo2(int slot, id ammoid, int count)
{
  //Keine Munition?
  if(!(ammoid->~IsAmmo())) return false;

  if(!Par(3)) Par(3) = this;
  var iSum = 0;

  //Suche
  for(var i=3; i<10 && Par(i); ++i)
  {
    //Keine Munition Regel
    var obj = Par(i) ->~ AmmoStoring();
    if(!obj) obj = Par(i);
    if(ObjectCount(NOAM))
      if(obj ->~ IsAmmoStorage())
        return true;
    iSum += GetAmmo2(slot,obj);
  }

  if(iSum >= count)
    return true;
  return false;
}

public func GetCharge()
{
  var charge;
  var ammoid = GetFMData(FM_AmmoID);

  //Beim Nachladen den Status anzeigen
  if(IsReloading())
  {
    charge = 1000*
      (GetReloadTime()+(GetFMData(FM_Reload)*GetAmmo(ammoid)/GetFMData(FM_AmmoLoad)))/
      GetFMData(FM_Reload)*MaxReloadAmount(GetUser())/GetFMData(FM_AmmoLoad);
  }
  //Ansonsten Ladestand anzeigen
  else
  {
    charge = 1000*GetAmmo(ammoid)/GetFMData(FM_AmmoLoad);
  }

  return charge;
}

public func GetRecharge()
{
  if(IsCanceling())
  {
    return 100*EffectVar(5,this,IsReloading())/GetFMData(FM_FinishReload);
  }

  if(!IsRecharging())
    return 100;
  var time = GetEffect("Recharge",this,0,6);
  return (100*time)/GetFMData(FM_Recharge);
}

/*----- Nachladen beenden-Effekt -----*/

public func FxNoFinishReloadStart(object pTarget, int iNumber, iTemp, int iTime)
{
  EffectVar(0,pTarget,iNumber) = iTime;
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

/*----- Nachladeeffekt -----*/
/*
  0: Komplette Nachladezeit
  1: Nachladen aktiv?
  2: Slot
  3: bereits nachgeladene Munition (Nur FM_SingleReload)
  4: Abgelaufene Nachladezeit
  5: Status -1 Vorbereiten | 0 Nachladen | +1 Beenden
*/

public func FxReloadStart(object pTarget, int iNumber, int iTemp, int iTime,iSlot)
{
  if(iTemp) return;
  
  EffectVar(0,pTarget,iNumber) = GetFMData(FM_Reload)*(GetFMData(FM_AmmoLoad)-GetAmmo2(iSlot))/GetFMData(FM_AmmoLoad);
  EffectVar(1,pTarget,iNumber) = 0; //Vorbereiten.
  EffectVar(2,pTarget,iNumber) = iSlot;
  EffectVar(3,pTarget,iNumber) = 0;  
  EffectVar(4,pTarget,iNumber) = EffectVar(0,pTarget,iNumber);
  EffectVar(5,pTarget,iNumber) = -1;

  //Reload-End-Spam verhindern
  if(GetFMData(FM_SingleReload))
    AddEffect("NoFinishReload", pTarget, 20, 1, pTarget, 0, GetFMData(FM_PrepareReload)+(GetFMData(FM_Reload)/GetFMData(FM_AmmoLoad))); //Vorbereitungszeit + 1
}

public func FxReloadTimer(object pTarget, int iNumber, int iTime)
{
  //Normales Nachladen...
  if(EffectVar(1,pTarget,iNumber))
  {
    if(GetFMData(FM_SingleReload))
    {
      if(iTime-GetFMData(FM_PrepareReload) >= GetFMData(FM_Reload)*(EffectVar(3,pTarget,iNumber)+1)/GetFMData(FM_AmmoLoad))
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
      return;
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
          
        return;
      }
    }
    
    //Beenden...
    if(EffectVar(5,pTarget,iNumber) >= +1)
    {
      EffectVar(5,pTarget,iNumber)++;
      if(EffectVar(5,pTarget,iNumber) >= GetFMData(FM_FinishReload))
      {
        OnFinishReloadStop(EffectVar(2,pTarget,iNumber));
        if(GetFMData(FM_SingleReload))
        	OnSingleReloadStop(EffectVar(2,pTarget,iNumber));
				return -1;
      }
    }
  }
}

public func FxReloadStop(object pTarget, int iNumber, int iReason, bool fTemp)
{
  //Nicht nachladen
  if(!GetAlive(GetUser()) && GetCategory(GetUser())&C4D_Living) return;
  
  //Temporäre Aufrufe ignorieren
  if(fTemp) return;
  Message(" ",pTarget);
  
  if(iReason == 0)
  {
    if(!GetFMData(FM_SingleReload))
      if(EffectVar(4,pTarget,iNumber) <= 0)
        EffectVar(3,pTarget,iNumber) = GetFMData(FM_AmmoLoad)-GetAmmo2(EffectVar(2,pTarget,iNumber));

    pTarget->Reloaded(GetUser(),EffectVar(2,pTarget,iNumber),EffectVar(3,pTarget,iNumber));
  }
}

/*----- Zustände -----*/

/* Ist am Abbrechen */

public func IsCanceling(int iSlot)
{
  var effect = IsReloading(iSlot);
  if(effect)
  {
    if(EffectVar(5,this,effect) == +1)
      return true;
  }
}

/* Ist am Vorbereiten */

public func IsPreparing(int iSlot)
{
  var effect = IsReloading(iSlot);
  if(effect)
  {
    if(EffectVar(5,this,effect) == -1)
      return true;
  }
}

/* Ladezeit abrufen */

public func GetReloadTime(int iSlot)
{
  var effect = IsReloading(iSlot);
  if(effect)
  {
    return EffectVar(0,this,effect)-EffectVar(4,this,effect);
  }
}

/* Ist am Nachladen */

public func IsReloading(int iSlot)
{
  if(!iSlot)
    return GetEffect("Reload",this);
  
  for(var i;i<=GetEffectCount("Reload",this);i++)
  {
    if(EffectVar(2,this,GetEffect("Reload",this,i))==iSlot)
      return GetEffect("Reload",this,i);
  }
}

/*----- Waffe nachladen -----*/

public func Reload(int iFM)
{
  if(!iFM) iFM = firemode;
  var iSlot = GetSlot(iFM);
  if(IsReloading(iSlot)) return false;

  //Verzögerung? Abbrechen
  if(IsRecharging()) RemoveEffect("Recharge", this);
  
  //Hat schon genug?!
  if(GetAmmo2(iSlot,ammoid) >= GetFMData(FM_AmmoLoad))
    return false;

  //Nicht genug Munition um nachzuladen
  var ammoid = GetFMData(FM_AmmoID,iFM);
  var ammousage = GetFMData(FM_AmmoUsage,iFM);
  if(!CheckAmmo(ammoid,ammousage,GetUser()))
    return false;

  //Entleeren falls noch voll
  if(!GetFMData(FM_SingleReload,iFM) || !CheckAmmo(ammoid,GetFMData(FM_AmmoLoad,iFM),GetUser()))
    if(CheckAmmo2(iSlot,ammoid,ammousage,this))
      Empty2(iSlot);

  if(GetFMData(FM_Auto,iFM))
    OnAutoStop();
  OnReload(iFM);
    
  //Stossfeuer gegebenfalls stoppen
  if(GetEffect("BurstFire", this))
  {
    RemoveEffect("BurstFire", this);
  }
    
  AddEffect("Reload", this, 1, 1, this, 0, 0,iSlot);

  return true;
}

/*----- Werfen -----*/

public func ControlThrow(caller)
{
  //Nutzer ist Schütze
  SetUser(caller);

  var meleeattacked;
  //Nahkampfangriff möglich?
  if(!GetEffect("StrikeRecharge", this) && GetMCData(MC_CanStrike) && (caller->~ReadyToFire() || caller->~ReadyToAttack()) && !caller->~IsAiming() && (GetFMData(FM_Aim) == 0 || GetUser()->~IsAiming() || GetUser()->~AimOverride()))
  {
    var dir = GetDir(GetUser());
    //Ziele finden
    var obj = FindObjects(Find_InRect(-15+10*dir,-10,20,20),Find_OCF(OCF_Alive),Find_NoContainer(),Find_Exclude(caller));
    for(var target in obj)
    {
      //Ziel feindlich?
      if(target && CheckEnemy(GetUser(),target))
      {
        //Ziel am kriechen?
        if(WildcardMatch(GetAction(target),"*Crawl*")) //kriecht der Feind?
        {
          //Erhöhten Schaden verursachen
          DoDmg(GetMCData(MC_Damage)*3/2,DMG_Melee,target,0,GetController(GetUser())+1,GetID());
          //Ziel zum Aufstehen zwingen
          ObjectSetAction(target, "KneelUp");
        }
        else
        {
          //Schaden verursachen
          DoDmg(GetMCData(MC_Damage),DMG_Melee,target,0,GetController(GetUser())+1,GetID());
          SetCommand(GetUser(), "");
          SetComDir(COMD_None, GetUser());

          //Ziel schleudern
          var pwr = GetMCData(MC_Power), angle = GetMCData(MC_Angle);
          if(GetProcedure(target) != "SWIM")
          {
            if(!dir)
              dir--;
            SetXDir(Sin(angle*dir,pwr),target,10);
            SetYDir(-Cos(angle*dir,pwr),target,10);
            ObjectSetAction(target, "Tumble");
          }
        }
        if(GetOwner(target) != NO_OWNER && Hostile(GetOwner(target), GetController(GetUser())))
          if(!GetAlive(target) || IsFakeDeath(target))
            DoAchievementProgress(1, AC14, GetOwner(GetUser()));

        //Soundeffekte
        Sound("ClonkMelee*.ogg", 0, this);
        Sound("WPN2_Punch.ogg", 0, this);
        meleeattacked = true;
        AddEffect("StrikeRecharge", this, 1, 1, this);
      }
    }
  }

  //Automatischen Schuss beenden, wenn erneut Werfen gedrückt
  if(IsRecharging())
  {
    if(!GetPlrCoreJumpAndRunControl(GetController(caller)))
      StopAutoFire();

    return 1;
  }

  //Kein Feuer erlauben wenn Kolbenschlag ausgeführt
  if(meleeattacked)
    return 1;

  //Unterstützt der Schussmodus das zielen aber es wird nicht gezielt?
  if(GetFMData(FM_Aim)>0 && !(GetUser()->~IsAiming()) && !(GetUser()->~AimOverride()))
  {
    //Bereit zum Zielen?
    if(GetUser()->~ReadyToAim())
      //Zielen starten
      GetUser()->StartAiming();

    //Nachladen?
    var ammoid = GetFMData(FM_AmmoID);
    var ammousage = GetFMData(FM_AmmoUsage);
    //Nachladen wenn möglich sofern Munition verbraucht
    if(!CheckAmmo(ammoid,ammousage,this))
      if(CheckAmmo(ammoid,ammousage,GetUser()))
      {
        Reload();
      }
      //Nicht genügend Munition
      else
      {
        //Unmöglichkeit des Nachladens angeben
        PlayerMessage(GetOwner(caller), "$NotEnoughAmmo$", caller, ammoid);
      }
    return 1;
  }

  //Feuern! Fehlgeschlagen?
  if(!Fire())
  {
    var ammoid = GetFMData(FM_AmmoID);
    var ammousage = GetFMData(FM_AmmoUsage);
    //Nachladen wenn möglich sofern Munition verbraucht
    if(!CheckAmmo(ammoid,ammousage,this))
      if(CheckAmmo(ammoid,ammousage,GetUser()))
      {
        Reload();
      }
      //Nicht genügend Munition
      else
      {
        //Unmöglichkeit des Nachladens angeben
        PlayerMessage(GetOwner(caller), "$NotEnoughAmmo$", caller, ammoid);
        Sound("WPN2_Empty.ogg");
      }
  }
  else
  {
    if(GetFMData(FM_BurstAmount) > 1 && !GetEffect("BurstFire", this))
    {
      var rechargetime = GetFMData(FM_BurstRecharge);
      if(rechargetime)
        AddEffect("BurstFire", this, 1, rechargetime, this, 0, GetFMData(FM_BurstAmount));
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

public func ControlUpdate(object caller, int comdir, bool dig, bool throw)
{
  SetUser(caller);

  //Bei JnR automatischen Schuss beenden, wenn Werfen losgelassen
  if(IsRecharging() && !throw)
  {
    if(GetEffect("BurstFire", this))
    {
      stopburst = true;
    }
    else
    {
      StopAutoFire();
    }
  }
  //Nicht nachladen, wenn die Munition ausging und der Controller rechtzeitig werfen wieder loslässt
  else if(!throw)
    ClearScheduleCall(this, "Reload");
}

/*----- Stoßfeuereffekt -----*/

public func FxBurstFireStart(object pTarget, int iEffectNumber, int iTemp, iAmount)
{
  EffectVar(0,pTarget,iEffectNumber) = iAmount;
  EffectVar(1,pTarget,iEffectNumber) = -1;
}

public func FxBurstFireTimer(object pTarget, int iNumber, int iTime)
{
  if(!GetUser()) return;
  if(!GetAlive(GetUser()) && GetCategory(GetUser())&C4D_Living) return;
  if(this != Contents(0, GetUser())) return -1;
  if(!pTarget->GetFMData(FM_BurstAmount)) return -1;
  
  EffectVar(0,pTarget,iNumber)--;
  if(EffectVar(0,pTarget,iNumber) > 0)
    pTarget->Fire();
  else
  {
    if(stopburst)
    {
      stopburst = false;
      return -1;
    }
    if(GetPlrCoreJumpAndRunControl(GetController(pTarget)))
    {
      if(EffectVar(1,pTarget,iNumber) > 0)
      {
        EffectVar(1,pTarget,iNumber)--;
      }
      else
      if(EffectVar(1,pTarget,iNumber) < 0)
      {
        EffectVar(1,pTarget,iNumber) = pTarget->GetFMData(FM_BurstRecharge);
      }
      else
      {
        EffectVar(0,pTarget,iNumber) = GetFMData(FM_BurstAmount)+1;
        EffectVar(1,pTarget,iNumber) = -1;
      }
    }
    else
    {
      return -1;
    }
  }
}

public func FxRechargeStop(object pTarget, int iNumber, int iReason, bool fTemp)
{
  //Kein Schütze?
  if(!GetUser()) return;
  //Schütze lebt und ist ein Lebewesen?
  if(!GetAlive(GetUser()) && GetCategory(GetUser())&C4D_Living) return;
  if(GetEffect("ForceNoStop", this))
    stopauto = true;
  //Automatisch weiterschießen, mit JnR auch bei normalen Waffen
  if(GetFMData(FM_Auto, firemode, GetFireTec(firemode)) || GetPlrCoreJumpAndRunControl(GetController(pTarget)))
  {
    //Abbruch bei keiner Munition oder Abbruchbefehl
    if(GetFMData(FM_BurstAmount, firemode, GetFireTec(firemode)) || stopauto || Contents(0, GetUser()) != this && GetUser() == Contained(this) || !(GetUser()->ReadyToFire()) || !CheckAmmo(GetFMData(FM_AmmoID), GetFMData(FM_AmmoUsage)))
    {
      //Callback bei AutoFire
      if(GetFMData(FM_Auto, firemode, GetFireTec(firemode)))
        OnAutoStop(firemode);

      if(!GetEffect("Recharge", pTarget)) stopauto = false;
      shooting = false;
    }
    else
    {
      //Ansonsten Feuer frei
      Shoot(pTarget);
    }
  }
  if (GetEffect("ForceNoStop", this))
  {
    stopauto = false;
	RemoveEffect("ForceNoStop", this);
  }
}

/*----- Schuss -----*/

public func Fire()
{
  //Abbruch bei Maximalstreuung
  if(GetSpread()+GetFMData(FM_SpreadAdd) >= CH_MaxSpread)
    return false;

  //Schütze gibt Feuerfreigabe?
  if(!(GetUser()->~ReadyToFire())) return false; 

  //Lade nach?
  if(IsReloading())
  {
    //Waffe ein Revolversystem?
    if(GetFMData(FM_SingleReload))
      //Laden am beenden?
      if(!GetEffect("NoFinishReload", this))
        //Nicht am Vorbereiten, abbrechen oder gerade mit dem Nachladen begonnen?
      	if(!IsPreparing() || !IsCanceling() || EffectVar(3,this,GetEffect("Reload",this)))
          //Nachladen beenden
          FinishReload();
    //Ansonsten nichts unternehmen
    return false;
  }

  var ammoid = GetFMData(FM_AmmoID);
  var ammousage = GetFMData(FM_AmmoUsage);

  //Waffenmodus leer?
  if(!CheckAmmo(ammoid,ammousage,this))
  {
    OnEmpty(firemode);
    return false;
  }

  //Callback
  if(GetFMData(FM_Auto))
    OnAutoStart(firemode);

  //Ansonsten: Feuer frei!
  Shoot();

  return true;
}

private func Shoot(object caller)
{
  var user = GetUser();
  if(user)
  {
    var x,y;
    user->WeaponEnd(x,y);
    
    if(!PathFree(GetX(),GetY(),GetX()+x,GetY()+y) && !GBackSolid(x, y))
    {
      PlayerMessage(GetOwner(user), "$NotAbleToShoot$", user);
      return false;
    }
  }

  var ammoid = GetFMData(FM_AmmoID);
  //soll er zielen, zielt aber nicht?
  if(!(GetUser()->~IsAiming()) && GetFMData(FM_Aim)>0)
    stopauto=true;//abbrechen
  //Feuern...
  if(CheckFireTec())
    Call(Format("Fire%dT%d",firemode,GetFireTec()));
  else
    Call(Format("Fire%d",firemode));
  DoSpread(+GetFMData(FM_SpreadAdd));

  //Munition abziehen
  if(ratecount == 1)
  {
    var muni = GetFMData(FM_AmmoUsage);
    DoAmmo(ammoid,-muni);
    ratecount = GetFMData(FM_AmmoRate);
  }
  else
    --ratecount;

  //Pause nach einem Schuss
  if(GetAmmo(ammoid)) Recharge();
  else
  {
    OnEmpty();
    if(GetFMData(FM_Auto)) OnAutoStop(firemode);

    //Automatisch nachladen, wenn die Feuertaste nach 5 Frames noch gedrückt ist
    if(GetPlrCoreJumpAndRunControl(GetController(GetUser())))
      ScheduleCall(this, "Reload", 5);
  }
  //HZCK soll Munition doch bitte neu anschauen
  GetUser()->~UpdateCharge();
  return true;
}

/*----- Nachladen -----*/

/* Nachladen abbrechen */

public func CancelReload(int iSlot)
{
  //Nicht am nachladen?
  if(!IsReloading(iSlot)) return false;

  //Kein Slot?
  if(!iSlot)
  {
    RemoveEffect("Reload", this);
    return true;
  }

  for(var i;i<=GetEffectCount("Reload",this);i++)
  {
    if(EffectVar(2,this,GetEffect("Reload",this,i))==iSlot)
    {
      RemoveEffect("Reload", this,i);
      return true;
    }
  }
}

/* Nachladen langsam abbrechen */

public func FinishReload(int iSlot)
{
  //Nicht am nachladen?
  if(!IsReloading(iSlot)) return false;

  //Kein Slot?
  if(!iSlot)
  {
    var effect = GetEffect("Reload",this);
    EffectVar(4,this,effect) = 0;//Färtig!11
    return true;
  }
  
  for(var i;i<=GetEffectCount("Reload",this);i++)
  {
    if(EffectVar(2,this,GetEffect("Reload",this,i))==iSlot)
    {
      var effect = GetEffect("Reload",this,i);
      EffectVar(4,this,effect) = 0;//Färtig!11
      return true;
    }
  }
}

/* Nachladen pausieren */

public func PauseReload(iSlot)
{
  //Nicht am nachladen?
  if(!IsReloading(iSlot)) return false;

  //Kein Slot?
  if(!iSlot)
  {
    if(EffectVar(1,this,GetEffect("Reload",this)) == 0) return false;
    EffectVar(1,this,GetEffect("Reload",this)) = 0;
    return true;
  }
  
  for(var i;i<=GetEffectCount("Reload",this);i++)
  {
    if(EffectVar(2,this,GetEffect("Reload",this,i))==iSlot)
    {
      if(EffectVar(1,this,GetEffect("Reload",this,i)) == 0) return false;
      EffectVar(1,this,GetEffect("Reload",this,i)) = 0;
      return true;
    }
  }
}

/* Nachladen wiederaufnehmen */

public func ResumeReload(iSlot)
{
  //Nicht am nachladen?
  if(!IsReloading(iSlot)) return false;

  //Kein Slot?
  if(!iSlot)
  {
    if(EffectVar(1,this,GetEffect("Reload",this)) == 1) return false;
    EffectVar(1,this,GetEffect("Reload",this)) = 1;
    return true;
  }
  
  for(var i;i<=GetEffectCount("Reload",this);i++)
  {
    if(EffectVar(2,this,GetEffect("Reload",this,i))==iSlot)
    {
      if(EffectVar(1,this,GetEffect("Reload",this,i)) == 1) return(false);
      EffectVar(1,this,GetEffect("Reload",this,i)) = 1;
      return true;
    }
  }
}

/* Waffe ist nachgeladen */

private func Reloaded(caller,slot,amount)
{
  //Munitionsart identifizieren
  var ammoid = GetFMData(FM_AmmoID);
  //Munitionsmenge feststellen
  amount = Min(amount,MaxReloadAmount(caller)-GetAmmo2(slot));
  //Munitionsmenge an Waffe übergeben
  DoAmmo2(slot, ammoid, amount, this);
  //Munition aus Clonk entfernen
  DoAmmo(ammoid, -amount, caller);

  //Geladene Munitionsmenge angeben
  HelpMessage(GetOwner(caller),"$Reloaded$",caller,amount,ammoid);

  //Callback
  OnReloaded(firemode,slot);
}

/*----- Feuermodus (z.B. Kugeln, Granaten, Raketen...) -----*/

/* Modus umschalten */

private func CycleFM(int iDir)
{
  var fm = firemode;
  fm += iDir;
  if(iDir < 0)
  {
    if(!fm) fm = GetFMCount();
  }
  if(iDir > 0)
  {
    if(!GetFMData(FM_Name, fm)) fm = 1;
  }
  if(fm != firemode) SetFireMode(fm);
  if(GetSpeedMenu())
    GetSpeedMenu()->NoClose();
  return 1;
}

/* Modus festlegen */

public func SetFireMode(int i)
{
  if((i > GetFMCount()) || i < 1) return;

  if(IsReloading()) return;
    if(IsRecharging()) StopAutoFire();
    RemoveEffect("BurstFire", this);
    while(GetEffect("Recharge", this)) RemoveEffect("Recharge", this); 

  //Gleicher Modus: Nur nachladen wenn nicht mehr voll und nicht nachladend
  if(i == firemode)
  {
    if(CheckAmmo(GetFMData(FM_AmmoID, i),GetFMData(FM_AmmoLoad),this)) return; 
    if(IsReloading(i)) return;
  }

  CancelReload(firemode);
   
  //Schussmodus umstellen
  firemode = i;
  stopauto = false;
  ratecount = GetFMData(FM_AmmoRate, i);
  
  ResumeReload(i);
  
  //Sound
  Sound("WPN2_Switch*.ogg");
  
  //Gewählten Feuermodus angeben
  HelpMessage(GetOwner(GetUser()),"$FireModeChanged$",GetUser(),GetFMData(FM_Name),GetFMData(FM_AmmoID));

  return 1;
}

/* Feuermodusinfos abrufen */

public func GetFMData(int data, int i, int t)
{
  //Vom Feuermodus i Information holen
  //i nicht angegeben? Muss nicht, ist aktueller dann
  if(!i) i=firemode;
  if(!t) t=GetFireTec(i);

  var value,ammoid;
  if(CheckFireTec(t,i))
  {
    value = ObjectCall(this, Format("FMData%dT%d",i,t), data);
    ammoid = ObjectCall(this, Format("FMData%dT%d",i,t), FM_AmmoID);
  }
  else
  {
    value = ObjectCall(this, Format("FMData%d",i), data);//Abwärtskomplatiblität
    ammoid = ObjectCall(this, Format("FMData%d",i), FM_AmmoID);
  }

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

public func GetFMData()
{
  //Bei Einzel- und Stoßfeuer ist JnR zu schnell, bremsen
  if (Par() == FM_Recharge && GetPlrCoreJumpAndRunControl(GetController(GetUser())) && !GetFMData(FM_Auto))
    return _inherited(...)+2;
  return _inherited(...);
}

/*----- Kolbenschlag -----*/

public func IsRecharging()
{
  return _inherited(...) || GetEffect("StrikeRecharge", this);
}

protected func FxStrikeRechargeTimer()
{
  var iTime = GetEffect("StrikeRecharge", this, 0, 6), iFullTime = GetMCData(MC_Recharge), pHUD;
  //HUD updaten
  if(Contained() && Contents(0, Contained()) == this)
    if (pHUD = FindObject2(Find_ID(1HUD), Find_Owner(GetController(Contained()))))
      pHUD->~Recharge(iTime, iFullTime);
  //Effekt abbrechen?
  if (iTime >= iFullTime)
    return -1;
}

protected func FxStrikeRechargeStop()
{
  stopauto = false;
}

/* Kolbenschlagstandardwerte */

public func GetMCData(int data)
{
  if(data == MC_CanStrike)	return 1;	//Waffe kann Kolbenschlag ausführen
  if(data == MC_Damage)		return 20;	//Schaden eines Kolbenschlages
  if(data == MC_Recharge)	return 40;	//Zeit nach Kolbenschlag bis erneut geschlagen oder gefeuert werden kann
  if(data == MC_Power)		return 20;	//Wie weit das Ziel durch Kolbenschläge geschleudert wird
  if(data == MC_Angle)		return 45;	//Mit welchem Winkel das Ziel durch Kolbenschläge geschleudert wird
}

/*----- Feuertechniken (z.B. Einzel-, Stoß-, Dauerfeuer...) -----*/

/* Technik umschalten */

private func CycleFT(int iDir)
{
  var fm = firemode;
  var ft = GetFireTec(fm);
  ft += iDir;
  if(iDir < 0)
  {
    if(!ft) ft = GetFTCount(fm)-1;
  }
  if(iDir > 0)
  {
    if(ft > GetFTCount(fm)-1) ft = 1;
  }
  if(ft != GetFireTec(fm)) SetFireTec(ft, fm);
  if(GetSpeedMenu())
    GetSpeedMenu()->NoClose();
  return 1;
}

/* Technik festlegen */

public func SetFireTec(int iFT,int iFM, bool bNoCalls)
{
  //Nicht mehr weiterfeuern
  if(IsRecharging()) StopAutoFire();
  RemoveEffect("BurstFire", this);
  if(GetFMData(FM_Auto)) while(GetEffect("Recharge", this)) RemoveEffect("Recharge", this); 

  if(!iFM) iFM = firemode;
  if(!GetFMData(FT_Condition,iFM,iFT)) return false;

  var last = aFM_FireTec[iFM-1];
  aFM_FireTec[iFM-1] = iFT;
  
  //diverse sachen aktualisieren
  ratecount = GetFMData(FM_AmmoRate, iFM);
  
  if (GetFMData(FM_Auto, iFM, iFT))
    if (!GetEffect("Recharge", this))
      stopauto = false;
	else
	  AddEffect("ForceNoStop", this);
  
  if(!bNoCalls && (last != iFT))
  {
    Sound("WPN2_Switch*.ogg");
    OnSelectFT(iFM,iFT,last);
  }
  
  return true;
}

/* Feuertechnikinfos abrufen */

public func GetFireTec(int iFM)
{
  if(!iFM) iFM = firemode;
  return aFM_FireTec[iFM-1];
}

/* Feuertechnik prüfen */

public func CheckFireTec(int iFT, int iFM)
{
  if(!iFM) iFM=firemode;
  if(!iFT) iFT=GetFireTec(iFM);

  if(ObjectCall(this, Format("FMData%dT%d",iFM,iFT),FT_Name))
    return true;
  return false;
}

/*----- Streuung -----*/

/* Streuung erhöhen */

public func DoSpread(int iChange)
{
  //Nutzer festlegen
  var user = this->~GetUser();
  //Kein Nutzer?
  if(!user) return;
  //Nutzer kein Clonk?
  if(!user->~IsClonk()) return;

  //Streuung erhöhen
  user->DoSpread(iChange,GetFMData(FM_MaxSpread));
}

/* Streuung abfragen */

public func GetSpread()
{
  //Nutzer festlegen
  var user = this->~GetUser();
  //Kein Nutzer?
  if(!user) return;
  //Nutzer kein Clonk?
  if(!user->~IsClonk()) return;

  //Streuung abfragen
  return user->GetSpread();
}

/* Streuung hinzufügen */

public func Selection(object pContainer)
{
  //Startstreuung hinzufügen
  var diff = GetFMData(FM_StartSpread) - GetSpread();
  if(diff > 0)
    DoSpread(diff);

  //Callback
  this->~OnSelect(firemode);
  //Laden wiederaufnehmen
  this->~ResumeReload();
}

/* Spezialzeugs wegen Silencer! */

global func FxShowWeaponTimer(object pTarget, int iNumber, int iTime)
{
  //Waffe aktualisieren:
  var xoff, yoff, r;  //Offset, Winkel
  var dodraw;
  //kein Inventar oder falsche Aktion
  if(!Contents(0,pTarget))
    return EffectCall(pTarget, iNumber, "Reset");
  //Die Waffe momentan überhaupt anzeigen?
  if(!(pTarget->~WeaponAt(xoff, yoff, r)))
    return EffectCall(pTarget, iNumber, "Reset");
  var obj = Contents(0,pTarget), id=GetID(obj);
  //Waffe nicht mehr aktuell
  if(obj && EffectVar(6, pTarget, iNumber) != obj) {
    //neues Objekt ist Waffe, oder ein Objekt, das gezeichnet werden soll
    if(obj->~IsWeapon() || obj->~IsDrawable())
    {
	  dodraw = true;
      EffectVar(0, pTarget, iNumber) = id;
      EffectVar(6, pTarget, iNumber) = obj;
      SetGraphics(0, pTarget,id, WeaponDrawLayer, GFXOV_MODE_Object,0,GFX_BLIT_Parent,obj);
    }
    //neues Objekt ist keine Waffe
    else
      return EffectCall(pTarget, iNumber, "Reset");
  }

  id = EffectVar(0, pTarget, iNumber);
  obj = EffectVar(6, pTarget, iNumber);

  if(!obj) return -1;

  //Ausrichtung nach Blickrichtung des Clonks
  //Variablen für die Transformation

  var width, height;  //Breiten- und Höhenverzerrung der Waffe
  var xskew, yskew;   //Zerrung der Waffe, wird zur Rotation gebraucht
  var size;           //Größe der Waffe in der Hand: 1000 = 100%
  //Variablen für die Position
  var xaim, yaim;     //Offset, dass sich durch zielen ergibt
  var dir;            //Richtung in die das Objekt schaut
  
  //schnell noch Rotation dazurechnen oder so!
  if(GetEffect("StrikeRecharge", obj))
  	r += -Max(Sin(GetEffect("StrikeRecharge", obj, 0, 6)*90/(obj->~GetMCData(MC_Recharge)/4),20),0);
  else
  	r += obj->~HandR();
	
  if (!dodraw && r == EffectVar(1, pTarget, iNumber) && GetDir(pTarget) == EffectVar(7, pTarget, iNumber) && GetAction(pTarget) == EffectVar(8, pTarget, iNumber))
    return;
  
  //Variablen mit Werten versehen
  width = height = xskew = yskew = 1;
  size = id->~HandSize();
  if(!size) size = 1000;
  dir  = GetDir()*2-1;
  if(r > 180 || r < -180)
    dir *= -1;
  r *= dir;

  var xfact = size * obj->~HandX();    //Attachpunkte dazurechnen
  var yfact = size * obj->~HandY();

  xoff += Cos(r,xfact)/1000 + dir*Sin(r,yfact)/1000;
  yoff -= Cos(r,yfact)/1000 - dir*Sin(r,xfact)/1000;

  if(dir == 1) 
    height = xskew = yskew = -1;

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
  var brlx = id->~BarrelXOffset();
  var brly = id->~BarrelYOffset();

  //abspeichern, damit abrufbar
  r = -r-90;
  var r2 = (Angle(0,0,w-brlx/1000,brly/1000)-90)*dir;
  var dist = Distance(0,0,w*1000-brlx,brly);
  EffectVar(1, pTarget, iNumber) = r;
  EffectVar(2, pTarget, iNumber) = xoff-Sin(r,size*w);
  EffectVar(3, pTarget, iNumber) = yoff+Cos(r,size*w);
  EffectVar(4, pTarget, iNumber) = xoff+Sin(r+r2,size*(dist))/1000;
  EffectVar(5, pTarget, iNumber) = yoff-Cos(r+r2,size*(dist))/1000;
  EffectVar(7, pTarget, iNumber) = GetDir(pTarget);
  EffectVar(8, pTarget, iNumber) = GetAction(pTarget);
}

global func FxShowWeaponReset(object pTarget, int iNumber)
{
  EffectVar(0, pTarget, iNumber) = 0;
  var wpn = EffectVar(6, pTarget, iNumber);
  if(wpn)
  {
    SetObjDrawTransform(1000,0,0,0,1000,0,wpn);
    //Silencer
    SetObjDrawTransform(1000,0, wpn->~BarrelXOffset()-(GetDefWidth()*500),0,1000, wpn->~BarrelYOffset(),wpn,1);
    EffectVar(6, pTarget, iNumber) = 0;
  }
  SetGraphics(0, pTarget, 0, WeaponDrawLayer);
  return FX_OK;
}

/*----- Allgemein -----*/

/* Schuss erstellen */

global func SALaunchBullet(int iX, int iY, int iOwner, int iAngle, int iSpeed, int iDist, int iDmg, int iRemoveTime, id idType)
{
  //ID des Projektils identifizieren
  var ammoid = idType;
  //Standard nutzen wenn nicht gefunden
  if(!ammoid) ammoid = SHTX;

  var iSize = 2;
  var iTrail = iDmg*10;

  //Projektil erstellen
  var ammo = CreateObject(ammoid,iX,iY,iOwner);
  ammo->CustomLaunch(iAngle,iSpeed,iDist,iSize,iTrail,iDmg,iRemoveTime);

  return ammo;
}

/* Patronenhülse erstellen */

global func SABulletCasing(int iX, int iY, int iXDir, int iYDir, int iSize, int iColor, id idType)
{
  //ID des Projektils identifizieren
  var ammoid = idType;
  //Standard nutzen wenn nicht gefunden
  if(!ammoid) ammoid = SHTX;

  //Patronenhülse erstellen
  return ammoid->CustomBulletCasing(GetX()+iX,GetY()+iY,iXDir,iYDir,iSize,iColor);
}

/* Mündungsfeuer erstellen */

global func SAMuzzleFlash(int iSize, object pTarget, int iX, int iY, int iAngle, int iColor, id idType)
{
  //ID des Projektils identifizieren
  var ammoid = idType;
  //Standard nutzen wenn nicht gefunden
  if(!ammoid) ammoid = SHTX;

  //Mündungsfeuer erstellen
  return ammoid->CustomMuzzleFlash(iSize,pTarget,GetX()+iX,GetY()+iY,iAngle,iColor);
}

func Hit()
{
  Sound("WPN2_Hit*.ogg");
}
