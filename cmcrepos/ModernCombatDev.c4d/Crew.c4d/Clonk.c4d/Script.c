/*--- der CMC Clonk ---*/

#strict 2
#include CLNK


/** Includes **/

//#include L_DR	// can have drone (with context menu)
#include L_GE	// can use gear (with context menu)
#include L_LA	// can climb on ladders	
//#include L_AG	// is agil
#include L_JP	// can use jetpack
#include L_CA	// can aim

#include L_A2 // has cmc agility
#include L_ND // can use grenades
#include L_CR // can crawl
#include L_SK // uses skins


/** Konstanten **/

// Waffenanzeige
static const WeaponDrawLayer = 1; //Layer in dem die Waffen gezeichnet werden

// Crosshair
static const CH_ShowAlways = 1;

static const CH_MaxSpread = 500;

static const CH_WalkSpread = 80;
static const CH_JumpSpread = 140;
static const CH_ScaleSpread = 200;
static const CH_HangleSpread = 200;

static const CH_CrawlSpreadReduction = 4;
static const CH_AimSpreadReduction = 4;
static const CH_StandSpreadReduction = 3;


/** Lokale Variablen **/

local ammobag;
local wpneffect;
local LastDmgType;
local punchtype; //Muss das sein? Effekte würdens auch tun. :/
local iPain, iPainFactor, pRedHurt;
local killicon;
local LastDmgType;
local spread;
local handr;


/** Allgemeines **/

public func SkinCount() { return 2; }
public func WeaponCollectionLimit() { return 1; }
public func ObjectCollectionLimit() { return 4; }
public func MaxContentsCount() { return WeaponCollectionLimit() + ObjectCollectionLimit(); } // Irritiert jetzt iwie.

protected func Initialize()
{
  wpneffect = AddEffect("ShowWeapon",this,1,1,this,HZCK);

	InitCrosshair();
  AmmoStoring();
  GetHUD(this);

  return inherited(...);
}

protected func Death()
{
  if(ammobag)
    RemoveObject(ammobag);

  if(!this->~Redefine2(CLNK))
    this->~Redefine(CLNK);

  return _inherited(...);
}

protected func Destruction()
{
  if(GetAlive(this))
    Kill();

  if(ammobag)
    RemoveObject(ammobag);
    
  return _inherited(...);
}

public func OnDmg(int iDamage, int iType)
{
	// Ausrüstung will vielleicht mitreden...
	var mod = _inherited(iDamage, iType);

	if(iType == DMG_Fire)		mod -= 5;
	if(iType == DMG_Energy)		mod += 25;
	if(iType == DMG_Bio)		mod -= 10;
	
	return mod;
}

public func CanUse(id idObj)
{
  if(idObj == HARM) return 0;
  return _inherited(idObj,...);
}

private func AbortWalk()
{
  if(GetAction() == "Dive")
  {
    var c = Contents();
    if(c)
    {
      if((c->~IsWeapon() && !c->~GetFMData(FM_Aim)) || c->~IsGrenade())
        SetAction("Jump");
    }
  }
}

protected func ContactBottom()
{
  return 0;
}

public func ReleaseLadderUp()
{
	//nach oben abspringen
	ReleaseLadder(-15*(GetDir()*2-1),-25);
}

public func JumpStart(bool bBackflip)
{
  // bewaffnet?
  CheckArmed();

  _inherited(bBackflip);
}

protected func ComNone()
{
  SetComDir(COMD_None);
}

private func IsJumping()
{
  if(GetAction() == "Dive")
    return true;
  return WildcardMatch(GetAction(),"Jump*");
}

public func StopHealing()
{
  _inherited(...);
  CheckArmed();
}


/* Munitionssystem */

private func AmmoBagContextCheck()
{
  return !FindObject(NOAM);
}

protected func ContextAmmobag(object pCaller)
{
  [$AmmoBag$|Image=AMOC|Condition=AmmoBagContextCheck]
  var ammo, x;
  //Menü erzeugen
  CreateMenu(AMOC,this,0,0,GetName(0,ABAG),0,1,1);
  
  //Munition finden und hinzufügen
  while(ammo = Contents(x,ammobag))
  {
    //nur Ammo
    if(!(ammo->~IsAmmo())) continue;
    //hinzufügen
    AddMenuItem(GetName(ammo),"PackAmmo", GetID(ammo), this, GetAmmo(GetID(ammo)),ObjectNumber(pCaller),GetDesc(ammo));
    //Zählervariable erhöhen
    x++;
  }
  return 1;
}

