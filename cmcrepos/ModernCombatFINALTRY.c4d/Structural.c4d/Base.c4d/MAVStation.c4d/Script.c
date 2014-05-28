/*-- MAV Station --*/
 
#strict 2
#include CSTR
 
local crosshair;
local controller;
local pMAV, fMAVExistence;
local iXSpawnOff;
local iYSpawnOff;
local iBuyCooldown;

public func RemoveTracer()			{return IsDestroyed();}		//Tracer entfernen, wenn zerstört
public func DisableCH()				{return true;}			//Eventuelles Fadenkreuz des Clonks ausblenden
public func MaxDamage()				{return 150;}
public func IsMachine()				{return true;}
public func IsBulletTarget()			{return false;}
public func IsThreat()				{return !IsDestroyed();}
public func UpdateCharge()			{return (pMAV && GetAction(pMAV) == "Flying" && !pMAV->~IsDestroyed());}
public func AttractTracer(object pTracer)	{return GetPlayerTeam(GetController(pTracer)) != GetTeam() && !IsDestroyed();}
public func IsStill()				{return true;}
public func IsAiming()				{return true;}
public func StartRepair()			{return true;}
protected func RejectContents()			{return true;}
public func BonusPointCondition()		{return false;}
public func VaryingDir()			{return 3;}


/* Initialisierung */

public func Initialize() 
{
  _inherited();

  //Kauf-Cooldown zurücksetzen
  iBuyCooldown = 0;
  AddEffect("Light", this, 1, 5, this);
  SetAction("Ready");
}

public func FxLightTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  iBuyCooldown--;

  if(fMAVExistence && (!pMAV || pMAV->IsDestroyed()))
  {
    fMAVExistence = false;
    iBuyCooldown = 250; //Zahl x 5 = Framezahl
    SetAction("Disabled");
  }

  if(iBuyCooldown > 0)
    CreateParticle("PSpark",-4,-13,0,0,30,RGB(255,255,0),this);
  else
  {
    if(GetAction() == "Disabled")
      SetAction("Ready");
    if(GetUser() && fMAVExistence)
      CreateParticle("PSpark",-4,-13,0,0,30,RGB(255,0,0),this);
    else
      CreateParticle("PSpark",-4,-13,0,0,30,RGB(0,255,0),this);
  }
}

/* Spawnkoordinaten festlegen */

public func Set(int iX, int iY, bool fGlobal)
{
  iXSpawnOff = iX - GetX()*fGlobal;
  iYSpawnOff = iY - GetY()*fGlobal;
}

/* Zerstörung */

public func OnDestruction()
{
  //MAV deaktivieren
  if(pMAV && !pMAV->IsDestroyed())
    pMAV->Wait();

  //Aktion und Grafik setzen
  SetOwner(-1);  
  RemoveEffect("Light", this);
  ChangeDef(BDSN);
  this->~Initialize();

  //Pilot vorhanden? Auswerfen
  if(this->GetUser())
  {
    var pUser = this->GetUser();

    ObjectSetAction(pUser, "Walk");
    SetActionTargets(0, 0, pUser);
    pUser->SetHUDTarget(0);
    pUser->~ShowCH();
  }

  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",8,15,0,5,250,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastParticles("ConcreteSplinter", 8, 100, 0, 0, 40, 15, RGB(40, 20, 20));
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastParticles("ConcreteSplinter", 4, 100, 0, 0, 40, 15);
  Sound("StructureHeavyHit*.ogg");
}

public func OnRepair()
{
  //Aktion und Grafik setzen
  SetAction("Ready");
  SetGraphics();
}

/* Schaden */

public func OnDmg(int iDmg, int iType)
{
  if(iType == DMG_Fire)		return 60;	//Feuer
  if(iType == DMG_Explosion)	return;		//Explosionen und Druckwellen
  if(iType == DMG_Bio)		return 100;	//Säure und biologische Schadstoffe

  return 50;
}

/* Aktivität */

