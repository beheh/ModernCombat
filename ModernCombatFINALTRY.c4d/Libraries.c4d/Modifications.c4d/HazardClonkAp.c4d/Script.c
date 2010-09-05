/*-- Kampfclonk Appendto --*/

#strict 2
#appendto HZCK

local crosshair;
local aCollected;
local HUDTarget;

public func SetHUDTarget(pTarget)
{
  HUDTarget = pTarget;
}

protected func Control2Grab(string command)
{
  if(GetProcedure() == "PUSH" && GetActionTarget())
      return GetActionTarget()->~Control2Grab(command,this,...);
}

protected func ControlThrow()
{
  if(this->~Control2Grab("ControlThrow")) return true;
  return _inherited();
}

protected func ControlDigDouble()
{
  if(this->~Control2Grab("ControlDigDouble")) return true;
  return _inherited();
}

protected func ControlDownDouble()
{
  if(this->~Control2Grab("ControlDownDouble")) return true;
  return _inherited();
}

public func ControlUp()
{
  RemoveEffect("IntMouseAiming", this);
  return _inherited(...);
}

public func ControlDown() 
{
  RemoveEffect("IntMouseAiming", this);
  return _inherited(...);
}

func ResetShowWeapon(object pNew)
{
  var effect = GetEffect("ShowWeapon",this); 
  if(!effect) return false;
  
  if(pNew)
  {
    EffectVar(0, this, effect) = GetID(pNew);
    EffectVar(1, this, effect) = pNew;
  }
  else
  {
    EffectVar(0, this, effect) = 0;
    EffectVar(1, this, effect) = 0;
  }
  
  return true;
}

public func DoAiming(int iChange)
{
  //zielen wir überhaupt?
  if(!IsAiming())
    return;

  var angle = Abs(crosshair->GetAngle()) + iChange;

  // Winkel anpassen, wenn keine freie Auswahl (klassische Steuerung)
  if(!GetPlrCoreJumpAndRunControl(GetController()))
    angle = angle-angle%AIM_Step;
    
  /*if(ControlledCanAim())
  {
    angle = GetControlled()->SetAimAngle(angle);
  }
  else
  {*/
    // Winkel wird zu groß?
    if(angle > AIM_Max || angle < 0)
      return;

    if(GetDir() == DIR_Left)
      angle = 360-angle;
  //}

  crosshair->SetAngle(angle);
  this->~UpdateAiming();
}

/* HazardGear */
//Nicht jede Clonkart kann alles tragen. (z.B. Sanitäter)

public func EquipGear(object pGear)
{
  // überhaupt benutzbar
  if(!pGear || !pGear->~IsHazardGear() || !HazardGearSupported(pGear)) return false;
  return _inherited(pGear);
}

public func HazardGearSupported(object pGear)//! -> Hazard 2.0
{
  return true; //Standartmäßig alles schlucken.
}

private func AmmoBagContextCheck()
{
  return !FindObject(NOAM);
}

/* Munition aus dem Gürtel nehmen */
protected func ContextAmmobag(object pCaller)
{
  [$AmmoBag$|Image=ABOX|Condition=AmmoBagContextCheck]
  var ammo, x;
  //Menü erzeugen
  CreateMenu(ABOX,this,0,0,GetName(0,ABAG),0,1,1);
  
  var ambg = ammobag;
  
  //Munition finden und hinzufügen
  while(ammo = Contents(x,ambg))
  {
    // nur Ammo
    if(!ammo ->~IsAmmo()) continue;
    //hinzufügen
    AddMenuItem(GetName(ammo),"PackAmmo", GetID(ammo), this, GetAmmo(GetID(ammo)),ObjectNumber(pCaller),GetDesc(ammo));
    //Zählervariable erhöhen
    x++;
  }
  return true;
}