protected func PackAmmo(id idType, int iCaller)
{
  //Wir übersehen mal, das beliebig viel Muni verpackt werden könnte.
  var pCaller = Object(iCaller);
  
  var pPack = DoAmmoPack(idType);
  
  if(pPack)
  {
    var iChange = pPack->AmmoCount();
  }
  else
  {
    pPack = CreateObject(CUAM,0,0,GetOwner());

    pPack->SetAmmoID(idType);
    iChange = pPack->DoAmmoCount(GetAmmo(idType));
    DoAmmo(idType,-iChange);
  }
  
  if(!Collect(pPack,this))
  {
    DoAmmo(idType,+iChange); 
    RemoveObject(pPack);//What a fail. :C
  }
  else
    Sound("PackAmmo.ogg");
  
  if(this->~AmmoBagContextCheck())
  {
    var sel = GetMenuSelection(pCaller);
    ContextAmmobag(pCaller);
    SelectMenuItem(sel,pCaller);
  }
}

protected func DoAmmoPack(id idType)
{
  var ap = idType->~AmmoPackID();
  if(!ap || !FindDefinition(ap)) return 0;
  
  var my_count = GetAmmo(idType);
  var ap_count = ap->AmmoCount();

  if(my_count < ap_count) return 0;
  
  var pack = CreateObject(ap,0,0,GetOwner());
  DoAmmo(idType,-ap_count);
  
  return pack;
}

func FxAmmoCollectionStart(object pTarget, int iEffectNumber, int iTemp, id idAmmo, int iAmmoCount)
{
  if(iTemp)
    return 1;
  EffectVar(0,pTarget,iEffectNumber) = 100;
  EffectVar(1,pTarget,iEffectNumber) = idAmmo;
  EffectVar(2,pTarget,iEffectNumber) = iAmmoCount;
}

func FxAmmoCollectionEffect(string szNewEffectName, object pTarget, int iEffectNumber, int iNewEffectNumber, id idAmmo, int iAmmoCount)
{
  if(szNewEffectName == "AmmoCollection")
    //return -2;
    // wird nich addiert: alten löschen
    RemoveEffect(0,pTarget,iEffectNumber);
  return 0;
}

func FxAmmoCollectionAdd(object pTarget, int iEffectNumber, string szNewEffectName, int iNewEffectTimer, id idAmmo, int iAmmoCount)
{
  var x = 1;
  while(EffectVar(x,pTarget,iEffectNumber))
  {
    if(EffectVar(x,pTarget,iEffectNumber) == idAmmo)
      break;
    else
      x++;
  }
  EffectVar(x++,pTarget,iEffectNumber) = idAmmo;
  EffectVar(x,pTarget,iEffectNumber) += iAmmoCount;

  EffectVar(0,pTarget,iEffectNumber) += 100;
  return 1;
}

func FxAmmoCollectionTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  var x = 1, str = " ";

  while(EffectVar(x,pTarget,iEffectNumber))
    str = Format("%s|{{%i}} %4d",str,EffectVar(x++,pTarget,iEffectNumber),EffectVar(x++,pTarget,iEffectNumber));
  
  HelpMessage(GetOwner(pTarget),str,pTarget);
  if(iEffectTime >= EffectVar(0,pTarget,iEffectNumber))
    return -1;
}

public func UpdateCharge()
{
  // Nur wenn ich Cursor bin
  if(GetCursor(GetOwner()) != this) return;

  if(GetOwner() < -1) return;

  // in Gebäuden/Fahrzeugen
  if(Contained())
    if(Contained()->~UpdateCharge(this))
      return 1;

  // reitet
  if(IsRiding())
    if(GetActionTarget()->~UpdateCharge(this))
      return 1;

  // ggf. an angefasstes Objekt weiterleiten
  var Content = Contents();
  if(GetAction() == "Push")
  	if(GetActionTarget()->~IsWeapon())
  		Content = GetActionTarget();

  // HUD
  var hud = GetHUD();
  if(hud) hud->Update(Content,AmmoStoring(),this);

	
	/// Crosshair updaten ...
    
	if(!this()->~ReadyToFire() || !this()->~IsArmed() || (GetCursor(GetOwner()) != this()))
	{
		this->~HideCrosshair();
		return _inherited(...);
	}
  
  var c = Contents();

  ShowCrosshair();
  
  DoSpread(BoundBy(TestSpread()-spread,0,10));
  
  var unspread;
  if(c->~IsWeapon())
    unspread = c->GetFMData(FM_UnSpread);
  else
    unspread = c->~UnSpread();

    
  if(this()->~IsAiming())
  {
    if(IsCrawling())
      DoSpread(-(CH_CrawlSpreadReduction+unspread));
    else
      DoSpread(-(CH_AimSpreadReduction+unspread));
  }
  else
  {
    DoSpread(-(CH_StandSpreadReduction+unspread));
  }

  UpdateAiming();
	
  return 1;
}

public func AmmoStoring()
{
  // keiner da: neuen erstellen
  if(!ammobag)
  {
    ammobag = CreateObject(ABAG);
    //ammobag->Init(this);
  }
  return ammobag;
}


