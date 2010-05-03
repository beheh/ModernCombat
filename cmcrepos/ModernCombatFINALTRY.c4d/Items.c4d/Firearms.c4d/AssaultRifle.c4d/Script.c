/*-- Sturmgewehr --*/

#strict
#include WPN2

public func HandSize()      { return(1000); }
public func HandX()         { return(5000); }
public func HandY()         { return(1000); }

public func BarrelYOffset() { return(-2500); }
public func SelectionTime() { return(12*3); }


/* Kugeln */

public func FMData1(int data)
{
  if(data == FM_Name)               return("$Bullets$");
    
  if(data == FM_AmmoID)             return(STAM);
  if(data == FM_AmmoLoad)           return(30);
  
  if(data == FM_Reload)             return(90);
  if(data == FM_Recharge)           return(13);

  if(data == FM_Auto)               return(false);
  
  if(data == FM_Damage)             return(14);
  
  if(data == FM_Slot)               return(1);//Das Gewehr bekommt den Slot 1.
  
  if(data == FM_SpreadAdd) return(55);
  if(data == FM_StartSpread) return(40);

  return(Default(data));
}

/* Kugeln - Sto�feuer */

public func FMData1T1(int data)
{
  if(data == FT_Name)                 return("$Burst$");//Der Name der Feuertechnik.
  if(data == FM_Recharge)             return(15);
  if(data == FM_BurstAmount)          return(3);
  if(data == FM_BurstRecharge)        return(3);
  return(FMData1(data));
}

public func Fire1T1()//Sto�feuer schie�st 3 Mal hintereinander.
{
  var user = GetUser();
  var dir = GetDir(user)*2-1;
  var angle = user->AimAngle(10,0,true);
  var x,y;
  user->WeaponEnd(x,y);
  var ammo = SALaunchBullet(x,y,GetController(user),angle+RandomX(-2,+2),250,800,GetFMData(FM_Damage));
  ammo->Sound("ASTR_Fire*.ogg");

  // Effekte
  SAMuzzleFlash(RandomX(30,40),user,x,y,angle);
  SABulletCasing(x/3,y/3,-dir*14*(Random(1)+1),-(13+Random(2)),5);
}

public func BotData1(int data)
{
  if(data == BOT_Range)    return(600);

  return(Default(data));
}

/* Kugeln - Einzelfeuer */

public func FMData1T2(int data)
{
  if(data == FT_Name)                 return("$Single$");
  return(FMData1(data));//Standardwerte aus dem zugrunde liegendem Feuermodus nehmen.
}

public func Fire1T2()
{
  Fire1();
}

/* Kugeln - Schuss */

public func Fire1()//Standardschuss mit dem Gewehr.
{                  //Damit man ohne WPN2 auch die Waffe benutzen kann.
  var user = GetUser();
  var dir = GetDir(user)*2-1;
  var angle = user->AimAngle(10,0,true);
  var x,y;
  user->WeaponEnd(x,y);
  var ammo = SALaunchBullet(x,y,GetController(user),angle,250,800,GetFMData(FM_Damage));
  ammo->Sound("ASTR_Fire*.ogg");

  // Effekte
  SAMuzzleFlash(RandomX(30,40),user,x,y,angle);
  SABulletCasing(x/3,y/3,-dir*14*(Random(1)+1),-(13+Random(2)),5);
}

/* Granaten - Explosivgranaten */

public func FMData2(int data)
{
  if(data == FM_Name)     return("$Grenades$");
  if(data == FM_AmmoID)   return(GRAM);
  if(data == FM_AmmoLoad) return(1);

  if(data == FM_Reload)   return(80);
  if(data == FM_Recharge) return(1);

  if(data == FM_Damage)   return(20);
  
  if(data == FM_Slot)    return(2);//Der Granatenwerfer benutzt den zweiten Slot.
  
  if(data == FM_Icon) return(XSHL);
  
  if(data == FM_SpreadAdd) return(50);
  if(data == FM_StartSpread) return(80);

  return(Default(data));
}

public func FMData2T1(int data)
{
  if(data == FT_Name)                 return("$Explosive$");
  if(data == FM_Icon)                 return(M203);
  return(FMData2(data));
}

public func Fire2T1()
{  
  Fire2();
}

public func Fire2()
{  
  LaunchGrenade(ESHL, 90,Contained()->~AimAngle(0,0,true)+RandomX( -3, 3));
}

public func BotData2(int data)
{
  if(data == BOT_Range)    return(90);

  return(Default(data));
}

/* Granaten - Splittergranaten */

public func FMData2T2(int data)
{
  if(data == FT_Name)                 return("$Cluster$");
  if(data == FM_Icon)                 return(FSHL);
  if(data == FM_Damage)               return(3);
  return(FMData2(data));
}

public func Fire2T2()
{  
  LaunchGrenade(FSHL, 90,Contained()->~AimAngle(0,0,true)+RandomX( -3, 3));
}

/* Granaten - Rauchgranaten */

public func FMData2T3(int data)
{
  if(data == FT_Name)                 return("$Smoke$");
  if(data == FM_Icon)                 return(SSHL);
  if(data == FM_Damage)               return(3);
  return(FMData2(data));
}

public func Fire2T3()
{  
  LaunchGrenade(SSHL, 90,Contained()->~AimAngle(0,0,true)+RandomX( -3, 3));
}

/* Granaten - Schuss */

public func LaunchGrenade(id idg, int speed, int angle, int mode)
{
  var user = Contained();
  var dir = GetDir(user)*2-1;

  // Adjust angle
  angle = BoundBy(angle/*+GetYDir(user)*/+dir,-360,360);
  // calculate speed
  var xdir = Sin(angle,speed);
  var ydir = -Cos(angle,speed);

  var x,y;
  user->WeaponEnd(x,y);

  // create and launch
  var grenade=CreateObject(idg, x+xdir/10, y+ydir/10, GetController(user));
  grenade->Launch(xdir+GetXDir(user)/10, ydir/*+GetYDir(user)/20*/, GetFMData(FM_Damage,2));

  // effect
  grenade->Sound("ASTR_LauncherFire*.ogg");

  CreateParticle("Thrust",x,y,GetXDir(user),GetYDir(user),80,RGBa(255,200,200,0),0,0);

  for(var i=0; i<20; ++i) {
    CreateParticle("Smoke2",x+RandomX(-5,+5),y+RandomX(-5,+5),
                   GetXDir(user)+RandomX(0,xdir/4),GetYDir(user)+RandomX(0,ydir/4),
                   RandomX(80,140),RGBa(200,200,200,0),0,0);
  }
}

/* Allgemein */

func OnReload(i)
{
  if(i == 1)
  {
    Sound("ASTR_Reload.ogg");
  }
  if(i == 2)
  {
    Sound("ASTR_LauncherReload.ogg");
    var user = GetUser();
    var dir = GetDir(user)*2-1;
    SABulletCasing(dir*1,0,-dir*14*(Random(1)+1),-(13+Random(2)));
  }
}

func OnSelect()
{
   Sound("ASTR_Charge.ogg");
}

public func OnSelectFT(int iFireMode, int iFireTec)
{
  if(iFireMode == 2)
    Empty2(2);
}
