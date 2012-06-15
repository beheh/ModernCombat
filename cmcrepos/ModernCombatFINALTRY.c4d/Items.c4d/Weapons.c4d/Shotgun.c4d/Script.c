/*-- Shotgun --*/

#strict 2
#include WPN2

public func HandSize()		{return 1000;}
public func HandX()		{return 5000;}
public func HandY()		{return -500;}
public func BarrelYOffset()	{return -2000;}
public func SelectionTime()	{return 20;}
public func IsPrimaryWeapon()	{return true;}


/* Nahkampfangriff */

public func GetMCData(int data)
{
  if(data == MC_CanStrike)	return 1;
  if(data == MC_Damage)		return 20;
  if(data == MC_Recharge)	return 40;
  if(data == MC_Power)		return 20;
  if(data == MC_Angle)		return 45;
}

/* Kugeln */

public func FMData1(int data)
{
  if(data == FM_Name)		return "$Pellets$";
    
  if(data == FM_AmmoID)		return STAM;
  if(data == FM_AmmoLoad)	return 20;
  if(data == FM_AmmoUsage)	return 4;
  
  if(data == FM_SingleReload)	return 2;
  if(data == FM_PrepareReload)	return 10;
  if(data == FM_FinishReload)	return 25;
    
  if(data == FM_Reload)		return 75;
  if(data == FM_Recharge)	return 30;
  
  if(data == FM_Damage)		return 45;
  
  if(data == FM_SpreadAdd)	return 150;
  if(data == FM_StartSpread)	return 200;

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
  if(data == BOT_Power)   return(BOT_Power_3);
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
    ammo = SALaunchBullet(x,y,GetController(user),angle+RandomX(-6,+6),250+Random(20),300,10);
  }

  //Effekte
  MuzzleFlash(RandomX(40,55),user,x,y,angle,0, 5);
  AddEffect("Pump", this, 1, 1+GetFMData(FM_Recharge, 1)-25, this);
  Sound("PPGN_Fire*.ogg", 0, ammo);
  Echo("PPGN_Echo*.ogg");
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
  Sound("PPGN_Pump.ogg");

  if(!GetAmmo()) return;

  var user = GetUser();
  var dir = GetDir(user)*2-1;
  SABulletCasing(dir*1,0,-dir*14*(Random(1)+1),-(13+Random(2)),6,RGB(255));
}

/* Handeffekt */

public func HandR()
{
  var effect = IsReloading();
  if(effect)
    return -BoundBy(GetEffect(0,this,effect,6)*2,0,17);

  effect = IsRecharging();
  if(effect)
    return -BoundBy(GetEffect(0,this,effect,6)*2,0,17);
}

/* Allgemein */

func OnSelect()
{
  if(GetAmmo())
    Sound("PPGN_Pump.ogg");
  else
    Sound("MNGN_Charge.ogg");
}