/* Nahkampfsystem */



private func Fighting()
{
  if((GetCursor(GetOwner()) != this) || (GetPlayerType(GetController()) == C4PT_Script))//Rudimentäre KI Selbstverteidigung.
  {
    if(!Random(2))
      ControlThrow();
    else
    if(!Random(2))
      ControlDig();
  }
  return 1;
}

private func Punching()
{
  if(!Random(3)) Sound("Kime*");
  Sound("Punch*");
  
  if(punchtype == 0)
  {
    Punch(GetActionTarget());
  }
  else
  if(punchtype == 1)
  {
    if(GetAction(GetActionTarget()) != "Punch")
    {
      Fling(GetActionTarget(), 4 * GetDir() - 2, -1);
      Punch(GetActionTarget(), 10);
    }
    else
      Fling(this, 0, -1);//Schwupps. Selber hingefallen.
  }

  SetComDir(COMD_Stop);

  return 1;
}

public func InCloseCombat()
{
  return (GetProcedure() == "FIGHT") && GetActionTarget();
}

public func ControlThrow()
{
  if(!InCloseCombat()) return _inherited(...);
  
  if(GetAction() != "Fight")//Nicht in Ausgangsstellung?
    return 1;
    
  if(!Random(3)) Sound("Kime*");
  punchtype = 0;
  SetAction("Punch");
  
  return 1;
}

public func ControlThrowSingle()
{
  if(InCloseCombat()) return ControlThrow(...);
  return _inherited(...);
}

public func ControlThrowDouble()
{
  if(InCloseCombat()) return ControlThrow(...);
  return _inherited(...);
}

public func ControlDig()
{
  if(!InCloseCombat()) return _inherited(...);
  
  if(GetAction() != "Fight")//Nicht in Ausgangsstellung?
    return 1;
    
  if(!Random(3)) Sound("Kime*");
  punchtype = 1;
  SetAction("Punch");

  return 1;
}

public func ControlDigSingle()
{
  if(InCloseCombat()) return ControlDig(...);
  return _inherited(...);
}

public func ControlDigDouble()
{
  if(InCloseCombat()) return ControlDig(...);
  return _inherited(...);
}

/* Waffenanzeige */
public func WeaponAt(int &x, int &y, int &r)
{
  if(Contained()) return 0;

  var a = GetAction();
  var p = GetPhase();

  // x < 0: Richtung hinter den Clonk
  // x > 0: Richtung vor den Clonk
  x=0; y=0; r=0;

  // hält Waffe in der Hand
  if(a == "WalkArmed")
  {
    x=0;
    y=1000;
    return 1;
  }
  
  else if(a == "JumpArmed")
  {
    x=500-125*p;
    y=1000;
    return 1;
  }
  else if(a == "SwimArmed")
  {
    x=4000;
    return 1;
  }
  //Jetpack
  else if(a == "JetpackFlight")
  {
  
    x=4000;
    r=35;
    return 1;
  }

  // zielt mit der Waffe
  else if(IsAiming())
  {
		if(WeaponAtCrawlAim(x,y,r)) return 1;
    r = Abs(crosshair->GetAngle()) - 90;
    x=-Sin(65+p*20,4000)-1500;
    y=-Sin(55+p*10,5000);
    if(a == "AimLow")
      y += 4000;
    else if(a == "AimSquat")
    {
      x += 5000;
      y += 4500;
    }
    else if(a == "AimSquatLow")
    {
      x += 5000;
      y += 6000;
    }
    return 1;
  }

  return _inherited(x,y,r,...);
}

public func WeaponBegin(int &x, int &y)
{
  var number = GetEffect("ShowWeapon",this);
  if(!number)
    return 0;

  x = EffectVar(2, this, number)/1000;
  y = EffectVar(3, this, number)/1000;
}

public func WeaponEnd(int &x, int &y)
{
  var number = GetEffect("ShowWeapon",this);
  if(!number)
    return 0;
    
  x = EffectVar(4, this, number)/1000;
  y = EffectVar(5, this, number)/1000;
}

public func GetWeaponR()
{
  var number = GetEffect("ShowWeapon",this);
  if(!number)
    return 0;

  return EffectVar(1, this, number);
}

public func StartSquatAiming()
{
  // Abbrechen
  if(Contained()) return 1;
  if(GetAction() != "WalkArmed" && GetAction() != "Walk")
      return 1;

  SetXDir(0);
  if(Contents(0)->~GetFMData(FM_Aim) == 2 || Contents(0)->~GetFMData(FM_Aim) == -1)
    SetAction("AimSquatLow");
  else
    SetAction("AimSquat");
  SetPhase(AimAngle2Phase(90));
  SetComDir(COMD_Stop);

  InitCrosshair();
  ScheduleCall(this,"UpdateAiming",1);

  // Callback
  if(Contents(0)) Contents(0)->~AimStart();
}

