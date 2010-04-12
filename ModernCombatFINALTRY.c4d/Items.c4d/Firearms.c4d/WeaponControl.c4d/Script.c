/*-- Waffe 2.0 --*/

#strict 
#include WEPN

local stopauto,firemode,shooting,ratecount;

local aSlot_Type;//Hält den Munitionstyp.
local aSlot_Amount;//Hält die Munitionsmenge.

local aFM_FireTec;//Hält die Feuertechnik.

local idBulletID;//Bei Projektielmunition hält dies die ProjektilID.

static const FM_Slot          = 13;//Der Slot vom Feuermodus.
static const FM_SingleReload  = 14;//Munition wird einzeln nachgeladen. (z.B. für Shotguns)
static const FM_PrepareReload = 15;//Zeit bevor das eigentliche Nachladen beginnt. (Nur interessant wenn auch FM_SingleReload benutzt wird.)
static const FM_FinishReload  = 16;//Zeit nach dem Nachladen. (Nur interessant wenn auch FM_SingleReload benutzt wird.)
static const FM_BurstAmount   = 17;
static const FM_BurstRecharge = 18;

static const FM_SpreadAdd     = 19;
static const FM_StartSpread   = 20;
static const FM_MaxSpread     = 21;
static const FM_UnSpread      = 22;

static const FT_Name          = 23;//Name der Feuertechnik.
static const FT_Icon          = 24;//Icondefinition der Feuertechnik.
static const FT_IconFacet     = 25;//Facet. Wie bei AddMenuItem.
static const FT_Condition     = 26;//Wie FM_Condition. Nur eben für Feuertechniken.

static const FM_SupportedSA   = 27;//WTH?!

public func IsWeapon2() {return(true);}//Diese Waffe benutzt das neue Waffensystem. Sie includiert also WPN2.
public func NoWeaponChoice() { return(GetID() == WPN2); }

public func OnSelectFT(int iFireMode, int iFireTec, int iLastFireTec){}

public func OnSingleReloadStart(int iSlot){}
public func OnSingleReloadStop(int iSlot){}
public func OnPrepareReloadStop(int iSlot){}
public func OnFinishReloadStart(int iSlot){}

public func Default(int data)    // Standardeinstellungen
{
  if(data == FT_Name)      return(0);//Wichtig!!
  if(data == FT_Icon)      return(FICO);
  if(data == FT_IconFacet) return(0);
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
  
  //Alle Waffen haben Silenc0r! :X
  /*SetGraphics(0,this(),SILE,1,GFXOV_MODE_Base); 
  SetObjDrawTransform(
    1000,0, this()->~BarrelXOffset()-(GetDefWidth()*1000/2),
    0,1000, this()->~BarrelYOffset(),
  this(),1);*/
  
  return(inherited());
}

//Menü
public func ControlDigDouble(caller)
{
  return(FMMenu(caller));
}

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

/*public func FTMenu(caller)
{
  if(!caller) caller = GetUser();
  SetUser(caller);
  
  if(!CheckFireTec(2)) return();//Sollte mindestens 2 Feuertechniken haben.

  var name;
  var i = 1;
  
  //Das Menü für die Schussmodiauswahl
  CreateMenu(GetID(), GetUser(), 0, 0, "$FireTec$",0, C4MN_Style_Normal);
  
  //Solang suchen, bis keine Schussmodi mehr da sind
  while(name = GetFMData(FT_Name, firemode,i))
  {
    AddMenuItem(name,Format("SetFireTec(%d,%d)",i,firemode), GetFMData(FT_Icon, firemode,i), GetUser(), 0, i, Format("$DescFireTec$", name),2,GetFMData(FT_IconFacet, firemode,i));
    i++;
  }
  
  SelectMenuItem (aFM_FireTec[firemode-1]-1,GetUser());
  
  return(true);
}*/

