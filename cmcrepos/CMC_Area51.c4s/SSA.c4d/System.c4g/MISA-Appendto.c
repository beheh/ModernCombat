/*-- Minigun-Appendto --*/

#strict 2
#appendto MISA

public func Fire1()
{
  //Austritt bestimmen
  var user = GetUser();
  var angle = user->AimAngle();
  var dir = GetDir(user)*2-1;
  var iX;
  var iY;
  user->WeaponEnd(iX,iY);

  //Kugel abfeuern
  var ammo = CreateObject(SHTX, iX, iY, GetController(user));
  ammo->CustomLaunch(angle, 350, 650, 2, GetFMData(FM_Damage) * 10, GetFMData(FM_Damage));

  //var ammo = CreateObject(SHTX, iX, iY, GetController(user));
  //ammo->Launch(angle, 300, 600, 2, 100, 3, 1);

  //Effekte
  MuzzleFlash(40+Random(60),user,iX,iY,angle);
  BulletCasing(dir*1,0,-dir*14*(Random(1)+1),-(13+Random(2)),7,0,true);
  Sound("MISA_Fire.ogg",0,ammo);
  Echo("ACCN_Echo.ogg");
}
