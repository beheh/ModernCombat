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
public func UpdateCharge()			{return 1;}
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
    iBuyCooldown = 200; //Zahl x 5 = Framezahl
  }

  if(iBuyCooldown > 0)
    CreateParticle("PSpark",-4,-13,0,0,20,RGB(255,255,0),this);
  else
  {
    if(GetUser() && fMAVExistence)
      CreateParticle("PSpark",-4,-13,0,0,20,RGB(255,0,0),this);
    else
      CreateParticle("PSpark",-4,-13,0,0,20,RGB(0,255,0),this);
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
  if(pMAV)
    pMAV->Wait();

  //Aktion und Grafik setzen
  SetOwner(-1);  
  RemoveEffect("Light", this);
  ChangeDef(BDSN);
  this->~Initialize();

  //Pilot vorhanden? Auswerfen
  if(GetUser())
  {
    var pUser = GetUser();

    ObjectSetAction(pUser, "Walk");
    SetActionTargets(0, 0, pUser);
    pUser->SetHUDTarget(0);
    pUser->~ShowCH();
  }

  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",8,15,0,5,250,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastParticles("ConcreteSplinter", 8, 100, 0, 0, 40, 15, RGB(40, 20, 20));
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastParticles("ConcreteSplinter", 4, 100, 0, 0, 40, 15);
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
  if(iType == DMG_Projectile)	return 40;	//Projektile
  if(iType == DMG_Fire)		return 60;	//Feuer
  if(iType == DMG_Explosion)	return;		//Explosionen und Druckwellen
  if(iType == DMG_Energy)	return 50;	//Energiewaffen
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

    if(controller)
    {
      controller->SetHUDTarget(0);
      controller->~ShowCH();
      SetPlrView(GetOwner(controller), controller);
      Sound("CockpitRadio.ogg", true, 0, 100, GetOwner(controller)+1, -1);
    }

    //MAV deaktivieren
    if(pMAV)
      pMAV->Wait();

    //MAV-Station neutralisieren und auf Bereitschaft umstellen
    SetOwner(-1, this);
    controller = 0;
    SetAction("Ready");
    return;
  }

  //Pilot steuert nicht oder kein MAV? Abbrechen und MAV-Station auf Bereitschaft wechseln
  if(GetAction() == "Controlling" && (!pMAV || pMAV->IsDestroyed()))
  {
    SetPlrView(GetOwner(controller), controller);
    Sound("BKHK_SwitchFail.ogg", true, this, 100, GetOwner(controller) + 1);
    Sound("CockpitRadio.ogg", true, 0, 100, GetOwner(controller)+1, -1);
    SetAction("Ready");
  }

  //Funktionstüchtig? Ansonsten abbrechen
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
    CreateParticle("FlashLight", -2, -13, 0, 0 , 45, rgb, this);
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
    pMAV->Start();
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
    if(pMAV)
    {
      pMAV->Wait();
      Sound("BKHK_SwitchFail.ogg", true, this, 100, GetOwner(controller) + 1);
    }

    //MAV-Station neutralisieren und auf Bereitschaft umstellen
    SetOwner(-1, this);
    controller = 0;
    SetAction("Ready");

    return(1);
  }
  return(0);
}

/* Steuerung */

public func ControlLeft(object pByObj)
{
  if(pMAV)
    pMAV->ControlLeft(pByObj);
  return true;
}

public func ControlLeftDouble(object pByObj)
{
  if(pMAV)
    pMAV->ControlLeftDouble(pByObj);
  return true;
}

public func ControlLeftReleased(object pByObj)
{
  if(pMAV)
    pMAV->ControlLeftReleased(pByObj);
  return true;
}

public func ControlRight(object pByObj)
{
  if(pMAV)
    pMAV->ControlRight(pByObj);
  return true;
}

public func ControlRightDouble(object pByObj)
{
  if(pMAV)
    pMAV->ControlRightDouble(pByObj);
  return true;
}

public func ControlRightReleased(object pByObj)
{
  if(pMAV)
    pMAV->ControlRightReleased(pByObj);
  return true;
}

public func ControlDown(object pByObj)
{
  if(pMAV)
    pMAV->ControlDown(pByObj);
  return true;
}

public func ControlDownReleased(object pByObj)
{
  if(pMAV)
    pMAV->ControlDownReleased(pByObj);
  return true;
}

public func ControlDownDouble(object pByObj)
{
  if(pMAV)
    pMAV->ControlDownDouble(pByObj);
  return true;
}

protected func ControlUp(object pByObj)
{
  if(pMAV)
    pMAV->ControlUp(pByObj);
  return true;
}

public func ControlUpReleased(object pByObj)
{
  if(pMAV)
    pMAV->ControlUpReleased(pByObj);
  return true;
}

protected func ControlUpDouble(object pByObj)
{
  if(pMAV)
    pMAV->ControlUpDouble(pByObj);
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

  //MAV deaktivieren
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
    //Abbruch wenn kauf gerade nicht möglich
    if(iBuyCooldown > 0)
      return true;

    //Genug Geld? Abziehen und MAV erstellen
    if(GetWealth(GetOwner(pByObj)) >= GetDefCoreVal("Value", "DefCore", MAVE))
    {
      //Geld einziehen
      DoWealth(GetOwner(pByObj), -GetDefCoreVal("Value", "DefCore", MAVE));

      //MAV erstellen
      pMAV = CreateObject(MAVE,iXSpawnOff,iYSpawnOff,GetOwner(this));
      pByObj->SetHUDTarget(pMAV->GetAttWeapon());
      pMAV->Start();
      SpawnEffect(pMAV);
      fMAVExistence = true;
      Sound("BKHK_Switch.ogg", true, this, 100, GetOwner(pByObj) + 1);
      Sound("CockpitRadio.ogg", true, 0, 100, GetOwner(pByObj)+1, +1);
      SetAction("Controlling");
    }
    else
      PlayerMessage(GetOwner(pByObj), "$NoMoney$", this);
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
    pMAV->ControlThrow(pByObj);
  }

  return true;
}