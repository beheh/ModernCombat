/*-- Schrotflinte --*/

#strict 2
#include WPN2

local casings;

public func HandSize()		{return 1000;}
public func HandX()		{return 5000;}
public func HandY()		{return -500;}
public func BarrelYOffset()	{return -2000;}
public func IsPrimaryWeapon()	{return true;}

public func SelectionTime()	{return 20;}	//Anwahlzeit


/* Kompatible Waffenaufsätze */

func PermittedAtts()
{
  return AT_ExtendedMag | AT_Bayonet | AT_Laserpointer | AT_Flashlight;
}

/* Nahkampfangriff */

public func GetMCData(int data)
{
  if(data == MC_CanStrike)	return 1;					//Waffe kann Kolbenschlag ausführen
  if(data == MC_Damage)		return 20 + (iAttachment == AT_Bayonet)*6;	//Schaden eines Kolbenschlages
  if(data == MC_Recharge)	return 40 + (iAttachment == AT_Bayonet)*10;	//Zeit nach Kolbenschlag bis erneut geschlagen oder gefeuert werden kann
  if(data == MC_Power)		return 20;					//Wie weit das Ziel durch Kolbenschläge geschleudert wird
  if(data == MC_Angle)		return 45;					//Mit welchem Winkel das Ziel durch Kolbenschläge geschleudert wird
}

/* Kugeln */

public func FMData1(int data)
{
  if(data == FM_Name)		return "$Pellets$";

  if(data == FM_AmmoID)		return STAM;						//ID der Munition
  if(data == FM_AmmoLoad)	return 20 + (iAttachment == AT_ExtendedMag)*8;		//Magazingröße
  if(data == FM_AmmoUsage)	return 4;						//Munition pro Schuss

  if(data == FM_SingleReload)	return 2;						//Zeit des einzelnen Nachladens bei Revolversystemen
  if(data == FM_PrepareReload)	return 10;						//Zeit bevor das eigentliche Nachladen beginnt
  if(data == FM_FinishReload)	return 25;						//Zeit nach dem Nachladen

  if(data == FM_Reload)		return 75 + (iAttachment == AT_ExtendedMag) * 30;	//Zeit des einzelnen Nachladens bei Revolversystemen
  if(data == FM_Recharge)	return 30;						//Zeit bis erneut geschossen werden kann

  if(data == FM_Damage)		return 45;						//Schadenswert

  if(data == FM_SpreadAdd)	return 150 - (iAttachment == AT_Laserpointer)*3;	//Bei jedem Schuss hinzuzuaddierende Streuung
  if(data == FM_StartSpread)	return 320 - (iAttachment == AT_Laserpointer)*70;	//Bei Auswahl der Waffe gesetzte Streuung
  if(data == FM_MaxSpread)	return 570 - (iAttachment == AT_Laserpointer)*70;	//Maximaler Streuungswert
  if(data == FM_MinSpread)	return 120 - (iAttachment == AT_Laserpointer)*20;	//Minimal mögliche Streuung

  if(data == FM_MultiHit)	return 3;						//Anzahl möglicher Treffer pro Kugel
  if(data == FM_MultiHitReduce)	return 50;						//Schadensreduzierung pro Treffer

  return Default(data);
}

/* Kugeln - Streuschuss */

public func FMData1T1(int data)
{
  if(data == FT_Name)		return "$SpreadShot$";
  return FMData1(data);
}

public func Fire1T1()
{
  Fire1();
}

public func BotData1(int data)
{
  if(data == BOT_Range)		return 100;
  if(data == BOT_Power)		return(BOT_Power_3);
  return Default(data);
}

/* Kugeln - Schuss */

public func Fire1()
{
  //Austritt bestimmen
  var user = GetUser();
  var angle = user->AimAngle(10,0,true);
  var x,y;
  user->WeaponEnd(x,y);

  //Kugeln abfeuern
  var ammo;
  var j = GetFMData(FM_Damage, 1)/10;
  for(var i; i < j; i++)
  {
    angle = user->AimAngle(10,0,true);

    ammo = SALaunchBullet(x,y,GetController(user),angle,250+Random(20),300,10);
  }

  //Effekte
  MuzzleFlash(RandomX(40,55),user,x,y,angle,0, 5);
  AddEffect("Pump", this, 1, 1+GetFMData(FM_Recharge, 1)-25, this);
  Sound("PPGN_Fire*.ogg", 0, ammo);
  Echo("PPGN_Echo*.ogg");

  //Patronenhülse vorhanden
  casings = 1;
}

/* Nachladen */

func OnReload()
{
  Sound("WPN2_Handle*.ogg");
}

func OnFinishReloadStart()
{
  AddEffect("Pump", this, 1, 1+GetFMData(FM_Recharge, 1)-25, this);
}

func OnSingleReloadStart()
{
  Sound("PPGN_Reload*.ogg");
}

public func FxPumpStop(object pTarget)
{
  if(GetUser())
  {
    Sound("PPGN_Pump.ogg");

    //Patrone auswerfen
    if(casings)
    {
      var user = GetUser();
      var dir = GetDir(user)*2-1;
      SABulletCasing(dir*1,0,-dir*14*(Random(1)+1),-(13+Random(2)),6,RGB(255,0,0));
      casings = 0;
    }
  }
}

/* Handeffekt */

public func ReloadAnimation()		{return true;}
public func RechargeAnimation()		{return true;}
public func MaxReloadRotation()		{return 17;}
public func MaxRechargeRotation()	{return 17;}
public func ReloadAnimationSpeed()	{return 2;}

/* Allgemein */

func OnSelect()
{
  if(GetAmmo())
    Sound("PPGN_Pump.ogg");
  else
    Sound("MNGN_Charge.ogg");
}