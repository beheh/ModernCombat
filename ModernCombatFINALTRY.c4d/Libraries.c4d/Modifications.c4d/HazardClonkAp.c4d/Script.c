/*-- Kampfclonk Appendto --*/

#strict
#appendto HZCK

local crosshair;

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
  if(this->~Control2Grab("ControlThrow")) return(1);
  return(_inherited(...));
}

protected func ControlDigDouble()
{
  if(this->~Control2Grab("ControlDigDouble")) return(1);
  return(_inherited(...));
}

protected func ControlDownDouble()
{
  if(this->~Control2Grab("ControlDownDouble")) return(1);
  return(_inherited());
}

public func ControlUp()
{
  RemoveEffect("IntMouseAiming", this);
  return(_inherited()); 
}

public func ControlDown() 
{
  RemoveEffect("IntMouseAiming", this);
  return(_inherited()); 
}

func ResetShowWeapon(object pNew)
{
  var effect = GetEffect("ShowWeapon",this()); 
  if(!effect) return(false);
  
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
  
  return(true);
}

public func DoAiming(int iChange)
{
  //zielen wir überhaupt?
  if(!IsAiming())
    return();

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
      return();

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
  if(!pGear) return(0);
  if(!(pGear->~IsHazardGear())) return(0);
  if(!HazardGearSupported(pGear)) return(0);
  return(_inherited(pGear));
}

public func HazardGearSupported(object pGear)//! -> Hazard 2.0
{
  return(true);//Standartmäßig alles schlucken.
}

private func AmmoBagContextCheck()
{
  return(!FindObject(NOAM));
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
  return(1);
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
  if(GetAction() eq "Crawl" && Contents())
    if(Contents()->~CanAim() && Contents()->~IsEquipment())
      return(true);
  return(_inherited(...));
}

/* Schaden */
public func OnHit(int iDmg, int iType, object pFrom)
{
  this()->~CalcPain(iDmg);
  //this()->~HurtSounds(-iDmg,iType);
    
  return(_inherited(iDmg,iType,pFrom));
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
  InitCH();
  //if(Contained())
    //HideCH();
  return(_inherited());
}

/*func HandR()
{
  return(0);//return(handr/CH_Spread_Prec);
}*/

public func GetCrosshair()
{
  return(crosshair);
}

private func GetSpreadAOff()
{
  handr = RandomX(-spread/2/CH_Spread_Prec,+spread/2/CH_Spread_Prec);
  return(handr);
}

private func GetSpreadAOff2()
{
  handr = RandomX(-spread/2/CH_Spread_Prec,+spread/2/CH_Spread_Prec);
  return(handr*(100/CH_Spread_Prec));
}

private func TestSpread()
{
  var proc = GetProcedure();
  
  if(proc eq "WALK")
  {
    if(GetComDir())
      if(spread < CH_WalkSpread)
        return(CH_WalkSpread);
    return(0);
  }
  
  if(proc eq "SWIM")
  {
    if(spread < CH_WalkSpread)
      return(CH_WalkSpread);
    return(0);
  }
  
  if(proc eq "FLIGHT")
  {
    if(spread < CH_JumpSpread)
      return(CH_JumpSpread);
    return(0);
  }

  if((proc eq "SCALE")||(GetAction() eq "ScaleLadder"))
  {
    if(spread < CH_ScaleSpread)
      return(CH_ScaleSpread);
    return(0);
  }
  
  if(proc eq "HANGLE")
  {
    if(spread < CH_HangleSpread)
      return(CH_HangleSpread);
    return(0);
  }
  
  return(0);
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
  if(pCursor != this()) return();

  if(GetOwner() < -1) return();

  // in Gebäuden/Fahrzeugen
  if(Contained())
    if(Contained()->~UpdateCharge(this()))
      return(1);

  // reitet
  if(this->IsRiding())
    if(GetActionTarget()->~UpdateCharge(this()))
      return(1);

  // ggf. an angefasstes Objekt weiterleiten
  var Content = Contents();
  if(GetAction() S= "Push")
  	if(GetActionTarget()->~IsWeapon())
  		Content = GetActionTarget();

  // HUD
  var hud = GetHUD();
  if(hud) hud->Update(Content, this->AmmoStoring(),this());

  return(1);
}

global func AimAngleEx(int iMaxAngle, int iRange, bool bSpread)
{
  if(!this()) return(0);

  var angle = this()->~AimAngle2(iMaxAngle,iRange,bSpread);
  if(!angle)
    angle = this()->AimAngle(iMaxAngle,iRange,bSpread)*100;
    
  return(angle);
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

   return(angle);
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

	return(angle);
}