public func IsSquatAiming() { return WildcardMatch(GetAction(), "AimSquat*"); }

public func AimOverride()
{
	if(Contained()) return Contained()->~AimOverride();
	
	return _inherited(...);
}

public func ReadyToFire()
{
   var a = GetAction();

   // In Gebäude
   if(Contained()) return Contained()->~ReadyToFire();
   // Reitet
   if(IsRiding()) return GetActionTarget()->~ReadyToFire();
   // Schiebt
   if(a == "Push")
   	if(GetActionTarget()->~IsWeapon())
   		return true;

   // Nur beim Laufen, Schwimmen oder Springen
   if(a == "WalkArmed"
   || a == "SwimArmed"
   || a == "JumpArmed")
		return true;
   
   // JetpackFlight fehlt noch, sowie Aim*... das steht in den Funktionalitäten
   return _inherited(...);
}

private func CheckArmed()
{
  if (GetAction() == "Walk") if (IsArmed2()) return SetAction("WalkArmed");
  if (GetAction() == "Jump") if (IsArmed2()) return SetAction("JumpArmed");
  //if (GetAction() == "Swim") if (IsArmed2()) return SetAction("SwimArmed");
  if (GetAction() == "WalkArmed") if (!IsArmed2()) return SetAction("Walk");
  if (GetAction() == "JumpArmed") if (!IsArmed2()) return SetAction("Jump");
  if (GetAction() == "SwimArmed") if (!IsArmed2()) return SetAction("Swim");
  
  return _inherited(...);
}

// Der Clonk ist bewaffnet (auch mit einer Waffe die nicht angezeigt wird)
// also Waffen mit denen man Zielen muss
public func IsArmed()
{
  if(!Contents()) return; 
  if(Contents()->~IsWeapon())
    return true;
  return false;
}
// Der Clonk hat einen Gegenstand in der Hand, der gezeichnet wird
public func IsArmed2()
{
  if(!Contents()) return;
  if(Contents()->~IsDrawable() ||
    (IsArmed() && (Contents()->~GetFMData(FM_Aim) <= 0)))
    return true;
  return false;
}

protected func Hangling()
{
  PauseReloading();
  if(!GetEffect("ScaleReloading",this))
    AddEffect("ScaleReloading",this,1,1,this,CIVC);
}

protected func Scaling()
{
  PauseReloading();
  if(!GetEffect("ScaleReloading",this))
    AddEffect("ScaleReloading",this,1,1,this,CIVC);
  return _inherited(...);
}

func FxScaleReloadingTimer()
{
  if(!Contents())
    return;

  if(!WildcardMatch(GetAction(),"*Scale*") && !WildcardMatch(GetAction(), "*Hangle*"))
  {
    ResumeReloading();
    return -1;
  }
  if(Contents()->~IsReloading())
  	PauseReloading();
}

public func PauseReloading()
{
  var c = Contents();
  // only if the weapon is an aim-weapon
  if(c)
    if(c->~IsWeapon())
      if(c->GetFMData(FM_Aim) > 0)
        c->PauseReload();
}

public func ResumeReloading()
{
  var c = Contents();
  // only if the weapon is an aim-weapon
  if(c)
    if(c->~IsWeapon())
      if(c->GetFMData(FM_Aim) > 0)
        c->ResumeReload();
}



/* Inventarsystem */
private func Control2Contents(string command)
{
  // Haben wir was angefasst?
  if(GetAction() == "Push")
    return 0;
  // Pause Reload: nicht wieder anfangen ey!!!
  if(command == "ControlThrow")
  {
    if(WildcardMatch(GetAction(),"Scale*") || GetAction() == "Hangle")
      return 1;
  }
	
  //Callback verhindert?
  if(GetEffect("SelectItem",Contents()))
  {
    if(command == "ControlUpdate")
    {
       if(ObjectCall(Contents(), command, this, Par(1)))
         return 1;
       else
         return 0;
    }
    
    if((command == "ControlThrow") || (command == "ControlDig"))
      return 1;
    else
      return 0;
  }
  // Getragenes Objekt hat spezielle Steuerungsauswertung
  if(ObjectCall(Contents(), command, this, Par(1), Par(2), Par(3), Par(4), Par(5), Par(6), Par(7)))
    return 1;
  return 0;
}

