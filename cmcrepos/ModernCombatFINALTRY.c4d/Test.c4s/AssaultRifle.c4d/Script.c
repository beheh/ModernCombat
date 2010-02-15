/*-- Sturmgewehr --*/

#strict
#include WPN2

public func HandSize()      { return(1000); }
public func HandX()         { return(5000); }
public func HandY()         { return(2000); }

public func BarrelYOffset() { return(-2500); }
public func SelectionTime() { return(12*3); }


//Kugeln - Stoßfeuer

public func FMData1(int data)
{
  if(data == FM_Name)               return("$Bullets$");
    
  if(data == FM_AmmoID)             return(STAM);
  if(data == FM_AmmoLoad)           return(30);
  
  if(data == FM_Reload)             return(130);
  if(data == FM_Recharge)           return(13);

  if(data == FM_Auto)               return(false);
  
  if(data == FM_Damage)             return(15);
  
  if(data == FM_Slot)               return(1);//Das Gewehr bekommt den Slot 1.
  
  if(data == FM_SpreadAdd) return(68);
  if(data == FM_StartSpread) return(70);

  return(Default(data));
}

public func FMData1T1(int data)
{
  if(data == FT_Name)                 return("$Burst$");//Der Name der Feuertechnik.
  if(data == FT_IconFacet)            return(FICO_Burst);
  if(data == FM_Recharge)             return(33);
  if(data == FM_BurstAmount)          return(3);
  if(data == FM_BurstRecharge)        return(3);
  return(FMData1(data));
}

public func Fire1T2() //Stoßfeuer-Feuertechnik benutzt den Standard.
{
  Fire1();
}

public func BotData1(int data)
{
  if(data == BOT_Range)    return(600);

  return(Default(data));
}

public func Fire1T1()//Stoßfeuer schießst 3 Mal hintereinander.
{
  var user = GetUser();
  var dir = GetDir(user)*2-1;
  var angle = user->AimAngle(10,0,true);
  var x,y;
  user->WeaponEnd(x,y);
  var ammo = SALaunchBullet(x,y,GetController(user),angle+RandomX(-2,+2),250,800,GetFMData(FM_Damage));
  ammo->Sound("M16A_Fire*.ogg");

  // Effekte
  SAMuzzleFlash(RandomX(30,40),user,x,y,angle);
  SABulletCasing(x/3,y/3,-dir*14*(Random(1)+1),-(13+Random(2)),5);
}

//Feuer!
public func Fire1()//Standardschuss mit dem Gewehr.
{                  //Damit man ohne WPN2 auch die Waffe benutzen kann.
  var user = GetUser();
  var dir = GetDir(user)*2-1;
  var angle = user->AimAngle(10,0,true);
  var x,y;
  user->WeaponEnd(x,y);
  var ammo = SALaunchBullet(x,y,GetController(user),angle,250,800,GetFMData(FM_Damage));
  ammo->Sound("M16A_Fire*.ogg");

  // Effekte
  SAMuzzleFlash(RandomX(30,40),user,x,y,angle);
  SABulletCasing(x/3,y/3,-dir*14*(Random(1)+1),-(13+Random(2)),5);
}


//Kugeln - Einzelfeuer

public func FMData1T2(int data)
{
  if(data == FT_Name)                 return("$Single$");
  if(data == FT_IconFacet)            return(FICO_Single);
  return(FMData1(data));//Standardwerte aus dem zugrunde liegendem Feuermodus nehmen.
}


//Granaten - Explosivgranaten

public func FMData2(int data)
{
  if(data == FM_Name)     return("$Grenades$");
  if(data == FM_AmmoID)   return(GRAM);
  if(data == FM_AmmoLoad) return(1);

  if(data == FM_Reload)   return(70);
  if(data == FM_Recharge) return(1);

  if(data == FM_Damage)   return(20);
  
  if(data == FM_Slot)    return(2);//Der Granatenwerfer benutzt den zweiten Slot.
  
  if(data == FM_Icon) return(M203);
  
  if(data == FM_SpreadAdd) return(50);
  if(data == FM_StartSpread) return(80);

  return(Default(data));
}

public func FMData2T1(int data)
{
  if(data == FT_Name)                 return("$Explosive$");
  if(data == FT_IconFacet)            return(FICO_Explosive);
  if(data == FM_Icon)                 return(M203);
  return(FMData2(data));
}

public func Fire2T1()
{  
  Fire2();
}

public func BotData2(int data)
{
  if(data == BOT_Range)    return(90);

  return(Default(data));
}

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
  grenade->Sound("M203_Fire*.ogg");

  CreateParticle("Thrust",x,y,GetXDir(user),GetYDir(user),80,RGBa(255,200,200,0),0,0);

  for(var i=0; i<20; ++i) {
    CreateParticle("Smoke2",x+RandomX(-5,+5),y+RandomX(-5,+5),
                   GetXDir(user)+RandomX(0,xdir/4),GetYDir(user)+RandomX(0,ydir/4),
                   RandomX(80,140),RGBa(200,200,200,0),0,0);
  }
}


//Granaten - Splittergranaten

public func FMData2T2(int data)
{
  if(data == FT_Name)                 return("$Cluster$");
  if(data == FT_IconFacet)            return(FICO_Cluster);
  if(data == FM_Icon)                 return(M23C);
  if(data == FM_Damage)               return(3);
  return(FMData2(data));
}

public func Fire2()//Schuss mit dem Granatenwerfer.
{  
  LaunchGrenade(M203, 90,Contained()->~AimAngle(0,0,true)+RandomX( -3, 3));
}

public func Fire2T2()
{  
  LaunchGrenade(M23C, 90,Contained()->~AimAngle(0,0,true)+RandomX( -3, 3));
}

/* Allgemein */

func OnReload(i)
{
  if(i == 1)
  {
    Sound("M16A_Reload.ogg");
  }
  if(i == 2)
  {
    Sound("M203_Reload.ogg");
    var user = GetUser();
    var dir = GetDir(user)*2-1;
    SABulletCasing(dir*1,0,-dir*14*(Random(1)+1),-(13+Random(2)));
  }
}

func OnSelect()
{
   Sound("M16A_Charge.ogg");
}

public func OnSelectFT(int iFireMode, int iFireTec)
{
  if(iFireMode == 2)
    Empty2(2);
}