protected func PackAmmo(id idType, int iCaller)
{
  //Wir übersehen mal, das beliebig viel Muni verpackt werden könnte.
  var pCaller = Object(iCaller);

  //Selbstgepackte Munition sollte immer CUAM sein.

  var pPack = CreateObject(CUAM,0,0,GetOwner());

  pPack->SetAmmoID(idType);
  var iChange = pPack->DoAmmoCount(GetAmmo(idType));
  DoAmmo(idType,-iChange);
  
  if(!Collect(pPack,this))
  {
    DoAmmo(idType,+iChange); 
    RemoveObject(pPack); //What a fail. :C
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

public func ReadyToFire() {
  if (GetProcedure() == "SWIM")
    return;
  if(GetAction() == "Crawl" && Contents() && Contents()->~CanAim() && Contents()->~IsEquipment())
      return true;
  return _inherited(...);
}

//Wegen SwimArmed
public func ReadyToAttack() {
	if(ReadyToFire()) return true;
	if(GetProcedure() == "SWIM") return true;
	return false;
}

public func ReadyToGrenade() {
	if(!ReadyToAttack()) return;
	if(Contents())
		if(Contents()->~IsGrenade()) return true;
	return false;
}

/* Schaden */
public func OnHit(int iDmg, int iType, object pFrom)
{
  this->~CalcPain(iDmg);
  //this->~HurtSounds(-iDmg,iType);
    
  return _inherited(iDmg,iType,pFrom);
}

/* neues Zielsystem */
static const CH_ShowAlways = 1;

static const CH_MaxSpread = 500;

static const CH_WalkSpread = 80;
static const CH_JumpSpread = 140;
static const CH_ScaleSpread = 200;
static const CH_HangleSpread = 200;

static const CH_CrawlSpreadReduction = 4;
static const CH_AimSpreadReduction = 4;
static const CH_StandSpreadReduction = 3;

local crosshair,ammobag;
local spread, handr;

public func Initialize()
{
	aCollected = CreateArray();
  InitCH();
  //if(Contained())
    //HideCH();
  return _inherited(...);
}

/*func HandR()
{
  return false;//return handr/CH_Spread_Prec;
}*/

public func GetCrosshair()
{
  return crosshair;
}

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
    if(GetComDir() && spread < CH_WalkSpread)
      return CH_WalkSpread;
    return false;
  }
  
  if(proc == "SWIM")
  {
    if(spread < CH_WalkSpread)
      return CH_WalkSpread;
    return false;
  }
  
  if(proc == "FLIGHT")
  {
    if(spread < CH_JumpSpread)
      return CH_JumpSpread;
    return false;
  }

  if((proc == "SCALE")||(GetAction() == "ScaleLadder"))
  {
    if(spread < CH_ScaleSpread)
      return CH_ScaleSpread;
    return false;
  }
  
  if(proc == "HANGLE")
  {
    if(spread < CH_HangleSpread)
      return CH_HangleSpread;
    return false;
  }
  
  return false;
}

public func UpdateCH() {
  if((!this->~ReadyToFire() && !this->~ReadyToGrenade()) || !this->~IsArmed() || GetCursor(GetOwner()) != this)
    {
      HideCH();
      return;
    }
  
  var c = Contents();

  ShowCH();
  
  DoSpread(BoundBy(TestSpread()-spread,0,10));
  
  var unspread;
  if(c->~IsWeapon())
    unspread = c->GetFMData(FM_UnSpread);
  else
    unspread = c->~UnSpread();

    
  if(this->~IsAiming())
    if(this->~IsCrawling())
      DoSpread(-(CH_CrawlSpreadReduction+unspread));
    else
      DoSpread(-(CH_AimSpreadReduction+unspread));
  else
    DoSpread(-(CH_StandSpreadReduction+unspread));

  this->~UpdateAiming();
  return true;
}

local pIcon;

protected func DeathAnnounce(int plr, object clonk, int killplr)
{
  if(!clonk) clonk = this;
  if(!clonk) return;
  var r = CLNK->DeathAnnounce(plr, clonk, killplr, true, this->~GetAssist()+1);
  if(r) this->DoPoints();
  return r;
}

public func UpdateCharge()
{
  UpdateCH();
  
  // Nur wenn ich Cursor bin
  var pCursor = GetCursor(GetOwner());
  if(pCursor && pCursor != this) pCursor = pCursor->~GetRealCursor(); 
  if(pCursor != this) return;

  if(GetOwner() < -1) return;

  // in Gebäuden/Fahrzeugen
  if(Contained() && Contained()->~UpdateCharge(this))
    return true;

  // reitet
  if(this->IsRiding() && GetActionTarget()->~UpdateCharge(this))
    return true;

  // ggf. an angefasstes Objekt weiterleiten
  var Content = Contents();
  if(GetAction() == "Push" && GetActionTarget()->~IsWeapon())
  	Content = GetActionTarget();
  // ggf. an Gebäude/Fahrzeug weiterleten
  if(Contained() && Contained()->~IsWeapon())
  	Content = Contained();
  // ggf. speziell zugewiesenes Objekt
  if(HUDTarget)
    Content = HUDTarget;

  // HUD
  var hud = GetHUD();
  if(hud) hud->Update(Content, this->AmmoStoring(),this);

  return true;
}