public func ControlSpecial()
{
  [$CtrlInventoryDesc$|Image=INVT]
  
  // ControlSpecial an Container weitergeben (z.B. Fahrzeuge)
  if(Contained())
  {
    if(Contained()->~ContainedSpecial(this))
      return 1;
  }
  // Keine Items?
  if(!Contents())
    return;
  // Hardcode: BR-Bombe darf man nicht abwählen
  if(Contents()->GetID() == GBRB)
    return;
  // wenn wir zielen, wollen wir nur Waffen haben
  if(IsAiming() && (Contents(0)->~IsWeapon() || Contents(0)->~IsGrenade()))
  {
  	// nächste Waffe suchen
  	for(var i = 1; i < ContentsCount(); i++)
  		if(Contents(i)->~IsWeapon() || Contents(i)->~IsGrenade())
  		{
  			// zur Waffe wechseln
  			ShiftContents(0,0,Contents(i)->GetID(),true);
  			break;
  		}
  }
  else
	  // Inventory verschieben
  	ShiftContents(0,0,0,true);

  UpdateCharge();
}

public func FxSelectItemTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  return -1;
}

public func Collection(object pObj, bool fPut)
{
  if(pObj->~SelectionTime())
  {
    if(!GetEffect("SelectItem",pObj))
      AddEffect("SelectItem",pObj,20,pObj->~SelectionTime(),0,GetID());
  }
  return _inherited(pObj,fPut,...);
}

public func Ejection(object pObj)
{
  RemoveEffect("SelectItem",pObj);
  return _inherited(pObj,...);
}


/* Hilfesystem */
protected func ContextHelpMessagesOn(object pCaller)
{
  [$CtxHelpMessagesOn$|Image=CXIN|Condition=HelpMessagesOff]
  SetPlrExtraData(GetOwner(), "Hazard_NoHelpMsg", false);
  Sound("Click", 1, 0,0, GetOwner()+1);
}

protected func ContextHelpMessagesOff(object pCaller)
{
  [$CtxHelpMessagesOff$|Image=CXIN|Condition=HelpMessagesOn]
  SetPlrExtraData(GetOwner(), "Hazard_NoHelpMsg", true);
  Sound("Click", 1, 0,0, GetOwner()+1);
}

public func HelpMessagesOn() { return !GetPlrExtraData(GetOwner(), "Hazard_NoHelpMsg"); }
public func HelpMessagesOff() { return !HelpMessagesOn(); }


/* Mini-KI */
public func GetTarget(int iAngle, int iMaxAngle, int iRange)
{
  // Parameter zurechtstutzen
  while(iAngle<0) iAngle+=360;
  while(iAngle>360) iAngle-=360;
  if(!iMaxAngle) iMaxAngle = 0;
  if(!iRange) iRange = 350;

  // Landschaft durchsuchen
  // es wird nur auf Lebewesen gezielt
  var targets = FindTargets(this, iRange, iMaxAngle, iAngle, false);
  var target;
  
  // nächstes Ziel finden...
	for(var t in targets)
  {
		if(!target)
    {
			target = t;
			continue;
		}
		if(ObjectDistance(t) < ObjectDistance(target))
    {
			target = t;
			break;
		}
	}

  return target;
}

protected func ContextGuard(object pCaller)
{
  [$CtxGuard$|Image=FLA2|Condition=Outside]
  // Bewachen
  //SetAggroLevel(Aggro_Guard);
  Sound("Confirm*");
}


/* Steuerung */
protected func ControlCommand(szCommand, pTarget, iTx, iTy, pTarget2, Data) 
{
  if(szCommand == "Context") return 0;

  if (szCommand == "MoveTo")
  { 
    if(Control2Grab("ControlCommand")) return 0;
  
	  if(ControlAim("ControlCommand",Par(1),Par(2),Par(3))) return 1;

    if(ControlLadder("ControlCommand")) return 1;
  }
  
  return _inherited(szCommand, pTarget, iTx, iTy, pTarget2, Data); 
}

private func Control2Contents(string command)
{

  if(GetAction() == "Push")
    return 0;

  if(command == "ControlThrow")
  {
	  if(WildcardMatch(GetAction(),"Scale*") || GetAction() == "Hangle")
	    return 1;
  }
	
  // Getragenes Objekt hat spezielle Steuerungsauswertung
  if(ObjectCall(Contents(), command, this(), Par(1), Par(2), Par(3), Par(4), Par(5), Par(6), Par(7)))
    return 1;
  return 0;
}

protected func Control2Grab(string command)
{
  if(GetProcedure() == "PUSH")
  {
    if(GetActionTarget())
      GetActionTarget()->~Control2Grab(command,this,...);
      
    return true;
  }
  
  return false;
}

protected func ControlThrow()
{
  if(this->~Control2Grab("ControlThrow")) return 1;
  return _inherited(...);
}

protected func ControlDigDouble()
{
  if(this->~Control2Grab("ControlDigDouble")) return 1;
  return _inherited(...);
}

protected func ControlDownDouble()
{
  if(this->~Control2Grab("ControlDownDouble")) return 1;
  return _inherited(...);
}

