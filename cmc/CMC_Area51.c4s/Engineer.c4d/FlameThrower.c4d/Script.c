/*-- Flammenwerfer --*/

#strict 2
#include WPN2

public func HandSize()		{return(1000);}
public func HandX()		{return(5000);}
public func HandY()		{return(-2500);}

public func SelectionTime()	{return 36;}


/* Nahkampfangriff */

public func GetMCData(int data)
{
  if(data == MC_CanStrike)	return 1;
  if(data == MC_Damage)		return 20;
  if(data == MC_Recharge)	return 45;
  if(data == MC_Power)		return 20;
  if(data == MC_Angle)		return 45;
}

/* Flammen */

public func FMData1(int data)
{
  if(data == FM_Name)		return "$Flamethrower$";

  if(data == FM_AmmoID)		return STAM;
  if(data == FM_AmmoLoad)	return 10;

  if(data == FM_Reload)		return 125;
  if(data == FM_Recharge)	return 5;

  if(data == FM_Auto)		return false;
  
  if(data == FM_Damage)		return 22;
  
  if(data == FM_Slot)		return 1;
  
  if(data == FM_SpreadAdd)	return 20;
  if(data == FM_StartSpread)	return 100;
  if(data == FM_MaxSpread)	return 500;

  return Default(data);
}

/* Flammen - Dauerfeuer */

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
  if(data == BOT_Range)		return 800;
  if(data == BOT_DmgType)	return(DMG_Fire);
  if(data == BOT_Power)		return(BOT_Power_2);

  return Default(data);
}

/* Flammen - Schuss */

public func Fire1()
{
  var user = GetUser();
  var x,y; user->WeaponEnd(x,y);
  var angle = user->~AimAngle(20);
  
  var j = aSlot_Amount[0];
  for(var i = 0; i < j; i++)
  {
  	var flame = CreateObject(PSPR, x,y, GetController(user));
  	var xdir = +Sin(angle - i*4 + i * 2,65)+GetXDir(user);
  	var ydir = -Cos(angle - i*4 + i * 2,65)+GetYDir(user) - 10;


		SetXDir(xdir, flame);
		SetYDir(ydir, flame);
	}
	
	aSlot_Amount[0] = 0;
}

/* Sounds */

public func OnAutoStart(int i)
{
  Sound("FMTR_StartFire.ogg");
  Sound("FMTR_Fire.ogg",0,0,0,0,1);
}

public func OnAutoStop(int i)
{
  Sound("FMTR_EndFire.ogg");
  Sound("FMTR_Fire.ogg",0,0,0,0,-1);
}

public func OnReload()
{
  Sound("FMTR_Reload.ogg");
}

/* Allgemein */

public func OnSelect()
{
  Sound("MNGN_Charge.ogg");
}
