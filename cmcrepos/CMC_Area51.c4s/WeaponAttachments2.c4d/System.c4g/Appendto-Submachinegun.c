/*-- Maschinenpistolen-Erweiterung für Waffenupgrades --*/

#strict 2
#appendto SMGN

func Initialize()
{
   iPermittedAtts = AT_ExtendedMag | AT_Laserpointer | AT_Silencer | AT_Foregrip;
   return _inherited(...);
}

public func FMData1(int data)
{
  if(data == FM_AmmoLoad)	return 30 + (iAttachment == AT_ExtendedMag)*6;	//Magazingröße

  if(data == FM_SpreadAdd)	return 20 - (iAttachment == AT_Foregrip)*5;	//Bei jedem Schuss hinzuzuaddierende Streuung
  if(data == FM_StartSpread)	return 110 - (iAttachment == AT_Foregrip)*10;	//Bei Auswahl der Waffe gesetzte Streuung
  if(data == FM_MaxSpread)	return 410 - (iAttachment == AT_Foregrip)*10;	//Maximaler Streuungswert
  if(data == FM_MinSpread)	return 60 - (iAttachment == AT_Foregrip)*10;	//Kleinstmögliche Streuung

  return _inherited(data);
}

public func Fire1()
{
  //Austritt bestimmen
  var user = GetUser();
  var dir = GetDir(user)*2-1;
  var angle = user->AimAngle(15,0,true);
  var x,y;
  user->WeaponEnd(x,y);

  //Kugel abfeuern
  var ammo = SALaunchBullet(x,y,GetController(user),angle,270,550,GetFMData(FM_Damage), 0, 0, iAttachment == AT_Silencer);

  //Effekte
  SABulletCasing(x/3,y/3,-dir*14,-(14),4);
  if(iAttachment != AT_Silencer)
  {
    Sound("SMGN_Fire*.ogg", 0, ammo);
    MuzzleFlash(RandomX(30,35),user,x,y,angle,0, 4);
    Echo("SMGN_Echo.ogg");
  }
  else
  {
    Sound("WPN2_SilencedFire.ogg", 0, ammo, 100);
    Smoke(x,y,1);
  }
}
