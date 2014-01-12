/*-- Raketenwerfer --*/

#strict 2

#include WPN2

public func HandSize()		{return 850;}
public func HandX()		{return 1000;}
public func HandY()		{return -2500;}
public func BarrelXOffset()	{return -3500;}
public func IsSecondaryWeapon()	{return true;}

public func SelectionTime()	{return 45;}	//Anwahlzeit

local pRocket;


/* Kompatible Waffenaufsätze */

func PermittedAtts()
{
  return AT_Laserpointer;
}

/* Raketen */

public func FMData1(int data)
{
  if(data == FM_Name)		return "$Missiles$";

  if(data == FM_AmmoID)		return MIAM;	//ID der Munition
  if(data == FM_AmmoLoad)	return 1;	//Magazingröße

  if(data == FM_Reload)		return 180;	//Zeit für Nachladen
  if(data == FM_Recharge)	return 200;	//Zeit bis erneut geschossen werden kann

  if(data == FM_Aim)		return 1;	//Waffe kann nur zielend abgefeuert werden

  if(data == FM_Slot)		return 1;	//Slot des Feuermodus

  if(data == FM_SpreadAdd)	return 300;	//Bei jedem Schuss hinzuzuaddierende Streuung

  return Default(data);
}

/* Raketen - Optische Steuerung */

public func FMData1T1(int data)
{
  if(data == FT_Name)		return "$Optical$";

  return FMData1(data);
}

public func Fire1T1()
{
  Fire1();
}

public func BotData1(int data)
{
  if(data == BOT_Range)		return 600;
  if(data == BOT_DmgType)	return DMG_Explosion;
  if(data == BOT_Power)		return(BOT_Power_LongLoad);
  if(data == BOT_Precision)	return 3;
  return Default(data);
}

public func Fire1()
{
  LaunchRocket(MISL,Contained()->~AimAngle(10));
}

/* Raketen - Ungelenkt/Lastersteuerung */

public func FMData1T2(int data)
{
  if(data == FT_Name)
    if(GetAttachment() != AT_Laserpointer)
      return "$Unguided$";
    else
      return "$Laser$";

  return FMData1(data);
}

public func Fire1T2()
{
  if(GetAttachment() != AT_Laserpointer)
    LaunchRocket(MISL,Contained()->~AimAngle(10), true);
  else
    LaunchRocket(MISL,Contained()->~AimAngle(10), false, true);
}

/* Raketen - Schuss */

public func LaunchRocket(id rid, int angle, bool unguided, bool fLaserGuided)
{
  //Austritt bestimmen
  var user = GetUser();
  var x,y;
  user->WeaponEnd(x,y);

  //Rakete abfeuern
  var rocket = CreateObject(rid,x,y+10,GetController(user));
  rocket->Launch(angle, user, unguided, fLaserGuided);
  Sound("RTLR_Launch*.ogg", 0, rocket);
  SetController(GetController(user), rocket);

  //Sicht auf Rakete
  if(!unguided) SetPlrView(GetController(user), rocket);
  pRocket = rocket;

  //Effekte
  var ax, ay, xdir, ydir;
  user->WeaponBegin(ax,ay);
  xdir = ax-x;
  ydir = ay-y;

  if(GetEffectData(EFSM_ExplosionEffects) > 1) Sparks(5,RGB(255,128),ax-x,ay-y);
  if(GetEffectData(EFSM_ExplosionEffects) > 0)
  {
    CreateParticle("Thrust",ax,ay,xdir/2,ydir/2,80,RGBa(255,200,200,0),0,0);

    for(var i=0; i<20; ++i)
    {
      var rand = RandomX(-10,+10);
      CreateParticle("Smoke2",ax+Sin(angle,rand),ay-Cos(angle,rand),
      		RandomX(0,2*xdir),RandomX(0,2*ydir),
      		RandomX(80,140),RGBa(220,200,180,0),0,0);

      var rand = RandomX(-10,+10);
      CreateParticle("BlastSpark1",ax+Sin(angle,rand),ay-Cos(angle,rand),
      		RandomX(0,2*xdir),RandomX(0,2*ydir),
      		RandomX(40,70),RGBa(220,200,180,0),0,0);
    }
  }
  Echo("RTLR_Echo.ogg");
}