global func AimAngleEx(int iMaxAngle, int iRange, bool bSpread)
{
  if(!this) return false;

  var angle = this->~AimAngle2(iMaxAngle,iRange,bSpread);
  if(!angle)
    angle = this->AimAngle(iMaxAngle,iRange,bSpread)*100;
    
  return angle;
}

public func AimAngle2(int iMaxAngle, int iRange, bool bSpread)//Präzision 100. Also von 100 bis 36000.
{
   var angle;
   var x,y,r;
   this->~WeaponAt(x,y,r);
   
   if(!this->~IsAiming())
     angle = (90+r)*(GetDir()*2-1);
   else
     angle = crosshair->GetAngle();
  
   if(iRange)
   {
     var target = this->~GetTarget(angle,iMaxAngle,iRange);
     if(target)
       angle = Angle(GetX(),GetY(),GetX(target),GetY(target));
   }
   
   if(bSpread)
     angle += GetSpreadAOff2();

   return angle;
}

public func AimAngle(int iMaxAngle, int iRange, bool bSpread)
{
	var angle;
	var x,y,r;
	this->~WeaponAt(x,y,r);

	if(!IsAiming()) {
		angle = (90+r)*(GetDir()*2-1);
		if(Contents())
			if(Contents()->~IsGrenade())
				angle = (60+r)*(GetDir()*2-1);
	}
	else
	  angle = crosshair->GetAngle();

	if(iRange)
	{
	  var target = this->~GetTarget(angle,iMaxAngle,iRange);
	  if(target)
		angle = Angle(GetX(),GetY(),GetX(target),GetY(target));
	}
	 
	if(bSpread)
	  angle += GetSpreadAOff();

	return angle;
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

private func InitCrosshair()
{
  InitCH();
  Sound("WPN2_Deploy*.ogg");
}

public func InitCH()
{
  if(!crosshair)
    crosshair = CreateObject(CRH2,0,0,GetOwner());
  crosshair->Set(this);
  crosshair->SetAngle(90);
  
  ResetCH();
}

public func ShowCH()
{
  if(!crosshair)
    InitCH();
  if(!crosshair)
    return;
	
  if(Contained(crosshair))
    Exit(crosshair);
    
  if(GetActionTarget(0, crosshair) != this)
    crosshair->Reset(this);
   
  if(!this->~IsAiming())
  {
    //if((crosshair->GetAngle() != -90) || (crosshair->GetAngle() != +90))
      //ResetCH();

    var dummy1, dummy2, r;
    this->WeaponAt(dummy1, dummy2, r);
      
    if(!GetDir())
      crosshair->SetAngle(-90-r);
    else
      crosshair->SetAngle(+90+r);
  }
}
public func ResetCH()
{
  if(!crosshair) return;
  crosshair->Reset(this);

  crosshair->SetSpread(spread);
  
  /*if(GetDir() == DIR_Left)
    crosshair->SetAngle(-90);
  else
    crosshair->SetAngle(+90);*/
  
  ScheduleCall(this,"UpdateAiming",1);
}

public func HideCH()
{
  if(!crosshair) return;
  
  handr = 0;
  crosshair->UnSet();
}

public func IsAiming()
{
  if(Contained() && Contained()->~CanAim())
    return true;
  
  if(GetProcedure() == "PUSH" && GetActionTarget()->~CanAim())
    return true;

  return _inherited();
}

public func Entrance(object pContainer)
{
  HideCH();
  return _inherited(pContainer);
}

protected func Departure()      // Gebäude verlassen
{
  UpdateCharge();
  return _inherited(...);
}

public func Destruction()
{
  if(crosshair)
    RemoveObject(crosshair);
  return _inherited();
}

public func StartAiming() //Wegen fehlendem Hazard-Feature.
{
  if(Contained()) return Contained()->~StartAiming();

  SetXDir();

  if(Contents()->~GetFMData(FM_Aim) == 2 || Contents()->~GetFMData(FM_Aim) == 0)
    SetAction("AimLow");
  else
    SetAction("Aim");

  SetPhase(this->~AimAngle2Phase(90));
  SetComDir(COMD_Stop);

  InitCrosshair();
  
  ScheduleCall(this,"UpdateAiming",1);

  // Callback
  if(Contents()) Contents()->~AimStart();
}

public func StartSquatAiming() { // Anfangen in der Hocke zu zielen

  if(Contained()) return Contained()->~StartAiming();

  SetXDir();
  if(Contents()->~GetFMData(FM_Aim) == 2 || Contents()->~GetFMData(FM_Aim) == 0)
    SetAction("AimSquatLow");
  else
    SetAction("AimSquat");
    
  SetPhase(this->~AimAngle2Phase(90));
  SetComDir(COMD_Stop);

  InitCrosshair();
  
  ScheduleCall(this,"UpdateAiming",1);

  // Callback
  if(Contents()) Contents()->~AimStart();
}

public func StopAiming()
{
  if(crosshair)
  {
    ResetCH();
    if(!GetDir())
      crosshair->SetAngle(-90);
    else
      crosshair->SetAngle(+90);
  }
  return _inherited();
}

public func SetAiming(int iAngle, bool fForceExact)
{
  //zielen wir überhaupt?
  if(!this->~IsAiming())
    return;

  // Winkel anpassen, wenn keine freie Auswahl (klassische Steuerung)
  if(!GetPlrCoreJumpAndRunControl(GetController()) && !fForceExact)
    iAngle = iAngle-iAngle%AIM_Step;

  // Winkel wird zu groß?
  if(iAngle > AIM_Max || iAngle < 0)
    return;

  if(GetDir() == DIR_Left)
    iAngle = 360-iAngle;
  crosshair->SetAngle(iAngle);

  this->~UpdateAiming();
}

/* Nahkampfsystem */
private func Fighting()
{
  if(GetCursor(GetOwner()) != this || GetPlayerType(GetController()) == C4PT_Script)//Rudimentäre KI Selbstverteidigung.
  {
    if(!Random(2))
     ControlThrow();
    else if(!Random(2))
      ControlDig();
  }
  return true;
}

local punchtype; //<-lol

private func Punching()
{
  if (!Random(3)) Sound("Kime*");
  Sound("Punch*");
  
  if(!punchtype)
    Punch(GetActionTarget());
  else if(punchtype == 1)
  {
    if(GetAction(GetActionTarget()) != "Punch")
    {
      Fling(GetActionTarget(), 4 * GetDir() - 2, -1);
      Punch(GetActionTarget(),10);
    }
    else
      Fling(this, 0, -1);//Schwupps. Selber hingefallen.
  }

  SetComDir(COMD_Stop);

  return true;
}

public func InCloseCombat()
{
  return GetProcedure() == "FIGHT" && GetActionTarget();
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
    return true;
    
  if (!Random(3)) Sound("Kime*");
  punchtype = 1;
  SetAction("Punch");

  return true;
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

private func Control2Contents(string command)
{
  // Haben wir was angefasst?
  if(GetAction() == "Push")
    return false;
  // Pause Reload: nicht wieder anfangen ey!!!
/*  if(command == "ControlThrow")
  {
    if(WildcardMatch(GetAction(),"Scale*") || GetAction() == "Hangle")
      return true;
  }*/ //Auskommentiert wegen C4
	
  //Callback verhindert?
  if(GetEffect("SelectItem",Contents()))
  {
    if(command == "ControlUpdate")
      return Contents()->~ControlUpdate(this, Par(1));
    
    return command == "ControlThrow" || command == "ControlDig";
  }
  // Getragenes Objekt hat spezielle Steuerungsauswertung
  return ObjectCall(Contents(), command, this, Par(1), Par(2), Par(3), Par(4), Par(5), Par(6), Par(7));
}

protected func NoContext() {}

protected func ContextHelpMessagesOn() {
	[0|Image=ROCK|Condition=NoContext]
	 _inherited(...);
	ContextSettings(Par());
	SelectMenuItem(1, Par());
}

protected func ContextHelpMessagesOff() {
	[0|Image=ROCK|Condition=NoContext]
	_inherited(...);
	ContextSettings(Par());
	SelectMenuItem(1, Par());
}

protected func ContextSettings(object pCaller) {
	[$Settings$|Image=CSTR]
	CreateMenu(CSTR, pCaller, pCaller, 0, "$Settings$", 0, C4MN_Style_Context, false);
	if(pCaller->QuickInventoryOff())
	  AddMenuItem("$CtxQuickInventoryOff$", Format("SetQuickInventoryOn(Object(%d))", ObjectNumber(pCaller)), ICN2, pCaller);
	else
	  AddMenuItem("$CtxQuickInventoryOn$", Format("SetQuickInventoryOff(Object(%d))", ObjectNumber(pCaller)), QKIN, pCaller);
	if(pCaller->HelpMessagesOff())
	  AddMenuItem("$CtxHelpMessagesOff$", Format("ContextHelpMessagesOn(Object(%d))", ObjectNumber(pCaller)), ICN2, pCaller);
	else
	  AddMenuItem("$CtxHelpMessagesOn$", Format("ContextHelpMessagesOff(Object(%d))", ObjectNumber(pCaller)), CXIN, pCaller);
	return true;
}

public func SetQuickInventoryOn(object pCaller) {
  SetPlrExtraData(GetOwner(), "CMC_QuickInv", true);
  Sound("Click", 1, 0,0, GetOwner()+1);
  ContextSettings(Par());
}

public func SetQuickInventoryOff(object pCaller) {
  SetPlrExtraData(GetOwner(), "CMC_QuickInv", false);
  Sound("Click", 1, 0,0, GetOwner()+1);
  ContextSettings(Par());
}

public func QuickInventoryOn() { return GetPlrExtraData(GetOwner(), "CMC_QuickInv"); }
public func QuickInventoryOff() { return !QuickInventoryOn(); }

public func SelectQuickInventory(int iIndex) {
	if(!Contents()) return false;
	if(!iIndex) return;
	iIndex--;
	var aiming = IsAiming() && Contents()->~CanAim();
	var angle = Abs(AimAngle());
	if(aiming) StopAiming();
	while(iIndex-- > 0) {
		ShiftContents();
	}
	ShiftContents(0, 0, 0, true);
	if(Contents(0)->~CanAim() && aiming) {
		if(this->IsSquatAiming() || Contents()->~GetFMData(FM_Aim) != 1) {
	 		StartSquatAiming();
		}
		else {
			StartAiming();
		}
		SetAiming(angle, true);
  }
	UpdateCharge();
	
	return true;
}

public func GetContentsOffset(object pTo) {
	var i = 0;
	var j = 0;
	while(Contents(i) != pTo && j < ContentsCount()) {
		if(i > ContentsCount()-1) i = 0;
		i++;
		j++;
	}
	return j;
}

public func ControlSpecial()
{
  [$CtrlInventoryDesc$|Image=INVT]
  
	// ControlSpecial an Container weitergeben (z.B. Fahrzeuge)
	if(Contained() && Contained()->~ContainedSpecial(this))
	  return true;
	if(QuickInventoryOn()) {
		if(!Contents() || !aCollected) return;
		
		if(Contents()->~RejectShift() || GetID(Contents()) == GBRB)
		  return;

  	var ring = CreateSpeedMenu(0, this);
  	var i = 0;
  	var j = 0;
  	while(i <= 4) {
  		j = i+1;
  		if(j == 5) j = 0;
			if(aCollected[i]) {
				if(aCollected[i]->Contained() == this) {
  				var overlay = ring->Add(j, GetName(aCollected[i]),"SelectQuickInventory",GetContentsOffset(aCollected[i]),RICO);
  				SetGraphics("",ring,GetID(aCollected[i]),overlay,GFXOV_MODE_IngamePicture);
  			}
  		}
  		i++;
  	}
  	ring->AddTopInfoItem(Format("<c ffff00>$QuickInventory$</c>|$ContentsCount$", ContentsCount()));
  }
  else {
		// Keine Items?
		if(!Contents()) return;
		// Hardcode: BR-Bombe darf man nicht abwählen
		if(GetID(Contents()) == GBRB)
		  return;
		// Manche Sachen dürfen einfach nicht
		if(Contents()->~RejectShift())
			return;
		// wenn wir zielen, wollen wir nur Waffen haben
		if(IsAiming() && Contents()->~CanAim())
		{
		  var angle = Abs(AimAngle());
			// nächste Waffe suchen
			for(var i = 1; i < ContentsCount(); i++)
				if(Contents(i)->~CanAim())
				{
					// zur Waffe wechseln
					ShiftContents(0,0,Contents(i)->GetID(),true);
					break;
				}
		  if(this->IsSquatAiming() || Contents()->~GetFMData(FM_Aim) != 1) {
		 		StartSquatAiming();
		  }
		  else {
		  	StartAiming();
		  }
		  SetAiming(angle, true);
		}
		else
			// Inventory verschieben
			ShiftContents(0,0,0,true);
		UpdateCharge();
  }
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

public func Collection2(object pObj)
{
	if(!pObj || Contained(pObj) != this) return;
	var i = 0;
	while(aCollected[i]) {
		if(aCollected[i] == pObj) return;
		i++;
	}
	i = 0;
	while(aCollected[i]) {
		if(Contained(aCollected[i]) == this) {
			i++;
		}
		else {
			break;
		}
	}
	aCollected[i] = pObj;
}

public func Ejection(object pObj)
{
  RemoveEffect("SelectItem",pObj);
  return _inherited(pObj,...);
}

public func ControlContents(id idTarget)
{
  if(Contents())
    RemoveEffect("SelectItem",Contents());

  var target = FindContents(idTarget);
  if(target)
  {
    if(target->~SelectionTime())
    {
      if(!GetEffect("SelectItem",target))
        AddEffect("SelectItem",target,20,target->~SelectionTime(),0,GetID());
      ScheduleCall(this, "CheckArmed", 1);
      return false;
    }
  }
  return _inherited(idTarget);
}

protected func ContentsDestruction()      // Wenn Inhaltsobjekte verschwinden
{
  // nach Waffe suchen
  ScheduleCall(this, "CheckArmed", 1);
  UpdateCharge();
  // nächstes Objekt benachrichtigen
  Schedule("CheckContentsDestruction();", 1);
}

protected func CheckContentsDestruction() {
  if(Contents()) Contents()->~Selection(this);
}

protected func ScalingLadder() {
	return this->Hangling(); //Funktional identisch
}

/* Gibt jetzt auch FAPHeal und so*/

public func IsHealing () { return WildcardMatch(GetAction(), "*Heal*"); }

/* KI-Erweiterung */
//Reanimiert jetzt sogar Verbündete, foll toll!11

/*

public func GetReanimationTarget(pFrom, &body, &defi, fUnderAttack) {
	var distance = 200;
	if(fUnderAttack) distance = 50;
	body = FindObject2(Find_Func("IsFakeDeath"), Find_Category(C4D_Living), Find_Distance(distance, AbsX(GetX(pFrom)), AbsY(GetY(pFrom))), Find_Allied(GetOwner(pFrom)), Sort_Distance(AbsX(GetX(pFrom)), AbsY(GetY(pFrom))));
	if(body) {
		if(ContentsCount(CDBT, pFrom)) {
			defi = FindObject2(Find_ID(CDBT), Find_Container(pFrom));
			if(!defi->~Ready()) {
				pFrom->~DropObject(defi);
				defi = false;
			}
		}
		if(!defi) defi = FindObject2(Find_ID(CDBT), Find_Or(Find_Container(Contained(body)), Find_Container(pFrom), Find_NoContainer()), Find_Func("Ready"), Find_Distance(distance, AbsX(GetX(pFrom)), AbsY(GetY(pFrom))), Sort_Distance(AbsX(GetX(pFrom)), AbsY(GetY(pFrom))));
		if(defi) {
			return true;
		}
	}
	body = false;
	return false;
}

public func FxAggroTimer(object pTarget, int no)
{
  if(Contained()) return;
	// Erst mal schauen ob wir dringend helfen können
	var body, defi;
	GetReanimationTarget(pTarget, body, defi, EffectVar(1, this(), no)); //Checkt auch nach Defi
	if(body) {
		if(IsAiming()) StopAiming();
		if(Contents() == defi && GetProcedure(pTarget) && ObjectDistance(body, pTarget) < 10) {
			ScheduleCall(defi, "Activate", 1, 0, pTarget);
		}
		else {
			if(!Contained(defi) || Contained(defi) != pTarget) {
				if(GetCommand(pTarget) != "Get") SetCommand(pTarget, "Get", defi);
			}
			else if(Contents() != defi) { //Nicht ausgewählt?
				ShiftContents(pTarget, 0, CDBT);
			}
			else {
				SetMacroCommand(pTarget, "MoveTo", body, 0,0,0, EffectVar(0, pTarget, no));
			}
			return(1);
		}
	}
	// Verletzt?
	if(!pTarget->~IsHealing() && pTarget->GetEnergy() < pTarget->GetPhysical("Energy") * 2/3 / 1000) {
		// Medic, FAP dabei und kein Dragnin? Entpacken!
		if(!ContentsCount(DGNN, pTarget) && ContentsCount(FAPK, pTarget)) {
			var pFAP = FindObject2(Find_ID(FAPK), Find_Container(pTarget), Find_Func("CanUnpack", pTarget));
			if(pFAP)
				pFAP->ControlThrow(pTarget);
		}
		// Und schnell mal nach Dragnin suchen
		var pDragnin = FindObject2(Find_ID(DGNN), Find_Container(pTarget));
			if(pDragnin) pDragnin->Activate(pTarget);
	}
	// Okay - und sonst noch wer in meiner Nähe stark verletzt?
	//var friends;
	for(var friend in FindObjects(Find_Category(OCF_Living), Find_NoContainer(), Find_Distance(50, AbsX(GetX(pTarget)), AbsY(GetY(pTarget))), Sort_Distance())) {
		if(!friend->~IsHealing() && friend->GetEnergy() < friend->GetPhysical("Energy") * 1/3 / 1000) {
			// Medic, FAP dabei und kein Dragnin? Entpacken!
			if(!ContentsCount(DGNN, pTarget) && ContentsCount(FAPK, pTarget)) {
				var pFAP = FindObject2(Find_ID(FAPK), Find_Container(pTarget), Find_Func("CanUnpack", pTarget));
				if(pFAP)
					pFAP->ControlThrow(pTarget);
			}
			// Und schnell mal nach Dragnin suchen
			var pDragnin = FindObject2(Find_ID(DGNN), Find_Container(pTarget));
			if(pDragnin) {
				if(ObjectDistance(friend, pTarget) < 10) {
					pDragnin->ControlThrow(pTarget);
				}
				else {
					SetMacroCommand(pTarget, "MoveTo", friend, 0,0,0, EffectVar(0, pTarget, no));
				}
			}
			break;
		}
	}
  // Wir haben ein Ziel?
  if(EffectVar(1, this, no) && (pTarget->~GetSpread() < 80 || ObjectDistance(pTarget, EffectVar(1, this, no))) < 30) { EffectCall(this(), no, "Fire"); return 1; }
  // Zielen beenden
  if(IsAiming()) StopAiming();
//  Message("@No target", this());
  // Ziel suchen
  var dir = GetDir()*2-1;
  // Vorne
  var target = GetTarget(90*dir, 90);
  // Hinten
  if(!target)
    if((!GetCommand() && !GetMacroCommand()) || EffectVar(0, this(), no) != 1)
      target = GetTarget(-90*dir, 90);
  // Gefunden?
  if(!target)
  {
  	// Nichts gefunden :(
    // -> Bescheid geben!
    if(EffectVar(99, this(), no))
    {
      if(Contained())
        Contained()->~HandleAggroFinished(this());
      else if(IsRiding())
        GetActionTarget()->~HandleAggroFinished(this());
      
      EffectVar(99, this(), no);
    }
    // -> Waffen durchchecken
    CheckIdleWeapon();
    return;
  }
  // Super
  EffectVar(1, this(), no) = target;
  EffectVar(99,this(), no) = true; // wir haben ein Ziel \o/
}

// Wenn iLevel = 1 (Aggro_Shoot) werden keine Waffen mit FM_Aim ausgewählt
public func SelectWeapon(int iLevel, object pTarget, bool fFireModes)
{
  // Entfernung zum Ziel
  var dist = ObjectDistance(pTarget);
  // Keine Waffen in Inventar?
  if(!CustomContentsCount("IsWeapon")) return;
  // Bevorzugten Schadenstyp bestimmen
  var preftype = GetPrefDmgType(pTarget), type;
  // Alle durchgehen und passende prüfen
  for(var i=0,obj,fav,mode,favmode ; obj = Contents(i) ; mode++)
  {
    // Nix Waffe
    if(!(obj->~IsWeapon())) { i++; mode = -1; continue; }
    // Feuermodus
    if(mode && !fFireModes) { i++; mode = -1; continue; }
    if(!(obj->GetFMData(FM_Name, mode))) { i++; mode = -1; continue; }
    if(mode == obj->GetFireMode() && mode) continue;
    // Nix gut
    if(obj->GetFMData(FM_Aim, mode)>0)
      if(iLevel == 1 || !WildcardMatch(GetAction(), "*Walk*"))
        continue;
    // Keine Munition dafür?
    if(!NoAmmo() && !(obj->GetCharge()) && !GetAmmo(obj->GetFMData(FM_AmmoID, mode)))
      continue;
    // EMP nur gegen Maschinen
    if(obj->GetBotData(BOT_EMP, mode))
      if(!(pTarget->~IsMachine()))
        continue;
    // Kein Favorit bisher?
    if(!fav)
    {
      fav = obj;
      type = fav->GetBotData(BOT_DmgType, mode);
      favmode = mode;
    }
    else
    {
      // Favorit hat nicht genug Reichweite
      if(fav->GetBotData(BOT_Range, favmode) < dist)
      {
        // Neue Waffe hat mehr
        if(obj->GetBotData(BOT_Range, mode) > dist)
        {
          fav = obj;
          type = obj->GetBotData(BOT_DmgType, mode);
          favmode = mode;
        }
      }
      else
      {
        // Favorit hat genug Reichweite -> nur wechseln, wenn Schadenstyp besser
        if(pTarget->~OnDmg(obj->GetBotData(BOT_DmgType, mode)) < pTarget->~OnDmg(type) &&
          // Allerdings darf die Waffe nicht zu schwach sein
          fav->GetBotData(BOT_Power, favmode)-1 <= obj->GetBotData(BOT_Power, mode))
        {
          // Neuer Favorit
          fav = obj;
          type = fav->GetBotData(BOT_DmgType);
          favmode = mode;
        }
        else
        {
          // Stärke der neuen Waffe ist größer oder Favorit ist ein Langlader
          if(fav->GetBotData(BOT_Power, favmode) < obj->GetBotData(BOT_Power, mode) ||
             (fav->GetBotData(BOT_Power, favmode) == BOT_Power_LongLoad && (fav->IsReloading() || !(fav->GetCharge()))))
          {
            // Waffe hat keine extralange Nachladezeit
            if(obj->GetBotData(BOT_Power, mode) != BOT_Power_LongLoad)
            {
              // Neuer Favorit
              fav = obj;
              type = fav->GetBotData(BOT_DmgType);
              favmode = mode;
            }
            // Waffe sollte nicht nachladen und nicht leer sein
            else if(obj->GetCharge() != 0 && !(obj->IsReloading()))
              {
                // Neuer Favorit
                fav = obj;
                type = fav->GetBotData(BOT_DmgType);
                favmode = mode;
              }
          }
        } 
      }
      // Reichweite passt
      if(fav->GetBotData(BOT_Range, favmode) >= dist)
        // Schadenstyp auch
        if(preftype == type)
          // Stärke auch
            if(fav->GetBotData(BOT_Power, favmode) >= BOT_Power_3)
              break;
    }
  }
  // Auswählen
  if(!fav) return;
  // Feuermodus wechseln?
  if(fFireModes)
    if(favmode && favmode != fav->GetFireMode())
      fav->SetFireMode(favmode);
  if(ContentsCount() == 1) return(1);
  return(ShiftContents(0,0,fav->GetID()));
}

protected func MacroComMoveTo()
{
	var x, y;
	x = GetMacroCommand(0,2);
	y = GetMacroCommand(0,3);
	if(!inherited()) return;
	SetCommand(this, "MoveTo", 0, x, y);
	return;
}*/
