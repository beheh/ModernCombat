/*-- Kampfclonk Appendto --*/

#strict
#appendto HZCK

local crosshair;
local aCollected;

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
  var effect = GetEffect("ShowWeapon",this()); 
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
  this()->~UpdateAiming();
}

/* HazardGear */
//Nicht jede Clonkart kann alles tragen. (z.B. Sanitäter)

public func EquipGear(object pGear)
{
  // überhaupt benutzbar
  if(!pGear) return false;
  if(!(pGear->~IsHazardGear())) return false;
  if(!HazardGearSupported(pGear)) return false;
  return _inherited(pGear);
}

public func HazardGearSupported(object pGear)//! -> Hazard 2.0
{
  return true;//Standartmäßig alles schlucken.
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
  CreateMenu(ABOX,this(),0,0,GetName(0,ABAG),0,1,1);
  
  var ambg = LocalN("ammobag");
  
  //Munition finden und hinzufügen
  while(ammo = Contents(x,ambg))
  {
    // nur Ammo
    if(!(ammo ->~IsAmmo())) continue;
    //hinzufügen
    AddMenuItem(GetName(ammo),"PackAmmo", GetID(ammo), this(), GetAmmo(GetID(ammo)),ObjectNumber(pCaller),GetDesc(ammo));
    //Zählervariable erhöhen
    x++;
  }
  return true;
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
  
  if(!Collect(pPack,this()))
  {
    DoAmmo(idType,+iChange); 
    RemoveObject(pPack);//What a fail. :C
  }
  else
    Sound("PackAmmo.ogg");
  
  if(this()->~AmmoBagContextCheck())
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
  if(GetAction() == "Crawl" && Contents())
    if(Contents()->~CanAim() && Contents()->~IsEquipment())
      return true;
  return _inherited(...);
}

/* Schaden */
public func OnHit(int iDmg, int iType, object pFrom)
{
  this()->~CalcPain(iDmg);
  //this()->~HurtSounds(-iDmg,iType);
    
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
    if(GetComDir())
      if(spread < CH_WalkSpread)
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
  if(!this()->~ReadyToFire() || !this()->~IsArmed() || (GetCursor(GetOwner()) != this()))
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

    
  if(this()->~IsAiming())
  {
    if(this->~IsCrawling())
      DoSpread(-(CH_CrawlSpreadReduction+unspread));
    else
      DoSpread(-(CH_AimSpreadReduction+unspread));
  }
  else
  {
    DoSpread(-(CH_StandSpreadReduction+unspread));
  }

  this()->~UpdateAiming();
}

local pIcon;

protected func DeathAnnounce(int plr, object clonk, int killplr)
{
  if(!clonk) clonk = this();
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
  if(pCursor && pCursor != this()) pCursor = pCursor->~GetRealCursor(); 
  if(pCursor != this()) return;

  if(GetOwner() < -1) return;

  // in Gebäuden/Fahrzeugen
  if(Contained())
    if(Contained()->~UpdateCharge(this()))
      return true;

  // reitet
  if(this->IsRiding())
    if(GetActionTarget()->~UpdateCharge(this()))
      return true;

  // ggf. an angefasstes Objekt weiterleiten
  var Content = Contents();
  if(GetAction() == "Push")
  	if(GetActionTarget()->~IsWeapon())
  		Content = GetActionTarget();

  // HUD
  var hud = GetHUD();
  if(hud) hud->Update(Content, this->AmmoStoring(),this());

  return true;
}

global func AimAngleEx(int iMaxAngle, int iRange, bool bSpread)
{
  if(!this()) return false;

  var angle = this()->~AimAngle2(iMaxAngle,iRange,bSpread);
  if(!angle)
    angle = this()->AimAngle(iMaxAngle,iRange,bSpread)*100;
    
  return angle;
}