public func ControlContents(id idTarget)
{
  if(Contents())
  {
    RemoveEffect("SelectItem",Contents());
  }

  var target = FindContents(idTarget);
  if(target)
  {
    if(target->~SelectionTime())
    {
      if(!GetEffect("SelectItem",target))
        AddEffect("SelectItem",target,20,target->~SelectionTime(),0,GetID());
      return 0;
    }
  }

  // Zielobjekt will gar nicht, dass wir was machen
  if(idTarget->~NoControlContents()) return;
  // Hardcode: BR-Bombe darf man nicht abwählen
  if(Contents())
    if(Contents()->GetID() == GBRB)
      return 1;

  // controlel Aim -> aufhören zu zielen außer die nächste Waffe kann es
  // Siehe ChangeWeapon
  if(ControlAim("ControlContents",idTarget)) return 1;

  // Hast du noch einen letzten Wunsch, Contents(0)??!
  if(Contents(0)) Contents(0)->~Deselection(this);
  // Rotieren
  if (!ShiftContents(0, 0, idTarget)) return 1;
  // Waffe ziehen/wegstecken
  CheckArmed();
  // Munitionsanzeige updaten
  UpdateCharge();
  // Objekt benachrichtigen
  if(Contents(0)) Contents(0)->~Selection(this);
}

private func ChangeWeapon(object pTarget)
{
  // Zielaktion anpassen
  if(pTarget->~IsWeapon() || pTarget->~IsGrenade())
  {
    var phase = GetPhase();
    if(pTarget->~GetFMData(FM_Aim) == 2 || pTarget->~GetFMData(FM_Aim) == -1)
      SetAction("AimSquatLow");
    else
      SetAction("AimSquat");
      
    SetPhase(phase);
  }
  else
  {
    StopAiming();
  }
}

protected func ContentsDestruction()
{
  ScheduleCall(this(), "CheckArmed", 1);
  UpdateCharge();
  if(Contents(1)) Schedule("Contents(0)->~Selection(this());", 1);
}

protected func Departure(pContainer)
{
  CheckArmed();
  return _inherited(pContainer,...);
}


protected func RejectCollect(id idObj, object pObj)
{
  // Für die KI
  var effect;
  if(effect = GetEffect("CollectionException", pObj))
    if(EffectVar(0, pObj, effect) == this)
      return 1;
  // Spawnpunkt-Hack
  if(idObj == SPNP) return;
  // Munitionspaket?
  if(pObj ->~ IsAmmoPacket())
    // Davon kann man in jeden Fall _eines_ im Inventar haben
    if(!CustomContentsCount("IsAmmoPacket"))
      return 0;
  //Waffe?
  if(pObj->~IsWeapon())
  {
    //Sonderbehandlung?
    if(!(pObj ->~ OnCollection(this)))
    {
      //nein. Standardprozedur:
      //schon so eine Waffe im Inventar? Oder bereits 3 andere Waffen?
      if(ContentsCount(idObj) || CustomContentsCount("IsWeapon") >= 3)
        return 1;  //Ja, nicht aufnehmen
      else
        return 0;
    }
    else
      return 0;
  }
  // Einsammellimit für Ausrüstung
  if(pObj ->~ IsEquipment() && ContentsCount(idObj) > 0)
    return 1;
  //Wieviel haben wir denn schon im inventar?
  if(ContentsCount() - CustomContentsCount("IsWeapon") >= 2)
    return 1;
  
  // nicht angelegte Ausrüstung nochmal aufsammeln
  for(var gear in aGear)
    if(gear)
      if(GetID(gear) == idObj)
        return 1;
  
  // Ok
  return 0;
}

public func CustomContentsCount(string funcname)
{
  // Alle Inventarobjekte durchlaufen und Waffen zählen
  for(var i=0,j,obj ; obj = Contents(i) ; i++)
    if(ObjectCall(obj, funcname))
      j++;
  // Waffen gezählt
  return j;
}

protected func ControlSpecial()
{
  [$CtrlInventoryDesc$|Image=INVT]
  if(!Contents()) return;
  // Hardcode: BR-Bombe darf man nicht abwählen
  if(Contents()->GetID() == GBRB)
    return;
  // wenn wir zielen, wollen wir nur Waffen haben
  if(IsAiming() && Contents(0)->~IsWeapon())
  {
  	// nächste Waffe suchen
  	for(var i = 1; i < ContentsCount(); i++)
  		if(Contents(i)->~IsWeapon())
  		{
  			// zur Waffe wechseln
  			ShiftContents(0,0,Contents(i)->GetID(),true);
  			break;
  		}
  }
  else
	  // Inventory verschieben
  	ShiftContents(0,0,0,1);
  UpdateCharge();
}

