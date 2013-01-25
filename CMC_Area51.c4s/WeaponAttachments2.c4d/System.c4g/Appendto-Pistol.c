/*-- Pistolen-Erweiterung für Waffenupgrades --*/

#strict 2
#appendto PSTL


func Initialize()
{
   iPermittedAtts = AT_Silencer | AT_Laserpointer | AT_TracerDart;
   return _inherited(...);
}

public func FMData2(int data)
{
  if(iAttachment != AT_TracerDart) return 0;
  return _inherited(data);
}

public func Fire1()
{
  //Austritt bestimmen
  var user = GetUser();
  var dir = GetDir(user)*2-1;
  var angle = user->AimAngle(20,0,true);
  var x,y;
  user->WeaponEnd(x,y);

  //Kugel abfeuern
  var ammo = SALaunchBullet(x,y,GetController(user),angle,250,400,GetFMData(FM_Damage), 0, 0, iAttachment == AT_Silencer);

  //Effekte
  SABulletCasing(x/3,y/3,-dir*14*(Random(1)+1),-(13+Random(2)),4);
  if(iAttachment != AT_Silencer)
  {
    Sound("PSTL_Fire*.ogg", 0, ammo);
    MuzzleFlash(RandomX(35,40),user,x,y,angle,0, 4);
    Echo("PSTL_Echo.ogg");
  }
  else
  {
    Sound("WPN2_SilencedFire.ogg", 0, ammo, 1);
    Smoke(x,y,1);
  }
}