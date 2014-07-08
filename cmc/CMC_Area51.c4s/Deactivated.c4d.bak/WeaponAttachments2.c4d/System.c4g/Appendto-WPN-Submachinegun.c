/*-- Maschinenpistolen-Erweiterung für Waffenupgrades --*/

#strict 2
#appendto SMGN

func PermittedAtts()
{
  return AT_ExtendedMag | AT_Laserpointer | AT_Silencer | AT_Flashlight;
}

public func FMData1(int data)
{
  if(data == FM_Reload)		return _inherited(data) + (iAttachment == AT_ExtendedMag)*16;	//Zeit für Nachladen

  if(data == FM_AmmoLoad)	return 30 + (iAttachment == AT_ExtendedMag)*9;			//Magazingröße

  if(data == FM_SpreadAdd)	return 20 - (iAttachment == AT_Laserpointer)*4;			//Bei jedem Schuss hinzuzuaddierende Streuung
  if(data == FM_StartSpread)	return 110 - (iAttachment == AT_Laserpointer)*20;		//Bei Auswahl der Waffe gesetzte Streuung
  if(data == FM_MaxSpread)	return 410 - (iAttachment == AT_Laserpointer)*110;		//Maximaler Streuungswert
  if(data == FM_MinSpread)	return 60 - (iAttachment == AT_Laserpointer)*20;		//Kleinstmögliche Streuung

  if(data == FM_Damage)		return 6 - (iAttachment == AT_Silencer)*(Random(10)<6);		//Schadenswert

  return _inherited(data);
}

public func FMData1T2(int data)
{
  if(data == FM_MinSpread)	return FMData1(data) - (iAttachment == AT_Laserpointer)*10;	//Kleinstmögliche Streuung

  return _inherited(data);
}

public func FMData1T3(int data)
{
  if(data == FM_MinSpread)	return FMData1(data) - (iAttachment == AT_Laserpointer)*20;	//Kleinstmögliche Streuung

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
  if(iAttachment != AT_Silencer)
  {
    SABulletCasing(x/3,y/3,-dir*14,-(14),4);
    Sound("SMGN_Fire*.ogg", 0, ammo);
    MuzzleFlash(RandomX(30,35),user,x,y,angle,0, 4);
    Echo("SMGN_Echo.ogg");
  }
  else
  {
    Sound("WPN2_SilencerFire*.ogg", 0, ammo, 0, GetOwner(user)+1);
    Sound("WPN2_SilencerFire*.ogg", 0, ammo, 10);
    
    if(GetEffect("Silencer", this))
      EffectVar(0, this, GetEffect("Silencer", this)) -= BoundBy(10, 0, EffectVar(0, this, GetEffect("Silencer", this)));
  }

  //Klickgeräusch bei wenig Munition
  if(Inside(GetAmmo(GetFMData(FM_AmmoID)), 1, GetFMData(FM_AmmoLoad)/3))
    Sound("MNGN_Click.ogg", 0, ammo, 0, GetOwner(user)+1);
}

func OnReload()
{
  if(iAttachment == AT_ExtendedMag)
    Sound("SMGN_Reload2.ogg");
  else
    Sound("SMGN_Reload.ogg");
}