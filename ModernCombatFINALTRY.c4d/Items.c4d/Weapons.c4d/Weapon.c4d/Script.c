/*-- Waffe --*/

#strict 2
#include WEPN

local stopauto,firemode,shooting,ratecount,stopburst;
local iAttachment,pLaser,pBeam,pLight;

local aSlot_Type;			//Munitionstyp
local aSlot_Amount;			//Munitionsmenge
local aFM_FireTec;			//Feuertechnik


//Info: Bei neuer FM_DATA muss MaxFMDataCount() entsprechend angepasst werden
static const FM_Slot		= 13;			//Slot des Feuermodus
static const FM_SingleReload	= 14;			//Zeit des einzelnen Nachladens bei Revolversystemen (z.B. für Schrotflinten)
static const FM_PrepareReload	= 15;			//Zeit bevor das eigentliche Nachladen beginnt (nur interessant wenn auch FM_SingleReload benutzt wird)
static const FM_FinishReload	= 16;			//Zeit nach dem Nachladen (nur interessant wenn auch FM_SingleReload benutzt wird)
static const FM_BurstAmount	= 17;			//Anzahl Schussabrufe pro Burst
static const FM_BurstRecharge	= 18;			//Zeit zwischen einzelnen Bursts
static const FM_ProjectileID	= 36;			//ID des verschossenen Projektils

static const FM_SpreadAdd	= 19;			//Bei jedem Schuss hinzuzuaddierende Streuung
static const FM_StartSpread	= 20;			//Bei Auswahl der Waffe gesetzte Streuung
static const FM_MaxSpread	= 21;			//Maximaler Streuungswert
static const FM_UnSpread	= 22;			//Bei jedem Schuss abzuziehende Streuung
static const FM_NoAmmoModify	= 23;			//Kein Ent-/Nachladen möglich
static const FM_MinSpread	= 33;			//Kleinstmögliche Streuung

static const FM_MultiHit	= 34;			//Anzahl möglicher Treffer pro Kugel
static const FM_MultiHitReduce	= 35;			//Schadensreduzierung pro Treffer

static const FT_Name		= 24;			//Name der Feuertechnik
static const FT_Icon		= 25;			//Icondefinition der Feuertechnik
static const FT_IconFacet	= 26;			//Facet, siehe AddMenuItem
static const FT_Condition	= 27;			//Wie FM_Condition, für Feuertechniken

static const MC_CanStrike	= 28;			//Waffe kann Kolbenschlag ausführen
static const MC_Damage		= 29;			//Schaden eines Kolbenschlages
static const MC_Recharge	= 30;			//Zeit nach Kolbenschlag bis erneut geschlagen oder gefeuert werden kann
static const MC_Power		= 31;			//Wie weit das Ziel durch Kolbenschläge geschleudert wird
static const MC_Angle		= 32;			//Mit welchem Winkel das Ziel durch Kolbenschläge geschleudert wird

static const BOT_Precision	= 105;			//Bestimmt die nötige Winkeldifferenz zwischen Zielwinkel und Winkel von Bot und Zielobjekt

static const AT_NoAttachment	= 0;			//Kein Waffenaufsatz
static const AT_ExtendedMag	= 1;			//Erweitertes Magazin
static const AT_Bayonet		= 2;			//Bajonett
static const AT_Laserpointer	= 4;			//Laserpointer
static const AT_Silencer	= 8;			//Schalldämpfer
static const AT_Flashlight	= 16;			//Taschenlampe

public func MaxFMDataCount()	{return 36;}		//Anzahl FM/FT-Datentypen
public func IsWeapon2()		{return true;}		//Nutzt/inkludiert neues Waffensystem WPN2
public func IsPrimaryWeapon()	{return true;}		//Standard für QuickInventory
public func NoWeaponChoice()	{return GetID() == WPN2;}