public func FMMenu(clonk)
{
  if(!clonk) clonk = GetUser();
  SetUser(clonk);
  
  var ring = CreateSpeedMenu(0,clonk);
  
  var overlay;
  
  overlay = ring->AddThrowItem("$Reload$","ManualReload",firemode,RICO);
  SetGraphics("1",ring,RICO,overlay,GFXOV_MODE_IngamePicture);

  overlay = ring->AddLeftItem("$Left$","CycleFM",-1,RICO);
  SetGraphics("3",ring,RICO,overlay,GFXOV_MODE_IngamePicture);

  overlay = ring->AddRightItem("$Right$","CycleFM",+1,RICO);
  SetGraphics("3",ring,RICO,overlay,GFXOV_MODE_IngamePicture);

  overlay = ring->AddUpItem("$FireTec$","FTCycle",firemode,RICO);
  SetGraphics("4",ring,RICO,overlay,GFXOV_MODE_IngamePicture);
  
  overlay = ring->AddDownItem("$AmmoType$","ManualEmpty",firemode,RICO);
  SetGraphics("2",ring,RICO,overlay,GFXOV_MODE_IngamePicture);
}
public func CycleSA(int fm)
{
  var safe = 10;
  var a = [];
  var j = 0;
  
  for(var i = 0, idR; idR = GetDefinition(i, C4D_Vehicle) ; i++)
  {
    if(idR->~IsSpecialAmmo())
    {
      if(idR == idBulletID) j = i;
      a[GetLength(a)] = idR;
    }
  }
  
  j++;
  if(j >= GetLength(a)-1)
    j = 0;

  SetSpecialAmmo2(a[j],fm);
  if(GetSpeedMenu())
    GetSpeedMenu()->NoClose();
}

private func CycleFM(int iDir)
{
  var fm = firemode;
  fm = fm + iDir % GetFMCount();
  
  if(!GetFMData(FM_Name,fm) || !fm) {
    if(iDir < 0) {
      fm = GetFMCount();
    }
    else {
      fm = 1;
    }
  }

  SetFireMode(fm);
  if(GetSpeedMenu())
    GetSpeedMenu()->NoClose();
  return 1;
}

public func FMMenuOld(caller)//O_o
{
  if(!caller) caller = GetUser();

  SetUser(caller);

  var name;
  // Das Menü für die Schussmodiauswahl
  CreateMenu(GetID(), GetUser(), 0, 0, "$FireMode$", C4MN_Extra_Info, 0, true);
  // Solang suchen, bis keine Schussmodi mehr da sind
  for(var i = 1; name=GetFMData(FM_Name, i); i++ )
  {
    // Vorraussetzung muss erfüllt sein
    if(GetFMData(FM_Condition, i))
    {
      var id = GetFMData(FM_AmmoID, i);
      var iconid = GetFMData(FM_Icon, i);
      var obj = CreateObject(iconid);
      // Wir haben ein Objekt fürs Menü und legen die Ammografik drauf
      SetGraphics(0, obj, id, 1, GFXOV_MODE_Picture);
      SetObjDrawTransform(650,0,5000,0,650,5000, obj, 1);
      
      // Eintragstext
      var entryname=Format("%s (%s)", name, GetName(0, id));
      
      // Genug Ammo für zumindest einen Schuss vorhanden sein
      if(!CheckAmmo(id,GetFMData(FM_AmmoUsage,i),GetUser(),this()))
      {
        // Name des Eintrags wird grau
        entryname=Format("<c %x>%s</c>", RGB(100,100,100), entryname);
        // Eintrag grau
        obj->SetClrModulation(RGB(100,100,100));
      }
      // Eintrag hinzufügen.
      AddMenuItem(entryname, "ChangeFireMode", GetID(this()), GetUser(), 0, i, Format("$DescFireMode$", name), 4, obj); 
      // Objekte natürlich wieder löschen
      RemoveObject(obj);
    }
  }
  
  SelectMenuItem (firemode-1,GetUser());
  
  return(true);
}

private func ChangeFireMode(unused, i)
{
  //HAR HAR! Nein... denk nicht mal daran. :P
  // Immer noch genügend Munition da?
  /*if(!CheckAmmo(GetFMData(FM_AmmoID,i),GetFMData(FM_AmmoUsage,i),GetUser(),this()))
  {
    // Nein. Mecker.
    Failure(id, i);
    // Menü will geupdatet werden, wahrscheinlich
    var item=GetMenuSelection(GetUser());
    CloseMenu(GetUser());
    FMMenu(GetUser());
    SelectMenuItem (item, GetUser());
    // Fertig.
    return(); 
  }*/
  
  Sound("WPN2_Switch*.ogg");
  
  CloseMenu(GetUser());
  var last = firemode;
  
  // Schussmodus umstellen
  if(SetFireMode(i))
  {
    if(!GetAmmo2(GetSlot(i)) && (i != last))
      return(Reload(i));
  }
  
  if(last == i)
    return(WeaponMenu(i));
}