public func DoSpread(int iChange, int iMax)
{
  var wpn = Contents();
  if(!wpn) return();
  if(!wpn->~IsWeapon() && !wpn->~IsGrenade()) return();
  
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
    return();
	
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
  if(!crosshair) return();
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
  if(!crosshair) return();
  
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

  return(_inherited());
}

public func Entrance(object pContainer)
{
  HideCH();
  
  return(_inherited(pContainer));
}

protected func Departure(pContainer)      // Gebäude verlassen
{
  UpdateCharge();
  return(_inherited(pContainer,...));
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
  SetComDir(COMD_Stop());

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
    return();

  // Winkel anpassen, wenn keine freie Auswahl (klassische Steuerung)
  if(!GetPlrCoreJumpAndRunControl(GetController()) && !fForceExact)
    iAngle = iAngle-iAngle%AIM_Step;

  // Winkel wird zu groß?
  if(iAngle > AIM_Max || iAngle < 0)
    return();

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
  return(1);
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
    if(GetAction(GetActionTarget()) ne "Punch")
    {
      Fling(GetActionTarget(), 4 * GetDir() - 2, -1);
      Punch(GetActionTarget(),10);
    }
    else
      Fling(this(), 0, -1);//Schwupps. Selber hingefallen.
  }

  SetComDir(COMD_Stop);

  return(1);
}

public func InCloseCombat()
{
  if((GetProcedure() eq "FIGHT") && GetActionTarget())//ToDo: Wirklich? :o
    return(true);
  return(false);
}

public func ControlThrow()
{
  if(!InCloseCombat()) return(_inherited(...));
  
  if(GetAction() ne "Fight")//Nicht in Ausgangsstellung?
    return(1);
    
  if(!Random(3)) Sound("Kime*");
  punchtype = 0;
  SetAction("Punch");
  
  return(1);
}

public func ControlThrowSingle()
{
  if(InCloseCombat()) return(ControlThrow(...));
  return(_inherited(...));
}

public func ControlThrowDouble()
{
  if(InCloseCombat()) return(ControlThrow(...));
  return(_inherited(...));
}

public func ControlDig()
{
  if(!InCloseCombat()) return(_inherited(...));
  
  if(GetAction() ne "Fight")//Nicht in Ausgangsstellung?
    return(1);
    
  if (!Random(3)) Sound("Kime*");
  punchtype = 1;
  SetAction("Punch");

  return(1);
}

public func ControlDigSingle()
{
  if(InCloseCombat()) return(ControlDig(...));
  return(_inherited(...));
}

public func ControlDigDouble()
{
  if(InCloseCombat()) return(ControlDig(...));
  return(_inherited(...));
}

public func FxSelectItemTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  return(-1);
}

private func Control2Contents(string command)
{
  // Haben wir was angefasst?
  if(GetAction() S= "Push")
    return(0);
  // Pause Reload: nicht wieder anfangen ey!!!
/*  if(command S= "ControlThrow")
  {
    if(WildcardMatch(GetAction(),"Scale*") || GetAction() S= "Hangle")
      return(1);
  }*/ //Auskommentiert wegen C4
	
  //Callback verhindert?
  if(GetEffect("SelectItem",Contents()))
  {
    if(command eq "ControlUpdate")
    {
       if(ObjectCall(Contents(), command, this(), Par(1)))
         return(1);
       else
         return(0);
    }
    
    if((command eq "ControlThrow") || (command eq "ControlDig"))
      return(1);
    else
      return(0);
  }
  // Getragenes Objekt hat spezielle Steuerungsauswertung
  if(ObjectCall(Contents(), command, this(), Par(1), Par(2), Par(3), Par(4), Par(5), Par(6), Par(7)))
    return(1);
  return(0);
}

public func ControlSpecial()
{
  [$CtrlInventoryDesc$|Image=INVT]
  
  // ControlSpecial an Container weitergeben (z.B. Fahrzeuge)
  if(Contained())
  {
    if(Contained()->~ContainedSpecial(this()))
      return(1);
  }
  // Keine Items?
  if(!Contents()) return();
  // Hardcode: BR-Bombe darf man nicht abwählen
  if(Contents()->GetID() == GBRB)
    return();
  // Manche Sachen dürfen einfach nicht
  if(Contents()->~RejectShift())
  	return();
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

public func Collection(object pObj, bool fPut)
{
  if(pObj->~SelectionTime())
  {
    if(!GetEffect("SelectItem",pObj))
      AddEffect("SelectItem",pObj,20,pObj->~SelectionTime(),0,GetID());
  }
  return(_inherited(pObj,fPut,...));
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
      return(0);
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
