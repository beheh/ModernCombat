/*-- Slingshot --*/

#strict 2
#include WPN2

public func HandSize()		{return 1050;}
public func HandX()		{return 4000;}
public func HandY()		{return 1000;}
public func BarrelXOffset()	{return -1000;}
public func BarrelYOffset()	{return -2000;}
public func IsPrimaryWeapon()	{return true;}

public func SelectionTime()	{return 42;}	//Anwahlzeit


/* Nahkampfangriff */

public func GetMCData(int data)
{
  if(data == MC_CanStrike)	return 1;	//Waffe kann Kolbenschlag ausführen
  if(data == MC_Damage)		return 20;	//Schaden eines Kolbenschlages
  if(data == MC_Recharge)	return 45;	//Zeit nach Kolbenschlag bis erneut geschlagen oder gefeuert werden kann
  if(data == MC_Power)		return 20;	//Wie weit das Ziel durch Kolbenschläge geschleudert wird
  if(data == MC_Angle)		return 45;	//Mit welchem Winkel das Ziel durch Kolbenschläge geschleudert wird
}

/* Granaten */

public func FMData1(int data)
{
  if(data == FM_Name)		return "$Grenades$";

  if(data == FM_AmmoID)		return GRAM;	//ID der Munition
  if(data == FM_AmmoLoad)	return 8;	//Magazingröße

  if(data == FM_SingleReload)	return 1;	//Zeit des einzelnen Nachladens bei Revolversystemen
  if(data == FM_PrepareReload)	return 30;	//Zeit bevor das eigentliche Nachladen beginnt
  if(data == FM_FinishReload)	return 35;	//Zeit nach dem Nachladen

  if(data == FM_Recharge)	return 50;	//Zeit bis erneut geschossen werden kann

  if(data == FM_Damage)		return 20;	//Schadenswert

  if(data == FM_SpreadAdd)	return 200;	//Bei jedem Schuss hinzuzuaddierende Streuung
  if(data == FM_StartSpread)	return 80;	//Bei Auswahl der Waffe gesetzte Streuung
  if(data == FM_MaxSpread)	return 400;	//Maximaler Streuungswert

  return Default(data);
}

/* Granaten - Explosivgranaten */

public func FMData1T1(int data)
{
  if(data == FT_Name)		return "$Explosive$";

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

/* Granaten - Splittergranaten */

public func FMData1T2(int data)
{
  if(data == FT_Name)		return "$Cluster$";

  if(data == FM_Damage)		return FMData1(data) - 15;	//Schadenswert
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

  if(data == FM_Damage)		return FMData1(data) - 15;	//Schadenswert

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
  if(!Stuck(grenade)) SetPosition(GetX(grenade)+xdir/10,GetY(grenade)+ydir/10,grenade);
  SetController(GetController(user), grenade);
  grenade->Launch(xdir+GetXDir(user)/5, ydir/*+GetYDir(user)/4*/, GetFMData(FM_Damage,2));

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
      		RandomX(100,160),RGBa(200,200,200,0),0,0);

      CreateParticle("BlastSpark1",x+RandomX(-5,+5),y+RandomX(-5,+5),
      		GetXDir(user)+RandomX(0,xdir/4),GetYDir(user)+RandomX(0,ydir/4),
      		RandomX(60,90),RGBa(200,200,200,0),0,0);
    }
  }
  Sound("SGST_Fire.ogg", 0, grenade);
  Echo("SGST_Echo.ogg");
  Schedule("Sound(\"SGST_Pump.ogg\")", 5);
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

/* Handeffekt */

public func HandR()
{
  var effect = IsReloading();
  if(effect)
    return -BoundBy(GetEffect(0,this,effect,6)*1,0,17);
}

/* Allgemein */

func OnSelect()
{
  Sound("SGST_Charge.ogg");
}