//private func ManualReload(unused,fm)
private func ManualReload(fm)
{
  Reload(fm);
  //WeaponMenu(fm);
  if(GetSpeedMenu())
    GetSpeedMenu()->NoClose();
}

private func ManualEmpty(unused,fm)
{
  Sound("WPN2_Unload.ogg");
  Empty2(GetSlot(fm));
  //WeaponMenu(fm);
}

public func WeaponMenu(int iFM, int iSel)
{
  if(!iSel)
  {
    if(GetMenu(GetUser()) == GetID())
      iSel = GetMenuSelection(GetUser());
    else
      iSel = -1;
  }
  else
  {
    iSel--;
  }

  CloseMenu(GetUser());
  CreateMenu(GetID(), GetUser(), 0, 0, " ", 0, 0, true);
  
  AddMenuItem ("$Empty$","ManualEmpty",WICO,GetUser(),0,iFM,0,2,WICO_Empty);
  AddMenuItem ("$Reload$","ManualReload",WICO,GetUser(),0,iFM,0,2,WICO_Reload);//Std.
  
  /*if((!GetAmmo2(GetSlot(iFM)) || GetFMData(FM_SingleReload,iFM)) && !IsReloading())
    AddMenuItem ("$Reload$","ManualReload",WICO,GetUser(),0,iFM,0,2,WICO_Reload);
  else
    AddMenuItem ("$Empty$","ManualEmpty",WICO,GetUser(),0,iFM,0,2,WICO_Empty);*/

  FTMenu(iFM);
  
  //AddMenuItem ("$AmmoType$","SAMenu",WICO,GetUser(),0,iFM,0,2,WICO_ATSelect);
  //AddMenuItem ("<Zieltechnik>","FTMenu()",WICO,GetUser(),0,0,0,2,WICO_Aiming);//Noch nicht.
  
  CustomWMItems(iFM);
  
  SetMenuSize (0,3, GetUser());
  
  if(iSel >= 0)
    SelectMenuItem(iSel,GetUser());
  else
    SelectMenuItem(1,GetUser());
  
  return(true);
}

public func SAMenu(unused, fm)
{
  if(GetFMData(FM_AmmoID) != STAM) return();

  CloseMenu(GetUser());
  CreateMenu(GetID(), GetUser(), 0, 0, "$AmmoType$",0,C4MN_Style_Context);
  
  var sel,j = 0;
  for(var i = 0, idR; idR = GetDefinition(i, C4D_Vehicle) ; i++)
    if(idR->~IsSpecialAmmo())
    {
      if(idR == idBulletID) sel = j;
      AddMenuItem(GetName(0,idR),"SetSpecialAmmo2",idR,GetUser(),0,fm,GetDesc(0,idR));
      j++;
    }

  SelectMenuItem (sel,GetUser());
}

//public func SetSpecialAmmo2(id idType, fm)
public func SetSpecialAmmo2(id idType, fm)
{
  Sound("WPN2_Switch*.ogg");
  SetSpecialAmmo(idType);
  //WeaponMenu(fm,3);
}

public func SetSpecialAmmo(id idType)
{
  idBulletID = idType;
}

public func GetSpecialAmmo() {return(idBulletID);}

public func CustomWMItems(int iFM){}

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

//Munikontrolle
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

