/*-- Kanone --*/

#strict 2
#include WPN2

public func HandSize()		{return 1000;}
public func BarrelXOffset()	{return 6000;}
public func NoWeaponChoice()	{return true;}


protected func Construction()
{
  AddEffect("IntNoSound", this, 1, 250);
  return _inherited(...);
}

/* Granaten */

public func FMData1(int data)
{
  if(data == FM_Name)		return "$Grenades$";

  if(data == FM_AmmoID)		return GRAM;	//ID der Munition
  if(data == FM_ProjectileID)	return ABLT;	//ID des Projektils
  if(data == FM_AmmoLoad)	return 1;	//Magazingröße

  if(data == FM_Reload)		return 205;	//Zeit für Nachladen
  if(data == FM_Recharge)	return 10;	//Zeit bis erneut geschossen werden kann

  if(data == FM_Slot)		return 1;	//Slot des Feuermodus

  if(data == FM_SpreadAdd)	return 300;	//Bei jedem Schuss hinzuzuaddierende Streuung
  if(data == FM_StartSpread)	return 100;	//Bei Auswahl der Waffe gesetzte Streuung
  if(data == FM_MaxSpread)	return 400;	//Maximaler Streuungswert

  return Default(data);
}

public func FMData1T1(int data)
{
  if(data == FT_Name)		return "$Explosive$";

  return FMData1(data);
}

public func Fire1T1()
{
  LaunchGrenade(ABLT, 120,Contained()->~AimAngle(0,0,true));
}

public func BotData1(int data)
{
  if(data == BOT_Range)		return 500;
  if(data == BOT_DmgType)	return DMG_Explosion;
  if(data == BOT_Power)		return(BOT_Power_2);
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

  //Geschoss abfeuern
  var grenade = CreateObject(idg, x, y, GetController(user));
  SetController(GetController(user), grenade);

  grenade->SetSpeed(xdir+GetXDir(user)/5, ydir);

  //Sicht auf Geschoss
  SetPlrView(GetController(user),grenade);
  SetPlrViewRange(100, grenade);

  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 0)
  {
    CreateParticle("Thrust",x,y,GetXDir(user),GetYDir(user),80,RGBa(255,200,200,0),0,0);

    for(var i=0; i<20; ++i)
    {
      CreateParticle("Smoke",x+RandomX(-5,+5),y+RandomX(-5,+5),
      		GetXDir(user)+RandomX(0,xdir/4),GetYDir(user)+RandomX(0,ydir/4),
      		RandomX(100,160));

      CreateParticle("BlastSpark1",x+RandomX(-5,+5),y+RandomX(-5,+5),
      		GetXDir(user)+RandomX(0,xdir/4),GetYDir(user)+RandomX(0,ydir/4),
      		RandomX(60,90),RGBa(200,200,200,0),0,0);
    }
  }
  MuzzleFlash(RandomX(100,150),user,x,y,angle,0, 5);
  Sound("CNNW_Fire*.ogg", 0, grenade);
  Echo("CNNW_Echo.ogg");
}

/* Allgemein */

func OnFinishReloadStart()
{
  if(!GetEffect("IntNoSound", this))
    Sound("CNNW_ReloadStop.ogg");
}

public func OnEmpty()
{
  if(Contained() && Contained()->~IsWeaponRack())
    Contained()->~OnEmpty();
}