protected func Ejection(object pObj)
{
  // Nach Waffe suchen
  CheckArmed();
  // Munitionsanzeige updaten
  UpdateCharge();
  // Objekt benachrichtigen
  if(Contents(0)) Contents(0)->~Selection();
}

protected func Collection2(object pObj)
{
  // das neue Item nach hinten verschieben (außer es ist Ammo)
  if(!(pObj->~IsAmmoPacket()) || NoAmmo())
    if (1 == ContentsCount(GetID(pObj)))
      if(!(pObj->GetOCF() & OCF_Living))
      {
        ShiftContents(0,0,0,0);
        pObj->~OnDeselect();
      }
  UpdateCharge();
}

public func AmmoTransfered(object pAP)
{
  AddEffect("AmmoCollection",this,100,10,this,0,pAP->AmmoID(),pAP->AmmoCount());
}




protected func ControlUpdate(object clonk, int comdir, bool dig, bool throw)
{
  if(Control2Grab("ControlUpdate", comdir, dig, throw)) return 0;
  if(Control2Contents("ControlUpdate", comdir, dig, throw)) return 1;
  if(ControlAim("ControlUpdate", comdir, dig, throw)) return 1;
  if(ControlLadder("ControlUpdate", comdir, dig, throw)) return 1;

  return _inherited(clonk, comdir, dig, throw);
}

protected func ControlUp()
{
  if (ControlAim("ControlUp")) return 1;
  if (Control2Grab("ControlUp")) return 0;
  if (Control2Contents("ControlUp") ) return 1;
  if (ControlLadder("ControlUp") ) return 1;
  if (ControlAgility("ControlUp") ) return 1;
  return _inherited(...);
}

protected func ControlUpDouble()
{
  if (ControlAim("ControlUp")) return 1;
  if (Control2Grab("ControlUpDouble")) return 0;
  if (Control2Contents("ControlUpDouble") ) return 1;
  if (ControlJetpack("ControlUpDouble") ) return 1;
  if (ControlAgility("ControlUpDouble") ) return 1;
  return _inherited(...);
}

protected func ControlDown()
{
  if (ControlAim("ControlDown")) return 1;
  if (Control2Grab("ControlDown")) return 0;
  if (Control2Contents("ControlDown") ) return 1;
  if (ControlLadder("ControlDown") ) return 1;
  if(Contents(0))
    if (GetPlrDownDouble(GetOwner()))
		if(ReadyToSquatAim())
    {
       StartSquatAiming(); 
       return 1;
    }
  return _inherited(...);
}

private func ReadyToSquatAim()
{
	return !GetEffect("SquatAimTimeout") 
		&&	Contents(0)->~CanAim()
		&& !IsAiming()
		&& (GetAction() == "WalkArmed" || GetAction() == "Walk");
}

protected func ControlLeft()
{
  if (ControlAim("ControlLeft")) return 1;
  if (Control2Grab("ControlLeft")) return 0;
  if (Control2Contents("ControlLeft")) return 1;
  if (ControlLadder("ControlLeft") ) return 1;
  if (ControlJetpack("ControlLeft") ) return 1;
  if (ControlAgility("ControlLeft") ) return 1;

  return _inherited(...);
}

protected func ControlRight()
{
  if (ControlAim("ControlRight")) return 1;
  if (Control2Grab("ControlRight")) return 0;
  if (Control2Contents("ControlRight")) return 1;
  if (ControlLadder("ControlRight") ) return 1;
  if (ControlJetpack("ControlRight") ) return 1;
  if (ControlAgility("ControlRight") ) return 1;

  return _inherited(...);
}

protected func ControlLeftDouble()
{
  if(IsHealing())
  {
	  StopHealing();
	  return 1;
  }
  
  if (ControlAim("ControlLeftDouble")) return 1;
  if (Control2Grab("ControlLeftDouble")) return 0;
  if (Control2Contents("ControlLeftDouble")) return 1;
  if (ControlJetpack("ControlLeftDouble") ) return 1;
  if (ControlAgility("ControlLeftDouble") ) return 1;
  return _inherited(...);
}

protected func ControlRightDouble()
{
  if(IsHealing())
  {
	  StopHealing();
	  return 1;
  }
  if (ControlAim("ControlRightDouble")) return 1;
  if (Control2Grab("ControlRightDouble")) return 0;
  if (Control2Contents("ControlRightDouble")) return 1;
  if (ControlJetpack("ControlRightDouble") ) return 1;
  if (ControlAgility("ControlRightDouble") ) return 1;
  return _inherited(...);
}

protected func ControlDigDouble()
{
  if (GetAction() == "Push" || IsRiding())
  {
    GetActionTarget()->~ControlDigDouble(this);
    return 1;
  }
  if (Control2Grab("ControlDigDouble")) return 0;
  if (Control2Contents("ControlDigDouble")) return 1;
  return _inherited(...);
}

