/*-- Maschinengewehr --*/

#strict 2
#include WPN2

public func HandSize()		{return 800;}
public func HandX()		{return 5000;}
public func HandY()		{return 800;}
public func BarrelXOffset()	{return -1000;}
public func BarrelYOffset()	{return -2000;}
public func IsPrimaryWeapon()	{return true;}

public func SelectionTime()	{return 36;}	//Anwahlzeit


/* Nahkampfangriff */

public func GetMCData(int data)
{
  if(data == MC_CanStrike)	return 1;	//Waffe kann Kolbenschlag ausführen
  if(data == MC_Damage)		return 20;	//Schaden eines Kolbenschlages
  if(data == MC_Recharge)	return 45;	//Zeit nach Kolbenschlag bis erneut geschlagen oder gefeuert werden kann
  if(data == MC_Power)		return 20;	//Wie weit das Ziel durch Kolbenschläge geschleudert wird
  if(data == MC_Angle)		return 45;	//Mit welchem Winkel das Ziel durch Kolbenschläge geschleudert wird
}

/* Kugeln */

public func FMData1(int data)
{
  if(data == FM_Name)		return "$Bullets$";

  if(data == FM_AmmoID)		return STAM;	//ID der Munition
  if(data == FM_AmmoLoad)	return 60;	//Magazingröße

  if(data == FM_Reload)		return 220;	//Zeit für Nachladen
  if(data == FM_Recharge)	return 4;	//Zeit bis erneut geschossen werden kann

  if(data == FM_Auto)		return true;	//Automatikfeuer

  if(data == FM_Damage)		return 8;	//Schadenswert

  if(data == FM_SpreadAdd)	return 30;	//Bei jedem Schuss hinzuzuaddierende Streuung
  if(data == FM_StartSpread)	return 220;	//Bei Auswahl der Waffe gesetzte Streuung
  if(data == FM_MaxSpread)	return 420;	//Maximaler Streuungswert
  if(data == FM_MinSpread)	return 20;	//Kleinstmögliche Streuung

  return Default(data);
}

/* Kugeln - Dauerfeuer */

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
  if(data == BOT_Range)		return 250;
  if(data == BOT_Power)		return BOT_Power_LongLoad;

  return(Default(data));
}

/* Kugeln - Stoßfeuer */

public func FMData1T2(int data)
{
  if(data == FT_Name)		return "$Burst$";

  if(data == FM_Recharge)	return FMData1(data) + 26;	//Zeit bis erneut geschossen werden kann

  if(data == FM_BurstAmount)	return 4;			//Anzahl Schussabrufe pro Burst
  if(data == FM_BurstRecharge)	return 4;			//Zeit zwischen einzelnen Bursts

  if(data == FM_Damage)		return FMData1(data) + 1;	//Schadenswert

  if(data == FM_Auto)		return false;			//Kein Automatikfeuer

  return FMData1(data);
}

public func Fire1T2()
{
  Fire1();
}

/* Kugeln - Schuss */

public func Fire1()
{
  //Austritt bestimmen
  var user = GetUser();
  var dir = GetDir(user)*2-1;
  var angle = user->AimAngle(10,0,true);
  var x,y;
  user->WeaponEnd(x,y);

  //Kugel abfeuern
  var ammo = SALaunchBullet(x,y,GetController(user),angle,270,750,GetFMData(FM_Damage));

  //Effekte
  MuzzleFlash(RandomX(35,50),user,x,y,angle,0, 0);
  SABulletCasing(x/3,y/3,-dir*14*(Random(1)+1),-(13+Random(2)),5);
  Sound("MNGN_Fire.ogg", 0, ammo);
  Echo("MNGN_Echo.ogg");
}

/* Handeffekt */

public func HandR()
{
  var effect = IsReloading();
  if(effect)
    return Max(Sin(GetEffect(0,this,effect,6)*90/80,20),0);

  effect = IsRecharging();
  if(effect)
    return -BoundBy(GetEffect(0,this,effect,6)*1,0,1);
}

/* Allgemein */

protected func OnFireStop(int iSlot)
{
  Sound("MNGN_Click.ogg");
}

func OnReload()
{
  Sound("MNGN_Reload.ogg");
}

func OnSelect()
{
  Sound("MNGN_Charge.ogg");
}