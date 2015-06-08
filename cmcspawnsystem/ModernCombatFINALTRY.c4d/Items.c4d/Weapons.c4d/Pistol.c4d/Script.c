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


/* Kompatible Waffenaufsätze */

func PermittedAtts()
{
  return AT_ExtendedMag | AT_Silencer | AT_Laserpointer | AT_Flashlight;
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

  if(data == FM_AmmoID)		return STAM;						//ID der Munition
  if(data == FM_AmmoLoad)	return 13 + (iAttachment == AT_ExtendedMag)*6;		//Magazingröße

  if(data == FM_Reload)		return 40 + (iAttachment == AT_ExtendedMag)*17;		//Zeit für Nachladen

  if(data == FM_Recharge)	return 5;						//Zeit bis erneut geschossen werden kann

  if(data == FM_Auto)		return false;						//Kein Automatikfeuer

  if(data == FM_Damage)		return 12;						//Schadenswert

  if(data == FM_Slot)		return 1;						//Slot des Feuermodus

  if(data == FM_SpreadAdd)	return 70 - (iAttachment == AT_Laserpointer)*4;		//Bei jedem Schuss hinzuzuaddierende Streuung
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
  var ammo = SALaunchBullet(x,y,GetController(user),angle,250,400,GetFMData(FM_Damage), 0, 0, iAttachment == AT_Silencer);

  //Effekte
  if(iAttachment != AT_Silencer)
  {
    SABulletCasing(x/3,y/3,-dir*14*(Random(1)+1),-(13+Random(2)),4);
    Sound("PSTL_Fire*.ogg", 0, ammo);
    MuzzleFlash(RandomX(35,40),user,x,y,angle,0, 4);
    Echo("PSTL_Echo.ogg");
  }
  else
  {
    Sound("WPN2_SilencerFire*.ogg", 0, ammo, 0, GetOwner(user)+1);
    Sound("WPN2_SilencerFire*.ogg", 0, ammo, 10);
    Echo("WPN2_SilencerEcho.ogg");

    //Tarneffekt des Schützen schwächen
    if(GetEffect("Silencer", this))
      EffectVar(0, this, GetEffect("Silencer", this)) -= BoundBy(25, 0, EffectVar(0, this, GetEffect("Silencer", this)));
  }
}

/* Peilsender */

public func FMData2(int data)
{
  if(data == FM_Name)		return "$TracerDart$";

  if(data == FM_AmmoID)		return STAM;	//ID der Munition
  if(data == FM_AmmoLoad)	return 1;	//Magazingröße

  if(data == FM_Reload)		return 90;	//Zeit für Nachladen

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

  //Patronenhülse vorhanden
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
    if(iAttachment == AT_ExtendedMag)
      Sound("PSTL_Reload2.ogg");
    else
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