public func FxActivityTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  //Kein Pilot?
  if(!GetUser()) 
  {
    //Aktivität einstellen
    RemoveEffect("Activity", this);

    if(controller && GetOCF(controller) & OCF_Alive)
    {
      controller->SetHUDTarget(0);
      controller->~ShowCH();
      SetPlrView(GetOwner(controller), controller);
      Sound("CockpitRadio.ogg", true, 0, 100, GetOwner(controller)+1, -1);
    }

    //MAV deaktivieren
    if(pMAV && !pMAV->IsDestroyed())
    {
      pMAV->Wait();
      SetAction("Ready");
    }
    else
      SetAction("Disabled");

    //MAV-Station neutralisieren und auf Bereitschaft umstellen
    SetOwner(-1, this);
    controller = 0;
    return;
  }

  //Pilot steuert nicht oder kein MAV: Abbrechen und MAV-Station auf Bereitschaft wechseln
  if(GetAction() == "Controlling" && (!pMAV || pMAV->IsDestroyed()))
  {
    SetPlrView(GetOwner(controller), controller);
    Sound("BKHK_SwitchFail.ogg", true, this, 100, GetOwner(controller) + 1);
    Sound("CockpitRadio.ogg", true, 0, 100, GetOwner(controller)+1, -1);
    SetAction("Disabled");
  }

  //Nicht funktionstüchtig: Abbrechen
  if(EMPShocked()) return;
  if(IsDestroyed()) return;
  if(IsRepairing()) return;

  //Blinklicht (alle 30 Frames)
  if(!(iEffectTime%30))
  {
    if(GetTeam())
      var rgb = GetTeamColor(GetTeam());
    else if(GetOwner())
      var rgb = GetPlrColorDw(GetOwner());
    else
      var rgb = RGB(255, 255, 255);
    CreateParticle("FlashLight", -2, -13, 0, 0 , 60, rgb, this);
  }

  //HUD aktualisieren
  var User = GetUser();
  if(User)
  {
    var UserHUD = User->GetHUD();
    
    if(pMAV && pMAV->GetAction() == "Flying")
    {
      SetPlrView(GetController(), pMAV);
      if(UserHUD)
        UserHUD->Update(pMAV->GetAttWeapon(), User->AmmoStoring(),User);
    }
  }
}

/* EMP */

public func EMPShock()
{
  EMPShockEffect(20*35);
  return 1;
}

/* Eingangssteuerung */

protected func ActivateEntrance(pUser)
{
  //Nicht wenn bereits besetzt
  if(GetUser())
    return(0);

  //Nur lebende Clonks
  if(!pUser->~IsClonk())
    return(0);

  //Nur, wenn das Objekt noch steht
  if(IsDestroyed())
    return(0);

  ObjectSetAction(pUser, "RideStill", this());

  //Clonkposition anpassen
  SetOwner(GetOwner(pUser));
  controller = pUser;
  pUser->HideCH();

  //Besitzer aktualisieren
  if(pMAV && !pMAV->IsDestroyed())
  {
    if(Hostile(GetOwner(this), pMAV->GetOwner())) pMAV->Reload();
    pMAV->SetOwner(GetOwner(this));
    pMAV->Start(this);
    Sound("BKHK_Switch.ogg", true, this, 100, GetOwner(pUser) + 1);
    Sound("CockpitRadio.ogg", true, 0, 100, GetOwner(pUser)+1, +1);
    SetAction("Controlling");
  }

  Sound("StructureEnter*.ogg", true, this, 100, GetOwner(pUser) + 1);

  //Aktivität per Effekt starten
  if(!GetEffect("Activity",this))
    AddEffect("Activity", this, 1, 1 ,this);

  return(1);
}

public func GetUser()
{
  var pUser;
  if(pUser = FindObject2(Find_OCF(OCF_CrewMember), Find_ActionTarget(this), Find_Func("IsRiding")))
    return(pUser);
  else
    return pUser = 0;
}

private func ExitClonk(object pByObj)
{
  //Nutzer auswerfen
  if(pByObj == GetUser())
  {
    //Clonk positionieren und HUD reaktivieren
    ObjectSetAction(pByObj, "Walk"); 
    SetActionTargets(0, 0, pByObj); 
    pByObj->SetHUDTarget(0);
    controller->~ShowCH();
    SetPlrView(GetOwner(controller), controller);
    Sound("CockpitRadio.ogg", true, 0, 100, GetOwner(controller)+1, -1);
    Sound("StructureLeave*.ogg", true, this, 100, GetOwner(controller) + 1);

    //MAV vorhanden? Deaktivieren
    if(pMAV && !pMAV->IsDestroyed())
    {
      pMAV->Wait();
      Sound("BKHK_SwitchFail.ogg", true, this, 100, GetOwner(controller) + 1);
      SetAction("Ready");
    }
    else
      SetAction("Disabled");

    //MAV-Station neutralisieren und auf Bereitschaft umstellen
    SetOwner(-1, this);
    controller = 0;

    return(1);
  }
  return(0);
}

/* Steuerung */

public func ControlLeft(object pByObj)
{
  if(pMAV && !pMAV->IsDestroyed())
    pMAV->~ControlLeft(pByObj);
  return true;
}

public func ControlLeftDouble(object pByObj)
{
  if(pMAV && !pMAV->IsDestroyed())
    pMAV->~ControlLeftDouble(pByObj);
  return true;
}

public func ControlLeftReleased(object pByObj)
{
  if(pMAV && !pMAV->IsDestroyed())
    pMAV->~ControlLeftReleased(pByObj);
  return true;
}

