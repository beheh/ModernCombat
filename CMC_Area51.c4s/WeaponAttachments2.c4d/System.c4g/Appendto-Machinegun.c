/*-- Maschinengewehr-Erweiterung für Waffenupgrades --*/

#strict 2
#appendto MNGN

func PermittedAtts()
{
  return AT_Bayonet | AT_Laserpointer | AT_Foregrip;
}

public func FMData1(int data)
{

  if(data == FM_SpreadAdd)	return _inherited(data) - (iAttachment == AT_Foregrip)*5;	//Bei jedem Schuss hinzuzuaddierende Streuung
  if(data == FM_StartSpread)	return _inherited(data) - (iAttachment == AT_Foregrip)*10;	//Bei Auswahl der Waffe gesetzte Streuung
  if(data == FM_MaxSpread)	return _inherited(data) - (iAttachment == AT_Foregrip)*10;	//Maximaler Streuungswert

  return _inherited(data);
}

public func GetMCData(int data)
{
  if(data == MC_Damage)		
    return _inherited(data) + (iAttachment == AT_Bayonet)*6;	//Schaden eines Kolbenschlages
  
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
  var ammo = SALaunchBullet(x,y,GetController(user),angle,270,750,GetFMData(FM_Damage));

  //Effekte
  MuzzleFlash(RandomX(35,50),user,x,y,angle,0, 0);
  SABulletCasing(x/3,y/3,-dir*14*(Random(1)+1),-(13+Random(2)),5);
  Sound("MNGN_Fire.ogg", 0, ammo);
  Echo("MNGN_Echo.ogg");

  //Klickgeräusch bei wenig Munition
  if(Inside(GetAmmo(GetFMData(FM_AmmoID)), 1, GetFMData(FM_AmmoLoad)/3))
    Sound("MNGN_Click.ogg", 0, ammo, 0, GetOwner(user)+1);
}