public func OnSelectFT(int iFireMode, int iFireTec, int iLastFireTec)	{}
public func OnSingleReloadStart(int iSlot)				{}
public func OnSingleReloadStop(int iSlot)				{}
public func OnPrepareReloadStop(int iSlot)				{}
public func OnFinishReloadStart(int iSlot)				{}
public func OnFireStop(int iSlot)					{}
public func NeedBotControl()						{return false;}	//KI-Kontrolle

public func GetPathFreeX()	{return;}		//Waffenspezifische X-Koordinaten-Anrechnung für den Projektilaustritt
public func GetPathFreeY()	{return;}		//Waffenspezifische Y-Koordinaten-Anrechnung für den Projektilaustritt


/*----- Initialisierung -----*/

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

  UpdateFMDataCache();

  return inherited();
}

/* Standardeinstellungen */

public func Default(int data)
{
  if(data == FT_Name)		return;
  if(data == FT_Condition)	return true;
  if(data == FM_Slot)		return;
  if(data == BOT_Precision)	return 180;
  if(data == FM_MultiHit)	return 1;
  if(data == FM_MultiHitReduce)	return 100;
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
  if(GetAmmoCount(GetSlot()) < GetFMData(FM_AmmoLoad) && (clonk->~GetAmmo(GetFMData(FM_AmmoID)) >= GetFMData(FM_AmmoUsage) || FindObject(NOAM)) && !GetFMData(FM_NoAmmoModify))
  {
    overlay = ring->AddThrowItem("$Reload$", "ManualReload",firemode,SMIN);
    SetGraphics("1",ring,SMIN,overlay,GFXOV_MODE_IngamePicture);
  }

  //Manuell entladen
  if(GetAmmoCount(GetSlot()) && !GetFMData(FM_NoAmmoModify))
  {
    overlay = ring->AddDownItem("$AmmoType$","ManualEmpty",firemode,SMIN);
    SetGraphics("2",ring,SMIN,overlay,GFXOV_MODE_IngamePicture);
  }

  //Feuertechniken durchwechseln
  if(GetFTCount() > 2)
  {
    overlay = ring->AddLeftItem("$FireTecBack$","CycleFT",-1,SMIN);
    SetGraphics("5",ring,SMIN,overlay,GFXOV_MODE_IngamePicture);

    overlay = ring->AddRightItem("$FireTecForward$","CycleFT",+1,SMIN);
    SetGraphics("4",ring,SMIN,overlay,GFXOV_MODE_IngamePicture);

    var szName = "";

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
    overlay = ring->AddUpItem("$FireModeForward$","CycleFM",+1,SMIN);
    SetGraphics("3",ring,SMIN,overlay,GFXOV_MODE_IngamePicture);

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

private func IsSelecting()
{
  return GetEffect("SelectItem", this) != 0;
}

private func VerifySelection()
{
  if(IsSelecting())
  {
    PlayerMessage(GetOwner(GetUser()), "$CantUse$", GetUser());
    return;
  }
  //true zurückgeben um überprüfende Funktionen abzubrechen
  return true;
}

private func ManualReload(fm)
{
  if(IsReloading()) return false;
  if(GetFMData(FM_NoAmmoModify)) return false;
  if(!VerifySelection())
  {
    return;
  }
  return Reload(fm);
}

private func ManualEmpty(unused,fm)
{
  if(IsReloading()) return false;
  if(GetFMData(FM_NoAmmoModify)) return false;
  if(IsSelecting())
  {
    PlayerMessage(GetOwner(GetUser()), "$CantUse$", GetUser());
    return false;
  }
  Sound("ResupplyOut*.ogg");
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
  while(ObjectCall(this, Format("FMData%d", i), FM_Name))
    i++;
  return i-1;
}

public func GetFTCount(int iFM)
{
  if(!iFM) iFM = firemode;

  var i = 1;
  while(ObjectCall(this, Format("FMData%dT%d", iFM, i), FT_Name))
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