public func AimAngleChange(bool fJNR)
{
  //Schnelleres Zielen
  if(fJNR)
    return 4;
}

/* Raketenverfolgung */

private func Check()
{
  if(!pRocket || !Contained() || Contents(0, Contained()) != this || !Contained()->~IsClonk() || !pRocket->Guideable()) return;

  //Sicht auf existierende Rakete setzen
  if(Contained()->~IsAiming())
    if(pRocket)
      SetPlrView(GetOwner(Contained()), pRocket);
}

public func RejectSwitch()
{
  //Sicht wegnehmen
  if (!_inherited(...) && pRocket)
    SetPlrView(GetOwner(Contained()), Contained());
}

/* KI-Steuerung */

public func AI_NeedControl(object pBot, object pTarget)
{
  if(GetFireTec() == 2)
    return;

  if(pBot)
    pBot->DoMouseAiming(GetX(pBot) + (GetX(pTarget) - GetX(pRocket)), GetY(pBot) + (GetY(pTarget) - GetY(pRocket)), AimAngleChange(true));

  return pRocket && Distance(GetX(), GetY(), GetX(pRocket), GetY(pRocket)) < BotData1(BOT_Range);
}

public func AI_NeedAim(object pBot, object pTarget)
{
  return GetFireTec() != 2 && pRocket && Distance(GetX(), GetY(), GetX(pRocket), GetY(pRocket)) < BotData1(BOT_Range);
}

public func AI_IgnorePathFree(object pBot, object pTarget)
{
  return GetFireTec() != 2 && PathFree(GetX(pRocket), GetY(pRocket), GetX(pTarget), GetY(pTarget));
}

/* Laserpointer */

func FxLaserDotTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  if(aFM_FireTec[0] != 2 || IsReloading())
  {
    if(pBeam) RemoveObject(pBeam);
    if(pLaser) RemoveObject(pLaser);
    return;
  }
  else return _inherited(...);
}

/* Handeffekt */

public func HandR()
{
  var effect = IsReloading();
  if(effect)
    return Max(Sin(GetEffect(0,this,effect,6)*90/80,20),0);
  return;
}

/* Allgemein */

public func OnReload()
{
  Sound("RTLR_Reload.ogg");
}

public func OnSelect()
{
  Sound("RTLR_Charge.ogg");
}

/*----- Werfen -----*/

public func ControlThrow(caller)
{
  //Nutzer ist Schütze
  SetUser(caller);

  //Unterstützt der Schussmodus das zielen aber es wird nicht gezielt?
  if(GetFMData(FM_Aim)>0 && !(GetUser()->~IsAiming()) && !(GetUser()->~AimOverride()))
  {
    //Bereit zum Zielen?
    if(GetUser()->~ReadyToAim())
      //Zielen starten
      GetUser()->StartAiming();
    return 1;
  }

  //Feuern! Fehlgeschlagen?
  if(!Fire())
  {
    var ammoid = GetFMData(FM_AmmoID);
    var ammousage = GetFMData(FM_AmmoUsage);
    //Nachladen wenn möglich sofern Munition verbraucht
    if(pRocket && GetAction(pRocket) != "Fall" && !pRocket->IsDamaged() && pRocket->Guideable())
    {
    	if(pRocket && Contained() && Contents(0, Contained()) == this && Contained()->~IsClonk() && Contained()->~IsAiming() && GetEffect("Follow", pRocket))
    	{
    		pRocket->StartChasing();
    	}
		}
		else
		{
    	if(!CheckAmmo(ammoid,ammousage,this))
      	if(CheckAmmo(ammoid,ammousage,GetUser()))
      	{
        	Reload();
      	}
      	//Nicht genügend Munition
      	else
      	{
        	//Unmöglichkeit des Nachladens angeben
        	PlayerMessage(GetOwner(caller), "$NotEnoughAmmo$", caller, ammoid);
        	Sound("WPN2_Empty.ogg");
      	}
    }
  }

  return 1;
}
