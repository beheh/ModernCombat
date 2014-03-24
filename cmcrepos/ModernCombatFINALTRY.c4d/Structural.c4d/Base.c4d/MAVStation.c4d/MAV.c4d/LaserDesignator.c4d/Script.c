/*-- Lasermarkierer --*/

#strict 2

#include WPN2

public func HandSize()		{return 1000;}
public func HandX()		{return 1000;}
public func HandY()		{return -2500;}
public func NoWeaponChoice()	{return true;}


protected func Construction()
{
  AddEffect("IntNoSound", this, 1, 5);
  return _inherited(...);
}

/* Raketen - Optische Steuerung */

public func FMData1(int data)
{
  if(data == FM_Name)		return "$Missiles$";

  if(data == FM_AmmoID)		return MIAM;	//ID der Munition
  if(data == FM_ProjectileID)	return AGMS;	//ID des Projektils
  if(data == FM_AmmoLoad)	return 1;	//Magazingröße

  if(data == FM_Reload)		return 400;	//Zeit für Nachladen
  if(data == FM_Recharge)	return 10;	//Zeit bis erneut geschossen werden kann

  if(data == FM_Slot)		return 1;	//Slot des Feuermodus

  if(data == FM_SpreadAdd)	return 300;	//Bei jedem Schuss hinzuzuaddierende Streuung
  if(data == FM_StartSpread)	return 100;	//Bei Auswahl der Waffe gesetzte Streuung
  if(data == FM_MaxSpread)	return 400;	//Maximaler Streuungswert

  return Default(data);
}

public func FMData1T1(int data)
{
  if(data == FT_Name)		return "$Laser$";

  return FMData1(data);
}

public func Fire1T1()
{
  LaunchRocket(AGMS,Angle(GetX(), 0, GetX(Contained()->GetLaser()), GetY(Contained()->GetLaser())));
}

public func BotData1(int data)
{
  if(data == BOT_Range)		return 600;
  if(data == BOT_DmgType)	return DMG_Explosion;

  return Default(data);
}

/* Raketen - Abschuss */

public func LaunchRocket(id rid, int angle)
{
  //Austritt bestimmen
  var user = Contained();
  var x,y;
  user->WeaponEnd(x,y);

  //Rakete abfeuern
  var rocket = CreateObject(rid,x,-GetY()-450,GetController(user));
  AddEffect("IntHeliProtection", rocket, 1, 20, 0, BKHK, LocalN("heli", Contained()));
  rocket->Launch(angle, user);
  rocket->Sound("SATW_Launch.ogg");
  SetController(GetController(), rocket);

  Sound("JetConfirm*.ogg",0,0,0,GetOwner(user)+1);
  Sound("JetFlyBy*.ogg",0,rocket);
  rocket->Echo("RTLR_Echo.ogg");
}

/* Allgemein */

public func OnEmpty()
{
  if(Contained() && Contained()->~IsWeaponRack())
    Contained()->~OnEmpty();
}

private func Reloaded(caller,slot,amount)
{
  //Munitionsart identifizieren
  var ammoid = FMData1(FM_AmmoID);

  //Munitionsmenge feststellen
  amount = Min(amount,MaxReloadAmount(caller));

  //Munitionsmenge an Waffe übergeben
  DoAmmo2(slot, ammoid, amount, this);
  //Munition aus Clonk entfernen
  DoAmmo(ammoid, -amount, caller);

  //Geladene Munitionsmenge angeben
  if(GetAction(Contained()) == "Flying")
  {
    PlayerMessage(GetOwner(Contained()), "$Reloaded$", Contained());
    Sound("JetReporting*.ogg",0,0,0,GetOwner(Contained())+1);
  }
}