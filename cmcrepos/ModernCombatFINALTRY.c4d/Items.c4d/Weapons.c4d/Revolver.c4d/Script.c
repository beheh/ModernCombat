/*-- Revolver --*/

#strict 2
#include WPN2

local casings, casings2;

public func HandSize()		{return 800;}
public func HandX()		{return 4000;}
public func HandY()		{return -1300;}
public func BarrelYOffset()	{return -5000;}
public func SelectionTime()	{return 10;}
public func IsSecondaryWeapon()	{return true;}


/* Nahkampfangriff */

public func GetMCData(int data)
{
  if(data == MC_CanStrike)	return 1;
  if(data == MC_Damage)		return 10;
  if(data == MC_Recharge)	return 35;
  if(data == MC_Power)		return 18;
  if(data == MC_Angle)		return 45;
}

/* Kugeln */

public func FMData1(int data)
{
  if(data == FM_Name)		return "$Bullets$";
    
  if(data == FM_AmmoID)		return STAM;
  if(data == FM_AmmoLoad)	return 6;
  if(data == FM_AmmoUsage)	return 1;

  if(data == FM_Recharge)	return 15;

  if(data == FM_SingleReload)	return 7;
  if(data == FM_Reload)		return 90;
  if(data == FM_PrepareReload)	return 20;
  if(data == FM_FinishReload)	return 20;

  if(data == FM_Damage)		return 21;
  
  if(data == FM_Slot)		return 1;
  
  if(data == FM_SpreadAdd)	return 110;
  if(data == FM_StartSpread)	return 30;
  if(data == FM_MaxSpread)	return 220;
  if(data == FM_MinSpread)  return 20;

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
    
  if(data == FM_AmmoID)		return STAM;
  if(data == FM_AmmoLoad)	return 1;

  if(data == FM_Reload)		return 80;

  if(data == FM_Auto)		return false;
  
  if(data == FM_Damage)		return 0;
  
  if(data == FM_Slot)		return 2;

  if(data == FM_SpreadAdd)	return 60;
  if(data == FM_StartSpread)	return 10;
  if(data == FM_MaxSpread)	return 200;

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
  ammo->Launch(angle,180,800,20,1000);
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

public func HandR()
{
  var effect = IsReloading();
  if(effect)
    return -BoundBy(GetEffect(0,this,effect,6)*2,0,-20);

  effect = IsRecharging();
  if(effect)
    return -BoundBy(GetEffect(0,this,effect,6)*1,0,1);
}

/* Allgemein */

func OnSelect()
{
  Sound("PSTL_Charge.ogg");
}
