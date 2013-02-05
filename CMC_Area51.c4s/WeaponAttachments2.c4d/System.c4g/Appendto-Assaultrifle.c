/*-- Sturmgewehr-Erweiterung für Waffenupgrades --*/

#strict 2
#appendto ASTR

func PermittedAtts()
{
  return AT_ExtendedMag | AT_Bayonet | AT_Laserpointer | AT_Silencer | AT_Foregrip | AT_GrenadeLauncher;
}

public func FMData1(int data)
{
  if(data == FM_AmmoLoad)	
    return _inherited(data) + (iAttachment == AT_ExtendedMag)*3;	//Magazingröße

  if(data == FM_SpreadAdd)	return 50 - (iAttachment == AT_Foregrip)*5;	//Bei jedem Schuss hinzuzuaddierende Streuung
  if(data == FM_StartSpread)	return 100 - (iAttachment == AT_Foregrip)*10;	//Bei Auswahl der Waffe gesetzte Streuung
  if(data == FM_MaxSpread)	return 450 - (iAttachment == AT_Foregrip)*10;	//Maximaler Streuungswert

  return _inherited(data);
}

public func GetMCData(int data)
{
  if(data == MC_Damage)		
    return _inherited(data) + (iAttachment == AT_Bayonet)*6;	//Schaden eines Kolbenschlages
  
  return _inherited(data);
}

public func FMData2(int data)
{
  if(iAttachment != AT_GrenadeLauncher) return 0;
  return _inherited(data);
}

public func Fire1()
{
  //Austritt bestimmen
  var user = GetUser();
  var dir = GetDir(user)*2-1;
  var angle = user->AimAngle(10,0,true);
  var x,y;
  user->WeaponEnd(x,y);

  //Kugel abfeuern
  var ammo = SALaunchBullet(x,y,GetController(user),angle,270,800,GetFMData(FM_Damage), 0, 0, iAttachment == AT_Silencer);

  //Effekte
  SABulletCasing(x/3,y/3,-dir*14*(Random(1)+1),-(13+Random(2)),5);
  if(iAttachment != AT_Silencer)
  {
    Sound("ASTR_Fire*.ogg", 0, ammo);
    MuzzleFlash(RandomX(30,40),user,x,y,angle,0, 0);
    Echo("ASTR_Echo.ogg");
  }
  else
  {
    Sound("WPN2_SilencerFire*.ogg", 0, ammo, 0, GetOwner(user)+1);
    Sound("WPN2_SilencerFire*.ogg", 0, ammo, 10);
  }

  //Klickgeräusch bei wenig Munition
  if(Inside(GetAmmo(GetFMData(FM_AmmoID)), 1, GetFMData(FM_AmmoLoad)/3))
    Sound("MNGN_Click.ogg", 0, ammo, 0, GetOwner(user)+1);
}