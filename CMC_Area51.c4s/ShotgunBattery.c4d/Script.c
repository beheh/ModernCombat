/*-- Schrotbatterie --*/

#strict 2
#include WPN2

public func HandSize()		{return 1000;}
public func HandX()		{return 5000;}
public func HandY()		{return 600;}
public func BarrelXOffset()	{return -1000;}
public func BarrelYOffset()	{return -2000;}
public func NoWeaponChoice()	{return true;}


protected func Construction()
{
  AddEffect("IntNoSound", this, 1, 5);
  return _inherited(...);
}

/* Kugeln */

public func FMData1(int data)
{
  if(data == FM_Name)		return "$Battery$";

  if(data == FM_AmmoID)		return STAM;	//ID der Munition
  if(data == FM_AmmoLoad)	return 50;	//Magazingröße
  if(data == FM_AmmoUsage)	return 10;	//Munition pro Schuss

  if(data == FM_Reload)		return 250;	//Zeit für Nachladen
  if(data == FM_Recharge)	return 10;	//Zeit bis erneut geschossen werden kann

  if(data == FM_Damage)		return 20;	//Schadenswert

  if(data == FM_Slot)		return 1;	//Slot des Feuermodus

  if(data == FM_SpreadAdd)	return 150;	//Bei jedem Schuss hinzuzuaddierende Streuung
  if(data == FM_StartSpread)	return 80;	//Bei Auswahl der Waffe gesetzte Streuung
  if(data == FM_MaxSpread)	return 600;	//Maximaler Streuungswert

  return Default(data);
}

/* Kugeln - Streuschuss */

public func FMData1T1(int data)
{
  if(data == FT_Name)		"$SpreadShot$";

  return FMData1(data);
}

public func Fire1T1()
{
  Fire1();
}

public func BotData1(int data)
{
  if(data == BOT_Range)		return 200;
  if(data == BOT_Power)		return(BOT_Power_3);
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

  //Splitter abfeuern
  var i = 0;
  while(i < GetFMData(FM_AmmoUsage, 1))
  {
    var ammo = CreateObject(SHRP,x+RandomX(-10,10),y+RandomX(-10,10),GetController(user));
    ammo->Launch(angle+RandomX(-5,5),150+Random(50),400+Random(50),3,50,GetFMData(FM_Damage, 1));
    i++;
  }

  //Effekte
  MuzzleFlash(RandomX(80,100),user,x,y,angle,0, 5);
  CastParticles("BlastDirt",8,20,0,0,300,600);
  Sound("SBYW_Fire.ogg", 0, ammo);
  Echo("PPGN_Echo*.ogg");
}

/* Allgemein */

func OnReload()
{
  if(!GetEffect("IntNoSound", this))
    Sound("SBYW_Reload.ogg", false, this);
}

public func OnEmpty()
{
  if(Contained() && Contained()->~IsWeaponRack())
    Contained()->~OnEmpty();
}