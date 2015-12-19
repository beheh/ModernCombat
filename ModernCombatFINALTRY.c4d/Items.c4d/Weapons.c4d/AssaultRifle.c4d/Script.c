/*-- Sturmgewehr --*/

#strict 2
#include WPN2

local casing;

public func HandSize()		{return 1000;}
public func HandX()		{return 5000;}
public func HandY()		{return 1000;}
public func BarrelYOffset()	{return -2500;}
public func IsPrimaryWeapon()	{return true;}

public func SelectionTime()	{return 36;}	//Anwahlzeit


/* Kompatible Waffenaufsätze */

func PermittedAtts()
{
  return AT_ExtendedMag | AT_Bayonet | AT_Flashlight;
}

/* Nahkampfangriff */

public func GetMCData(int data)
{
  if(data == MC_CanStrike)	return 1;					//Waffe kann Kolbenschlag ausführen
  if(data == MC_Damage)		return 20 + (iAttachment == AT_Bayonet)*8;	//Schaden eines Kolbenschlages
  if(data == MC_Recharge)	return 40 + (iAttachment == AT_Bayonet)*10;	//Zeit nach Kolbenschlag bis erneut geschlagen oder gefeuert werden kann
  if(data == MC_Power)		return 20;					//Wie weit das Ziel durch Kolbenschläge geschleudert wird
  if(data == MC_Angle)		return 45;					//Mit welchem Winkel das Ziel durch Kolbenschläge geschleudert wird
}

/* Kugeln */

public func FMData1(int data)
{
  if(data == FM_Name)		return "$Bullets$";

  if(data == FM_AmmoID)		return STAM;					//ID der Munition
  if(data == FM_AmmoLoad)	return 30 + (iAttachment == AT_ExtendedMag)*6;	//Magazingröße

  if(data == FM_Reload)		return 90 + (iAttachment == AT_ExtendedMag)*16;	//Zeit für Nachladen
  if(data == FM_Recharge)	return 13;					//Zeit bis erneut geschossen werden kann

  if(data == FM_Auto)		return false;					//Kein Automatikfeuer

  if(data == FM_Damage)		return 14;					//Schadenswert

  if(data == FM_Slot)		return 1;					//Slot des Feuermodus

  if(data == FM_SpreadAdd)	return 55;					//Bei jedem Schuss hinzuzuaddierende Streuung
  if(data == FM_StartSpread)	return 100;					//Bei Auswahl der Waffe gesetzte Streuung
  if(data == FM_MaxSpread)	return 450;					//Maximaler Streuungswert

  return Default(data);
}

/* Kugeln - Stoßfeuer */

public func FMData1T1(int data)
{
  if(data == FT_Name)		return "$Burst$";

  if(data == FM_Recharge)	return FMData1(data) + 2;	//Zeit bis erneut geschossen werden kann

  if(data == FM_BurstAmount)	return 3;			//Anzahl Schussabrufe pro Burst
  if(data == FM_BurstRecharge)	return 3;			//Zeit zwischen einzelnen Bursts

  return FMData1(data);
}

public func Fire1T1()
{
  Fire1();
}

public func BotData1(int data)
{
  if(data == BOT_Range)		return 400;
  if(data == BOT_Power)		return(BOT_Power_3);
  return Default(data);
}

/* Kugeln - Einzelfeuer */

public func FMData1T2(int data)
{
  if(data == FT_Name)		return "$Single$";

  if(data == FM_Damage)		return 14;			//Schadenswert

  if(data == FM_SpreadAdd)	return FMData1(data) + 20;	//Bei jedem Schuss hinzuzuaddierende Streuung

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
  var ammo = SALaunchBullet(x,y,GetController(user),angle,270,800,GetFMData(FM_Damage));

  //Effekte
  SABulletCasing(x/3,y/3,-dir*14*(Random(1)+1),-(13+Random(2)),5);
  Sound("ASTR_Fire*.ogg", 0, ammo);
  MuzzleFlash(RandomX(30,40),user,x,y,angle,0, 0);
  Echo("ASTR_Echo.ogg");

  //Klickgeräusch bei wenig Munition
  if(Inside(GetAmmo(GetFMData(FM_AmmoID)), 1, GetFMData(FM_AmmoLoad)/3))
    Sound("MNGN_Click.ogg", 0, ammo, 0, GetOwner(user)+1);
}

/* Granaten */

