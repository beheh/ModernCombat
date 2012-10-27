/*-- Kampfclonk Appendto --*/

#strict 2
#appendto HZCK

local crosshair;
local aCollected;

local pHUDTarget;
local iGrenadeCount;

public func IsHealing()			{return WildcardMatch(GetAction(), "*Heal*");}
public func MaxGrenades()		{return 4;}					//Maximal im Inventar und Gürtel tragbare Granaten
public func DeathAnimationCount()	{return 0;}					//Anzahl Todesanimationen


/*----- Initialisierung -----*/

public func Initialize()
{
  InitCH();
  UpdateGrenadeCount();
  //if(Contained())
    //HideCH();
  return _inherited(...);
}

public func SetHUDTarget(pTarget)
{
  pHUDTarget = pTarget;
}

/*----- Zufällige Todesanimation -----*/

public func ChangeDeathAnim()
{
  var rnd = Random(DeathAnimationCount()) * !HasCrawled();

  SetAction(Format("CMC_Dead%d", rnd), GetActionTarget(), GetActionTarget(1));
  return true;
}

/*----- Steuerung -----*/

protected func Control2Grab(string command)
{
  if(GetProcedure() == "PUSH" && GetActionTarget())
  return GetActionTarget()->~Control2Grab(command,this,...);
}

/*protected func ControlThrow()
{
  return _inherited();
}*/

protected func ControlDigDouble()
{
  ClearMacroCommands();
  if(Control2Grab("ControlDigDouble")) return true;
  return _inherited();
}

protected func ControlDownDouble()
{
  if(Control2Grab("ControlDownDouble")) return true;

  if(GetAction() == "Push" || IsRiding())
  {
    if(GetActionTarget()->~ControlDownDouble(this))
      return true;
  }
  return _inherited();
}

protected func ControlUp()
{
  RemoveEffect("IntMouseAiming", this);
  ClearMacroCommands();
  SetAggroLevel(0,0,0,0,"ControlUp");

  if(ControlAim("ControlUp"))		return 1;
  if(Control2Grab("ControlUp"))		return 0;
  if(Control2Contents("ControlUp"))	return 1;
  if(ControlLadder("ControlUp"))	return 1;
  if(ControlAgility("ControlUp"))	return 1;
  if (FindObject2(Find_Func("IsBackDoor"), Find_AtPoint()) && GetEffect("NoDoorEntrance", this)) return 1;
  return(_inherited());
}

public func ControlDown() 
{
  RemoveEffect("IntMouseAiming", this);
  return _inherited(...);
}

/*----- Waffenfunktionen -----*/

public func WeaponEnd(&x, &y)
{
  var number = GetEffect("ShowWeapon",this);
  if(!number)
    return(0);
  EffectCall(this, number, "Update");
  return inherited(x, y);
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
  if(Contents())
  iChange = Contents()->~AimAngleChange(jnr) * conf;

  if(!iChange)
    if(jnr)
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

  var angle = BoundBy(Abs(crosshair->GetAngle()) + iChange, 0, AIM_Max);

  if(GetDir() == DIR_Left)
    angle = 360-angle;
  if(angle == crosshair->GetAngle())
    return;

  crosshair->SetAngle(angle);
  UpdateAiming();
}

public func Incineration(int iPlr)
{
  Extinguish();
  Schedule(Format("DoDmg(5, DMG_Fire, 0, 1, %d)", iPlr + 1), 1, 20, this);
  AddFireEffect(this, 30, FIRE_Red, 1);
}

/*----- HazardGear -----*/

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

public func ReadyToFire()
{
  if(GetProcedure() == "SWIM")
    return;
  if(GetAction() == "Crawl" && Contents() && Contents()->~CanAim() && Contents()->~IsEquipment())
    return true;

  //In Gebäude
  if(Contained()) return false;
  
  if(GetActionTarget())
  {
    //Reitet
    if(IsRiding()) return;
    //Schiebt
    if(GetProcedure() == "PUSH") return;
  }

  var a = GetAction();

  //Nur beim Laufen, Schwimmen oder Springen
  if(a == "WalkArmed" || a == "SwimArmed" || a == "JumpArmed")
    return true;

  //Importiert aus den Funktionalitäten: JetpackFlight und Aim
  if(a == "JetpackFlight") return true;

  if(WildcardMatch(a, "Aim*")) return true;
}

//Wegen SwimArmed
public func ReadyToAttack()
{
  if(ReadyToFire()) return true;
  if(GetProcedure() == "SWIM") return true;
  return false;
}

