/*-- Pistole --*/

#strict

#include WEPN
local marker;

// Anzeige in der Hand
public func HandSize() { return(800); }
public func HandX()    { return(5500); }
public func HandY()    { return(-1000); }
public func BarrelYOffset() { return(-2700); }

public func OnReload(int i)
{
  Sound("PistolLoad");
}

public func FMData1(int data)
{
  if(data == FM_Name)      return("$Standard$");
  if(data == FM_AmmoID)    return(STAM);
  if(data == FM_AmmoLoad)  return(12);

  if(data == FM_Reload)    return(80);
  if(data == FM_Recharge)  return(5);
  if(data == FM_Condition) return(!GetUpgrade(KLAS));

  if(data == FM_Damage)    return(16);
  
  if(data == FM_StartSpread) return(0);
  if(data == FM_SpreadAdd) return(80);

  return(Default(data));
}

public func BotData1(int data)
{
  if(data == BOT_Range)    return(500);
  if(data == BOT_Power)    return(BOT_Power_1);

  return(Default(data));
}

public func Fire1()    // Projektilschuss
{ 
  var user = GetUser();
  var angle = user->AimAngle(20) + RandomX(-1,+1);
  var x,y; user->WeaponEnd(x,y);
  var ammo = CreateObject(SHT1,x,y,GetController(user));
  ammo->Launch(angle,250,600,3,300,GetFMData(FM_Damage, 1));

  // Effekte
  MuzzleFlash(40,user,x,y,angle);
  var dir = GetDir(user)*2-1;
  BulletCasing(dir*6,3,-dir*5,-20,5);

  // Sound
  Sound("Pistol",0,ammo);
}

public func EnableAirstrike()
{
  marker = true;
  Message("Airstrike verfügbar!",GetUser());
  //SetFireMode(2);
  //DoAmmo(ENAM,1);
}

public func FMData2(int data)//Markierung
{
  if(data == FM_Name)      return("Airstrike");
  if(data == FM_AmmoID)    return(ENAM);
  if(data == FM_AmmoLoad)  return(0);

  if(data == FM_Reload)    return(1);
  if(data == FM_Recharge)  return(1);
  if(data == FM_Condition) return(marker);

  if(data == FM_Damage)    return(0);
  
  if(data == FM_StartSpread) return(0);
  if(data == FM_SpreadAdd) return(0);

  return(Default(data));
}

public func Fire2()
{ 
  var user = GetUser();
  var angle = user->AimAngle();
  var x,y; user->WeaponEnd(x,y);
  var dir = GetDir(user)*2-1;
  var laser = CreateObject(LASR,x,y,GetController(user));
  laser->Set(angle,5,500,25);
  laser->LaserEnd(x,y);
  x+=GetX();
  y+=GetY();
  
  Message("Koordinaten %d/%d",GetUser(),x,y);
  Airstrike(x,y);
  marker = false;
  SetFireMode(1);
  ScheduleCall(this(),"EnableAirstrike",35*20); 

  // Sound
  Sound("LaserShot",0,laser);
}

local firemode;

public func Fire()
{
  // bereit zum Schießen
  if(!(GetUser()->~ReadyToFire())) return(false); 

  // lädt grad nach... bitte nicht stören
  if(IsReloading()) return(false);

  var ammoid = GetFMData(FM_AmmoID);
  var ammousage = GetFMData(FM_AmmoUsage);
  // leer?
  if(firemode == 2)
  {
    if(!marker)
    {
      Message("Noch nicht bereit.",GetUser());
      Sound("Error");
      return(false);
    }
  }
  else if(!CheckAmmo(ammoid,ammousage,this()))
  {
    OnEmpty(firemode);
    return(false);
  }

  // Callback
  if(GetFMData(FM_Auto))
    OnAutoStart(firemode);

  // FEUAAA!!!1 OMGOMG
  Shoot();

  return(true);
}

global func Airstrike(int iX, int iY)
{
  return(AddEffect("Airstrike",0,30,10,0,0, iX, iY));
}

global func FxAirstrikeStart(object pTarget, int iEffectNumber, int iTemp, x, y)
{
  Sound("Morse");
  EffectVar(0,pTarget,iEffectNumber) = x;
  EffectVar(1,pTarget,iEffectNumber) = y;
}

global func FxAirstrikeTimer (object pTarget, int iEffectNumber, int iEffectTime)
{
  if((iEffectTime >= 35*7) && !EffectVar(2,pTarget,iEffectNumber))
  {
    EffectVar(2,pTarget,iEffectNumber) = true;
    Sound("Airstrike1");
  }

  if(iEffectTime >= 35*8)
  {
    //Ausräuchern!
    CastObjects (ABMB,10,20,EffectVar(0,pTarget,iEffectNumber),-50); 
    return(-1);
  }
}

/*global func FxAirstrikeStop (object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
}*/
