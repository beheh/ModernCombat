/*-- Slingshot --*/

#strict
#include WPN2

public func HandSize() { return(1050); }
public func HandX() { return(5000); }
public func HandY() { return(1000); }

public func BarrelXOffset() { return(-1000); }
public func BarrelYOffset() { return(-2000); }
public func SelectionTime() { return(14*3); }


/* Granaten - Explosivgranaten */

public func FMData1(int data)
{
  if(data == FM_Name)                 return("$Grenades$");

  if(data == FM_AmmoID)             return(GRAM);
  if(data == FM_AmmoLoad)           return(8);

  if(data == FM_Recharge)           return(60);

  if(data == FM_SingleReload)       return(1);
  if(data == FM_Reload)             return(35*6);
  if(data == FM_PrepareReload)      return(30);
  if(data == FM_FinishReload)       return(25);

  if(data == FM_Damage)    return(13);
  
  if(data == FM_SpreadAdd) return(70);
  if(data == FM_StartSpread) return(80);
  if(data == FM_MaxSpread) return(CH_MaxSpread/3*2);

  return(Default(data));
}

public func FMData1T1(int data)
{
  if(data == FT_Name)                 return("$Explosive$");
  if(data == FM_Icon)                 return(ESHL);
  return(FMData1(data));
}

public func Fire1T1()
{
   LaunchGrenade(ESHL, 100+Random(40),Contained()->~AimAngle(0,0,true));
}

public func BotData1(int data)
{
  if(data == BOT_Range)    return(500);

  return(Default(data));
}

public func LaunchGrenade(id idg, int speed, int angle, int mode)
{
  var user = Contained();
  var dir = GetDir(user)*2-1;

  //Anpassung des Winkels
  angle = BoundBy(angle/*+GetYDir(user)*/+dir,-360,360);
  //Geschwindigkeit einstellen
  var xdir = Sin(angle,speed);
  var ydir = -Cos(angle,speed);

  var x,y;
  user->WeaponEnd(x,y);

  //Granate abfeuern
  var grenade=CreateObject(idg, x+xdir/10, y+ydir/10, GetController(user));
  grenade->Launch(xdir/*+GetXDir(user)/2*/, ydir/*+GetYDir(user)/4*/, GetFMData(FM_Damage,2));
  
  //Sicht auf Granate (wie beim Bogen auf die Pfeile)
  if(!(user ->~ IsMachine()))
  {
    SetPlrView(GetController(user),grenade);
    SetPlrViewRange(100, grenade);
  }

  //Effekte
  grenade->Sound("SGST_Fire.ogg");
  CreateParticle("Thrust",x,y,/*GetXDir(user)*/,/*GetYDir(user)*/,30,RGBa(255,200,200,0),0,0);
  for(var i=0; i<10; ++i)
  {
   CreateParticle("Smoke2",x+RandomX(-3,+3),y+RandomX(-3,+3),
                   /*GetXDir(user)+*/RandomX(0,xdir/8),/*GetYDir(user)+*/RandomX(0,ydir/8),
		   RandomX(30,80),RGBa(200,200,200,0),0,0);
  }
  Sound("SGST_Pump.ogg");
}

/* Granaten - Splittergranaten */

public func FMData1T2(int data)
{
  if(data == FT_Name)                 return("$Cluster$");
  if(data == FM_Icon)                 return(FSHL);
  if(data == FM_Damage)               return(2);
  return(FMData1(data));
}

public func Fire1T2()
{  
  LaunchGrenade(FSHL, 100,Contained()->~AimAngle(0,0,true)+RandomX( -3, 3));
}

public func BotData2(int data)
{
  if(data == BOT_Range)    return(500);

  return(Default(data));
}

/* Granaten - Rauchgranaten */

public func FMData1T3(int data)
{
  if(data == FT_Name)                 return("$Smoke$");
  if(data == FM_Icon)                 return(SSHL);
  if(data == FM_Damage)               return(2);
  return(FMData1(data));
}

public func Fire1T3()
{  
  LaunchGrenade(SSHL, 100,Contained()->~AimAngle(0,0,true)+RandomX( -3, 3));
}

public func BotData3(int data)
{
  if(data == BOT_Range)    return(500);

  return(Default(data));
}

/* Nachladen */

func OnReload()
{
  Sound("SGST_ReloadStart.ogg");
}

func OnSingleReloadStart()
{
  Sound("SGST_Loading.ogg");
}

func OnReloaded()
{
  Sound("SGST_ReloadStop.ogg");
}

/* Allgemein */

func OnSelect()
{
  Sound("SGST_Charge.ogg");
}