public func ControlRight(object pByObj)
{
  if(pMAV && !pMAV->IsDestroyed())
    pMAV->~ControlRight(pByObj);
  return true;
}

public func ControlRightDouble(object pByObj)
{
  if(pMAV && !pMAV->IsDestroyed())
    pMAV->~ControlRightDouble(pByObj);
  return true;
}

public func ControlRightReleased(object pByObj)
{
  if(pMAV && !pMAV->IsDestroyed())
    pMAV->~ControlRightReleased(pByObj);
  return true;
}

public func ControlDown(object pByObj)
{
  if(pMAV && !pMAV->IsDestroyed())
    pMAV->~ControlDown(pByObj);
  return true;
}

public func ControlDownReleased(object pByObj)
{
  if(pMAV && !pMAV->IsDestroyed())
    pMAV->~ControlDownReleased(pByObj);
  return true;
}

public func ControlDownDouble(object pByObj)
{
  if(pMAV && !pMAV->IsDestroyed())
    pMAV->~ControlDownDouble(pByObj);
  return true;
}

protected func ControlUp(object pByObj)
{
  if(pMAV && !pMAV->IsDestroyed())
    pMAV->~ControlUp(pByObj);
  return true;
}

public func ControlUpReleased(object pByObj)
{
  if(pMAV && !pMAV->IsDestroyed())
    pMAV->~ControlUpReleased(pByObj);
  return true;
}

protected func ControlUpDouble(object pByObj)
{
  if(pMAV && !pMAV->IsDestroyed())
    pMAV->~ControlUpDouble(pByObj);
  return true;
}

public func ControlDigSingle(object pByObj)
{
  //Kein MAV: Abbrechen
  if(!pMAV || pMAV->IsDestroyed())
    return true;

  //MAV zielt? Einstellen
  if(pMAV->IsAiming())
  {
    pMAV->EndAim();
    return true;
  }

  //MAV (de-)aktivieren
  if(pMAV->GetAction() != "Flying")
  {
    pMAV->Start(this);
    Sound("BKHK_Switch.ogg", true, this, 100, GetOwner(pByObj) + 1);
    Sound("CockpitRadio.ogg", true, 0, 100, GetOwner(pByObj)+1, +1);
    SetAction("Controlling");
    return true;
  }

  pMAV->Wait();

  //Sicht zurücksetzen und Station auf Bereitschaft umstellen
  SetPlrView(GetOwner(controller), controller);
  Sound("CockpitRadio.ogg", true, 0, 100, GetOwner(pByObj)+1, -1);
  SetAction("Ready");
  Sound("BKHK_SwitchFail.ogg", true, this, 100, GetOwner(pByObj) + 1);
  return true;
}

protected func ControlDigDouble(object pByObj)
{
  //Clonk aussteigen lassen
  ExitClonk(pByObj);
  return true;
}

protected func ControlThrow(object pByObj)
{
  //Kein MAV? Kaufversuch starten
  if(!fMAVExistence && (!pMAV || pMAV->IsDestroyed()))
  {
    //Abbruch wenn Kauf gerade nicht möglich
    if(iBuyCooldown > 0)
    {
      PlayerMessage(GetOwner(pByObj), "$NoMAV$", this);
      return true;
    }

    //Genug Geld? Abziehen und MAV erstellen
    if(GetWealth(GetOwner(pByObj)) >= GetDefCoreVal("Value", "DefCore", MAVE))
    {
      //Geld einziehen
      DoWealth(GetOwner(pByObj), -GetDefCoreVal("Value", "DefCore", MAVE));

      //MAV erstellen
      pMAV = CreateObject(MAVE,iXSpawnOff,iYSpawnOff,GetOwner(this));
      pByObj->SetHUDTarget(pMAV->GetAttWeapon());
      pMAV->Start(this);
      SpawnEffect(pMAV);
      fMAVExistence = true;
      Sound("BKHK_Switch.ogg", true, this, 100, GetOwner(pByObj) + 1);
      Sound("CockpitRadio.ogg", true, 0, 100, GetOwner(pByObj)+1, +1);
      SetAction("Controlling");

      DirectEquipMenu(pByObj);
    }
    else
    {
      Sound("BKHK_SwitchFail.ogg", false, pByObj, 100, GetOwner(pByObj)+1);
      PlayerMessage(GetOwner(pByObj), Format("$NotEnoughMoney$",GetDefCoreVal("Value", "DefCore", MAVE)), this);
    }
  }
  else
  {
    //MAV-Kontrolle starten
    if(pMAV->GetAction() != "Flying")
    {
      Sound("BKHK_Switch.ogg", true, this, 100, GetOwner(pByObj) + 1);
      Sound("CockpitRadio.ogg", true, 0, 100, GetOwner(pByObj)+1, +1);
      SetAction("Controlling");
    }
    pMAV->~ControlThrow(pByObj);
  }

  return true;
}

