/*-- Maschinenpistolen-Erweiterung für Waffenupgrades --*/

#strict 2
#appendto SMGN

public func FMData1(int data)
{
  if(data == FM_AmmoLoad)	return 30 + (iAttachment == AT_ExtendedMag)*6;	//Magazingröße

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
  var ammo = SALaunchBullet(x,y,GetController(user),angle,270,550,GetFMData(FM_Damage));

  //Effekte
  SABulletCasing(x/3,y/3,-dir*14,-(14),4);
  if(iAttachment != AT_Silencer)
  {
    Sound("SMGN_Fire*.ogg", 0, ammo);
    MuzzleFlash(RandomX(30,35),user,x,y,angle,0, 4);
    Echo("SMGN_Echo.ogg");
  }
  else
    Sound("PSTL_TracerFire.ogg", 0, ammo);
}
