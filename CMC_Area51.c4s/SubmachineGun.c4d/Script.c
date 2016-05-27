/*-- Maschinenpistole --*/

#strict 2
#include WPN2

public func HandSize()		{return 730;}
public func HandX()		{return 5000;}
public func HandY()		{return 2500;}
public func BarrelYOffset()	{return -2000;}
public func IsPrimaryWeapon()	{return true;}

public func SelectionTime()	{return 33;}	//Anwahlzeit


/* Kompatible Waffenaufs�tze */

func PermittedAtts()
{
  return AT_Laserpointer | AT_Silencer | AT_Flashlight;
}

/* Nahkampfangriff */

public func GetMCData(int data)
{
  if(data == MC_CanStrike)	return 1;	//Waffe kann Kolbenschlag ausf�hren
  if(data == MC_Damage)		return 15;	//Schaden eines Kolbenschlages
  if(data == MC_Recharge)	return 38;	//Zeit nach Kolbenschlag bis erneut geschlagen oder gefeuert werden kann
  if(data == MC_Power)		return 18;	//Wie weit das Ziel durch Kolbenschl�ge geschleudert wird
  if(data == MC_Angle)		return 45;	//Mit welchem Winkel das Ziel durch Kolbenschl�ge geschleudert wird
}

/* Kugeln */

public func FMData1(int data)
{
  if(data == FM_Name)		return "$Bullets$";

  if(data == FM_AmmoID)		return STAM;						//ID der Munition
  if(data == FM_AmmoLoad)	return 40;						//Magazingr��e

  if(data == FM_Reload)		return 65;						//Zeit f�r Nachladen
  if(data == FM_Recharge)	return 3;						//Zeit bis erneut geschossen werden kann

  if(data == FM_Auto)		return true;						//Automatikfeuer
  
  if(data == FM_Damage)		return 4;						//Schadenswert
  
  if(data == FM_Slot)		return 1;						//Slot des Feuermodus
  
  if(data == FM_SpreadAdd)	return 20 - (iAttachment == AT_Laserpointer)*4;		//Bei jedem Schuss hinzuzuaddierende Streuung
  if(data == FM_StartSpread)	return 70 - (iAttachment == AT_Laserpointer)*20;	//Bei Auswahl der Waffe gesetzte Streuung
  if(data == FM_MaxSpread)	return 410 - (iAttachment == AT_Laserpointer)*110;	//Maximaler Streuungswert
  if(data == FM_MinSpread)	return 60 - (iAttachment == AT_Laserpointer)*20;	//Kleinstm�gliche Streuung

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

/* Kugeln - Sto�feuer */

public func FMData1T2(int data)
{
  if(data == FT_Name)		return "$Burst$";

  if(data == FM_Recharge)	return FMData1(data) + 3;					//Zeit bis erneut geschossen werden kann

  if(data == FM_Auto)		return false;							//Kein Automatikfeuer

  if(data == FM_Damage)		return FMData1(data) + 1;					//Schadenswert

  if(data == FM_BurstAmount)	return 3;							//Anzahl Schussabrufe pro Burst
  if(data == FM_BurstRecharge)	return 2;							//Zeit zwischen einzelnen Bursts

  if(data == FM_SpreadAdd)	return FMData1(data) + 5;					//Bei jedem Schuss hinzuzuaddierende Streuung
  if(data == FM_MinSpread)	return FMData1(data) - (iAttachment == AT_Laserpointer)*10;	//Kleinstm�gliche Streuung

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

  if(data == FM_Recharge)	return FMData1(data) + 5;					//Zeit bis erneut geschossen werden kann

  if(data == FM_Auto)		return false;							//Kein Automatikfeuer

  if(data == FM_Damage)		return FMData1(data) + 7;					//Schadenswert

  if(data == FM_SpreadAdd)	return FMData1(data) - 2;					//Bei jedem Schuss hinzuzuaddierende Streuung
  if(data == FM_MinSpread)	return FMData1(data) - (iAttachment == AT_Laserpointer)*20;	//Kleinstm�gliche Streuung

  if(data == FM_MultiHit)	return 3;							//Anzahl m�glicher Treffer pro Kugel
  if(data == FM_MultiHitReduce)	return 50;							//Schadensreduzierung pro Treffer

  return FMData1(data);
}

public func Fire1T3()
{
  Fire1();
}

/* Kugeln - Schuss */

public func Fire1()
{
/*
  //Austritt bestimmen
  var user = GetUser();
  var dir = GetDir(user)*2-1;
  var angle = user->AimAngle(15,0,true);
  var x,y;
  user->WeaponEnd(x,y);

  //Kugel abfeuern
  var ammo = SALaunchBullet(x,y,GetController(user),angle,270,550,GetFMData(FM_Damage), 0, 0, iAttachment == AT_Silencer);
*/

  //Austritt bestimmen
  var user = GetUser();
  var dir = GetDir(user)*2-1;
  var angle = user->AimAngle(25,0,true);
  var x,y;
  user->WeaponEnd(x,y);

  //Kugel abfeuern
  var ammo = CreateObject(EBLT,x,y,GetController(user));
  ammo->Launch(angle,180,1000,20,1000);

  //Effekte
  if(iAttachment != AT_Silencer)
  {
    SABulletCasing(x/3,y/3,-dir*14,-(14),4);
    Sound("P29W_Fire.ogg", 0, ammo);
    MuzzleFlash(RandomX(30,35),user,x,y,angle,0, 4);
    Echo("P29W_Echo.ogg");
  }
  else
  {
    Sound("WPN2_SilencerFire*.ogg", 0, ammo, 0, GetOwner(user)+1);
    Sound("WPN2_SilencerFire*.ogg", 0, ammo, 10);
    Echo("WPN2_SilencerEcho.ogg");

    //Tarneffekt des Sch�tzen schw�chen
    if(GetEffect("Silencer", this))
      EffectVar(0, this, GetEffect("Silencer", this)) -= BoundBy(10, 0, EffectVar(0, this, GetEffect("Silencer", this)));
  }

  //Klickger�usch bei wenig Munition
  if(Inside(GetAmmo(GetFMData(FM_AmmoID)), 1, GetFMData(FM_AmmoLoad)/3))
    Sound("MNGN_Click.ogg", 0, ammo, 0, GetOwner(user)+1);
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
  Sound("P29W_Charge.ogg");
}

func OnReload()
{
  Sound("P29W_Reload.ogg");
}