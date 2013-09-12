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

protected func ControlDigDouble()
{
  ClearMacroCommands();
  if(Control2Grab("ControlDigDouble")) return true;
  return _inherited();
}

protected func ControlDownDouble()
{
  if(Control2Grab("ControlDownDouble")) return true;
  if(ControlLadder("ControlDownDouble")) return true;

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
  if(FindObject2(Find_Func("IsBackDoor"), Find_AtPoint()) && GetEffect("NoDoorEntrance", this))	return 1;
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
  //Wird gezielt?
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

//Nicht jede Clonkart kann alles tragen (z.B. Sanitäter)

public func EquipGear(object pGear)
{
  //Benutzbarkeit prüfen
  if(!pGear || !pGear->~IsHazardGear() || !HazardGearSupported(pGear)) return false;
  return _inherited(pGear);
}

public func HazardGearSupported(object pGear)
{
  return true;
}

/*----- Feuerbereitschaft -----*/

public func ReadyToFire()
{
  //Nicht wenn am schwimmen,...
  if(GetProcedure() == "SWIM")
    return;

  if(GetAction() == "Crawl" && Contents() && Contents()->~CanAim() && Contents()->~IsEquipment())
    return true;

  //...wenn verschachtelt,...
  if(Contained()) return false;

  if(GetActionTarget())
  {
    //...reitend...
    if(IsRiding()) return;
    //...oder schiebend
    if(GetProcedure() == "PUSH") return;
  }

  var a = GetAction();

  //Nur beim Laufen, Schwimmen, Springen...
  if(a == "WalkArmed" || a == "SwimArmed" || a == "JumpArmed")
    return true;

  //...oder speziellen Jetpack- oder Aim-Aktionen
  if(a == "JetpackFlight") return true;
  if(WildcardMatch(a, "Aim*")) return true;
}

public func ReadyToAttack()
{
  //Spezialtest für SwimArmed
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

static const CH_ShowAlways = 1;			//Fadenkreuzanzeige

static const CH_MaxSpread = 500;		//Maximale Streuung

static const CH_WalkSpread = 80;		//Streuung beim Laufen
static const CH_JumpSpread = 140;		//Streuung beim Springen
static const CH_ScaleSpread = 200;		//Streuung beim Klettern
static const CH_HangleSpread = 200;		//Streuung beim Hangeln

static const CH_StandSpreadReduction = 3;	//Streureduktion beim Laufen
static const CH_AimSpreadReduction = 4;		//Streureduktion beim Zielen
static const CH_CrawlSpreadReduction = 4;	//Streureduktion beim Kriechen

local crosshair,ammobag;
local spread, handr;

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
  //Keine Feuerbereitschaft: Verstecken
  if((!ReadyToFire() && !ReadyToGrenade()) || !IsArmed())
    return HideCH();

  var c = Contents();

  //Streuung hinzufügen
  DoSpread(BoundBy(TestSpread()-spread,0,10));

  var unspread, minspread;

  //Waffeninformationen einholen
  if(c->~IsWeapon())
  {
    unspread = c->GetFMData(FM_UnSpread);
    minspread = c->GetFMData(FM_MinSpread);
  }
  else
  {
    unspread = c->~UnSpread();
    minspread = c->~MinSpread();
  }

  //Streuung abziehen
  if(IsAiming())
    if(IsCrawling())
      DoSpread(-(CH_CrawlSpreadReduction+unspread), 0, minspread);
    else
      DoSpread(-(CH_AimSpreadReduction+unspread), 0, minspread);
  else
    DoSpread(-(CH_StandSpreadReduction+unspread), 0, minspread);

  if(GetCursor(GetOwner()) != this)
  {
    HideCH();
    return;
  }

  ShowCH();

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

//Death ohne Effektlöschung
protected func Death()
{
  var plr = GetOwner();

  //Sound und Meldung
  Sound("Die");
  DeathAnnounce(GetOwner(), this, GetKiller());
  NoDeathAnnounce();

  if(GetPlayerType(GetOwner()) == C4PT_Script)
    GameCallEx("RelaunchPlayer", GetOwner(), this, GetKiller());
  else
  {
    var gotcrew;
    for(var i; i < GetCrewCount(plr); i++)
      if(GetOCF(GetCrew(plr, i)) & OCF_Alive)
        gotcrew = true;

    if(!gotcrew)
      GameCallEx("RelaunchPlayer", GetOwner(), this, GetKiller());
  }

  if(ammobag)
    RemoveObject(ammobag);

  FadeOut(this);

  Death2(...);
  if(HasGear()) TakeOffGear();

  return true;
}

public func UpdateCharge(bool fForceUpdate)
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

  //Gegebenenfalls an angefasstes Objekt weiterleiten
  var Content = Contents();
  if(GetAction() == "Push" && GetActionTarget() && GetActionTarget()->~IsWeapon())
    Content = GetActionTarget();

  //Gegebenenfalls an Gebäude/Fahrzeug weiterleiten
  if(Contained() && Contained()->~IsWeapon())
    Content = Contained();

  //Gegebenenfalls an speziell zugewiesenes Objekt weiterleiten
  if(pHUDTarget)
    Content = pHUDTarget;

  //HUD
  var hud = GetHUD();
  if(hud) hud->Update(Content, AmmoStoring(),this, fForceUpdate);

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

public func DoSpread(int iChange, int iMax, int iMin)
{
  var wpn = Contents();
  if(!wpn) return;
  if(!wpn->~IsWeapon() && !wpn->~IsGrenade()) return;
  
  if(iMax) iChange = Max(0,BoundBy(spread+iChange, 0, iMax)-spread);
  spread = BoundBy(spread+iChange, iMin, CH_MaxSpread);
  
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

protected func Departure()	//Gebäude verlassen
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

public func StartAiming()	//Wegen fehlendem Hazard-Feature
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

public func StartSquatAiming()	//Anfangen in der Hocke zu zielen
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

protected func NoContext()	{}

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

  //Spielerdaten zurücksetzen
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
  {
    ResetPlayerAchievements(GetOwner());
    ResetPlayerRibbons(GetOwner());
  }

  PlayerMessage(GetOwner(), "$ResetDone$", this);
}

/* Ausrüstung ablegen */

protected func FunnyBug()	{return HasGear() || HasAttachmentWeapons();}

protected func ContextUnbuckle(object pCaller)
{
  [$CtxUnbuckleDesc$|Image=HARM|Condition=FunnyBug]
  if(!HasGear() && !HasAttachmentWeapons())
    return;

  CreateMenu(HARM, 0, 0, 0, 0, 0, 1);

  //Alle Ausrüstungen auflisten
  for(var i; i < GetLength(aGear); ++i) 
  {
    var gear = aGear[i];
    if(gear)
      AddMenuItem("$CtxUnbuckleItem$", Format("TakeOffGear(0, %d)", i), GetID(gear));
  }

  //Alle Waffenaufsätze auflisten
  var obj, i = 0;
  while(obj = Contents(i++))
    if(obj->~IsWeapon2() && obj->~GetAttachment() != AT_NoAttachment)
      AddMenuItem(Format("$CtxDetachAttachment$", GetName(0, AttachmentIcon(obj->GetAttachment()))), "DetachAttachment", 0, this, 0, obj, 0, 4, obj);

  return true;
}

public func DetachAttachment(dummy, object pWeapon)
{
  if(!pWeapon->~IsWeapon2() || pWeapon->~GetAttachment() == AT_NoAttachment)
    return;

  var att = pWeapon->~GetAttachment();
  pWeapon->SetAttachment(AT_NoAttachment);

  //Waffenaufsatzobjekt erstellen
  var obj = CreateObject(WNAT, 0, 0, GetOwner());
  obj->SetAttachment(att);
  Collect(obj, this);

  //Waffen-HUD aktualisieren
  UpdateCharge(true);

  Sound("WNAT_RemoveAttachement.ogg");

  return true;
}

public func HasAttachmentWeapons()
{
  var obj, i;
  while(obj = Contents(i++))
    if(obj->~IsWeapon2() && obj->~GetAttachment() != AT_NoAttachment)
      return true;
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

  //Hat schon eine Box
  if(FindContents(CUAM, pCaller))
  {
    PlayerMessage(GetOwner(pCaller), "$NoSpace$", pCaller);
    return true;
  }

  //Selbstgepackte Munition als CUAM erstellen
  var pPack = CreateObject(CUAM,0,0,GetOwner());
  pPack->SetAmmoID(idType);
  var iChange = pPack->DoAmmoCount(GetAmmo(idType));

  //Munition aus dem Munitionsgürtel entnehmen
  DoAmmo(idType,-iChange);

  if(!Collect(pPack,this))
  {
    DoAmmo(idType,+iChange);
    return RemoveObject(pPack);
  }

  //Inventar auf erstellte Munition wechseln
  ShiftContents(pCaller, 0, CUAM);

  Sound("PackAmmo.ogg");

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

public func Initialize()
{
  QINV_MenuOrder = [RMEN_UpItem, RMEN_RightItem, RMEN_DownItem,  RMEN_LeftItem];
  QINV_MenuItemIds = [];
  QINV_GrenadeTemporary = 0;
  return _inherited();
}

//Gibt die erste auch im Inventar vorhande ID einer Menükategorie zurück
public func GetQuickInventoryMenuItem(int iMenu)
{
  var aItems = GetQuickInventoryMenuItems(iMenu);
  for(var i = 0; i < GetLength(aItems); i++)
  {
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

//Prüft, ob die nachfolgenden Seiten über anwählbare Items verfügen
private func QuickInventoryNextPageActive(array aItems, int iCurrentPage)
{
  for(var i = (iCurrentPage+1)*3; i < GetLength(aItems); i++)
    if(aItems[i] && (ContentsCount(aItems[i]) || GetGrenade(aItems[i])))
      return true;
  
  return false;
}

public func QuickInventory(int iMenu, int iPage)
{
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
        //Spezifisch das Icon von gepackten Munitionsboxen erfassen
        if(idItem == CUAM)
          idItem = FindContents(CUAM)->~SpeedMenuIcon();

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
    var aItems = GetQuickInventoryMenuItems(iMenu);
    if(GetLength(aItems))
    {
      //Erstes Objekt auswählen
      QuickInventorySelect(GetQuickInventoryMenuItem(iMenu), true);

      //Vorhanden Objekte zählen
      var iCount = GetQuickInventoryMenuItemCount(iMenu);

      //Wenn weniger als zwei, nicht zeichnen
      if(iCount < 2) return;

      //Submenü zeichnen
      var pRing = CreateSpeedMenu(0, this);
      for(var i = iPage*3; i < GetLength(aItems); i++)
      {
        if(i >= iPage*3+3)
        {
          if(QuickInventoryNextPageActive(aItems, iPage))
            pRing->AddLeftItem("$NextPage$", "QuickInventoryPaging", [iMenu, iPage+1], SM05);

          break;
        }
        if(aItems[i])
        {
          if(!ContentsCount(aItems[i]) && !GetGrenade(aItems[i])) continue;
          var idIcon = aItems[i];

          //Spezifisch das Icon von gepackten Munitionsboxen erfassen
          if(idIcon == CUAM)
            idIcon = FindContents(CUAM)->~SpeedMenuIcon();

          pRing->Add(QINV_MenuOrder[i-iPage*3], GetName(0, aItems[i]), "QuickInventorySelect", aItems[i], idIcon, 0, SMIN);
        }
      }
      pRing->AddThrowItem("$Use$", "QuickInventoryThrow", 0, SM04);

      //Verschiedene Menünamen einblenden
      var szName = "";
      if(iMenu == QINV_PrimaryMenu)
        szName = "$PrimaryWeapons$";
     if(iMenu == QINV_SecondaryMenu)
        szName = "$SecondaryWeapons$";
      if(iMenu == QINV_EquipmentMenu)
        szName = "$Equipment$";
      if(iMenu == QINV_ObjectMenu)
        szName = "$Objects$";
      if(iMenu == QINV_GrenadeMenu)
        szName = "$Grenades$";
      pRing->AddTopInfoItem(Format("<c ffff00>$QuickInventory$</c>|%s", szName));
    }
  }
}

private func QuickInventoryPaging(array aData)
{
  QuickInventory(aData[0], aData[1]);
}

private func QuickInventoryThrow()
{
  ControlThrow();
}

private func QuickInventorySelect(id idObject, bool fSaveTemporary)
{
  var fFound = SelectInventory(idObject);
  if(!fFound)
  {
    fFound = GrabGrenade(idObject);
    //Wenn wir jetzt exakt eine Granate herausgeholt haben
    if(fSaveTemporary && fFound && !QINV_GrenadeTemporary && ContentsCount(idObject))
      QINV_GrenadeTemporary = idObject;
  }

  if(QINV_GrenadeTemporary && !fSaveTemporary)
  {
    var pGrenade = 0;
    if(GetID(Contents()) != QINV_GrenadeTemporary) pGrenade = FindObject2(Find_ID(QINV_GrenadeTemporary), Find_Container(this));
    if(pGrenade) StoreGrenade(pGrenade);
    QINV_GrenadeTemporary = 0;
  }

  return fFound;
}

private func QuickInventoryStore(object pObj)
{
  //Menü ermitteln
  //Manuell?
  var iMenu = pObj->~CustomQuickInventoryMenu();
  //Primärwaffe
  if(!iMenu && pObj->~IsPrimaryWeapon() && !pObj->~IsSecondaryWeapon()) iMenu = QINV_PrimaryMenu;
  //Sekundärwaffe
  if(!iMenu && pObj->~IsSecondaryWeapon()) iMenu = QINV_SecondaryMenu;
  //Einsatzobjekt
  if(!iMenu && pObj->~IsEquipment() && !pObj->~IsGrenade()) iMenu = QINV_EquipmentMenu;
  //Granaten
  if(!iMenu && pObj->~IsGrenade()) iMenu = QINV_GrenadeMenu;
  //Sonst einfach nur Objekt
  if(!iMenu) iMenu = QINV_ObjectMenu;

  //Und einsortieren
  var fExists = false;
  if(!QINV_MenuItemIds) QINV_MenuItemIds = [];
  if(!QINV_MenuItemIds[iMenu]) QINV_MenuItemIds[iMenu] = [];
  for(var i = 0; i < GetLength(QINV_MenuItemIds[iMenu]); i++)
  {
    if(QINV_MenuItemIds[iMenu][i] == GetID(pObj)) fExists = true;
    if(fExists) break;
  }
  if(!fExists)
  {
    var idObject;
    var fReplace = false;
    //Nicht mehr relevante IDs ermitteln
    for(var i = 0; i < GetLength(QINV_MenuItemIds[iMenu]); i++)
    {
      idObject = QINV_MenuItemIds[iMenu][i];
      if(!ContentsCount(idObject) && !GetGrenade(idObject))
      {
        fReplace = true;
        break;
      }
    }
    if(fReplace)
    {
      //Nicht mehr relevante Referenz bestimmen
      var idOld = QINV_MenuItemIds[iMenu][i];
      QINV_MenuItemIds[iMenu][i] = GetID(pObj);
      //Nicht mehr relevante Referenz verschieben
      for(i++; i < GetLength(QINV_MenuItemIds[iMenu]); i++)
      {
        if(QINV_MenuItemIds[iMenu][i] == GetID(pObj))
        {
          QINV_MenuItemIds[iMenu][i] = idObject;
          break;
        }
      }
    }
    else
    {
      //Kein Verschieben notwendig
      QINV_MenuItemIds[iMenu][GetLength(QINV_MenuItemIds[iMenu])] = GetID(pObj);
    }
  }
}

public func SelectInventory(id idObject)
{
  if(!idObject || !Contents()) return;
  if(!ContentsCount(idObject)) return;
  if(GetID(Contents()) == idObject) return true;
  var aiming = IsAiming() && Contents()->~CanAim();
  var angle = Abs(AimAngle());
  if(aiming) StopAiming();
  while(GetID(Contents()) != idObject)
  {
    ShiftContents(this, 0, idObject, true);
  }
  if(Contents()->~CanAim() && aiming)
  {
    if(IsSquatAiming() || Contents()->~GetFMData(FM_Aim) != 1)
    {
      StartSquatAiming();
    }
    else
    {
      StartAiming();
    }
    SetAiming(angle, true);
  }
  if(crosshair) crosshair->UpdateAngle();
  UpdateCharge();

  return true;
}

public func ControlSpecial()
{
  [$CtrlInventoryDesc$|Image=INVT]

  ClearMacroCommands();

  //ControlSpecial an Container weitergeben (z.B. Fahrzeuge)
  if(Contained() && Contained()->~ContainedSpecial(this))
    return true;
  if(QuickInventoryOn())
  {
    if(Contents())
      if(Contents()->~RejectShift() || GetID(Contents()) == GBRB)
        return;

    QuickInventory(QINV_MainMenu);
  }
  else
  {
    //Keine Items?
    if(!Contents()) return;
    //Hardcode: BR-Bombe darf man nicht abwählen
    if(GetID(Contents()) == GBRB)
      return;
    //Manche Sachen dürfen einfach nicht
    if(Contents()->~RejectShift())
      return;
    //Wenn wir zielen, wollen wir nur Waffen haben
    if(IsAiming() && Contents()->~CanAim())
    {
      var angle = Abs(AimAngle());
      // nächste Waffe suchen
      var start = Contents(0);
      for(var i = 1; i < ContentsCount(); i++)
      {
        ShiftContents();
        var obj = Contents(0);
        if(obj->~CanAim() || obj == start)
        {
          ShiftContents(0, true);
          ShiftContents(0, 0, 0, true);
          break;
        }
      }
      if(IsSquatAiming() || Contents()->~GetFMData(FM_Aim) != 1)
      {
        StartSquatAiming();
      }
      else
      {
        StartAiming();
      }
      SetAiming(angle, true);
    }
    else
    //Inventar verschieben
    ShiftContents(0,0,0,true);
    if(crosshair) crosshair->UpdateAngle();
    UpdateCharge();
  }
}

public func Collection(object pObj, bool fPut)
{
  if(pObj->~SelectionTime())
  {
    AddEffect("SelectItem",pObj,20,pObj->~SelectionTime(),0,GetID());
  }
  return _inherited(pObj,fPut,...);
}

public func Collection2(object pObj)
{
  //Winkel des Fadenkreuzes aktualisieren, falls vorhanden
  if(crosshair) ScheduleCall(crosshair, "UpdateAngle", 1);
  //Granatenzahl im HUD neu berechnen
  if(pObj->~IsGrenade()) UpdateGrenadeCount();
  if(!pObj || Contained(pObj) != this) return;

  if(!pObj->~RejectOwnerChange())
    SetOwner(GetOwner(), pObj);

  //QuickInventar
  QuickInventoryStore(pObj);
}

public func Ejection(object pObj)
{
  RemoveEffect("SelectItem",pObj);
  UpdateGrenadeCount();

  var cObj = Contents();
  if(cObj && cObj->~SelectionTime())
  {
    AddEffect("SelectItem", cObj, 20, cObj->~SelectionTime(), 0, GetID());
    ScheduleCall(this, "CheckArmed", 1);
  }

  return _inherited(pObj,...);
}

public func ControlContents(id idTarget)
{
  //Durch Direktauswahl können wir nicht zuverlässig bestimmen, welches Objekt vorher da war
  //Daher einmal durchiterieren, und den Effekt überall zurücksetzen
  for(var i = 0; i < ContentsCount(); i++)
  {
    RemoveEffect("SelectItem",Contents(i));
  }

  var target = FindContents(idTarget);
  if(target)
  {
    if(target->~SelectionTime())
    {
      AddEffect("SelectItem",target,20,target->~SelectionTime(),0,GetID());
      ScheduleCall(this, "CheckArmed", 1);
      return false;
    }
  }
  return _inherited(idTarget);
}

protected func ContentsDestruction()      // Wenn Inhaltsobjekte verschwinden
{
  //Nach Waffe suchen
  ScheduleCall(this, "CheckArmed", 1);
  UpdateCharge();
  //Nächstes Objekt benachrichtigen
  Schedule("CheckContentsDestruction();", 1);
}

protected func CheckContentsDestruction()
{
  if(Contents()) Contents()->~Selection(this);
}

protected func ScalingLadder()
{
  Sound("ClonkStep*.ogg", 0, 0, 25);
  Sound("ClonkRustle*.ogg", 0, 0, 25);

  return Hangling(); //Funktional identisch
}

protected func GetObject2Drop(object pObj)
{
//if(GetProcedure() == "PUSH" && GetID(GetActionTarget()) == FKDT)
//{
    var dropobj, i;

    if(pObj->~IsWeapon())
    {
      //Hat die Waffe schon: Geht nicht
      if(FindContents(GetID(pObj)))
        return;

      //Pistole? Dann weg damit
      if(dropobj = FindContents(PSTL))
        return dropobj;

      //Sonst die hinterste Waffe
      for (i = 0; i < ContentsCount(); i++)
        if(Contents(i) && Contents(i)->~IsWeapon())
          dropobj = Contents(i);
      return dropobj;
    }

    //Granate?
    if(pObj->~IsGrenade())
    {
      //Hat schon genug Granaten
      if(GrenadeCount() >= MaxGrenades())
        return;
      //Granate in den Gürtel verfrachten
      pObj->~Activate(this);
      return;
    }

    //Pack? Versuchen, zusammenzulegen
    if(pObj->~IsPack())
    {
      var other = FindContents(GetID(pObj), this);
      if(other)
      {
        pObj->~JoinPack(other, this);
        return;
      }
    }

    //Objekt - hinterstes Objekt rauswerfen
    for (i = 0; i < ContentsCount(); i++)
      if(Contents(i) && !(Contents(i)->~IsWeapon()) && !Contents(i)->~IsGrenade())
        dropobj = Contents(i);
    return dropobj;
//}
  //return _inherited(pObj, ...);
}

/*----- Allgemeines -----*/

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
  var nade, i = ContentsCount(0, pGrenadeStoring)-1;
  while(i >= 0) {
    nade = Contents(i, pGrenadeStoring);
    i--;
    if(FindInArray4K(id_list,GetID(nade)) != -1) continue;
    id_list[GetLength(id_list)] = GetID(nade);

    AddMenuItem(Format("<c %x>%.2s</c>",nade->Color(),GetName(nade)), "GrabGrenade", GetID(nade), pCaller,
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

public func GrabGrenades(object pInto)
{
  for(var pGrenade in FindObjects(Find_Container(pGrenadeStoring)))
  {
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
  QuickInventoryStore(pGrenade);
  Enter(pGrenadeStoring, pGrenade);
  return(true);
}

public func UpdateGrenadeCount()
{
  iGrenadeCount = ObjectCount2(Find_Container(pGrenadeStoring)) + ObjectCount2(Find_Func("IsGrenade"),Find_Container(this));
  return true;
}

public func GrenadeCount(id type)
{
  if(!type) return iGrenadeCount;
  return ObjectCount2(Find_ID(type), Find_Container(pGrenadeStoring)) + ObjectCount2(Find_ID(type), Find_Container(this));
}

public func GetGrenadeStoring()
{
  if(!pGrenadeStoring)
    pGrenadeStoring = CreateObject(GRNS,0,0,GetOwner());
  return pGrenadeStoring;
}

/*----- Overloads -----*/

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
  //Das neue Item nach hinten verschieben (außer es ist Munition oder Granaten)
  if(!(pObj->~IsAmmoPacket()) || !(pObj->~IsGrenade()) || NoAmmo())
    if(1 == ContentsCount(GetID(pObj)))
      if(!(GetOCF(pObj) & OCF_Living))
      {
        ShiftContents(0,0,0,0);
        pObj ->~ OnDeselect();
      }
  UpdateCharge();
  return _inherited(pObj);
}

protected func RejectCollect(id idObj, object pObj)
{
  if(GetEffect("CannotBeCollected", pObj))
    return true;

  //Für die KI
  var effect;
  if(effect = GetEffect("CollectionException", pObj))
    if(EffectVar(0, pObj, effect) == this)
      return 1;
  //Spawnpunkt-Hack
  if(idObj == SPNP) return;
  //Munitionspaket?
  if(pObj ->~ IsAmmoPacket())
    // Davon kann man in jeden Fall _eines_ im Inventar haben
    if(!CustomContentsCount("IsAmmoPacket"))
      return;

  //Waffenaufsatz?
  if(FindContents(idObj, this) && pObj->~IsWeapon2() && pObj->~GetAttachment() != AT_NoAttachment && Contained(pObj))
  {
    var att = CreateObject(WNAT, 0, 0, GetOwner());
    if(Collect(att, this))
    {
      att->SetAttachment(pObj->GetAttachment());
      Sound("Merge.ogg");

      //Überschreibt die "Holen nicht möglich"-Nachricht
      Schedule(Format("PlayerMessage(%d, \"$AttachmentCollected$\", this)", GetOwner(), GetName(0, AttachmentIcon(pObj->GetAttachment()))), 1, 0, this);

      pObj->~SetAttachment(AT_NoAttachment);
    }
    else if(att)
      RemoveObject(att);

    return true;
  }

  //Waffenaufsatzobjekte erhalten einen zusätzlichen Inventarslot
  if(pObj->~IsAttachmentPack())
    if(!CustomContentsCount("IsAttachmentPack"))
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

  //Pack?
  if(pObj->~IsPack())
  {
    var other = FindContents(idObj);
    if(other)
    {
      pObj->~JoinPack(other, this);
      return true;
    }
  }

  //Wieviel haben wir denn schon im inventar?
  if(ContentsCount() - (CustomContentsCount("IsWeapon") + CustomContentsCount("IsGrenade") + CustomContentsCount("IsAttachmentPack")) >= ObjectCollectionLimit())
    return 1;

  // nicht angelegte Ausrüstung nochmal aufsammeln
  for(var gear in aGear)
    if(gear)
      if(GetID(gear) == idObj)
        return 1;

  // Ok
  return;
}

global func FxCannotBeCollectedStart(object target, int nr, temp, int time)
{
  if(!time)
    time = 30;

  //Maximale Dauer in Frames, die das Objekt nicht eingesammelt werden kann
  EffectVar(0, target, nr) = time;
}

global func FxCannotBeCollectedTimer(object target, int nr, int time)
{
  if(time >= EffectVar(0, target, nr))
    return -1;
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

public func RejectEntrance(object pIntoObj)
{
  if(pIntoObj->~IsBackDoor()) return pIntoObj->~RejectCollect(GetID(), this);
  return _inherited(pIntoObj, ...);
}

/*----- CMC Agilität -----*/

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
  var jnr = 1;
  if(GetPlrCoreJumpAndRunControl(GetController())) 
    if(GetComDir() & COMD_Left && GetComDir() & COMD_Right)
      jnr *= -1;
  SetXDir(jnr*42*((GetXDir()>0)*2-1) * jump / 100);
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
  Sound("ClonkStep*.ogg", 0, 0, 50);
  Sound("ClonkRustle*.ogg", 0, 0, 25);
  SetComDir(!GetComDir());
  ScheduleCall(0, "BackFlipBoost", 1, 1);
}

/* Agilität */

private func ControlAgility(string strControl)
{
  //Nich in Flüssigkeiten
  if(InLiquid())
    return;

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
        AddEffect("IntWallJump",this,10,15,this);
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
        AddEffect("IntWallJump",this,10,15,this);

    var iEff = AddEffect("ControlStack", this, 110, 5, this);
    EffectVar(0, this, iEff) = COMD_Right;
    return;
  }

  //Doppellinks
  if(strControl == "ControlLeftDouble")
    if(GetEffect("IntWallJump",this) && !GetContact(this))
    {
      SetAction("Jump");
      SetDir(DIR_Left);
      SetComDir(COMD_Left);
      WallJumpBoost();
      StopWallJump();

      //Effekte
      Sound("ClonkAction*.ogg");
      Sound("ClonkStep*.ogg", 0, 0, 50);
      Sound("ClonkRustle*.ogg", 0, 0, 25);
      return 1;
    }

  //Doppelrechts
  if(strControl == "ControlRightDouble")
    if(GetEffect("IntWallJump",this) && !GetContact(this))
    {
      SetAction("Jump");
      SetDir(DIR_Right);
      SetComDir(COMD_Right);
      WallJumpBoost();
      StopWallJump();

      //Effekte
      Sound("ClonkAction*.ogg");
      Sound("ClonkStep*.ogg", 0, 0, 50);
      Sound("ClonkRustle*.ogg", 0, 0, 25);
      return 1;
    }

  //Einmal links
  if(strControl == "ControlLeftSingle")
    if(IsJumping())
    {
      SetDir(DIR_Left);
      SetComDir(COMD_Left);
      return 1;
    }

  //Einmal rechts
  if(strControl == "ControlLeftSingle")
    if(IsJumping())
    {
      SetDir(DIR_Right);
      SetComDir(COMD_Right);
      return 1;
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

private func IsJumping()
{
  if(GetAction() == "Dive")
    return(true);
  return(WildcardMatch(GetAction(),"Jump*") || WildcardMatch(GetAction(),"JetpackFlight"));
}

/* Backflip starten */

public func JumpStart(bool bBackflip)
{
  CheckArmed();

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
    Sound("ClonkStep*.ogg", 0, 0, 25);
    Sound("ClonkRustle*.ogg", 0, 0, 25);
    ScheduleCall(0, "BackFlipBoost", 1, 1);
  }
}

public func FxControlStackEffect(string newEffect, object pTarget, int iNo)
{
  if(newEffect != "ControlStack") return;
  RemoveEffect(0, pTarget, iNo);
  return -2;
}

/* Kriechen */

static const CRAWL_AIM_Max = 50;
local crosshair;


/* Hinlegen */

global func StopFakeDeath(object pTarget)
{
  if(_inherited(pTarget))
  {
    if(pTarget->~CanCrawl() && !pTarget->~CanStandUp() && !GBackLiquid(GetX(pTarget), GetY(pTarget)))
    {
      pTarget->SetAction("Crawl");
    }
  }
     
  return true;
}

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
  {
    AddEffect("HasCrawled", this, 1, 1, this);
    return SetAction("StartCrawl");
  }

  if(IsAiming())
  {
    AddEffect("HasCrawled", this, 1, 1, this);
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

public func HasCrawled()
{
  return GetEffect("HasCrawled", this);
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

public func CanCrawl()
{
  return true;
}

protected func StartCrawl()
{
  //Wenn nötig: Effekt erzeugen 
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
    SetPosition(GetX()+dir,GetY()+8+3);
    while(i-- > 0)
    {
      if(!Stuck())
      {
        //SetPosition(GetX()-dir,GetY());
        break;
      }
      SetPosition(GetX()+dir,GetY());
    }

    if(Stuck()) AutoUnstuck(this,0,-10);

    //SetPosition(GetX()+(GetDir()*2-1)*-4,GetY()+8+5);
    SetAction("Scale");
    return 1;
  }
  if(IsCrawling())  return;
  // Shape und Vertices zurücksetzen
  RemoveEffect("Crawl", this);
  SetAction("FlatUp"); 
}

private func UpdateVertices()
{
  if(IsCrawling()) return;

  var x,y,r;
  WeaponAt(x, y, r);
  SetVertex(0, 0, x / 1000 * (GetDir() * 2 - 1), 0, 2);
  SetVertex(0, 1, y / 1000, 0, 2);
}

private func ResetVertices()
{
  if(IsCrawling()) return;

  SetVertex(0, 0, 0, 0, 2);
  SetVertex(0, 1, 0, 0, 2);
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

public func FxNoCrawlTimer()	{return -1;}

public func FxCrawlStart(object pClonk, int iNum)
{
  //Physical, Shape und Vertixes anpassen
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

public func FxCrawlStop(object pClonk, int iNum, int iReason)
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
  if(iReason == 4)
    AddEffect("HasCrawled", pClonk, 1, 1, this);
}

public func FxHasCrawledTimer(object pTarget)
{
  if(GetAlive(pTarget) && !pTarget->~IsCrawling() && GetAction(pTarget) != "FlatUp" && !WildcardMatch(GetAction(pTarget), "*Dead*"))
    return -1;

  return true;
}

/* Überladungen */

public func ControlThrow()
{
  if(GetAction() == "StartCrawl")
    return 1;
    
  if(IsCrawling() && IsArmed() && !ReadyToFire() && ReadyToAim())
  {
    StartAiming();
    return 1;
  }
  if(IsCrawling() && !IsAiming())
  {
    var obj = Contents();
    if(obj)
    {
      if(!obj->~CanAim())
      {
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
      else
      {
        StartAiming();
        return 1;
      }
    }
  }

  //Bei vorherigem Doppel-Stop nur Ablegen 
  if(GetPlrDownDouble(GetOwner()))
  {
    AddEffect("SquatAimTimeout", this(), 1, 15, this());
    if(Contents(0) && (Contents(0)->~IsWeapon() || Contents(0)->~LockInventory()))
    {
      return GetInvLockMode();
    }
    return(0);
  }

  ClearMacroCommands();
  SetAggroLevel(0,0,0,0,"ControlThrow");

  //Steuerung an gerittenes Objekt weitergeben
  if(IsRiding())
    if(GetActionTarget()->~ControlThrow(this()))
      return(1); 

  if(Control2Grab("ControlThrow"))	return(0);
  if(Control2Contents("ControlThrow"))	return(1);
  if(ControlLadder("ControlThrow"))	return(1);

  //Steuerung an Effekt weitergeben 
  if(Control2Effect("ControlThrow"))	return(1);

  //Reiten und Werfen
  if(IsRiding())
    if(Contents(0))
    {
      SetAction("RideThrow");
      return(1);
    }
  //Keine überladene Steuerung
  return(0);
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

public func ControlUpDouble()
{
  if(GetAction() == "Push" || IsRiding())
  {
    if(GetActionTarget()->~ControlUpDouble(this))
      return true;
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
  if(GetAction() == "AimCrawl") if(!IsArmed2()) return SetAction("Crawl");
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
  if(!crosshair) return;

  if(IsCrawling())
  {
    //Falls wir kriechen, Änderung auf Maximum beschränken
    iChange = BoundBy(Abs(crosshair->GetAngle()) + iChange, 90-CRAWL_AIM_Max, 90+CRAWL_AIM_Max) - Abs(crosshair->GetAngle());
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

/* Fallschirmsteuerung */

public func ControlLeft()
{
  var parachute = GetParachute();
  if(parachute)
    parachute->ControlLeft(...);

  return _inherited(...);
}

public func ControlLeftDouble()
{
  if(GetAction() == "Push" || IsRiding())
  {
    if(GetActionTarget() && GetActionTarget()->~ControlLeftDouble(this))
      return true;
  }
  return _inherited(...);
}

public func ControlRight()
{
  var parachute = GetParachute();
  if(parachute)
    parachute->ControlRight(...);

  return _inherited(...);
}

public func ControlRightDouble()
{
  if(GetAction() == "Push" || IsRiding())
  {
    if(GetActionTarget()->~ControlRightDouble(this))
      return true;
  }
  return _inherited(...);
}

public func ControlDown()
{
  var parachute = GetParachute();
  if(parachute)
    parachute->ControlDown(...);

  return _inherited(...);
}

public func ControlUpdate()
{
  var parachute = GetParachute();
  if(parachute)
    parachute->ControlUpdate(...);

  return _inherited(...);
}

global func GetParachute(object target) 
{ 
  if(!target && !(target = this))
    return false;

  return FindObject2(Find_ID(PARA), Find_ActionTarget(target));
}

protected func CheckStuck()
{
  //Verhindert Festhängen am Mittelvertex
  if(!GetXDir()) if(Abs(GetYDir()) < 5)
    if(GBackSolid(0, GetVertex(0,1,this())+1))
      SetPosition(GetX(), GetY() + 1);
}