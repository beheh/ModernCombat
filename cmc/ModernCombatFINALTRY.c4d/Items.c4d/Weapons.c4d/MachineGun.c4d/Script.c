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


/* Kompatible Waffenaufsätze */

func PermittedAtts()
{
  return AT_ExtendedMag | AT_Silencer | AT_Bayonet | AT_Laserpointer | AT_Flashlight;
}

/* Nahkampfangriff */

public func GetMCData(int data)
{
  if(data == MC_CanStrike)	return 1;					//Waffe kann Kolbenschlag ausführen
  if(data == MC_Damage)		return 20 + (iAttachment == AT_Bayonet)*8;	//Schaden eines Kolbenschlages
  if(data == MC_Recharge)	return 45 + (iAttachment == AT_Bayonet)*7;	//Zeit nach Kolbenschlag bis erneut geschlagen oder gefeuert werden kann
  if(data == MC_Power)		return 20;					//Wie weit das Ziel durch Kolbenschläge geschleudert wird
  if(data == MC_Angle)		return 45;					//Mit welchem Winkel das Ziel durch Kolbenschläge geschleudert wird
}

/* Kugeln */

public func FMData1(int data)
{
  if(data == FM_Name)		return "$Bullets$";

  if(data == FM_AmmoID)		return STAM;						//ID der Munition
  if(data == FM_AmmoLoad)	return 60 + (iAttachment == AT_ExtendedMag)*20;		//Magazingröße

  if(data == FM_Reload)		return 220 + (iAttachment == AT_ExtendedMag)*16;	//Zeit für Nachladen
  if(data == FM_Recharge)	return 4;						//Zeit bis erneut geschossen werden kann

  if(data == FM_Auto)		return true;						//Automatikfeuer

  if(data == FM_Damage)		return 8;						//Schadenswert

  if(data == FM_SpreadAdd)	return 30 - (iAttachment == AT_Laserpointer)*3;		//Bei jedem Schuss hinzuzuaddierende Streuung
  if(data == FM_StartSpread)	return 220 - (iAttachment == AT_Laserpointer)*20;	//Bei Auswahl der Waffe gesetzte Streuung
  if(data == FM_MaxSpread)	return 420 - (iAttachment == AT_Laserpointer)*70;	//Maximaler Streuungswert
  if(data == FM_MinSpread)	return 20 - (iAttachment == AT_Laserpointer)*10;	//Kleinstmögliche Streuung

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

  if(data == FM_Recharge)	return FMData1(data) + 20;	//Zeit bis erneut geschossen werden kann

  if(data == FM_BurstAmount)	return 4;			//Anzahl Schussabrufe pro Burst
  if(data == FM_BurstRecharge)	return 4;			//Zeit zwischen einzelnen Bursts

  if(data == FM_Damage)		return FMData1(data) + 1;	//Schadenswert

  if(data == FM_Auto)		return false;			//Kein Automatikfeuer

  if(data == FM_SpreadAdd)	return FMData1(data) + 10;	//Bei jedem Schuss hinzuzuaddierende Streuung

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
  var ammo = SALaunchBullet(x,y,GetController(user),angle,270,750,GetFMData(FM_Damage), 0, 0,iAttachment == AT_Silencer);

  //Effekte
  if(iAttachment != AT_Silencer)
  {
    MuzzleFlash(RandomX(35,50),user,x,y,angle,0, 0);
    SABulletCasing(x/3,y/3,-dir*14*(Random(1)+1),-(13+Random(2)),5);
    Sound("MNGN_Fire.ogg", 0, ammo);
    Echo("MNGN_Echo.ogg");
  }
  else
  {
    Sound("WPN2_SilencerFire*.ogg", 0, ammo, 0, GetOwner(user)+1);
    Sound("WPN2_SilencerFire*.ogg", 0, ammo, 30);
    Echo("WPN2_SilencerEcho.ogg");

    //Tarneffekt des Schützen schwächen
    if(GetEffect("Silencer", this))
      EffectVar(0, this, GetEffect("Silencer", this)) -= BoundBy(25, 0, EffectVar(0, this, GetEffect("Silencer", this)));
  }

  //Klickgeräusch bei wenig Munition
  if(Inside(GetAmmo(GetFMData(FM_AmmoID)), 0, 1))
    Sound("MNGN_Empty.ogg", 0, this, 0, GetOwner(user)+1);
  else
    if(Inside(GetAmmo(GetFMData(FM_AmmoID)), 1, GetFMData(FM_AmmoLoad)/3))
      Sound("MNGN_Click.ogg", 0, ammo, 0, GetOwner(user)+1);
}

/* Nachladen */

func OnReload()
{
  if(iAttachment == AT_ExtendedMag)
    Sound("MNGN_Reload2.ogg");
  else
    Sound("MNGN_Reload.ogg");
}

/* Handeffekt */

public func ReloadAnimation()		{return true;}
public func RechargeAnimation()		{return true;}
public func MaxRechargeRotation()	{return 1;}

public func HandR()
{
  var effect = IsReloading();
  if(effect)
    return Max(Sin(GetEffect(0,this,effect,6)*90/80,20),0);
  else
    return _inherited(...);
}

/* Allgemein */

protected func OnFireStop(int iSlot)
{
  //Streuung erhöhen wenn Dauerfeuer
  if(iSlot == 1)
    DoSpread(+8);

  Sound("MNGN_Click.ogg");
}

func OnSelect()
{
  Sound("MNGN_Charge.ogg");
}