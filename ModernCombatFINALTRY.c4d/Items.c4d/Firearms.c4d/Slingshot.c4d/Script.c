/*-- Slingshot --*/

#strict 2
#include WPN2

public func HandSize()		{return 1050;}
public func HandX()		{return 4000;}
public func HandY()		{return 1000;}
public func BarrelXOffset()	{return -1000;}
public func BarrelYOffset()	{return -2000;}
public func SelectionTime()	{return 42;}


/* Nahkampfangriff */

public func GetMCData(int data)
{
  if(data == MC_CanStrike)	return 1;
  if(data == MC_Damage)		return 20;
  if(data == MC_Recharge)	return 45;
  if(data == MC_Power)		return 20;
  if(data == MC_Angle)		return 45;
}

/* Granaten - Explosivgranaten */

public func FMData1(int data)
{
  if(data == FM_Name)		return "$Grenades$";

  if(data == FM_AmmoID)		return GRAM;
  if(data == FM_AmmoLoad)	return 8;

  if(data == FM_Recharge)	return 50;

  if(data == FM_SingleReload)	return 1;
  if(data == FM_Reload)		return 210;
  if(data == FM_PrepareReload)	return 30;
  if(data == FM_FinishReload)	return 35;

  if(data == FM_Damage)		return 20;

  if(data == FM_SpreadAdd)	return 200;
  if(data == FM_StartSpread)	return 80;
  if(data == FM_MaxSpread)	return 400;

  return Default(data);
}

public func FMData1T1(int data)
{
  if(data == FT_Name)		return "$Explosive$";
  if(data == FM_Icon)		return ESHL;

  return FMData1(data);
}

public func Fire1T1()
{
  LaunchGrenade(ESHL, 100+Random(40),Contained()->~AimAngle(0,0,true));
}

public func BotData1(int data)
{
  if(data == BOT_Range)		return 500;
  if(data == BOT_DmgType)	return DMG_Explosion;
  if(data == BOT_Power)		return(BOT_Power_2);
  return Default(data);
}

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
  if(!Stuck(grenade)) SetPosition(GetX(grenade)+xdir/10,GetY(grenade)+ydir/10,grenade);
  SetController(GetController(user), grenade);
  grenade->Launch(xdir/*+GetXDir(user)/2*/, ydir/*+GetYDir(user)/4*/, GetFMData(FM_Damage,2));
  
  //Sicht auf Granate wenn der Schütze zielt
  if(!(user ->~ IsMachine()) && user->~IsAiming())
  {
    SetPlrView(GetController(user),grenade);
    SetPlrViewRange(100, grenade);
  }

  //Effekte
  Schedule("Sound(\"SGST_Pump.ogg\")", 5);
  if(GetEffectData(EFSM_ExplosionEffects) > 0)
  {
    CreateParticle("Thrust",x,y,GetXDir(user),GetYDir(user),80,RGBa(255,200,200,0),0,0);

    for(var i=0; i<20; ++i)
    {
      CreateParticle("Smoke2",x+RandomX(-5,+5),y+RandomX(-5,+5),
      		GetXDir(user)+RandomX(0,xdir/4),GetYDir(user)+RandomX(0,ydir/4),
      		RandomX(100,160),RGBa(200,200,200,0),0,0);

      CreateParticle("BlastSpark1",x+RandomX(-5,+5),y+RandomX(-5,+5),
      		GetXDir(user)+RandomX(0,xdir/4),GetYDir(user)+RandomX(0,ydir/4),
      		RandomX(60,90),RGBa(200,200,200,0),0,0);
    }
  }
  Sound("SGST_Fire.ogg", 0, grenade);
}

/* Granaten - Splittergranaten */

public func FMData1T2(int data)
{
  if(data == FT_Name)		return "$Cluster$";
  if(data == FM_Icon)		return FSHL;

  if(data == FM_Damage)		return 5;

  return FMData1(data);
}

public func Fire1T2()
{
  LaunchGrenade(FSHL, 100+Random(40),Contained()->~AimAngle(0,0,true));
}

public func BotData2(int data)
{
  if(data == BOT_Range)		return 500;

  return Default(data);
}

/* Granaten - Rauchgranaten */

public func FMData1T3(int data)
{
  if(data == FT_Name)		return "$Smoke$";
  if(data == FM_Icon)		return SSHL;

  if(data == FM_Damage)		return 5;
  return FMData1(data);
}

public func Fire1T3()
{
  LaunchGrenade(SSHL, 100+Random(40),Contained()->~AimAngle(0,0,true));
}

public func BotData3(int data)
{
  if(data == BOT_Range)		return 500;

  return Default(data);
}

/* Nachladen */

func OnReload()
{
  Sound("SGST_ReloadStart.ogg");
}

func OnSingleReloadStart()
{
  Sound("SGST_Loading.ogg");
}

func OnFinishReloadStart()
{
  Sound("SGST_ReloadStop.ogg");
}

/* Allgemein */

func OnSelect()
{
  Sound("SGST_Charge.ogg");
}