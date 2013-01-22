/*-- Maschinenpistole --*/

#strict 2
#include WPN2

public func HandSize()		{return 800;}
public func HandX()		{return 2500;}
public func HandY()		{return -2000;}
public func BarrelYOffset()	{return -5500;}
public func IsPrimaryWeapon()	{return true;}

public func SelectionTime()	{return 30;}	//Anwahlzeit


/* Nahkampfangriff */

public func GetMCData(int data)
{
  if(data == MC_CanStrike)	return 1;	//Waffe kann Kolbenschlag ausführen
  if(data == MC_Damage)		return 15;	//Schaden eines Kolbenschlages
  if(data == MC_Recharge)	return 38;	//Zeit nach Kolbenschlag bis erneut geschlagen oder gefeuert werden kann
  if(data == MC_Power)		return 18;	//Wie weit das Ziel durch Kolbenschläge geschleudert wird
  if(data == MC_Angle)		return 45;	//Mit welchem Winkel das Ziel durch Kolbenschläge geschleudert wird
}

/* Kugeln */

public func FMData1(int data)
{
  if(data == FM_Name)		return "$Bullets$";

  if(data == FM_AmmoID)		return STAM;	//ID der Munition
  if(data == FM_AmmoLoad)	return 30;	//Magazingröße

  if(data == FM_Reload)		return 70;	//Zeit für Nachladen
  if(data == FM_Recharge)	return 3;	//Zeit bis erneut geschossen werden kann

  if(data == FM_Auto)		return true;	//Automatikfeuer
  
  if(data == FM_Damage)		return 6;	//Schadenswert
  
  if(data == FM_Slot)		return 1;	//Slot des Feuermodus
  
  if(data == FM_SpreadAdd)	return 20;	//Bei jedem Schuss hinzuzuaddierende Streuung
  if(data == FM_StartSpread)	return 110;	//Bei Auswahl der Waffe gesetzte Streuung
  if(data == FM_MaxSpread)	return 410;	//Maximaler Streuungswert
  if(data == FM_MinSpread)	return 60;	//Kleinstmögliche Streuung

  return Default(data);
}

/* Kugeln - Automatikfeuer */

public func FMData1T1(int data)
{
  if(data == FT_Name)		return "$Auto$";

  return FMData1(data);
}

public func Fire1T1()
{
  Fire1();
}

public func BotData1(int data)
{
  if(data == BOT_Range)		return 300;
  if(data == BOT_Power)		return(BOT_Power_3);
  return Default(data);
}

/* Kugeln - Stoßfeuer */

public func FMData1T2(int data)
{
  if(data == FT_Name)		return "$Burst$";

  if(data == FM_Recharge)	return FMData1(data) + 3;	//Zeit bis erneut geschossen werden kann

  if(data == FM_Auto)		return false;			//Kein Automatikfeuer

  if(data == FM_Damage)		return FMData1(data) + 2;	//Schadenswert

  if(data == FM_BurstAmount)	return 3;			//Anzahl Schussabrufe pro Burst
  if(data == FM_BurstRecharge)	return 2;			//Zeit zwischen einzelnen Bursts

  if(data == FM_SpreadAdd)	return FMData1(data) + 5;	//Bei jedem Schuss hinzuzuaddierende Streuung

  return FMData1(data);
}

public func Fire1T2()
{ 
  Fire1();
}

/* Kugeln - Einzelfeuer */

public func FMData1T3(int data)
{
  if(data == FT_Name)		return "$Single$";

  if(data == FM_Recharge)	return FMData1(data) + 5;	//Zeit bis erneut geschossen werden kann

  if(data == FM_Auto)		return false;			//Kein Automatikfeuer

  if(data == FM_Damage)		return FMData1(data) + 6;	//Schadenswert

  if(data == FM_SpreadAdd)	return FMData1(data) - 2;	//Bei jedem Schuss hinzuzuaddierende Streuung

  return FMData1(data);
}

public func Fire1T3()
{
  Fire1();
}

/* Kugeln - Schuss */

public func Fire1()
{
  //Austritt bestimmen
  var user = GetUser();
  var dir = GetDir(user)*2-1;
  var angle = user->AimAngle(15,0,true);
  var x,y;
  user->WeaponEnd(x,y);

  //Kugel abfeuern
  var ammo = SALaunchBullet(x,y,GetController(user),angle,270,550,GetFMData(FM_Damage));

  //Effekte
  MuzzleFlash(RandomX(30,35),user,x,y,angle,0, 4);
  SABulletCasing(x/3,y/3,-dir*14,-(14),4);
  Sound("SMGN_Fire*.ogg", 0, ammo);
  Echo("SMGN_Echo.ogg");
}

/* Handeffekt */

public func HandR()
{
  var effect = IsReloading();
  if(effect)
    return Max(Sin(GetEffect(0,this,effect,6)*20/5,20),0);
}

/* Allgemein */

func OnSelect()
{
  Sound("SMGN_Charge.ogg");
}

func OnReload()
{
  Sound("SMGN_Reload.ogg");
}