public func FMData2(int data)
{
  if(data == FM_Name)		return "$Grenades$";

  if(data == FM_AmmoID)		return GRAM;	//ID der Munition
  if(data == FM_AmmoLoad)	return 1;	//Magazingröße

  if(data == FM_Reload)		return 80;	//Zeit für Nachladen
  if(data == FM_Recharge)	return 1;	//Zeit bis erneut geschossen werden kann

  if(data == FM_Damage)		return 20;	//Schadenswert

  if(data == FM_Slot)		return 2;	//Slot des Feuermodus

  if(data == FM_SpreadAdd)	return 200;	//Bei jedem Schuss hinzuzuaddierende Streuung
  if(data == FM_StartSpread)	return 100;	//Bei Auswahl der Waffe gesetzte Streuung
  if(data == FM_MaxSpread)	return 400;	//Maximaler Streuungswert

  return Default(data);
}

/* Granaten - Explosivgranaten */

public func FMData2T1(int data)
{
  if(data == FT_Name)		return "$Explosive$";

  return FMData2(data);
}

public func Fire2T1()
{  
  Fire2();
}

public func Fire2()
{
  LaunchGrenade(ESHL, 90,Contained()->~AimAngle(0,0,true));
}

public func BotData2(int data)
{
  if(data == BOT_Range)		return 90;
  if(data == BOT_Power)		return(BOT_Power_2);
  return Default(data);
}

/* Granaten - Splittergranaten */

public func FMData2T2(int data)
{
  if(data == FT_Name)		return "$Cluster$";

  return FMData2(data);
}

public func Fire2T2()
{
  LaunchGrenade(FSHL, 90,Contained()->~AimAngle(0,0,true));
}

/* Granaten - Rauchgranaten */

public func FMData2T3(int data)
{
  if(data == FT_Name)		return "$Smoke$";

  if(data == FM_Damage)		return FMData2(data) + 10;	//Schadenswert

  return FMData2(data);
}

public func Fire2T3()
{
  LaunchGrenade(SSHL, 90,Contained()->~AimAngle(0,0,true));
}

/* Granaten - Schuss */

public func LaunchGrenade(id idg, int speed, int angle, int mode)
{
  //Austritt bestimmen
  var user = GetUser();
  var dir = GetDir(user)*2-1;
  var x,y;
  user->WeaponEnd(x,y);

  //Anpassung des Winkels
  angle = BoundBy(angle/*+GetYDir(user)*/+dir,-360,360);
  //Geschwindigkeit einstellen
  var xdir = Sin(angle,speed);
  var ydir = -Cos(angle,speed);

  //Granate abfeuern
  var grenade=CreateObject(idg, x, y, GetController(user));
  SetController(GetController(user), grenade);
  grenade->Launch(xdir+GetXDir(user)/5, ydir/*+GetYDir(user)/10*/, GetFMData(FM_Damage,2), 0, 0, 0, iAttachment, user);

  //Sicht auf Granate wenn der Schütze zielt
  if(!(user->~IsMachine()) && user->~IsAiming())
  {
    SetPlrView(GetController(user),grenade);
    SetPlrViewRange(100, grenade);
  }

  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 0)
  {
    CreateParticle("Thrust",x,y,GetXDir(user),GetYDir(user),80,RGBa(255,200,200,0),0,0);

    for(var i=0; i<20; ++i)
    {
      CreateParticle("Smoke2",x+RandomX(-5,+5),y+RandomX(-5,+5),
      		GetXDir(user)+RandomX(0,xdir/4),GetYDir(user)+RandomX(0,ydir/4),
      		RandomX(80,140),RGBa(200,200,200,0),0,0);

      CreateParticle("BlastSpark1",x+RandomX(-5,+5),y+RandomX(-5,+5),
      		GetXDir(user)+RandomX(0,xdir/4),GetYDir(user)+RandomX(0,ydir/4),
      		RandomX(40,70),RGBa(200,200,200,0),0,0);
    }
  }
  Sound("ASTR_LauncherFire*.ogg", 0, grenade);
  Echo("SGST_Echo.ogg");

  //Patronenhülse vorhanden
  casing = 1;
}

/* Handeffekt */

public func ReloadAnimation()		{return true;}
public func RechargeAnimation()		{return true;}
public func MaxReloadRotation()		{return 10;}
public func MaxRechargeRotation()	{ return 1;}
public func ReloadAnimationSpeed()	{ return 1 + !!IsReloading();}

/* Allgemein */

func OnReload(i)
{
  if(i == 1)
  {
    if(iAttachment == AT_ExtendedMag)
      Sound("ASTR_Reload2.ogg");
    else
      Sound("ASTR_Reload.ogg");
  }
  if(i == 2)
  {
    Sound("ASTR_LauncherReload.ogg");
    if(casing)
    {
      var user = GetUser();
      var dir = GetDir(user)*2-1;
      SABulletCasing(dir*1,0,-dir*14*(Random(1)+1),-(13+Random(2)),6,RGB(255,0,0));
      casing = 0;
    }
  }
}

func OnSelect()
{
  Sound("ASTR_Charge.ogg");
}

public func OnSelectFT(int iFireMode, int iFireTec)
{
  if(iFireMode == 2)
    Empty2(2);
}