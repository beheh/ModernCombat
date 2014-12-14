/*-- Armbrust --*/

#strict 2
#include WPN2

local drawing;

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
  return AT_Laserpointer | AT_Flashlight;
}

/* Nahkampfangriff */

public func GetMCData(int data)
{
  if(data == MC_CanStrike)	return 1;	//Waffe kann Kolbenschlag ausführen
  if(data == MC_Damage)		return 10;	//Schaden eines Kolbenschlages
  if(data == MC_Recharge)	return 45;	//Zeit nach Kolbenschlag bis erneut geschlagen oder gefeuert werden kann
  if(data == MC_Power)		return 0;	//Wie weit das Ziel durch Kolbenschläge geschleudert wird
}

/* Bolzen */

public func FMData1(int data)
{
  if(data == FM_Name)		return "$Bolts$";

  if(data == FM_AmmoID)		return GRAM;						//ID der Munition
  if(data == FM_AmmoLoad)	return 1;						//Magazingröße

  if(data == FM_Reload)		return 16;						//Zeit für Nachladen
  if(data == FM_Recharge)	return 14;						//Zeit bis erneut geschossen werden kann

  if(data == FM_Damage)		return 20;						//Schadenswert

  if(data == FM_SpreadAdd)	return 10 - (iAttachment == AT_Laserpointer)*5;		//Bei jedem Schuss hinzuzuaddierende Streuung
  if(data == FM_StartSpread)	return 20 - (iAttachment == AT_Laserpointer)*10;	//Bei Auswahl der Waffe gesetzte Streuung
  if(data == FM_MaxSpread)	return 100 - (iAttachment == AT_Laserpointer)*50;	//Maximaler Streuungswert

  return Default(data);
}

/* Bolzen - EMP-Bolzen */

public func FMData1T1(int data)
{
  if(data == FT_Name)		return "$EMPBolt$";

  return FMData1(data);
}

public func Fire()
{
  if(!drawing && CheckAmmo(FMData1(FM_AmmoID),1,this) && Contained()->~ReadyToFire())
  {
    Sound("ABOW_Bend.ogg");
    AddEffect("Drawing", this, 1, 1, this);
    return true;
  }
  else return _inherited(...);
}

public func Fire1T1()
{
  LaunchBolt(EPBT, 60 + drawing*2,Contained()->~AimAngle(0,0,true));
  RemoveEffect("Drawing", this);
  drawing = 0;
}

func FxDrawingTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  if(!Contained(pTarget) || Contained(pTarget)->Contents() != pTarget || !Contained(pTarget)->~ReadyToFire())
  {
    drawing = 0;
    PlayerMessage(GetOwner(), "", this);
    return -1;
  }

  drawing += 2;
  drawing = Min(100, drawing);
  PlayerMessage(GetOwner(), Format("%d%", drawing), this);
}

public func ControlDig()
{
  if(drawing)
  {
    RemoveEffect("Drawing", this);
    PlayerMessage(GetOwner(), "", this);
    drawing = 0;
    return true;
  }
  else
    return _inherited(...);
}

public func BotData1(int data)
{
  if(data == BOT_Range)		return 500;
  if(data == BOT_DmgType)	return DMG_Explosion;
  if(data == BOT_Power)		return(BOT_Power_2);

  return Default(data);
}

/* Bolzen - Schuss */

public func LaunchBolt(id idg, int speed, int angle, int mode)
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

  //Bolzen abfeuern
  var bolt=CreateObject(idg, x, y, GetController(user));
  SetController(GetController(user), bolt);
  bolt->Launch(xdir+GetXDir(user)/5, ydir/*+GetYDir(user)/4*/, GetFMData(FM_Damage,2));

  //Sicht auf Bolzen wenn der Schütze zielt
  if(!(user->~IsMachine()) && user->~IsAiming())
  {
    SetPlrView(GetController(user),bolt);
    SetPlrViewRange(100, bolt);
  }

  //Effekte
  Sound("ABOW_Fire*.ogg", 0, bolt);
}

/* Laserpointer */

func FxLaserDotTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  //Nutzer festlegen
  var user = this->~GetUser();
  var x, y, z;
  if(!user || !user->~IsClonk() || !user->WeaponAt(x, y, z) || !user->IsAiming() || Contents(0, user) != this || iAttachment != AT_Laserpointer)
  {
    RemoveTrajectory(pTarget);
    return;
  }

  var iAngle = EffectVar(1, user, GetEffect("ShowWeapon", user));
  var empty = IsReloading() || !GetCharge();
  AddTrajectory(pTarget, GetX(pTarget), GetY(pTarget), Sin(iAngle, 60 + drawing*2), -Cos(iAngle, 60 + drawing*2), 35*3, RGB(255*empty, 255*(!empty), 0));
}

/* Nachladen */

func OnReload()
{
  Sound("ABOW_Reload.ogg");

  //Eventuelles Spannen einstellen
  if(drawing)
  {
    RemoveEffect("Drawing", this);
    PlayerMessage(GetOwner(), "", this);
    drawing = 0;
    return true;
  }
}

/* Handeffekt */

public func ReloadAnimation()	{return true;}
public func MaxReloadRotation()	{return 17;}

/* Allgemein */

func OnSelect()
{
  if(GetAmmo())
    Sound("ABOW_Charge1.ogg");
  else
    Sound("ABOW_Charge2.ogg");
}