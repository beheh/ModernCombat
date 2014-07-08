/*-- Maschinenkanone --*/

#strict 2
#include WPN2

public func HandSize()		{return 1000;}
public func HandX()		{return 5000;}
public func HandY()		{return 600;}
public func NoWeaponChoice()	{return true;}


protected func Construction()
{
  AddEffect("IntNoSound", this, 1, 5);
  return _inherited(...);
}

/* Kugeln */

public func FMData1(int data)
{
  if(data == FM_Name)		return "$Bullets$";

  if(data == FM_AmmoID)		return GRAM;	//ID der Munition
  if(data == FM_AmmoLoad)	return 10;	//Magazingröße

  if(data == FM_Reload)		return 100;	//Zeit für Nachladen
  if(data == FM_Recharge)	return 8;	//Zeit bis erneut geschossen werden kann

  if(data == FM_Auto)		return true;	//Automatikfeuer

  if(data == FM_Damage)		return 9;	//Schadenswert

  if(data == FM_Slot)		return 1;	//Slot des Feuermodus

  if(data == FM_SpreadAdd)	return 25;	//Bei jedem Schuss hinzuzuaddierende Streuung
  if(data == FM_StartSpread)	return 100;	//Bei Auswahl der Waffe gesetzte Streuung
  if(data == FM_MaxSpread)	return 400;	//Maximaler Streuungswert

  return Default(data);
}

/* Kugeln - Automatikfeuer */

public func FMData1T1(int data)
{
  if(data == FT_Name)		return "$Auto$";

  return FMData1(data);
}

public func Fire1T1()
{
  LaunchGrenade(ERND, 100+Random(40),Contained()->~AimAngle(0,0,true));
}

public func BotData1(int data)
{
  if(data == BOT_Range)		return 500;

  return Default(data);
}

/* Granaten - Explosivgranaten */

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
    SetPlrViewRange(150, grenade);
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
  Sound("LCAC_Fire.ogg", 0, grenade);
  Echo("LCAC_Echo.ogg");
}

/* Allgemein */

func OnReload()
{
  if(!GetEffect("IntNoSound", this))
    Sound("LCAC_Reload.ogg", false, this);
}

public func OnEmpty()
{
  if(Contained() && Contained()->~IsWeaponRack())
    Contained()->~OnEmpty();
}

public func StopAutoFire()
{
  //Nur wenn er auch wirklich feuert
  if(IsRecharging() && !IsReloading())
    return _inherited(...);
}