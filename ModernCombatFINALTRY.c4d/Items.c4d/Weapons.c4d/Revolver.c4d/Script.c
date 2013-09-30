/*-- Revolver --*/

#strict 2
#include WPN2

local casings, casings2;

public func HandSize()		{return 800;}
public func HandX()		{return 4000;}
public func HandY()		{return -1300;}
public func BarrelYOffset()	{return -5000;}
public func IsSecondaryWeapon()	{return true;}

public func SelectionTime()	{return 10;}	//Anwahlzeit


/* Kompatible Waffenaufsätze */

func PermittedAtts()
{
  return AT_Laserpointer | AT_Flashlight;
}

/* Nahkampfangriff */

public func GetMCData(int data)
{
  if(data == MC_CanStrike)	return 1;	//Waffe kann Kolbenschlag ausführen
  if(data == MC_Damage)		return 10;	//Schaden eines Kolbenschlages
  if(data == MC_Recharge)	return 38;	//Zeit nach Kolbenschlag bis erneut geschlagen oder gefeuert werden kann
  if(data == MC_Power)		return 14;	//Wie weit das Ziel durch Kolbenschläge geschleudert wird
  if(data == MC_Angle)		return 50;	//Mit welchem Winkel das Ziel durch Kolbenschläge geschleudert wird
}

/* Kugeln */

public func FMData1(int data)
{
  if(data == FM_Name)		return "$Bullets$";

  if(data == FM_AmmoID)		return STAM;	//ID der Munition
  if(data == FM_AmmoLoad)	return 6;	//Magazingröße
  if(data == FM_AmmoUsage)	return 1;	//Munition pro Schuss

  if(data == FM_Reload)		return 90;	//Zeit für Nachladen
  if(data == FM_Recharge)	return 15;	//Zeit bis erneut geschossen werden kann

  if(data == FM_SingleReload)	return 7;	//Zeit des einzelnen Nachladens bei Revolversystemen
  if(data == FM_PrepareReload)	return 10;	//Zeit bevor das eigentliche Nachladen beginnt
  if(data == FM_FinishReload)	return 20;	//Zeit nach dem Nachladen

  if(data == FM_Damage)		return 21;	//Schadenswert

  if(data == FM_Slot)		return 1;	//Slot des Feuermodus

  if(data == FM_SpreadAdd)	return 110 - (iAttachment == AT_Laserpointer)*4;	//Bei jedem Schuss hinzuzuaddierende Streuung
  if(data == FM_StartSpread)	return 30 - (iAttachment == AT_Laserpointer)*20;	//Bei Auswahl der Waffe gesetzte Streuung
  if(data == FM_MaxSpread)	return 220 - (iAttachment == AT_Laserpointer)*70;	//Maximaler Streuungswert
  if(data == FM_MinSpread)	return 20 - (iAttachment == AT_Laserpointer)*10;	//Kleinstmögliche Streuung

  return Default(data);
}

/* Kugeln - Einzelfeuer */

public func FMData1T1(int data)
{
  if(data == FT_Name)		return "$Single$";

  return FMData1(data);
}

public func Fire1T1()
{
  Fire1();
}

public func BotData1(int data)
{
  if(data == BOT_Range)		return 400;
  if(data == BOT_Power)		return BOT_Power_2;
  return Default(data);
}

/* Kugeln - Schuss */

public func Fire1()
{
  //Austritt bestimmen
  var user = GetUser();
  var angle = user->AimAngle(20,0,true);
  var x,y;
  user->WeaponEnd(x,y);

  //Kugel abfeuern
  var ammo = SALaunchBullet(x,y,GetController(user),angle,250,400,GetFMData(FM_Damage));

  //Effekte
  MuzzleFlash(RandomX(40,45),user,x,y,angle,0, 4);
  Sound("RVLR_Fire.ogg", 0, ammo);
  Sound("MNGN_Click.ogg",0, 0, 50);
  Echo("PSTL_Echo.ogg");

  //Patronenhülse hinzufügen
  casings++;
}

/* Peilsender */

public func FMData2(int data)
{
  if(data == FM_Name)		return "$TracerDart$";

  if(data == FM_AmmoID)		return STAM;	//ID der Munition
  if(data == FM_AmmoLoad)	return 1;	//Magazingröße

  if(data == FM_Reload)		return 80;	//Zeit für Nachladen

  if(data == FM_Auto)		return false;	//Kein Automatikfeuer

  if(data == FM_Damage)		return 0;	//Schadenswert

  if(data == FM_Slot)		return 2;	//Slot des Feuermodus

  if(data == FM_SpreadAdd)	return 60;	//Bei jedem Schuss hinzuzuaddierende Streuung
  if(data == FM_StartSpread)	return 10;	//Bei Auswahl der Waffe gesetzte Streuung
  if(data == FM_MaxSpread)	return 200;	//Maximaler Streuungswert

  return Default(data);
}

/* Peilsender - Einzelfeuer */

public func FMData2T1(int data)
{
  if(data == FT_Name)		return "$Single$";

  return FMData2(data);
}

public func Fire2T1()
{
  Fire2();
}

/* Peilsender - Schuss */

public func Fire2()
{
  var user = GetUser();
  var angle = user->AimAngle(20,0,true);
  var x,y;
  user->WeaponEnd(x,y);
  var ammo = CreateObject(TRDT,x,y,GetController(user));
  ammo->Launch(angle,180,1000,20,1000);
  Sound("PSTL_TracerFire.ogg", 0, ammo);

  //Patronenhülse vorhanden
  casings2 = 1;
}

/* Nachladen */

public func OnReload(i)
{
  if(i == 1)
  {
    Sound("RVLR_ReloadStart.ogg");

    var j = casings;
    for(var i; i = j; j--)
    {
      var user = GetUser();
      var dir = GetDir(user)*2-1;
      SABulletCasing(dir*1,0,-dir*14*(Random(1)+1),-(10+Random(2)));
    }
    casings = 0;
  }
  if(i == 2)
  {
    Sound("PSTL_TracerReload.ogg");
    if(casings2)
    {
      var user = GetUser();
      var dir = GetDir(user)*2-1;
      SABulletCasing(dir*1,0,-dir*14*(Random(1)+1),-(13+Random(2)),6,RGB(150,150,150));
    }
    casings2 = 0;
  }
}

func OnSingleReloadStart()
{
  Sound("RVLR_Reloading*.ogg");
}

func OnFinishReloadStart(i)
{
  if(i == 1)
  {
    Sound("RVLR_ReloadStop.ogg");
  }
}

/* Handeffekt */

public func ReloadAnimation()		{return true;}
public func RechargeAnimation()		{return true;}
public func MaxReloadRotation()		{return -20;}
public func MaxRechargeRotation()	{return 1;}
public func ReloadAnimationSpeed()	{return 1 + !!IsReloading();}

/* Allgemein */

func OnSelect()
{
  Sound("PSTL_Charge.ogg");
}