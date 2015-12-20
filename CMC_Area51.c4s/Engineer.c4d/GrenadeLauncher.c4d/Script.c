/*-- Granatwerfer --*/

#strict 2
#include WPN2

local casing;

public func HandSize()		{return(1000);}
public func HandX()		{return(5000);}
public func HandY()		{return(-2500);}
public func IsSecondaryWeapon()	{return true;}

public func SelectionTime()	{return 40;}		//Anwahlzeit


/* Kompatible Waffenaufsätze */

func PermittedAtts()
{
  return AT_Flashlight;
}

/* Nahkampfangriff */

public func GetMCData(int data)
{
  if(data == MC_CanStrike)	return 1;		//Waffe kann Kolbenschlag ausführen
  if(data == MC_Damage)		return 20;		//Schaden eines Kolbenschlages
  if(data == MC_Recharge)	return 45;		//Zeit nach Kolbenschlag bis erneut geschlagen oder gefeuert werden kann
  if(data == MC_Power)		return 20;		//Wie weit das Ziel durch Kolbenschläge geschleudert wird
  if(data == MC_Angle)		return 45;		//Mit welchem Winkel das Ziel durch Kolbenschläge geschleudert wird
}

/* Granaten */

public func FMData1(int data)
{
  if(data == FM_Name)		return "$Grenades$";

  if(data == FM_AmmoID)		return GRAM;		//ID der Munition
  if(data == FM_AmmoLoad)	return 1;		//Magazingröße

  if(data == FM_Reload)		return 140;		//Zeit für Nachladen
  if(data == FM_Recharge)	return 5;		//Zeit bis erneut geschossen werden kann

  if(data == FM_Auto)		return false;		//Kein Automatikfeuer
  
  if(data == FM_Damage)		return 22;		//Schadenswert
  
  if(data == FM_Slot)		return 1;		//Slot des Feuermodus
  
  if(data == FM_SpreadAdd)	return 20;		//Bei jedem Schuss hinzuzuaddierende Streuung
  if(data == FM_StartSpread)	return 100;		//Bei Auswahl der Waffe gesetzte Streuung
  if(data == FM_MaxSpread)	return 500;		//Maximaler Streuungswert

  return Default(data);
}

/* Granaten - Phosphorgranaten */

public func FMData1T1(int data)
{
  if(data == FT_Name)		return "$Phosphor$";

  return FMData1(data);
}

public func Fire1T1()
{
  LaunchGrenade(PSHL, 120,Contained()->~AimAngle(0,0,true));
}

public func BotData1(int data)
{
  if(data == BOT_Range)		return 800;
  if(data == BOT_DmgType)	return(DMG_Fire);
  if(data == BOT_Power)		return(BOT_Power_2);

  return Default(data);
}

/* Granaten - Schuss */

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
  SetController(GetController(user), grenade);
  grenade->Launch(xdir+GetXDir(user)/5, ydir/*+GetYDir(user)/4*/, GetFMData(FM_Damage,2), 0, 0, 0, iAttachment, user);

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
      		RandomX(100,160),RGBa(200,200,200,0),0,0);

      CreateParticle("BlastSpark1",x+RandomX(-5,+5),y+RandomX(-5,+5),
      		GetXDir(user)+RandomX(0,xdir/4),GetYDir(user)+RandomX(0,ydir/4),
      		RandomX(60,90),RGBa(200,200,200,0),0,0);
    }
  }
  Sound("SGST_Fire*.ogg", 0, grenade);
  Echo("SGST_Echo.ogg");

  //Patronenhülse vorhanden
  casing = 1;
}

/* Handeffekt */

public func HandR()
{
  var effect = IsReloading();
  if(effect)
    return Max(Sin(GetEffect(0,this,effect,6)*90/80,20),0);
}

/* Allgemein */

func OnReload(i)
{
  Sound("GDLR_Reload.ogg");
  if(casing)
  {
    var user = GetUser();
    var dir = GetDir(user)*2-1;
    SABulletCasing(dir*1,0,-dir*14*(Random(1)+1),-(13+Random(2)),6,RGB(255,0,0));
    casing = 0;
  }
}

public func OnSelect()
{
  Sound("GDLR_Charge.ogg");
}
