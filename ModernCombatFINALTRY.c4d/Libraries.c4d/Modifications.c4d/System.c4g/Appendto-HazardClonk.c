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
  if(Control2Grab("ControlThrow")) return true;
  return _inherited();
}

protected func ControlDigDouble()
{
  if(Control2Grab("ControlDigDouble")) return true;
  return _inherited();
}

protected func ControlDownDouble()
{
  if(Control2Grab("ControlDownDouble")) return true;
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
  UpdateAiming();
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
  if((!ReadyToFire() && !ReadyToGrenade()) || !IsArmed() || GetCursor(GetOwner()) != this)
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

    
  if(IsAiming())
    if(IsCrawling())
      DoSpread(-(CH_CrawlSpreadReduction+unspread));
    else
      DoSpread(-(CH_AimSpreadReduction+unspread));
  else
    DoSpread(-(CH_StandSpreadReduction+unspread));

  UpdateAiming();
  return true;
}

local pIcon;

protected func DeathAnnounce(int plr, object clonk, int killplr)
{
  if(!clonk) clonk = this;
  if(!clonk) return;
  var r = CLNK->DeathAnnounce(plr, clonk, killplr, true, GetAssist()+1);
  if(r) DoPoints();
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
  if(IsRiding() && GetActionTarget()->~UpdateCharge(this))
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
  if(hud) hud->Update(Content, AmmoStoring(),this);

  return true;
}

public func AimAngleEx(int iMaxAngle, int iRange, bool bSpread)
{
  if(!this) return false;

  var angle = AimAngle2(iMaxAngle,iRange,bSpread);
  if(!angle)
    angle = AimAngle(iMaxAngle,iRange,bSpread)*100;
    
  return angle;
}

