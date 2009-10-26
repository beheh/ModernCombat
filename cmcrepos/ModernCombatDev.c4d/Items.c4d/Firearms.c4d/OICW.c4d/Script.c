/*-- OICW --*/

#strict
#include WPN2

public func HandSize()      { return(800); }
public func HandY()         { return(2000); }
public func HandX()         { return(5000); }
public func BarrelXOffset() { return(500); }

/*
  Wird aufgerufen, wenn eine Feuertechnik ausgewählt wurde.
*/
public func OnSelectFT(int iFireMode, int iFireTec)
{
  //...
}

func OnReload()
{
  Sound("OICW_Reload");
}

//Automatikfeuer - Kugeln
public func FMData1(int data)
{
  if(data == FM_Name)                 return("$Bullets$");
    
  if(data == FM_AmmoID)             return(STAM);
  if(data == FM_AmmoLoad)           return(30);
  
  if(data == FM_Reload)             return(75);
  if(data == FM_Recharge)           return(4);

  if(data == FM_Auto)               return(true);
  
  if(data == FM_Damage)    return(8);
  
  if(data == FM_Slot)    return(1);//Das Gewehr bekommt den Slot 1.
  
  if(data == FM_SpreadAdd) return(40);
  if(data == FM_StartSpread) return(70);
  if(data == FM_MaxSpread) return(CH_MaxSpread/3*2);

  return(Default(data));
}

public func BotData1(int data)
{
  if(data == BOT_Range)    return(500);

  return(Default(data));
}

//Feuertechniken fürs Gewehr.
public func FMData1T1(int data)//Automatik
{
  if(data == FT_Name)                 return("$Auto$");
  if(data == FT_IconFacet)            return(FICO_Auto);
  return(FMData1(data));//Standardwerte aus dem zugrunde liegendem Feuermodus nehmen.
}

public func FMData1T2(int data)
{
  if(data == FT_Name)                 return("$Burst$");
  if(data == FT_IconFacet)            return(FICO_Burst);
  if(data == FM_Recharge)             return(36);
  if(data == FM_BurstAmount)          return(3);
  if(data == FM_BurstRecharge)        return(3);
  if(data == FM_Auto)                 return(false);
  return(FMData1(data));
}

public func FMData1T3(int data)//Einzelfeuer
{
  if(data == FT_Name)                 return("$Single$");//Der Name der Feuertechnik.
  if(data == FM_Recharge)             return(10);
  if(data == FM_Auto)                 return(false);
  if(data == FT_IconFacet)            return(FICO_Single);
  if(data == FM_Damage)               return(FMData1(FM_Damage)*2);
  return(FMData1(data));
}

// Granatenwerferaufsatz
public func FMData2(int data)
{
  if(data == FM_Name)     return("$Grenades$");
  if(data == FM_AmmoID)   return(GRAM);
  if(data == FM_AmmoLoad) return(6);

  if(data == FM_Reload)   return(75);
  if(data == FM_Recharge) return(30);

  if(data == FM_Damage)   return(13);
  
  if(data == FM_Slot)    return(2);//Der Granatenwerfer benutzt den zweiten Slot.
  
  if(data == FM_Icon)    return(OICG);
  
  if(data == FM_Aim)     return(3);
  
  if(data == FM_SpreadAdd) return(50);
  if(data == FM_StartSpread) return(70);

  return(Default(data));
}

public func BotData2(int data)
{
  if(data == BOT_Range)    return(80);

  return(Default(data));
}

//Feuer!
public func Fire1()//Standardschuss mit dem Gewehr.
{                  //Damit man ohne WPN2 auch die Waffe benutzen kann.
  var user = GetUser();
  var dir = GetDir(user)*2-1;
  var angle = user->AimAngle(10,0,true);
  var x,y;
  user->WeaponEnd(x,y);
  var ammo = SALaunchBullet(x,y,GetController(user),angle+RandomX(-2,+2),250,450+Random(50),GetFMData(FM_Damage));
  ammo->Sound("OICW_Fire_Auto");

  // Effekte
  SAMuzzleFlash(RandomX(25,40),user,x,y-2,angle);
  SABulletCasing(dir*1,0,-dir*14*(Random(1)+1),-(13+Random(2)),5);
}

public func Fire1T1()//Automatik-Feuertechnik benutzt den Standard.
{
  Fire1();
}

public func Fire1T3()//Bei Einzelfeuer zielt man etwas genauer.
{
  var user = GetUser();
  var dir = GetDir(user)*2-1;
  var angle = user->AimAngle(10,0,true);
  var x,y;
  user->WeaponEnd(x,y);
  var ammo = SALaunchBullet(x,y,GetController(user),angle+RandomX(-1,+1),250,450+Random(50),GetFMData(FM_Damage));
  ammo->Sound("OICW_Fire");

  // Effekte
  SAMuzzleFlash(RandomX(25,40),user,x,y-2,angle);
  SABulletCasing(x/3,y/3,-dir*14*(Random(1)+1),-(13+Random(2)),5);
}

public func Fire1T2()//Bei Einzelfeuer zielt man etwas genauer.
{
  Fire1T3();
}

public func Fire2()//Schuss mit dem Granatenwerfer.
{
   LaunchGrenade(OICG, 80,Contained()->~AimAngle(0,0,true)+RandomX( -5, 5));
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
  grenade->Launch(xdir/*+GetXDir(user)/2*/, ydir/*+GetYDir(user)/4*/, GetFMData(FM_Damage,2));

  // effect
  grenade->Sound("oicw_grenade");

  CreateParticle("Thrust",x,y,/*GetXDir(user)*/,/*GetYDir(user)*/,30,RGBa(255,200,200,0),0,0);

  for(var i=0; i<10; ++i) {
    CreateParticle("Smoke2",x+RandomX(-3,+3),y+RandomX(-3,+3),
                   /*GetXDir(user)+*/RandomX(0,xdir/8),/*GetYDir(user)+*/RandomX(0,ydir/8),
                   RandomX(30,80),RGBa(200,200,200,0),0,0);
  }
}