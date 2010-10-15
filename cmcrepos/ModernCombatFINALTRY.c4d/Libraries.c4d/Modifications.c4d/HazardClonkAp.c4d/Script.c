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

public func ControlConf(int conf)
{
  var jnr = GetPlrCoreJumpAndRunControl(GetController());

  var iChange;
  if (Contents())
  iChange = Contents()->~AimAngleChange(jnr) * conf;

  if (!iChange)
    if (jnr)
	  iChange = conf * 2;
	else
	  iChange = conf * 10;

  DoAiming(iChange);
}

public func DoAiming(int iChange)
{
  //zielen wir überhaupt?
  if(!IsAiming())
    return;

  var angle = Abs(crosshair->GetAngle()) + iChange;

  if(angle > AIM_Max || angle < 0)
    return;
  if(GetDir() == DIR_Left)
    angle = 360-angle;

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
	  AddMenuItem("$CtxQuickInventoryOff$", Format("SetQuickInventoryOn(Object(%d))", ObjectNumber(pCaller)), SM06, pCaller);
	else
	  AddMenuItem("$CtxQuickInventoryOn$", Format("SetQuickInventoryOff(Object(%d))", ObjectNumber(pCaller)), SM05, pCaller);
	if(pCaller->HelpMessagesOff())
	  AddMenuItem("$CtxHelpMessagesOff$", Format("ContextHelpMessagesOn(Object(%d))", ObjectNumber(pCaller)), SM06, pCaller);
	else
	  AddMenuItem("$CtxHelpMessagesOn$", Format("ContextHelpMessagesOff(Object(%d))", ObjectNumber(pCaller)), CXIN, pCaller);

	AddMenuItem("$CtxResetAch$", "ContextResetAch", RWDS, pCaller, 0, 0, "$CtxResetAch$");

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

protected func ContextResetAch() {
  [0|Image=RWDS|Condition=NoContext]
  CreateMenu(RWDS, this, this, 0, GetName(0, RWDS), 0, C4MN_Style_Dialog);
  AddMenuItem("$ResetAchInfo$", 0, RWDS, this, 0, 0, " ");
  AddMenuItem("$ResetAchYes$", "ContextResetAchYes", CHOS, this, 0, 0, " ", 2, 3);
  AddMenuItem("$ResetAchNo$", "NoContext", SM06, this, 0, 0, " ");
  SelectMenuItem(2);
}

protected func ContextResetAchYes() {
  [0|Image=RWDS|Condition=NoContext]
  ResetPlayerAchievements(GetOwner());
  PlayerMessage(GetOwner(), "$ResetAchDone$", this);
}

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