public func ReadyToGrenade()
{
  if(!ReadyToAttack()) return;
  if(Contents())
    if(Contents()->~IsGrenade()) return true;
  return false;
}

/*----- CMC Zielsystem -----*/

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

public func UpdateCH()
{
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
  var r = CLNK->DeathAnnounce(plr, clonk, killplr, true, GetAssist(killplr)+1);
  if(r) DoPoints();
  return r;
}

public func UpdateCharge()
{
  UpdateCH();
  
  //Nur wenn ich Cursor bin
  var pCursor = GetCursor(GetOwner());
  if(pCursor && pCursor != this) pCursor = pCursor->~GetRealCursor(); 
  if(pCursor != this) return;

  if(GetOwner() == NO_OWNER) return;

  //In Gebäuden/Fahrzeugen
  if(Contained() && Contained()->~UpdateCharge(this))
    return true;

  //Reitet
  if(IsRiding() && GetActionTarget()->~UpdateCharge(this))
    return true;

  //Ggf. an angefasstes Objekt weiterleiten
  var Content = Contents();
  if(GetAction() == "Push" && GetActionTarget() && GetActionTarget()->~IsWeapon())
    Content = GetActionTarget();

  //Ggf. an Gebäude/Fahrzeug weiterleten
  if(Contained() && Contained()->~IsWeapon())
    Content = Contained();

  //Ggf. speziell zugewiesenes Objekt
  if(pHUDTarget)
    Content = pHUDTarget;

  //HUD
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

  if(!IsAiming())
  {
    angle = (90+r)*(GetDir()*2-1);
    if(Contents())
      if(Contents()->~IsGrenade())
      {
        if(!r) r += 60;
        angle = r*(GetDir()*2-1);
      }
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
  crosshair->SetAngle((GetDir()*2-1) * 90);
  
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
    var angle;
    
    if(!GetDir())
      angle = -90-r;
    else
      angle = +90+r;
    if(crosshair->GetAngle() != angle)
        crosshair->SetAngle(angle);
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

  if(GetProcedure() == "PUSH" && GetActionTarget() && GetActionTarget()->~CanAim())
  {
    if(GetActionTarget()->~IsWeapon() && !(GetActionTarget()->~IsWeapon2()))
      return false;
    else
      return true;
  }

  return _inherited();
}

public func Entrance(object pContainer)
{
  HideCH();
  return _inherited(pContainer);
}

protected func Departure()      //Gebäude verlassen
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

public func FxUpdateAimingTimer(object pTarget, int iNr)
{
  UpdateAiming();
  return true;
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

  if(!GetEffect("UpdateAiming", this))
    AddEffect("UpdateAiming", this, 1, 1, this);

  //Callback
  if(Contents()) Contents()->~AimStart();
}

public func StartSquatAiming() // Anfangen in der Hocke zu zielen
{
  if(Contained()) return Contained()->~StartAiming();

  SetXDir();
  if(Contents()->~GetFMData(FM_Aim) == 2 || Contents()->~GetFMData(FM_Aim) == 0)
    SetAction("AimSquatLow");
  else
    SetAction("AimSquat");

  SetPhase(AimAngle2Phase(90));
  SetComDir(COMD_Stop);

  InitCrosshair();

  if(!GetEffect("UpdateAiming", this))
    AddEffect("UpdateAiming", this, 1, 1, this);

  //Callback
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

  RemoveEffect("UpdateAiming", this);

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

public func UpdateAiming()
{
  if(!IsAiming())
    return ResetVertices();

  var a = crosshair->GetAngle();

  //Position des Attach-Vertex so verschieben, dass das Kreuz der Waffe aus ist
  UpdateVertices();

  if((GetDir() == DIR_Left && a > 0) ||
     (GetDir() == DIR_Right && a < 0))
    crosshair->ChangeDir();

  SetPhase(AimAngle2Phase(a));
}

/*----- Nahkampfsystem -----*/

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

local punchtype;

private func Punching()
{
  if(!Random(3)) Sound("Kime*");
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
    
  if(!Random(3)) Sound("Kime*");
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

/*----- Kontextmenü -----*/

/* Hilfen-Kontext deaktivieren */

protected func NoContext() {}

protected func ContextHelpMessagesOn()
{
  [0|Image=ROCK|Condition=NoContext]
  _inherited(...);
  ContextSettings(Par());
  SelectMenuItem(1, Par());
}

protected func ContextHelpMessagesOff()
{
  [0|Image=ROCK|Condition=NoContext]
  _inherited(...);
  ContextSettings(Par());
  SelectMenuItem(1, Par());
}

/* Einstellungen */

protected func ContextSettings(object pCaller)
{
  [$Settings$|Image=CSTR]
  if(GetMenu(pCaller) == CSTR)
    var iSel = GetMenuSelection(pCaller);

  //Einstellungsmenü erstellen
  CreateMenu(CSTR, pCaller, pCaller, 0, "$Settings$", 0, C4MN_Style_Context, true);

  //QuickInventar
  if(pCaller->QuickInventoryOff())
    AddMenuItem("$CtxQuickInventoryOff$", Format("SetQuickInventoryOn(Object(%d))", ObjectNumber(pCaller)), SM06, pCaller, 0, 0, "$CtxQuickInventoryDesc$");
  else
    AddMenuItem("$CtxQuickInventoryOn$", Format("SetQuickInventoryOff(Object(%d))", ObjectNumber(pCaller)), SM05, pCaller, 0, 0, "$CtxQuickInventoryDesc$");

  //Hazard-Hilfsnachrichten
  if(pCaller->HelpMessagesOff())
    AddMenuItem("$CtxHelpMessagesOff$", Format("ContextHelpMessagesOn(Object(%d))", ObjectNumber(pCaller)), SM06, pCaller, 0, 0, "$CtxHelpMessagesDesc$");
  else
    AddMenuItem("$CtxHelpMessagesOn$", Format("ContextHelpMessagesOff(Object(%d))", ObjectNumber(pCaller)), CXIN, pCaller, 0, 0, "$CtxHelpMessagesDesc$");

  //Inventarsperre
  if(pCaller->GetInvLockMode())
    AddMenuItem("$CtxInvLockOn$", Format("SwitchInventoryLockMode(Object(%d))", ObjectNumber(pCaller)), WPN2, pCaller, 0, 0, "$CtxInvLockDesc$");
  else
    AddMenuItem("$CtxInvLockOff$", Format("SwitchInventoryLockMode(Object(%d))", ObjectNumber(pCaller)), SM06, pCaller, 0, 0, "$CtxInvLockDesc$");

  //Kompakt-Death-Menü
  if(pCaller->ShorterDeathMenu())
    AddMenuItem("$CtxShorterDMOn$", Format("SwitchDeathMenuMode(Object(%d))", ObjectNumber(pCaller)), FKDT, pCaller, 0, 0, "$CtxShorterDMDesc$");
  else
    AddMenuItem("$CtxShorterDMOff$", Format("SwitchDeathMenuMode(Object(%d))", ObjectNumber(pCaller)), SM06, pCaller, 0, 0, "$CtxShorterDMDesc$");

  //Radiomusik
  if(pCaller->RadioMusicAct())
    AddMenuItem("$CtxRadioMusicOn$", Format("SwitchRadioMusicMode(Object(%d))", ObjectNumber(pCaller)), RDIO, pCaller, 0, 0, "$CtxRadioMusicDesc$");
  else
    AddMenuItem("$CtxRadioMusicOff$", Format("SwitchRadioMusicMode(Object(%d))", ObjectNumber(pCaller)), SM06, pCaller, 0, 0, "$CtxRadioMusicDesc$");

  //Achievements zurücksetzen
  AddMenuItem("$CtxResetData$", "ContextResetData", RWDS, pCaller, 0, 0, "$CtxResetDataDesc$");

  SelectMenuItem(iSel, pCaller);

  return true;
}

public func SwitchInventoryLockMode(object pCaller)
{
  SetPlrExtraData(GetOwner(), "CMC_InvLockMode", !GetInvLockMode());
  Sound("Click", 1, 0,0, GetOwner()+1);
  ContextSettings(Par()); 
}

public func ShorterDeathMenu()	{return GetPlrExtraData(GetOwner(), "CMC_DeathMenuMode");}
public func RadioMusicAct()	{return !GetPlrExtraData(GetOwner(), "CMC_RadioMusicMode");}	//CMC_RadioMusicMode = Wenn 0: an, wenn 1: aus

public func SwitchDeathMenuMode(object pCaller)
{
  SetPlrExtraData(GetOwner(), "CMC_DeathMenuMode", !ShorterDeathMenu());
  Sound("Click", 1, 0,0, GetOwner()+1);
  ContextSettings(pCaller); 
}

public func SwitchRadioMusicMode(object pCaller)
{
  SetPlrExtraData(GetOwner(), "CMC_RadioMusicMode", RadioMusicAct());
  Sound("Click", 1, 0,0, GetOwner()+1);
  if(!RadioMusicAct())
  {
    for(var radio in FindObjects(Find_Func("IsRadio")))
      radio->StopSong(GetOwner()+1);
  }
  else
    for(var radio in FindObjects(Find_Func("IsRadio")))
      radio->StartSong(GetOwner()+1);
  ContextSettings(pCaller);
}

/* Objekt ablegen */

public func CanDropItem()
{
  if(!GetInvLockMode()) return false;
  if(Contained()) return false;
  if(!Contents(0)) return false;
  if(!Contents(0)->~IsWeapon() && !Contents(0)->~LockInventory()) return false;

  return true;
}

public func ContextDrop()
{
  [$DropItem$|Image=WPN2|Condition=CanDropItem]
  AddEffect("CannotBeCollected", Contents(0), 101, 1);
  if(GetAction() == "Crawl")
    Exit(Contents(0),0,6);
  else
    Exit(Contents(0),0,10);

  return true;
}

public func GetInvLockMode()	{return GetPlrExtraData(GetOwner(), "CMC_InvLockMode");}

/* Quick Inventory */

public func SetQuickInventoryOn(object pCaller)
{
  SetPlrExtraData(GetOwner(), "CMC_QuickInv", true);
  Sound("Click", 1, 0,0, GetOwner()+1);
  ContextSettings(Par());
}

public func SetQuickInventoryOff(object pCaller)
{
  SetPlrExtraData(GetOwner(), "CMC_QuickInv", false);
  Sound("Click", 1, 0,0, GetOwner()+1);
  ContextSettings(Par());
}

public func QuickInventoryOn()	{return GetPlrExtraData(GetOwner(), "CMC_QuickInv");}
public func QuickInventoryOff()	{return !QuickInventoryOn();}

/* Spielerdaten zurücksetzen */

protected func ContextResetData()
{
  [0|Image=RWDS|Condition=NoContext]
  CreateMenu(RWDS, this, this, 0, GetName(0, RWDS), 0, C4MN_Style_Dialog);
  AddMenuItem("$ResetDataInfo$", 0, RWDS, this, 0, 0, " ");
  AddMenuItem("$ResetAll$", "ResetData(7)", CSTD, this, 0, 0, " ");
  AddMenuItem("$ResetSettings$", "ResetData(1)", CSTD, this, 0, 0, " ");
  AddMenuItem("$ResetStatistics$", "ResetData(2)", CSTD, this, 0, 0, " ");
  AddMenuItem("$ResetAchievements$", "ResetData(4)", CSTD, this, 0, 0, " ");
  AddMenuItem("$ResetAchNo$", "NoContext", SM06, this, 0, 0, " ");
  SelectMenuItem();
}

global func ResetSettings(int iPlr)
{
  SetPlrExtraData(iPlr, "CMC_InvLockMode", 0);
  SetPlrExtraData(iPlr, "CMC_DeathMenuMode", 0);
  SetPlrExtraData(iPlr, "CMC_RadioMusicMode", 0);
  SetPlrExtraData(iPlr, "CMC_QuickInv", 0);
  SetPlrExtraData(iPlr, "Hazard_NoHelpMsg", 0);

  //Radioinformationen
  if(GetPlrExtraData(iPlr, "CMC_RadioMusicMode"))
  {
    for(var radio in FindObjects(Find_Func("IsRadio")))
      radio->StopSong(iPlr+1);
  }
  else
    for(var radio in FindObjects(Find_Func("IsRadio")))
      radio->StartSong(iPlr+1);
}

protected func ResetData(int iData, bool fContinue)
{
  //[0|Image=RWDS|Condition=NoContext]
  if(!fContinue)
  {
    CreateMenu(RWDS, this, this, 0, GetName(0, RWDS), 0, C4MN_Style_Dialog);
    AddMenuItem("$ResetAchInfo$", 0, RWDS, this, 0, 0, " ");
    AddMenuItem("$ResetAchYes$", Format("ResetData(%d, true)", iData), CHOS, this, 0, 0, " ",2,3);
    AddMenuItem("$ResetAchNo$", "NoContext", SM06, this, 0, 0, " ");
    SelectMenuItem(2);
    return true;
  }
  if(iData & 1)
    ResetSettings(GetOwner());
  if(iData & 2)
    ResetPlayerStats(GetOwner());
  if(iData & 4)
    ResetPlayerAchievements(GetOwner());

  PlayerMessage(GetOwner(), "$ResetDone$", this);
}

/* Munitionsgürtel */

private func AmmoBagContextCheck()
{
  return !FindObject(NOAM);
}

/*----- Gürtelmunitionsentnahme -----*/

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
    //Nur Munition
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
  var pCaller = Object(iCaller);

  //Selbstgepackte Munition als CUAM erstellen
  var pPack = CreateObject(CUAM,0,0,GetOwner());

  pPack->SetAmmoID(idType);
  var iChange = pPack->DoAmmoCount(GetAmmo(idType));
  DoAmmo(idType,-iChange);
  
  if(!Collect(pPack,this))
  {
    DoAmmo(idType,+iChange); 
    return RemoveObject(pPack);
  }

  Sound("PackAmmo.ogg");

  //Menü schließen (nicht mehr als eine Munitionsbox tragbar)
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

/* QuickInventory */

static const QINV_MainMenu = 1;
static const QINV_PrimaryMenu = 2;
static const QINV_SecondaryMenu = 3;
static const QINV_EquipmentMenu = 4;
static const QINV_ObjectMenu = 5;
static const QINV_GrenadeMenu = 6;

local QINV_MenuOrder; //Reihenfolge im Menü
local QINV_MenuItemIds; //ID-Reihenfolge der aufgesammelten Objekte einer Kategorie
local QINV_GrenadeTemporary; //Granate zum Wegstecken

public func Initialize() {
  QINV_MenuOrder = [RMEN_UpItem, RMEN_RightItem, RMEN_DownItem,  RMEN_LeftItem];
  QINV_MenuItemIds = [];
  QINV_GrenadeTemporary = 0;
  return _inherited();
}

//Gibt die erste auch im Inventar vorhande ID einer Menükategorie zurück
public func GetQuickInventoryMenuItem(int iMenu) {
  var aItems = GetQuickInventoryMenuItems(iMenu);
  for(var i = 0; i < GetLength(aItems); i++) {
    if(ContentsCount(aItems[i]) || GetGrenade(aItems[i])) return aItems[i];
  }
  return false;
}

//Gibt alle IDs einer Menükategorie in der Aufsammelreihenfolge zurück
private func GetQuickInventoryMenuItems(int iMenu)
{
  if(!QINV_MenuItemIds[iMenu]) QINV_MenuItemIds[iMenu] = [];
  var aItems = [];
  //Durch die IDs iterieren und Items holen
  for(var i = 0; i < GetLength(QINV_MenuItemIds[iMenu]); i++)
  {
    aItems[i] = QINV_MenuItemIds[iMenu][i];
  }
  return aItems;
}

private func GetQuickInventoryMenuItemCount(int iMenu)
{
  var aItems = GetQuickInventoryMenuItems(iMenu);
  var iCount = 0;
  for(var i = 0; i < GetLength(aItems); i++)
  {
    if(ContentsCount(aItems[i]) || GetGrenade(aItems[i])) iCount++;
  }
  return iCount;
}

public func QuickInventory(int iMenu, int iPage) {
  if(!iMenu) iMenu = QINV_MainMenu;

  //Inventar überhaupt vorhanden?
  if(ContentsCount() + iGrenadeCount < 1) return;

  //Hauptmenü
  if(iMenu == QINV_MainMenu) {
    //Granate zurücksetzen
    QINV_GrenadeTemporary = 0;

    //Menü erzeugen
    var pRing = CreateSpeedMenu(0, this);

    //Verweise auf Submenüs
    var aMenus = [QINV_GrenadeMenu, QINV_PrimaryMenu, QINV_SecondaryMenu, QINV_EquipmentMenu, QINV_ObjectMenu];
    for(var i = 0; i < GetLength(aMenus); i++)
    {
      var iDisplayMenu = aMenus[i];
      var idItem = GetQuickInventoryMenuItem(iDisplayMenu);
      if(idItem)
      {
        var iItemCount = GetQuickInventoryMenuItemCount(iDisplayMenu);
        if(iItemCount < 2)
        {
          pRing->Add(i, GetName(0, idItem), "QuickInventory", iDisplayMenu, idItem, 0, SMIN);
        }
        else
        {
          pRing->Add(i, GetName(0, idItem), "QuickInventory", iDisplayMenu, idItem, 0, SMIN, "11");
        }
      }
    }

    //Informationen
    if(ContentsCount() != 1)
    {
      pRing->AddTopInfoItem(Format("<c ffff00>$QuickInventory$</c>|$ContentsCount$", ContentsCount()));
    }
    else
    {
      pRing->AddTopInfoItem(Format("<c ffff00>$QuickInventory$</c>|$ContentCount$", ContentsCount()));
    }
  }

  //Submenüs
  if(iMenu == QINV_PrimaryMenu || iMenu == QINV_SecondaryMenu || iMenu == QINV_EquipmentMenu || iMenu == QINV_ObjectMenu || iMenu == QINV_GrenadeMenu) {