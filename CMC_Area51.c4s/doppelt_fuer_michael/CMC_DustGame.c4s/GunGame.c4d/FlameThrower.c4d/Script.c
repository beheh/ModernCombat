/*-- Flammenwerfer --*/

#strict 2
#include WPN2

local fired;

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
  if(data == FM_AmmoLoad)	return 50;

  if(data == FM_Reload)		return 125;
  if(data == FM_Recharge)	return 5;

  if(data == FM_Auto)		return true;
  
  if(data == FM_Damage)		return 22;
  
  if(data == FM_Slot)		return 1;
  
  if(data == FM_SpreadAdd)	return 20;
  if(data == FM_StartSpread)	return 100;
  if(data == FM_MaxSpread)	return 500;

  if(data == FM_NoAmmoModify)	return 1;

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
  var flame = CreateObject(FFLM, x,y, GetController(user));
  var xdir = +Sin(angle,35)+GetXDir(user);
  var ydir = -Cos(angle,35)+GetYDir(user);

  flame -> Launch(xdir,ydir,0, GetFMData(FM_Damage, 1));
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

private func Check()
{
  if(!fired)
    return;

  if(Contained() && Contained()->~IsClonk())
  {
    if(Contained()->~IsAiming() && Contents(0, Contained()) == this)
    	return;
    ThrowAway();
  }
  else
    ThrowAway();
}

public func ThrowAway()
{
  //Kategorie wechseln
  SetCategory(C4D_Vehicle); 
  //Waffe auswerfen sofern verschachtelt
  if(Contained())
  {
    //Schützen verlassen
    var dir = +1;
    if(GetDir(GetUser()) == DIR_Right)
      dir = -1;

    Exit(0, 0, 0, Random((360)+1), dir,-3, Random(11)-5);

    Sound("AT4R_ThrowAway.ogg");
  }

  //Verschwinden
  FadeOut();
  
  return true;
}

private func OnEmpty()
{
  fired = true;
}

public func RejectEntrance()
{
  return fired;
}