private func SetFireMode(int i)
{
  if((i > GetFMCount()) || i < 1) {Message("Es wurde ein inexistenter FeuerModus gesetzt.|Bitte an den zuständigen Scripter weiterleiten:|{{%i}} FM: %d",this,GetID(),i); return();}//<- Gegeben! ^^

  // Gleicher Modus: Nur nachladen wenn nicht mehr voll und lädt nicht nach
  if(i == firemode)
  {
    if(CheckAmmo(GetFMData(FM_AmmoID, i),GetFMData(FM_AmmoLoad),this())) return(); 
    if(IsReloading(i)) return();
  }
  
  CancelReload(firemode);
  
  var fm = GetFMCount();
  for(var j;j<=fm;j++)//Alle Nachladeprozesse pausieren. Sinn?
    PauseReload(j);
  
  // Schussmodus umstellen
  firemode=i;
  stopauto=false;
  ratecount = GetFMData(FM_AmmoRate, i);
  
  ResumeReload(i);
  
  // Helpmessage
  HelpMessage(GetUser()->GetOwner(),"$FireModeChanged$",GetUser(),GetFMData(FM_Name),GetFMData(FM_AmmoID));
  
  return(1);
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
    //TODO: Hier gibts noch ein doofes Problem mit der Anzeige. :C (Nicht vollständiges Nachladen.)
    /*charge = 1000*
                (GetReloadTime()+
                (GetFMData(FM_Reload)*GetAmmo(ammoid)/GetFMData(FM_AmmoLoad)))/
            GetFMData(FM_Reload);*/
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
  
  /*
  if(IsPreparing())
  {
    return(100*GetEffect("Reload", this(), IsReloading(),6)/GetFMData(FM_PrepareReload));
  }*/

  if(!IsRecharging())
    return(100);
  var time = GetEffect("Recharge",this(),0,6);
  return((100*time)/GetFMData(FM_Recharge));
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
                                 //GetFMData(FM_Reload)*(GetFMData(FM_AmmoLoad)-GetAmmo2(iSlot))/MaxReloadAmount(GetUser());
  EffectVar(4,pTarget,iNumber) = EffectVar(0,pTarget,iNumber);
  
  EffectVar(2,pTarget,iNumber) = iSlot;
  
  //Vorbereiten.
  EffectVar(1,pTarget,iNumber) = 0;
  
  //Log("Vorbereiten - Start");
  EffectVar(5,pTarget,iNumber) = -1;//Vorbereiten.
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
    if(EffectVar(4,pTarget,iNumber)-- <= 0)//Fertig?
    {
      //Log("Nachladen - Stop");
      //Log("Beenden - Start");
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
      //Log("  Vorbereiten");
      if(iTime >= GetFMData(FM_PrepareReload))
      {
        //Log("Vorbereiten - Stop");
        //Log("Nachladen - Start");
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
      //Log("  Beenden");
      EffectVar(5,pTarget,iNumber)++;
      if(EffectVar(5,pTarget,iNumber) >= GetFMData(FM_FinishReload))
      {
        //Log("Beenden - Stop");
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
  var ammousage = GetFMData(FM_AmmoUsage,iFM);//TODO:->FireTec!!
  if(!CheckAmmo(ammoid,ammousage,GetUser()))
    return(false);

  // entleeren falls noch voll
  if(!GetFMData(FM_SingleReload,iFM) || !CheckAmmo(ammoid,GetFMData(FM_AmmoLoad,iFM),GetUser()))
    if(CheckAmmo2(iSlot,ammoid,ammousage,this()))
      Empty2(iSlot);

  if(GetFMData(FM_Auto,iFM))
    OnAutoStop();
  OnReload(iFM);
    
  AddEffect("Reload", this(), 1, 1, this(), 0, /*Max(1, reloadtime)*/0,iSlot);

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
  if(GetFMData(FM_Aim) && !(GetUser()->~IsAiming()) && !(GetUser()->~AimOverride()))
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
        AddEffect("BurstFire", this(), 1, rechargetime, this(),0,GetFMData(FM_BurstAmount));
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
}

public func Fire()
{
  if(GetSpread()+GetFMData(FM_SpreadAdd) >= CH_MaxSpread)//TODO: Ist ReadyToFire() nicht besser?
    return(false);

  // bereit zum Schießen
  if(!(GetUser()->~ReadyToFire())) return(false); 

  // lädt grad nach... bitte nicht stören..oder doch?
  if(IsReloading())
  {
    if(GetFMData(FM_SingleReload))
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

private func Reloaded(caller,slot,amount)//Waffe nachgeladen
{
  //Munitionsart wo rein muss
  var ammoid = GetFMData(FM_AmmoID);
  //Wie viel?
  amount = Min(amount,MaxReloadAmount(caller));
  //Hier einfügen
  DoAmmo2(slot, ammoid, amount, this());
  //Dem Clonk abziehen
  DoAmmo(ammoid, -amount, caller);

  HelpMessage(caller->GetOwner(),"$Reloaded$",caller,amount,ammoid);

  //Callback
  OnReloaded(firemode,slot);
}

/*public func GetCharge(int iSlot)//wie voll ist die Waffe
{
  var charge;
  var ammoid = GetFMData(FM_AmmoID);
  var ammoamount = MaxReloadAmount(GetUser());
  // lädt nach: Nachladestatus anzeigen
  if(IsReloading(iSlot))
  {
    // Nachladestatus in %
    var done = 1000*EffectVar(0, this(), IsReloading(iSlot))/GetFMData(FM_Reload);
    charge = done*ammoamount/GetFMData(FM_AmmoLoad);
  }
  // ansonsten: Ladestand anzeigen
  else
  {
    charge = 1000*GetAmmo(ammoid)/GetFMData(FM_AmmoLoad);
  }

  return(charge);
}*/

//Feuertechniken (Burst,etc.)
public func GetFireTec(int iFM)
{
  if(!iFM) iFM = firemode;
  return(aFM_FireTec[iFM-1]);
}

public func SetFireTec(int iFT,int iFM, bool bNoCalls)
{
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
    
  // Modifikationen
  /*var ammoid;
  if(CheckFireTec(t,i))
    ammoid = ObjectCall(this(), Format("FMData%dT%d",i,t), FM_AmmoID);
  else
    ammoid = ObjectCall(this(), Format("FMData%d",i), FM_AmmoID);*/
  
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

private func Shoot(object caller)// Feuern mit Feuermodus
{
  var user = GetUser();
  if(user)
  {
    var x,y;
    user->WeaponEnd(x,y);
    
    if(ObjectCount(NOWH))
      if(!PathFree4K(0,0,x,y,4))
        return(false);
  }

  var ammoid = GetFMData(FM_AmmoID);
  //soll er zielen, zielt aber nicht?
  if(!(GetUser()->~IsAiming()) && GetFMData(FM_Aim))
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

/* neues Zielsystem */
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

protected func Selection(object pContainer)
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
        
        //Silencer
        SetObjDrawTransform(1000,0, EffectVar(6,pTarget,iNumber)->~BarrelXOffset()-(GetDefWidth()*1000/2),0,1000, EffectVar(6,pTarget,iNumber)->~BarrelYOffset(),EffectVar(6,pTarget,iNumber),1);
        
        EffectVar(6, pTarget, iNumber) = 0;
      }
      SetGraphics(0, pTarget, 0, WeaponDrawLayer);
      return(FX_OK);
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

  //Waffe
  SetObjDrawTransform(1000,xskew,xoff,yskew,1000,yoff, pTarget, WeaponDrawLayer); //position
  SetObjDrawTransform(width,xskew,0,yskew,height,0, obj); //Größe und Rotation
  
  //Silencer! ;D
  var w = GetDefCoreVal("Width",0,id)/2;
  var brlx = DefinitionCall(id,"BarrelXOffset");
  var brly = DefinitionCall(id,"BarrelYOffset");
  
  SetObjDrawTransform(
    width,xskew,
    +(Cos(r,1000*(brlx-w*1000))/1000 - Sin(r,1000*dir*brly)/1000),
    yskew,height,
    -(Cos(r,1000*dir*brly)/1000 + Sin(r,1000*(brlx-w*1000))/1000),
  obj, 1);
  
  /* Backup ftw!
  
  SetObjDrawTransform(
    width,xskew,
    +(Cos(r,size*(brlx-w*1000))/1000 - Sin(r,size*brly)/1000),
    yskew,height,
    -(Cos(r,size*brly)/1000 + Sin(r,size*(brlx-w*1000))/1000),
  obj, 1);
  
  */
  
  
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
  if(!ammoid) ammoid = this()->~GetSpecialAmmo();
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
  if(!ammoid) ammoid = this()->~GetSpecialAmmo();
  if(!ammoid) ammoid = SHTX;
  
  return(ammoid->CustomBulletCasing(GetX()+iX,GetY()+iY,iXDir,iYDir,iSize,iColor));
}

global func SAMuzzleFlash(int iSize, object pTarget, int iX, int iY, int iAngle, int iColor, id idType)
{
  var ammoid = idType;
  if(!ammoid) ammoid = this()->~GetSpecialAmmo();
  if(!ammoid) ammoid = SHTX;
  
  return(ammoid->CustomMuzzleFlash(iSize,pTarget,GetX()+iX,GetY()+iY,iAngle,iColor));
}