protected func ControlDigSingle()
{
  if (Control2Grab("ControlDig")) return 0;
  if (Control2Contents("ControlDig")) return 1;
  if (ControlLadder("ControlDig") ) return 1;
  return _inherited(...);
}

protected func ContactBottom()
{
  if(GetAction() == "Swim")
    SetAction("Walk");
  if(GetAction() == "SwimArmed")
    SetAction("WalkArmed");
  return 1;
}

/* Steuerung Werfen */

protected func ControlThrow()
{
  // Bei vorherigem Doppel-Stop nur Ablegen  
  if( GetPlrDownDouble(GetOwner()) )
  {
    AddEffect("SquatAimTimeout", this, 1, 15, this);
    return inherited(...);
  }
  // Steuerung an gerittenes Objekt weitergeben
  if(IsRiding())
    if(GetActionTarget()->~ControlThrow(this))
      return 1; 

	if(CrawlTryAim()) return 1;
  if(Control2Grab("ControlThrow")) return 0;
  if(Control2Contents("ControlThrow")) return 1;
  if(ControlLadder("ControlThrow")) return 1;

  return inherited(...);
}

public func MacroComTumble()
{
  //... *dummy*
}

/* *** 'Kampfclonk' Append *** */
// Sollte mal in das bestehende Script richtig integriert werden. :S

func ResetShowWeapon(object pNew)
{
  var effect = GetEffect("ShowWeapon",this); 
  if(!effect) return false;
  
  if(pNew)
  {
    EffectVar(0, this(), effect) = GetID(pNew);
    EffectVar(1, this(), effect) = pNew;
  }
  else
  {
    EffectVar(0, this(), effect) = 0;
    EffectVar(1, this(), effect) = 0;
  }
  
  return true;
}

/* neues Zielsystem */

/*func HandR()
{
  return 0);//return handr/CH_Spread_Prec);
}*/

private func GetSpreadAOff()
{
  handr = RandomX(-spread/2/CH_Spread_Prec,+spread/2/CH_Spread_Prec);
  return handr;
}

private func GetSpreadAOff2()
{
  handr = RandomX(-spread/2/CH_Spread_Prec,+spread/2/CH_Spread_Prec);
  return handr*(100/CH_Spread_Prec);
}

private func TestSpread()
{
  var proc = GetProcedure();
  
  if(proc == "WALK")
  {
    if(GetComDir())
      if(spread < CH_WalkSpread)
        return CH_WalkSpread;
    return 0;
  }
  
  if(proc == "SWIM")
  {
    if(spread < CH_WalkSpread)
      return CH_WalkSpread;
    return 0;
  }
  
  if(proc == "FLIGHT")
  {
    if(spread < CH_JumpSpread)
      return CH_JumpSpread;
    return 0;
  }

  if((proc == "SCALE")||(GetAction() == "ScaleLadder"))
  {
    if(spread < CH_ScaleSpread)
      return CH_ScaleSpread;
    return 0;
  }
  
  if(proc == "HANGLE")
  {
    if(spread < CH_HangleSpread)
      return CH_HangleSpread;
    return 0;
  }
  
  return 0;
}

public func DoSpread(int iChange, int iMax)
{
  var wpn = Contents();
  if(!wpn) return;
  if(!wpn->~IsWeapon() && !wpn->~IsGrenade()) return;
  
  if(iMax) iChange = Max(0,BoundBy(spread+iChange,0,iMax)-spread);
  spread = BoundBy(spread+iChange,0,CH_MaxSpread);
  
  if(crosshair)
    crosshair->SetSpread(spread);
}

public func GetSpread()
{
  return spread;
}


/** Aktionsmanager **/

/*
public func OnStartAction()
{
	var action = GetAction();
	
	if((action == "Walk") || (action == "WalkArmed") || (action == "Swim") || (action == "SwimArmed"))
		CheckArmed();
	else if((action == "Scale") || (action == "ScaleDown"))
		Scaling();
	else if((action == "ScaleLadder") || (action == "Hangle"))
		PauseReloading();
	else if((action == "Dig") || (action == "Bridge"))
		Digging();
	else if((action == "Jump") || (action == "JumpArmed") || (action == "Dive"))
		JumpStart();
	else if((action == "Ride") || (action == "RideStill"))
		Riding();
	else if(action == "Chop")
		Chopping();
	else if(action == "Fight")
		Fighting();
	else if(action == "Build")
		Building();
	else if((action == "Throw") || (action == "RideThrow"))
		Throwing();
	else if((action == "Crawl") || (action == "CrawlFall") || (action == "CrawlArmed") || (action == "AimCrawl") || (action == "CrawlAim"))
		StartCrawl();
	
	StaminaCheckAction();
}
*/