/*-- Slingshot --*/

#strict 2
#include WPN2

local casings;

public func HandSize()		{return 1050;}
public func HandX()		{return 4000;}
public func HandY()		{return 1000;}
public func BarrelXOffset()	{return -1000;}
public func BarrelYOffset()	{return -2000;}
public func IsPrimaryWeapon()	{return true;}

public func SelectionTime()	{return 42;}	//Anwahlzeit


/* Kompatible Waffenaufsätze */

func PermittedAtts()
{
  return AT_ExtendedMag | AT_Bayonet | AT_Laserpointer | AT_Flashlight;
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

/* Granaten */

public func FMData1(int data)
{
  if(data == FM_Name)		return "$Grenades$";

  if(data == FM_AmmoID)		return GRAM;						//ID der Munition
  if(data == FM_AmmoLoad)	return 8 + (iAttachment == AT_ExtendedMag)*2;		//Magazingröße

  if(data == FM_SingleReload)	return 1;						//Zeit des einzelnen Nachladens bei Revolversystemen
  if(data == FM_PrepareReload)	return 25;						//Zeit bevor das eigentliche Nachladen beginnt
  if(data == FM_FinishReload)	return 30;						//Zeit nach dem Nachladen

  if(data == FM_Reload)		return 210 + (iAttachment == AT_ExtendedMag) * 52;	//Zeit für Nachladen
  if(data == FM_Recharge)	return 60;						//Zeit bis erneut geschossen werden kann

  if(data == FM_Damage)		return 20;						//Schadenswert

  if(data == FM_SpreadAdd)	return 270 - (iAttachment == AT_Laserpointer)*5;	//Bei jedem Schuss hinzuzuaddierende Streuung
  if(data == FM_StartSpread)	return 80 - (iAttachment == AT_Laserpointer)*20;	//Bei Auswahl der Waffe gesetzte Streuung
  if(data == FM_MaxSpread)	return 400 - (iAttachment == AT_Laserpointer)*50;	//Maximaler Streuungswert

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
  LaunchGrenade(ESHL, 120,Contained()->~AimAngle(0,0,true));
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
  LaunchGrenade(FSHL, 120,Contained()->~AimAngle(0,0,true));
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
  LaunchGrenade(SSHL, 120,Contained()->~AimAngle(0,0,true));
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
  var user = Contained();
  var dir = GetDir(user)*2-1;
  var x,y;
  user->WeaponEnd(x,y);

  //Anpassung des Winkels
  angle = BoundBy(angle/*+GetYDir(user)*/+dir,-360,360);
  //Geschwindigkeit einstellen
  var xdir = Sin(angle,speed);
  var ydir = -Cos(angle,speed);

  //Granate abfeuern
  var grenade = CreateObject(idg, x, y, GetController(user));
  SetController(GetController(user), grenade);
  grenade->Launch(xdir+GetXDir(user)/5, ydir/*+GetYDir(user)/4*/, GetFMData(FM_Damage,2), 0, 0, 0, iAttachment, user);

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
  Sound("SGST_Fire*.ogg", 0, grenade);
  Echo("SGST_Echo.ogg");
  Schedule("Sound(\"SGST_Pump.ogg\")", 30);

  //Klickgeräusch bei wenig Munition
  if(Inside(GetAmmo(GetFMData(FM_AmmoID)), 0, 1))
  {
    Sound("PPGN_Empty.ogg", 0, this, 0, GetOwner(user)+1);
    Sound("PPGN_Click.ogg", 0, grenade, 0, GetOwner(user)+1);
  }
  else
    if(Inside(GetAmmo(GetFMData(FM_AmmoID)), 1, GetFMData(FM_AmmoLoad)/3))
      Sound("PPGN_Click.ogg", 0, grenade, 0, GetOwner(user)+1);

  //Patronenhülse hinzufügen
  casings++;
}

/* Laserpointer */

func FxLaserDotTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  //Nutzer festlegen
  var user = this->~GetUser();
  var x, y, z;
  if(!user || !user->~IsClonk() && !user->~IsWeaponRack() || !user->WeaponAt(x, y, z) || !user->IsAiming() || Contents(0, user) != this || iAttachment != AT_Laserpointer)
  {
    RemoveTrajectory(pTarget);
    return;
  }

  var iAngle = EffectVar(1, user, GetEffect("ShowWeapon", user));
  var empty = IsReloading() || !GetCharge();
  AddTrajectory(pTarget, GetX(pTarget), GetY(pTarget), Sin(iAngle, 120), -Cos(iAngle, 120), 35*3, RGB(255*empty, 255*(!empty), 0));
}

/* Nachladen */

func OnReload()
{
  Sound("SGST_ReloadStart.ogg");

  var j = casings;
  for(var i; i = j; j--)
  {
    var user = GetUser();
    var dir = GetDir(user)*2-1;
    BulletCasing(dir*1,0,-dir*14*(Random(1)+1),-(10+Random(2)),6,RGB(255,0,0));
  }
  casings = 0;
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

public func ReloadAnimation()	{return true;}
public func MaxReloadRotation()	{return 17;}

/* Allgemein */

func OnSelect()
{
  Sound("SGST_Charge.ogg");
}