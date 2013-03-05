/*-- Sturmgewehr-Erweiterung für Waffenupgrades --*/

#strict 2
#appendto ASTR

func PermittedAtts()
{
  return AT_ExtendedMag | AT_Bayonet | AT_Laserpointer | AT_Silencer | AT_Foregrip;
}

public func FMData1(int data)
{
  if(data == FM_AmmoLoad)
    return _inherited(data) + (iAttachment == AT_ExtendedMag)*6;		//Magazingröße

  if(data == FM_SpreadAdd)	return 50 - (iAttachment == AT_Foregrip)*10;	//Bei jedem Schuss hinzuzuaddierende Streuung
  if(data == FM_StartSpread)	return 100 - (iAttachment == AT_Foregrip)*20;	//Bei Auswahl der Waffe gesetzte Streuung
  if(data == FM_MaxSpread)	return 450 - (iAttachment == AT_Foregrip)*150;	//Maximaler Streuungswert

  return _inherited(data);
}

public func FMData1T2(int data)
{
  if(data == FT_Name)		return "$Single$";

  if(data == FM_Damage)		return 14;	//Schadenswert

  if(data == FM_SpreadAdd)	return _inherited(data) + 20;	//Bei jedem Schuss hinzuzuaddierende Streuung

  return FMData1(data);
}

public func GetMCData(int data)
{
  if(data == MC_Damage)		
    return _inherited(data) + (iAttachment == AT_Bayonet)*6;			//Schaden eines Kolbenschlages
  
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
  if(iAttachment != AT_Silencer)
  {
    SABulletCasing(x/3,y/3,-dir*14*(Random(1)+1),-(13+Random(2)),5);
    Sound("ASTR_Fire*.ogg", 0, ammo);
    MuzzleFlash(RandomX(30,40),user,x,y,angle,0, 0);
    Echo("ASTR_Echo.ogg");
  }
  else
  {
    Sound("WPN2_SilencerFire*.ogg", 0, ammo, 0, GetOwner(user)+1);
    Sound("WPN2_SilencerFire*.ogg", 0, ammo, 10);
    
    if(GetEffect("Silencer", this))
      EffectVar(0, this, GetEffect("Silencer", this)) -= BoundBy(25, 0, EffectVar(0, this, GetEffect("Silencer", this)));
  }

  //Klickgeräusch bei wenig Munition
  if(Inside(GetAmmo(GetFMData(FM_AmmoID)), 1, GetFMData(FM_AmmoLoad)/3))
    Sound("MNGN_Click.ogg", 0, ammo, 0, GetOwner(user)+1);
}

public func LaunchGrenade(id idg, int speed, int angle, int mode)
{
  //Austritt bestimmen
  var user = Contained();
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
  grenade->Launch(xdir+GetXDir(user)/5, ydir/*+GetYDir(user)/10*/, GetFMData(FM_Damage,2), 0, 0, 0, iAttachment);

  //Sicht auf Granate wenn der Schütze zielt
  if(!(user->~IsMachine()) && user->~IsAiming())
  {
    SetPlrView(GetController(user),grenade);
    SetPlrViewRange(100, grenade);
  }

  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 0)
  {
    CreateParticle("Thrust",x,y,GetXDir(user),GetYDir(user),80,RGBa(255,200,200,0),0,0);

    for(var i=0; i<20; ++i)
    {
      CreateParticle("Smoke2",x+RandomX(-5,+5),y+RandomX(-5,+5),
      		GetXDir(user)+RandomX(0,xdir/4),GetYDir(user)+RandomX(0,ydir/4),
      		RandomX(80,140),RGBa(200,200,200,0),0,0);

      CreateParticle("BlastSpark1",x+RandomX(-5,+5),y+RandomX(-5,+5),
      		GetXDir(user)+RandomX(0,xdir/4),GetYDir(user)+RandomX(0,ydir/4),
      		RandomX(40,70),RGBa(200,200,200,0),0,0);
    }
  }
  Sound("ASTR_LauncherFire*.ogg", 0, grenade);
  Echo("SGST_Echo.ogg");

  //Patronenhülse vorhanden
  casing = 1;
}
