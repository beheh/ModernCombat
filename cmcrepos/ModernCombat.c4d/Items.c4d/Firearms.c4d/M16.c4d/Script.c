/*-- M16 --*/

#strict
#include WPN2

local attachment;

public func HandSize()      { return(1000); }
public func HandX()         { return(5000); }
public func HandY()         { return(2000); }

public func BarrelYOffset() { return(-2500); }
public func SelectionTime() { return(14*3); }

func OnReload(i)
{
  if(i == 1)
    Sound("M16A_Reload.ogg");
    
  if(i == 2)
    Sound("M203_Reload.ogg");
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

public func FMData1(int data)
{
  if(data == FM_Name)               return("$Bullets$");
    
  if(data == FM_AmmoID)             return(STAM);
  if(data == FM_AmmoLoad)           return(30);
  
  if(data == FM_Reload)             return(140);
  if(data == FM_Recharge)           return(13);

  if(data == FM_Auto)               return(false);
  
  if(data == FM_Damage)             return(15);
  
  if(data == FM_Slot)               return(1);//Das Gewehr bekommt den Slot 1.
  
  if(data == FM_SpreadAdd) return(68);
  if(data == FM_StartSpread) return(70);

  return(Default(data));
}

public func BotData1(int data)
{
  if(data == BOT_Range)    return(600);

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

public func FMData1T2(int data)
{
  if(data == FT_Name)                 return("$Single$");
  if(data == FT_IconFacet)            return(FICO_Single);
  return(FMData1(data));//Standardwerte aus dem zugrunde liegendem Feuermodus nehmen.
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

public func Fire1T2()//Automatik-Feuertechnik benutzt den Standard.
{
  Fire1();
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


// Granatenwerferaufsatz
public func FMData2(int data)
{
  if(data == FM_Name)     return("$Grenades$");
  if(data == FM_AmmoID)   return(GRAM);
  if(data == FM_AmmoLoad) return(1);

  if(data == FM_Reload)   return(80);
  if(data == FM_Recharge) return(1);

  if(data == FM_Damage)   return(20);
  
  if(data == FM_Slot)    return(2);//Der Granatenwerfer benutzt den zweiten Slot.
  
  if(data == FM_Icon) return(M203);
  
  if(data == FM_Condition) return(attachment == M203);
  if(data == FM_Aim) return(3);
  
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

public func FMData2T2(int data)
{
  if(data == FT_Name)                 return("$Cluster$");
  if(data == FT_IconFacet)            return(FICO_Cluster);
  if(data == FM_AmmoLoad)             return(2);
  if(data == FM_AmmoUsage)            return(2);
  if(data == FM_Icon)                 return(M23C);
  if(data == FM_Reload)               return(100);
  return(FMData2(data));
}

public func BotData2(int data)
{
  if(data == BOT_Range)    return(90);

  return(Default(data));
}

public func Fire2()//Schuss mit dem Granatenwerfer.
{  
  LaunchGrenade(M203, 90,Contained()->~AimAngle(0,0,true)+RandomX( -3, 3));
}

public func Fire2T1()
{  
  Fire2();
}

public func Fire2T2()
{  
  LaunchGrenade(M23C, 90,Contained()->~AimAngle(0,0,true)+RandomX( -3, 3));
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

  CreateParticle("Thrust",x,y,/*GetXDir(user)*/,/*GetYDir(user)*/,15,RGBa(255,200,200,0),0,0);

  for(var i=0; i<6; ++i)
  {
    CreateParticle("Smoke2",x+RandomX(-2,+2),y+RandomX(-2,+2),
                   /*GetXDir(user)+*/RandomX(0,xdir/8),/*GetYDir(user)+*/RandomX(0,ydir/8),
                   RandomX(15,50),RGBa(200,200,200,0),0,0);
  }
}
  
  
//Tuning. :>
public func IsUpgradeable(id uid)
{
  if(GetUpgrade(uid))
    return(false);
    
  if(attachment)
    return(false);
  
  if(uid == KRFL) return("$KRFLUpgradeDesc$");
}

public func OnUpgrade(id uid)
{
  var own = GetOwner(Contained());
  if(uid == KRFL)
  {
    SetGraphics("M203");
    attachment = M203;
    SetFireMode(2);
    if(Contained()) HelpMessage(own, "$KRFLUpgraded$", Contained());
    return(true);
  }
}

public func OnDowngrade(id uid)
{
  if(uid == KRFL)
  {
    SetGraphics();
    attachment = 0;
    SetFireMode(1);
    return(true);
  }
}