/* Modifikationsmenü */

func AddItem(id iItem, object pUser)
{
  var pItem = FindContents(iItem, pUser);
  if(iItem == C4EX) pItem = FindContents(C4PA, pUser);

  if(pMAV && !pMAV->IsDestroyed() && !LocalN("pItem", pMAV) && pItem && (pUser == GetUser()))
  {
    var iType;
    if(GetID(pItem) == AMPK) iType = 1;
    if(GetID(pItem) == FAPK) iType = 2;
    if(GetID(pItem) == BWTH) iType = 3;
    if(GetID(pItem) == RSHL) iType = 4;
    if(GetID(pItem) == BTBG) iType = 5;
    if(GetID(pItem) == CDBT) iType = 6;
    if(GetID(pItem) == C4PA) iType = 7;

    if(iType == 5)
    {
      pItem->DoPackPoints(-1);
      if(!pItem->GetPackPoints())
        RemoveObject(pItem);
      pItem = CreateObject(BTBG);
      pItem->SetPackPoints(1);
    }

    if(iType == 7 && pItem->GetPackPoints() && !pMAV->RejectC4Attach())
    {
      pMAV->AttachC4(pItem);
      CustomMessage(Format("$Updated$", GetName(0, C4EX)), pMAV, GetOwner(GetUser()));
      DirectEquipMenu(pUser, true);

      return true;
    }

    Enter(pMAV, pItem);
    LocalN("pItem", pMAV) = pItem;
    LocalN("iItemType", pMAV) = iType;
    LocalN("iHKShots", pMAV) = 5;
    SetPhase(iType, pMAV);
    pMAV->Sound("RSHL_Deploy.ogg");

    //Hilfsnachrichten
    var szStr = Format("$Updated$", GetName(pItem));
    CustomMessage(szStr, pMAV, GetOwner(GetUser()));

    if(GetAction() == "Flying")
    {
      CustomMessage(szStr, pMAV, GetOwner(), 0, 0, 0, 0, 0, MSG_Multiple);

      for(var i = 0; i < 2; i++)
      {
        if(iType == 1) ScheduleCall(pMAV, "AMP", 1, 0, true);
        if(iType == 2) ScheduleCall(pMAV, "FAP", 1, 0, true, 0);
        if(iType == 3) ScheduleCall(pMAV, "BlowTorch", 1, 0, true);
      }
    }
  }
}

func DummyFunc()
{
  //Nichts unternehmen; diese Func existiert für die Option "Nichts" im Menü
}

func DirectEquipMenu(object pByObj, bool fChooseC4)
{
  CreateMenu(MAVE, pByObj, this, 0, "$EquipMAV$", 0, C4MN_Style_Context);

  var fItemExists = false;

  AddMenuItem(Format("$AddItem$", "$Nothing$"), "DummyFunc", SM06, pByObj);
  if(FindContents(AMPK, pByObj)) {fItemExists++; AddMenuItem(Format("$AddItem$", GetName(0, AMPK)), "AddItem", AMPK, pByObj, 0, pByObj);}
  if(FindContents(FAPK, pByObj)) {fItemExists++; AddMenuItem(Format("$AddItem$", GetName(0, FAPK)), "AddItem", FAPK, pByObj, 0, pByObj);}
  if(FindContents(BWTH, pByObj)) {fItemExists++; AddMenuItem(Format("$AddItem$", GetName(0, BWTH)), "AddItem", BWTH, pByObj, 0, pByObj);}
  if(FindContents(RSHL, pByObj)) {fItemExists++; AddMenuItem(Format("$AddItem$", GetName(0, RSHL)), "AddItem", RSHL, pByObj, 0, pByObj);}
  if(FindContents(BTBG, pByObj)) {fItemExists++; AddMenuItem(Format("$AddItem$", GetName(0, BTBG)), "AddItem", BTBG, pByObj, 0, pByObj);}
  if(FindContents(CDBT, pByObj)) {fItemExists++; AddMenuItem(Format("$AddItem$", GetName(0, CDBT)), "AddItem", CDBT, pByObj, 0, pByObj);}

  var detonator = FindContents(C4PA, pByObj);
  if(detonator && detonator->GetPackPoints() && !pMAV->RejectC4Attach())
  {
    fItemExists++;
    AddMenuItem(Format("$AddItem$", GetName(0, C4EX)), "AddItem", C4EX, pByObj, 0, pByObj);
    if(fChooseC4)
      SelectMenuItem (fItemExists, pByObj);
  }

  if(!fItemExists)
    CloseMenu(pByObj);
}