public func AimAngle2(int iMaxAngle, int iRange, bool bSpread)//Präzision 100. Also von 100 bis 36000.
{
   var angle;
   var x,y,r;
   this()->~WeaponAt(x,y,r);
   
   if(!this()->~IsAiming())
     angle = (90+r)*(GetDir()*2-1);
   else
     angle = crosshair->GetAngle();
  
   if(iRange)
   {
     var target = this()->~GetTarget(angle,iMaxAngle,iRange);
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
	this()->~WeaponAt(x,y,r);

	if(!IsAiming()) {
		angle = (90+r)*(GetDir()*2-1);
		if(Contents())
			if(Contents()->~IsGrenade())
				angle = (60+r)*(GetDir()*2-1);
	}
	else {
		angle = crosshair->GetAngle();
	}

	if(iRange)
	{
		var target = this()->~GetTarget(angle,iMaxAngle,iRange);
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
  crosshair->Set(this());
  crosshair->SetAngle(90);
  
  ResetCH();
}

public func ShowCH()
{
  if(!crosshair)
    InitCH();
  if(!crosshair)
    return;
	
  if(crosshair->Contained())
    crosshair->Exit();
    
  if(crosshair->GetActionTarget() != this())
    crosshair->Reset(this());
   
  if(!this()->~IsAiming())
  {
    //if((crosshair->GetAngle() != -90) || (crosshair->GetAngle() != +90))
      //ResetCH();

    var dummy1, dummy2, r;
    this->WeaponAt(dummy1, dummy2, r);
      
    if(GetDir() == DIR_Left)
      crosshair->SetAngle(-90-r);
    else
      crosshair->SetAngle(+90+r);
  }
}
public func ResetCH()
{
  if(!crosshair) return;
  crosshair->Reset(this());

  crosshair->SetSpread(spread);
  
  /*if(GetDir() == DIR_Left)
    crosshair->SetAngle(-90);
  else
    crosshair->SetAngle(+90);*/
  
  ScheduleCall(this(),"UpdateAiming",1);
}

public func HideCH()
{
  if(!crosshair) return;
  
  handr = 0;
  crosshair->UnSet();
}

public func IsAiming ()
{
  if(Contained())
  {
    if(Contained()->~CanAim())
      return true;
  }
  
  if(GetProcedure() == "PUSH")
  {
    if(GetActionTarget())
      if(GetActionTarget()->~CanAim())
        return true;
  }

  return _inherited();
}

public func Entrance(object pContainer)
{
  HideCH();
  
  return _inherited(pContainer);
}

protected func Departure(pContainer)      // Gebäude verlassen
{
  UpdateCharge();
  return _inherited(pContainer,...);
}

public func Destruction()
{
  if(crosshair)
    RemoveObject(crosshair);
  return(_inherited());
}

public func StartAiming() //Wegen fehlendem Hazard-Feature.
{
  if(Contained()) return(Contained()->~StartAiming());

  SetXDir(0);

  if(Contents(0)->~GetFMData(FM_Aim) == 2 || Contents(0)->~GetFMData(FM_Aim) == 0)
    SetAction("AimLow");
  else
    SetAction("Aim");

  SetPhase(this()->~AimAngle2Phase(90));
  SetComDir(COMD_Stop);

  InitCrosshair();
  
  ScheduleCall(this(),"UpdateAiming",1);

  // Callback
  if(Contents(0)) Contents(0)->~AimStart();
}

public func StartSquatAiming() { // Anfangen in der Hocke zu zielen

  if(Contained()) return(Contained()->~StartAiming());

  SetXDir(0);
  if(Contents(0)->~GetFMData(FM_Aim) == 2 || Contents(0)->~GetFMData(FM_Aim) == 0)
    SetAction("AimSquatLow");
  else
    SetAction("AimSquat");
    
  SetPhase(this()->~AimAngle2Phase(90));
  SetComDir(COMD_Stop);

  InitCrosshair();
  
  ScheduleCall(this(),"UpdateAiming",1);

  // Callback
  if(Contents(0)) Contents(0)->~AimStart();
}

public func StopAiming()
{
  if(crosshair)
  {
    ResetCH();
    if(GetDir() == DIR_Left)
      crosshair->SetAngle(-90);
    else
      crosshair->SetAngle(+90);
  }
  return(_inherited());
}

public func SetAiming(int iAngle, bool fForceExact)
{
  //zielen wir überhaupt?
  if(!this()->~IsAiming())
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

  this()->~UpdateAiming();
}

/* Nahkampfsystem */
private func Fighting()
{
  if((GetCursor(GetOwner()) != this()) || (GetPlayerType(GetController()) == C4PT_Script))//Rudimentäre KI Selbstverteidigung.
  {
    if(!Random(2))
     ControlThrow();
    else if(!Random(2))
      ControlDig();
  }
  return true;
}

local punchtype;//<-lol

private func Punching()
{
  if (!Random(3)) Sound("Kime*");
  Sound("Punch*");
  
  if(punchtype == 0)
  {
    Punch(GetActionTarget());
  }
  else if(punchtype == 1)
  {
    if(GetAction(GetActionTarget()) != "Punch")
    {
      Fling(GetActionTarget(), 4 * GetDir() - 2, -1);
      Punch(GetActionTarget(),10);
    }
    else
      Fling(this(), 0, -1);//Schwupps. Selber hingefallen.
  }

  SetComDir(COMD_Stop);

  return true;
}

public func InCloseCombat()
{
  if((GetProcedure() == "FIGHT") && GetActionTarget())//ToDo: Wirklich? :o
    return true;
  return false;
}

public func ControlThrow()
{
  if(!InCloseCombat()) return(_inherited(...));
  
  if(GetAction() != "Fight")//Nicht in Ausgangsstellung?
    return true;
    
  if(!Random(3)) Sound("Kime*");
  punchtype = 0;
  SetAction("Punch");
  
  return true;
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
  if(!InCloseCombat()) return(_inherited(...));
  
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

public func FxSelectItemTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  return(-1);
}

private func Control2Contents(string command)
{
  // Haben wir was angefasst?
  if(GetAction() == "Push")
    return false;
  // Pause Reload: nicht wieder anfangen ey!!!
/*  if(command S= "ControlThrow")
  {
    if(WildcardMatch(GetAction(),"Scale*") || GetAction() S= "Hangle")
      return true;
  }*/ //Auskommentiert wegen C4
	
  //Callback verhindert?
  if(GetEffect("SelectItem",Contents()))
  {
    if(command == "ControlUpdate")
    {
       if(ObjectCall(Contents(), command, this(), Par(1)))
         return true;
       else
         return false;
    }
    
    if((command == "ControlThrow") || (command == "ControlDig"))
      return true;
    else
      return false;
  }
  // Getragenes Objekt hat spezielle Steuerungsauswertung
  if(ObjectCall(Contents(), command, this(), Par(1), Par(2), Par(3), Par(4), Par(5), Par(6), Par(7)))
    return true;
  return false;
}

protected func ContextQuickInventoryOn(object pCaller) {
  [$CtxQuickInventoryOn$|Image=QKIN|Condition=QuickInventoryOff]
  SetPlrExtraData(GetOwner(), "CMC_QuickInv", true);
  Sound("Click", 1, 0,0, GetOwner()+1);
}

protected func ContextQuickInventoryOff(object pCaller) {
  [$CtxQuickInventoryOff$|Image=QKIN|Condition=QuickInventoryOn]
  SetPlrExtraData(GetOwner(), "CMC_QuickInv", false);
  Sound("Click", 1, 0,0, GetOwner()+1);
}

public func QuickInventoryOn() { return(GetPlrExtraData(GetOwner(), "CMC_QuickInv")); }
public func QuickInventoryOff() { return(!QuickInventoryOn()); }

public func SelectQuickInventory(int iIndex) {
	if(!iIndex) return;
	iIndex--;
	var aiming = IsAiming() && Contents()->~CanAim();
	var angle = Abs(AimAngle());
	StopAiming();
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
	if(Contained())
	{
	  if(Contained()->~ContainedSpecial(this()))
	    return true;
	}
	if(QuickInventoryOn()) {
		if(!Contents() || !aCollected) return;
		
		if(Contents()->~RejectShift() || Contents()->GetID() == GBRB)
			return;

  	var ring = CreateSpeedMenu(0, this);
		if(aCollected[0]) if(aCollected[0]->Contained() == this) ring->AddUpItem(GetName(aCollected[0]),"SelectQuickInventory",GetContentsOffset(aCollected[0]),GetID(aCollected[0]));
		if(aCollected[1]) if(aCollected[1]->Contained() == this) ring->AddRightItem(GetName(aCollected[1]),"SelectQuickInventory",GetContentsOffset(aCollected[1]),GetID(aCollected[1]));
		if(aCollected[2]) if(aCollected[2]->Contained() == this) ring->AddDownItem(GetName(aCollected[2]),"SelectQuickInventory",GetContentsOffset(aCollected[2]),GetID(aCollected[2]));
		if(aCollected[3]) if(aCollected[3]->Contained() == this) ring->AddLeftItem(GetName(aCollected[3]),"SelectQuickInventory",GetContentsOffset(aCollected[3]),GetID(aCollected[3]));
		if(aCollected[4]) if(aCollected[4]->Contained() == this) ring->AddThrowItem(GetName(aCollected[4]),"SelectQuickInventory",GetContentsOffset(aCollected[4]),GetID(aCollected[4]));
  }
  else {
		// Keine Items?
		if(!Contents()) return;
		// Hardcode: BR-Bombe darf man nicht abwählen
		if(Contents()->GetID() == GBRB)
		  return;
		// Manche Sachen dürfen einfach nicht
		if(Contents()->~RejectShift())
			return;
		// wenn wir zielen, wollen wir nur Waffen haben
		if(IsAiming() && Contents(0)->~CanAim())
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
	if(!pObj) return;
	if(pObj->Contained() != this) return;
	var i = 0;
	while(aCollected[i]) {
		if(aCollected[i]->Contained() == this && aCollected[i] != pObj) {
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
  return(_inherited(pObj,...));
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
      ScheduleCall(this, "CheckArmed", 1);
      return false;
    }
  }
  return(_inherited(idTarget));
}

protected func ContentsDestruction()      // Wenn Inhaltsobjekte verschwinden
{
  // nach Waffe suchen
  ScheduleCall(this(), "CheckArmed", 1);
  UpdateCharge();
  // nächstes Objekt benachrichtigen
  Schedule("CheckContentsDestruction();", 1);
}

protected func CheckContentsDestruction() {
  if(Contents(0)) Contents(0)->~Selection(this());
}

protected func ScalingLadder() {
	return this->Hangling(); //Funktional identisch
}