public func AimAngle2(int iMaxAngle, int iRange, bool bSpread)//Präzision 100. Also von 100 bis 36000.
{
   var angle;
   var x,y,r;
   WeaponAt(x,y,r);
   
   if(!IsAiming())
     angle = (90+r)*(GetDir()*2-1);
   else
     angle = crosshair->GetAngle();
  
   if(iRange)
   {
     var target = GetTarget(angle,iMaxAngle,iRange);
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
	WeaponAt(x,y,r);

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
	  var target = GetTarget(angle,iMaxAngle,iRange);
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
   
  if(!IsAiming())
  {
    //if((crosshair->GetAngle() != -90) || (crosshair->GetAngle() != +90))
      //ResetCH();

    var dummy1, dummy2, r;
    WeaponAt(dummy1, dummy2, r);
      
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

  SetPhase(AimAngle2Phase(90));
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
    
  SetPhase(AimAngle2Phase(90));
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
  if(!IsAiming())
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

  UpdateAiming();
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

/* Kontextmenü */

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
    return RemoveObject(pPack); //What a fail. :C
  }

  Sound("PackAmmo.ogg");

  //Menü schließen, mehr als eine Box kann eh nicht getragen werden
  CloseMenu(pCaller);
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
		if(IsSquatAiming() || Contents()->~GetFMData(FM_Aim) != 1) {
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
		  if(IsSquatAiming() || Contents()->~GetFMData(FM_Aim) != 1) {
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
	return Hangling(); //Funktional identisch
}

protected func GetObject2Drop(object pObj)
{
  if (GetProcedure() == "PUSH" && GetID(GetActionTarget()) == FKDT) {
    var dropobj, i;

    if (pObj->~IsWeapon()) {

      //Hat die Waffe schon: Geht nicht
      if (FindContents(GetID(pObj)))
        return;

      //Pistole? Dann weg damit
      if (dropobj = FindContents(PSTL))
        return dropobj;

      //Sonst die hinterste Waffe
      for (i = 0; i < ContentsCount(); i++)
        if (Contents(i) && Contents(i)->~IsWeapon())
          dropobj = Contents(i);
      return dropobj;
    }
	
	//Granate?
	if (pObj->~IsGrenade()) {
	  //Hat schon genug Granaten
	  if (GrenadeCount() >= MaxGrenades())
	    return;
	  //Granate in den Gürtel verfrachten
	  pObj->~Activate(this);
	  return;
	}

    //Objekt - hinterstes Objekt rauswerfen
    else {
      for (i = 0; i < ContentsCount(); i++)
        if (Contents(i) && !(Contents(i)->~IsWeapon()) && !Contents(i)->~IsGrenade())
          dropobj = Contents(i);
      return dropobj;
    }
  }

  return _inherited(pObj, ...);
}

/* Gibt jetzt auch FAPHeal und so*/

public func IsHealing () { return WildcardMatch(GetAction(), "*Heal*"); }




/*-- Use Grenades --*/

public func MaxGrenades(){return 4;}	//Maximale Granaten im Inventar und Gürtel


/* Allgemeines */

local pGrenadeStoring;

protected func ContextGrenadeMenu(object pCaller)
{
  [$CtxGrenadeMenu$|Image=GRNS|Condition=HasGrenades]
  Sound("GrenadeCharge.ogg", 1, 0,0, GetOwner()+1);
  GrenadeMenu(pCaller);
}

protected func HasGrenades()
{
  return Contents(0, pGrenadeStoring);
}

public func GrenadeMenu(object pCaller)
{
  CreateMenu(GRNS,pCaller,this,0,"$CtxGrenadeMenu$",0,C4MN_Style_Context);
  
  var id_list = CreateArray();
  for(var nade in FindObjects(Find_Container(pGrenadeStoring)))
  {
    if(FindInArray4K(id_list,GetID(nade)) != -1) continue;
    id_list[GetLength(id_list)] = GetID(nade);
    
    AddMenuItem(Format("<c %x>%.1s</c>",nade->Color(),GetName(nade)), "GrabGrenade", GetID(nade), pCaller,
                ObjectCount2(Find_ID(GetID(nade)), Find_Container(pGrenadeStoring)),
                0, GetDesc(nade));
  }
}

public func GetGrenade(id type)
{
  return FindContents(type,pGrenadeStoring);
}

public func GrabGrenade(id type)
{
  var grenade = FindContents(type,pGrenadeStoring);
  if(!grenade) return;
  Enter(this, grenade);
  ShiftContents (this,false,type,true);
  return grenade;
}

public func GrabGrenades(object pInto) {
	for(var pGrenade in FindObjects(Find_Container(pGrenadeStoring))) {
		Enter(pInto, pGrenade);
	}
	return true;
}

public func StoreGrenade(object pGrenade)
{
  if(!pGrenade) return;

  if(Contained(pGrenade)  == this)
  {
    if(ObjectCount2(Find_Container(pGrenadeStoring)) >= MaxGrenades())
      return false;
  }
  else
  {
    if(GrenadeCount() >= MaxGrenades())
      return false;
  }
  Sound("GrenadeCharge.ogg", 1, 0,0, GetOwner()+1);
  Enter(pGrenadeStoring, pGrenade);
  return(true);
}

public func GrenadeCount(id type)
{
  if(!type) return ObjectCount2(Find_Container(pGrenadeStoring)) + ObjectCount2(Find_Func("IsGrenade"),Find_Container(this));
  return ObjectCount2(Find_ID(type), Find_Container(pGrenadeStoring)) + ObjectCount2(Find_ID(type), Find_Container(this));
}

public func GetGrenadeStoring()
{
  if(!pGrenadeStoring)
    pGrenadeStoring = CreateObject(GRNS,0,0,GetOwner());
  return pGrenadeStoring;
}

/* Overloads */

protected func Initialize()
{
  if(!pGrenadeStoring)
    pGrenadeStoring = CreateObject(GRNS,0,0,GetOwner());
    
  pGrenadeStoring->SetUser(this);
  
  return _inherited(...);
}

protected func Destruction()
{
  if(pGrenadeStoring)
    RemoveObject(pGrenadeStoring);
  return _inherited(...);
}

public func IsArmed()
{
  if(!Contents()) return; 
  if(Contents()->~IsWeapon())
    return true;
  if(Contents()->~IsGrenade())
    return true;
  return false;
}

protected func Collection2(object pObj)// Einsammeln
{
  // das neue Item nach hinten verschieben (außer es ist Ammo oder eine Granate)
  if(!(pObj->~IsAmmoPacket()) || !(pObj->~IsGrenade()) || NoAmmo())
    if (1 == ContentsCount(GetID(pObj)))
      if(!(GetOCF(pObj) & OCF_Living)) {
      ShiftContents(0,0,0,0);
      pObj ->~ OnDeselect();
    }
  UpdateCharge();
  return _inherited(pObj);
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
      return;
  
  //Waffe?
  if(pObj ->~ IsWeapon())
  {
    //Sonderbehandlung?
    if(!(pObj ->~ OnCollection(this)))
    {
      //nein. Standardprozedur:
      //schon so eine Waffe im Inventar? Oder bereits 3 andere Waffen?
      if(ContentsCount(idObj) || CustomContentsCount("IsWeapon") >= WeaponCollectionLimit())
        return 1;  //Ja, nicht aufnehmen
      else
        return;
    }
    else
      return;
  }
  
  //Granate?
  if(pObj ->~ IsGrenade())
  {
    if(pObj->~OnCollection(this))
      return;
    else
    {
      if(GrenadeCount() >= MaxGrenades())//Schon genug Granaten im Gürtel/Inventar?
      {
        return 1;
      }
      
      if(CustomContentsCount("IsGrenade"))
        ScheduleCall(pObj,"Activate",1,0,this);//Oha...

      return;
    }
  }

  //Wieviel haben wir denn schon im inventar?
  if(ContentsCount() - (CustomContentsCount("IsWeapon") + CustomContentsCount("IsGrenade")) >= ObjectCollectionLimit())
    return 1;
  
  // nicht angelegte Ausrüstung nochmal aufsammeln
  for(var gear in aGear)
    if(gear)
      if(GetID(gear) == idObj)
        return 1;
  
  // Ok
  return;
}

private func ChangeWeapon(object pTarget)
{
	if(pTarget->~IsWeapon() || pTarget->~IsGrenade())
  {
		var phase = GetPhase();
    if(IsCrawling())
    {
      SetAction("AimCrawl");
    }
    else if(pTarget->~GetFMData(FM_Aim) > 0)
    {
      if(WildcardMatch(GetAction(), "*Squat*"))
        SetAction("AimSquatLow");
      else
			  SetAction("AimLow");
    }
		else
    {
      if(WildcardMatch(GetAction(), "*Squat*"))
        SetAction("AimSquat");
      else
			  SetAction("Aim");
    }
	  SetPhase(phase);
	}
	else
  {
		StopAiming();
	}
}




/*-- CMC Agilität --*/

/* Check */

private func MayFlip()
{
  return (IsJumping() || GetAction() == "JetpackFlight") && !GetEffect("ExtraJump", this);
}

/* Geschwindigkeitsschub */

protected func WallJumpBoost()
{
  var jump = GetPhysical("Jump", 0) * 100 / GetPhysical("Jump", 0,0, GetID());
  SetXDir(25*(GetDir()*2-1) * jump / 100);
  SetYDir(-30 * jump / 100);
}

protected func BackFlipBoost()
{
  var jump = GetPhysical("Jump", 0) * 100 / GetPhysical("Jump", 0,0, GetID());
  SetXDir(42*((GetXDir()>0)*2-1) * jump / 100);
  SetYDir(-22 * jump / 100);
}

/* Wandsprung stoppen */

private func StopWallJump()
{
  var i = GetEffectCount(0,this);
  var j;
  while(i--)
   if(j = GetEffect("IntWallJump",this,i))
    RemoveEffect(0,this,j);
}

/* Backflip */

protected func BackFlip()
{
  Sound("ClonkAction*.ogg");
  SetComDir(!GetComDir());
  ScheduleCall(0, "BackFlipBoost", 1, 1);
}

/* Agilität */

private func ControlAgility(string strControl)
{
  //In Flüssigkeiten geht das Ganze nicht.
  if (InLiquid()) return;

  //Links
  if(strControl == "ControlLeft")
  {
   if(IsJumping())
   {
    SetDir(DIR_Left);
    SetComDir(COMD_Left); 
   }
   else
   if(GetProcedure() == "SCALE" && GetDir() != DIR_Left)
     AddEffect("IntWallJump",this,10,20,this);
   var iEff = AddEffect("ControlStack", this, 110, 5, this);
   EffectVar(0, this, iEff) = COMD_Left;
   return;
  }

  //Rechts
  if(strControl == "ControlRight")
  {
   if(IsJumping())
   {
    SetDir(DIR_Right);
    SetComDir(COMD_Right); 
   }
   else
   if(GetProcedure() == "SCALE" && GetDir() != DIR_Right)
    AddEffect("IntWallJump",this,10,20,this);

   var iEff = AddEffect("ControlStack", this, 110, 5, this);
   EffectVar(0, this, iEff) = COMD_Right;
   return;
  }

  //Doppellinks
  if(strControl == "ControlLeftDouble")
  {
   if(GetEffect("IntWallJump",this) && !GetContact(this))
   {
    SetAction("Jump");
    SetDir(DIR_Left);
    SetComDir(COMD_Left);
    WallJumpBoost();
    Sound("ClonkAction*.ogg");
    StopWallJump();
    return 1;
   }
  }
  //Doppelrechts
  if(strControl == "ControlRightDouble")
  {
   if(GetEffect("IntWallJump",this) && !GetContact(this))
   {
    SetAction("Jump");
    SetDir(DIR_Right);
    SetComDir(COMD_Right);
    WallJumpBoost();
    Sound("ClonkAction*.ogg");
    StopWallJump();
    return 1;
   }
  }

  //Einmal links
  if(strControl == "ControlLeftSingle")
  {
   if (IsJumping())
   {
    SetDir(DIR_Left);
    SetComDir(COMD_Left);
    return 1;
   }
  }

  //Einmal rechts
  if(strControl == "ControlLeftSingle")
  {
   if(IsJumping())
   {
    SetDir(DIR_Right);
    SetComDir(COMD_Right);
    return 1;
   }
  }

  //Springen
  if(strControl == "ControlUp")
  {
   if(WildcardMatch(GetAction(), "Scale*") && GetActTime() < 5)
           EffectVar(2, this, AddEffect("ControlStack", this, 110, 5, this)) = true;

   if(GetEffect("ControlStack", this))
           EffectVar(1, this, GetEffect("ControlStack", this)) = COMD_Up;

   return;
  }
}


/* Backflip starten */

public func JumpStart(bool bBackflip)
{
  var iEff, iComd;
  if((iEff = GetEffect("ControlStack", this)))
  {
   if(EffectVar(1, this, iEff) != COMD_Up && !bBackflip) return;
   iComd = EffectVar(0, this, iEff);

   RemoveEffect(0, this, iEff);
   if(iComd == COMD_Left)
   {
    if(GetDir() == DIR_Right)
     BackFlip();
   }
   if(iComd == COMD_Right)
   {
    if(GetDir() == DIR_Left)
     BackFlip();
   }
  }
  //Für Leiter
  _inherited(bBackflip);
}

public func ReleaseLadderStop()
{
  if(ReleaseLadder(-20*(GetDir()*2-1)))
  {
   Sound("ClonkAction*.ogg");
   ScheduleCall(0, "BackFlipBoost", 1, 1);
  }
}

public func FxControlStackEffect(string newEffect, object pTarget, int iNo)
{
  if(newEffect != "ControlStack") return;
  ChangeEffect(0, pTarget, iNo, 0, -1);
  return -2;
}




/*-- Kriechen --*/

static const CRAWL_AIM_Max = 50;
local crosshair;


/* Hinlegen */

public func StartCrawling()
{
  //Hinlegen
  if((GetProcedure() == "WALK") ||
     (GetProcedure() == "DIG") ||
     (GetProcedure() == "BRIDGE") ||
     (GetProcedure() == "BUILD") ||
     (GetProcedure() == "PUSH") ||
     (GetProcedure() == "CHOP") ||
     (GetProcedure() == "LIFT"))
    return SetAction("StartCrawl");

  if(IsAiming())
  {
   StopAiming();
   return SetAction("StartCrawl");
  }
  return false;
}

/* Aufstehen */

public func StopCrawling()
{
  if(!IsCrawling() || !CanStandUp2()) return false;
  SetXDir();
  return SetAction("FlatUp"); 
}

/* Checks */

public func IsCrawling()
{
  return WildcardMatch(GetAction(), "*Crawl*");
}

public func Ready2Crawl()
{
  return !GetEffect("NoCrawl") && !IsCrawling() && (GetAction() == "WalkArmed" || GetAction() == "Walk");
}

public func CanStandUp()
{
  //Nur wenn kein Material über dem Clonk ist
  return PathFree(GetX(), GetY(), GetX(), GetY()-7-5);
}

public func CanStandUp2()
{
  //Aufstehen möglich, wenn Material über Clonk grabbar ist
  if(GBackSolid(0,-7-5))
   if(!GetMaterialVal("DigFree","Material",GetMaterial(0,-7-5)))
    return false;

  return true;
}

/* Callbacks */

protected func StartCrawl()
{
  // Wenn nötig Effekt erzeugen 
  if(!GetEffect("Crawl", this)) AddEffect("Crawl", this, 1, 0, this);
}

protected func CheckCrawlFall()
{
  if(CanStandUp())
   SetAction("FlatUp"); 
  else
  {
   if(Abs(GetXDir()) < 2)
   {
    var x;
    if(!GBackSolid(-4,4-5))
     x--;
    if(!GBackSolid(+4,4-5))
     x++;

    if(x)
    {
     SetXDir(x*10);
    }
   }
  }
}

protected func AbortCrawl() 
{
  var act = GetAction();

  // Bei manchen Aktionen nicht abbrechen
  if(act == "Scale")  return SetAction("Crawl");	//Anstoßen an der Wand
  if(act == "Hangle") return SetAction("Crawl");	//Anstoßen an der Decke
  if(act == "Tumble") return SetAction("Crawl");	//Bei Objekttreffern liegen bleiben
  if(act == "Walk")   return SetAction("Crawl");	//Mysteriöse Walk-Aktion

  if(act != "AimCrawl" && WildcardMatch(act, "Aim*"))
  {
   //var phase = GetPhase();
   SetAction("AimCrawl");
   //SetPhase(phase);
   return 1;
  }

  if(WildcardMatch(act, "*Jump*"))
  {
   if(!CanStandUp())
    return SetAction("CrawlFall");
   else
   {
    RemoveEffect("Crawl", this);

    if(GetDir() == DIR_Left)
     SetDir(DIR_Right);
    else
     SetDir(DIR_Left);

    SetComDir(COMD_Stop);

    SetXDir();
    AddEffect("IntCrawl2Scale",this,10,1,this);

    var i = 10;
    var dir = -(GetDir()*2-1);
    SetPosition(GetX()+dir,GetY()+8+5);
    while(i-- > 0)
    {
     if(!Stuck())
     {
      SetPosition(GetX()-dir,GetY());
      break;
     }
     SetPosition(GetX()+dir,GetY());
    }

		if(Stuck()) AutoUnstuck(this,0,-10);

    //SetPosition(GetX()+(GetDir()*2-1)*-4,GetY()+8+5);
    SetAction("Scale");
    return 1;
   }
  }

  if(IsCrawling())  return;
  // Shape und Vertices zurücksetzen
  RemoveEffect("Crawl", this);
  SetAction("FlatUp"); 
}

private func UpdateVertices()
{
  if(GetAction() == "AimCrawl") return;

  var x,y,r;
  WeaponAt(x,y,r);
  SetVertex(0,0,x/1000 * (GetDir()*2-1),0,2);
  SetVertex(0,1,y/1000,0,2);
}

private func ResetVertices()
{
  if(GetAction() == "AimCrawl") return;

  SetVertex(0,0,0);
  SetVertex(0,1,0);
}

public func FxIntCrawl2ScaleTimer()
{
  SetXDir();
  if(Stuck()) AutoUnstuck(this,0,-10);
  return -1;
}

protected func UpdateTransferZone()
{
  if(IsCrawling()) 
  { 
   RemoveEffect("Crawl", this);
   AddEffect("Crawl", this, 1, 0, this);
  }
  return _inherited();
}

/* Kriech-Effekt */

public func FxNoCrawlTimer() { return -1; }

public func FxCrawlStart(pClonk, iNum)
{
  // Physical, Shape und Vertixes anpassen
  SetPhysical("Walk", GetPhysical("Walk", 0, pClonk)/4, PHYS_StackTemporary);
  SetShape(-8, 2-5, 16, 8);
  SetVertexXY(0, 0,5-5);
  SetVertexXY(1, 0,2-5);
  SetVertexXY(2, 0,9-5);
  SetVertexXY(3,-2,3-5);
  SetVertexXY(4, 2,3-5);
  SetVertexXY(5,-4,3-5);
  SetVertexXY(6, 4,3-5);
  SetPosition(GetX(),GetY()+5);
}

public func FxCrawlStop(pClonk, iNum)
{
  //Physical, Shape und Vertices zurücksetzen
  ResetPhysical(pClonk, "Walk");
  SetShape(-8, -10, 16, 20);
  SetVertexXY(0, 0, 0);
  SetVertexXY(1, 0,-7);
  SetVertexXY(2, 0, 9);
  SetVertexXY(3,-2,-3);
  SetVertexXY(4, 2,-3);
  SetVertexXY(5,-4, 3);
  SetVertexXY(6, 4, 3);
  SetPosition(GetX(),GetY()-5);
}

/* Überladungen */

protected func ControlThrow()
{
  if(GetAction() == "StartCrawl")
    return 1;
    
  if(IsCrawling() && IsArmed() && !ReadyToFire() && ReadyToAim()) {
    StartAiming();
    return 1;
  }
  if(IsCrawling() && !IsAiming()) {
    var obj = Contents();
    if(obj) {
			if(!obj->~CanAim()) {
				if(obj->~ControlThrow(this)) return 1;
				var dir = -(GetDir()*2-1);
				var angle = -45;

				Exit(obj);
				SetController(GetOwner(this), obj);
				SetPosition(GetX(this)-(dir*8),GetY(this), obj);
			
				SetR(angle, obj);
			
				while(Stuck(obj))
					SetPosition(GetX(obj),GetY(obj)-1,obj);

				SetXDir(+Sin(angle,30)*dir, obj);
				SetYDir(-Cos(angle,30), obj);
				SetRDir(RandomX(-6,6), obj);
				return 1;
			}
			else {
  			StartAiming();
  			return 1;
			}
		}
	}
	return _inherited();
}

public func ControlUp()
{
  if(IsCrawling() && !IsAiming())
  {
   StopCrawling();
   return 1;
  }
  return _inherited(...);
}

public func ControlDig()
{
  if(GetPlrDownDouble(GetOwner()) && !IsCrawling() && Ready2Crawl() && !Contained())
  {
   StartCrawling();
   return true;
  }
  return _inherited(...);
}

public func ControlDigSingle()
{
  //Beim Kriechen kann man nicht graben
  if(IsCrawling()) return 1;
  return _inherited(...);
}

public func ReadyToAim()
{
  var val = _inherited();
  if(val) return val;
  if(/*GetAction() == "CrawlArmed" || */GetAction() == "Crawl") return 1;
}

private func CheckArmed()
{
  if(GetAction() == "AimCrawl") if (!IsArmed2()) return SetAction("Crawl");
  return _inherited();
}

public func WeaponAt(&x, &y, &r)
{
  if(IsAiming() && (GetAction() == "AimCrawl"))
  {
   r = (Abs(crosshair->GetAngle())-90);
   x = 8000;
   y = 7000-5000;
   return 1;
  }

  return _inherited(x,y,r);
}

public func DoAiming(int iChange)
{
  if(IsCrawling())
  {
   var angle = Abs(crosshair->GetAngle()) + iChange;
   if(!Inside(angle,90-CRAWL_AIM_Max,90+CRAWL_AIM_Max) && !Inside(angle,270-CRAWL_AIM_Max,270+CRAWL_AIM_Max))
    return;
  }
  return _inherited(iChange);
}

public func DoMouseAiming(int iTx, int iTy)
{
  if(IsCrawling())
  {
   var angle = Normalize(Angle(GetX(),GetY(),iTx,iTy),-180);
   if(!Inside(Abs(angle),90-CRAWL_AIM_Max,90+CRAWL_AIM_Max))
    return;
  }
  return _inherited(iTx,iTy);
}

public func StopAiming()
{
  if(!IsCrawling()) return _inherited();
  var val = _inherited();
  if(val)
   SetAction("Crawl");
  return val;
}