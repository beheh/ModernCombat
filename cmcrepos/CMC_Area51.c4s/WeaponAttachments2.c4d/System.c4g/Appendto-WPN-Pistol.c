/*-- Pistolen-Erweiterung f�r Waffenupgrades --*/

#strict 2
#appendto PSTL

func PermittedAtts()
{
  return AT_ExtendedMag | AT_Silencer | AT_Laserpointer | AT_Anthrax;
}

public func FMData1(int data)
{
  if(data == FM_AmmoLoad)
    return _inherited(data) + (iAttachment == AT_ExtendedMag)*6;		//Magazingr��e

  if(data == FM_Damage)		return 12 - (iAttachment == AT_Silencer)*((Random(10)<6)+(Random(10)<6));	//Schadenswert

  if(data == FM_SpreadAdd)	return 60 - (iAttachment == AT_Laserpointer)*4;		//Bei jedem Schuss hinzuzuaddierende Streuung
  if(data == FM_StartSpread)	return 30 - (iAttachment == AT_Laserpointer)*20;	//Bei Auswahl der Waffe gesetzte Streuung
  if(data == FM_MaxSpread)	return 220 - (iAttachment == AT_Laserpointer)*70;	//Maximaler Streuungswert
  if(data == FM_MinSpread)	return 20 - (iAttachment == AT_Laserpointer)*10;	//Kleinstm�gliche Streuung

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
  if(iAttachment != AT_Silencer)
  {
    SABulletCasing(x/3,y/3,-dir*14*(Random(1)+1),-(13+Random(2)),4);
    Sound("PSTL_Fire*.ogg", 0, ammo);
    MuzzleFlash(RandomX(35,40),user,x,y,angle,0, 4);
    Echo("PSTL_Echo.ogg");
  }
  else
  {
    Sound("WPN2_SilencerFire*.ogg", 0, ammo, 0, GetOwner(user)+1);
    Sound("WPN2_SilencerFire*.ogg", 0, ammo, 10);

    if(GetEffect("Silencer", this))
      EffectVar(0, this, GetEffect("Silencer", this)) -= BoundBy(25, 0, EffectVar(0, this, GetEffect("Silencer", this)));
  }
}
