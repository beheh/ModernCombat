/*-- MAV Station --*/
 
#strict 2
#include CSTR
 
local crosshair;
local controller;
local pMav;
 
public func ReadyToFire()			{return 1;}			//Ständige Feuerbereitschaft
public func RemoveTracer()			{return IsDestroyed();}		//Tracer entfernen, wenn zerstört
public func DisableCH()				{return true;}			//Eventuelles Fadenkreuz des Clonks ausblenden
public func MaxDamage()				{return 100;}
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


/* Initialisierung */

public func Initialize() 
{
  SetAction("Ready");

  //Standardwerte setzen

  return _inherited();
}


/* Zerstörung */

public func OnDestruction()
{

  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",8,15,0,5,250,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastParticles("ConcreteSplinter", 8, 100, 0, 0, 40, 15, RGB(40, 20, 20));
  CastParticles("Sandbag", 10, 70, 0,0, 35, 45, RGBa(228,228,228,0), RGBa(250,250,250,50));
}

public func Destruction()
{

  if(GetUser())
  {
    var pUser = GetUser();

    ObjectSetAction(pUser, "Walk");
    SetActionTargets(0, 0, pUser);
    pUser->SetHUDTarget(0);
    pUser->~ShowCH();
  }
}

public func OnRepair()
{
  SetAction("Ready");
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

public func FxActivityTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  if(!GetUser()) 
  {
    RemoveEffect("Activity", this);

    if(controller)
    {
      controller->SetHUDTarget(0);
      controller->~ShowCH();
      SetPlrView(GetOwner(controller), controller);
    }

    SetOwner(-1, this);
    controller=-1;
    return;
  }

  //Funktionstüchtig?
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
    CreateParticle("FlashLight", 0, 4, 0, 0 , 45, rgb, this);
  }

  //HUD aktualisieren
  var User = GetUser();
  if(User)
  {
    var UserHUD = User->GetHUD();
    if(UserHUD)
      UserHUD->Update(this, User->AmmoStoring(),User);
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

  //Sound
  Sound("RSHL_Deploy.ogg", true, this, 100, GetOwner(pUser) + 1);


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
    pUser=0;
}

private func ExitClonk(object pByObject)
{
  //Nutzer auswerfen
  if(pByObject == GetUser())
  { 
    ObjectSetAction(pByObject, "Walk"); 
    SetActionTargets(0, 0, pByObject); 
    pByObject->SetHUDTarget(0);
    controller->~ShowCH();
    
    if(pMav)
    {
  		pMav->SetAction("Idle");
  		pMav->Idle(pByObject);
  	}

    SetOwner(-1, this);
  		
    return(1); 
  }
  return(0);
}

/* Steuerung */

public func ControlLeft(object pByObj)
{
	if(pMav)
		pMav->ControlLeft(pByObj);
  return true;
}

public func ControlLeftDouble(object pByObj)
{
	if(pMav)
		pMav->ControlLeftDouble(pByObj);
  return true;
}

public func ControlLeftReleased(object pByObj)
{
	if(pMav)
		pMav->ControlLeftReleased(pByObj);
  return true;
}

public func ControlRight(object pByObj)
{
	if(pMav)
		pMav->ControlRight(pByObj);
  return true;
}

public func ControlRightDouble(object pByObj)
{
	if(pMav)
		pMav->ControlRightDouble(pByObj);
  return true;
}

public func ControlRightReleased(object pByObj)
{
	if(pMav)
		pMav->ControlRightReleased(pByObj);
  return true;
}

public func ControlDown(object pByObj)
{
	if(pMav)
		pMav->ControlDown(pByObj);
  return true;
}

public func ControlDownDouble(object pByObj)
{
	if(pMav)
		pMav->ControlDownDouble(pByObj);
  return true;
}

protected func ControlUp(object pByObj)
{
	if(pMav)
		pMav->ControlUp(pByObj);
  return true;
}

protected func ControlUpDouble(object pByObj)
{
	if(pMav)
		pMav->ControlUpDouble(pByObj);
  return true;
}

public func ControlDigSingle(object pByObj)
{
	if(!pMav)
		return;

	if(pMav->GetAction() == "Idle")
	{
  	if (!pMav->IsDestroyed())
  		pMav->SetAction("Flying");
  }
  else
  {
  	pMav->SetAction("Idle");
  	pMav->Idle();
  }

  return true;
}

protected func ControlDigDouble(object pByObj)
{
  ExitClonk(pByObj);
  return true;
}

protected func ControlThrow(object byObj)
{
	if(!pMav || pMav->IsDestroyed())
  	pMav = CreateObject(MAVE,0,0,GetOwner(this));
  	
  pMav->SetAction("Flying");
  	
  
  return true;
  
  
}

public func ControlUpdate(object byObj, int comdir, bool dig, bool throw) 
{
  if(throw)
    return ControlThrow(byObj);
  else
    return true;
}