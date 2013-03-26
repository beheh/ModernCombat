/*-- Pistole --*/

#strict 2
#include WPN2

local casing;

public func HandSize()		{return 800;}
public func HandX()		{return 4000;}
public func HandY()		{return -1200;}
public func BarrelYOffset()	{return -5000;}
public func IsSecondaryWeapon()	{return true;}

public func SelectionTime()	{return 10;}	//Anwahlzeit


/* Nahkampfangriff */

public func GetMCData(int data)
{
  if(data == MC_CanStrike)	return 1;	//Waffe kann Kolbenschlag ausf�hren
  if(data == MC_Damage)		return 10;	//Schaden eines Kolbenschlages
  if(data == MC_Recharge)	return 38;	//Zeit nach Kolbenschlag bis erneut geschlagen oder gefeuert werden kann
  if(data == MC_Power)		return 14;	//Wie weit das Ziel durch Kolbenschl�ge geschleudert wird
  if(data == MC_Angle)		return 50;	//Mit welchem Winkel das Ziel durch Kolbenschl�ge geschleudert wird
}

/* Kugeln */

public func FMData1(int data)
{
  if(data == FM_Name)		return "$Bullets$";
    
  if(data == FM_AmmoID)		return STAM;	//ID der Munition
  if(data == FM_AmmoLoad)	return 15;	//Magazingr��e
  
  if(data == FM_Reload)		return 40;	//Zeit f�r Nachladen
  if(data == FM_Recharge)	return 5;	//Zeit bis erneut geschossen werden kann

  if(data == FM_Auto)		return false;	//Kein Automatikfeuer

  if(data == FM_Damage)		return 12;	//Schadenswert
  
  if(data == FM_Slot)		return 1;	//Slot des Feuermodus
  
  if(data == FM_SpreadAdd)	return 60;	//Bei jedem Schuss hinzuzuaddierende Streuung
  if(data == FM_StartSpread)	return 30;	//Bei Auswahl der Waffe gesetzte Streuung
  if(data == FM_MaxSpread)	return 220;	//Maximaler Streuungswert
  if(data == FM_MinSpread)	return 20;	//Kleinstm�gliche Streuung

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
  if(data == BOT_Power)		return(BOT_Power_1);
  return Default(data);
}

/* Kugeln - Schuss */

public func Fire1()
{
  //Austritt bestimmen
  var user = GetUser();
  var dir = GetDir(user)*2-1;
  var angle = user->AimAngle(20,0,true);
  var x,y;
  user->WeaponEnd(x,y);

  //Kugel abfeuern
  var ammo = SALaunchBullet(x,y,GetController(user),angle,250,400,GetFMData(FM_Damage));

  //Effekte
  MuzzleFlash(RandomX(35,40),user,x,y,angle,0, 4);
  SABulletCasing(x/3,y/3,-dir*14*(Random(1)+1),-(13+Random(2)),4);
  Sound("PSTL_Fire*.ogg", 0, ammo);
  Echo("PSTL_Echo.ogg");
}

/* Peilsender */

public func FMData2(int data)
{
  if(data == FM_Name)		return "$TracerDart$";
    
  if(data == FM_AmmoID)		return STAM;	//ID der Munition
  if(data == FM_AmmoLoad)	return 1;	//Magazingr��e

  if(data == FM_Reload)		return 90;	//Zeit f�r Nachladen

  if(data == FM_Auto)		return false;	//Kein Automatikfeuer
  
  if(data == FM_Damage)		return;		//Schadenswert
  
  if(data == FM_Slot)		return 2;	//Slot des Feuermodus

  if(data == FM_SpreadAdd) return 60;		//Bei jedem Schuss hinzuzuaddierende Streuung
  if(data == FM_StartSpread) return 10;		//Bei Auswahl der Waffe gesetzte Streuung
  if(data == FM_MaxSpread) return 200;		//Maximaler Streuungswert

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
  //Austritt bestimmen
  var user = GetUser();
  var angle = user->AimAngle(20,0,true);
  var x,y;
  user->WeaponEnd(x,y);

  //Kugel abfeuern
  var ammo = CreateObject(TRDT,x,y,GetController(user));
  ammo->Launch(angle,180,1000,20,1000);
  Sound("PSTL_TracerFire.ogg", 0, ammo);

  //Patronenh�lse vorhanden
  casing = 1;
}

/* Handeffekt */

public func ReloadAnimation()		{return true;}
public func MaxReloadRotation()		{return -20;}
public func ReloadAnimationSpeed()	{return 2;}

/* Allgemein */

func OnSelect()
{
  Sound("PSTL_Charge.ogg");
}

public func OnReload(i)
{
  if(i == 1)
  {
    Sound("PSTL_Reload.ogg");
  }
  if(i == 2)
  {
    Sound("PSTL_TracerReload.ogg");
    if(casing)
    {
      var user = GetUser();
      var dir = GetDir(user)*2-1;
      SABulletCasing(dir*1,0,-dir*14*(Random(1)+1),-(13+Random(2)),6,RGB(150,150,150));
      casing = 0;
    }
  }
}