/*-- RPG-Launcher --*/

#strict

#include H2WP

// Anzeige in der Hand
public func HandSize() { return(850); }
public func HandX()    { return(3000); }
public func HandY()    { return(0);  }

public func HandR()
{
  var effect = IsReloading();
  if(effect)
    return(Max(Sin(GetEffect(0,this(),effect,6)*90/40,20),0));
    
  return(0);
}

public func FMData1(int data)
{
  if(data == FM_Name)     return(GetName(0,RPGA));
  if(data == FM_Icon)     return(RPGA);	
  if(data == FM_AmmoID)   return(RPGM);
  if(data == FM_AmmoLoad) return(1);

  if(data == FM_Reload)   return(80);
  if(data == FM_Recharge) return(80);

  if(data == FM_Aim)      return(3);

  if(data == FM_Damage)   return(35);

  return(Default(data));
}

public func BotData1(int data)
{
  if(data == BOT_Range)    return(600);
  if(data == BOT_DmgType)  return(DMG_Explosion);

  return(Default(data));
}

public func FMData1T1(int data)
{
  if(data == FT_Name)                 return(" ");
  if(data == FT_IconFacet)            return(FICO_Single);//Anderes Icon. Aber zuerst muss das neue Waffenmenü fertig sein.
  return(FMData1(data));
}

public func FMData1T2(int data)
{
  if(data == FT_Name)                 return("Laser");
  if(data == FT_IconFacet)            return(FICO_Single);
  return(FMData1(data));
}

public func Fire1() { LaunchRocket(RPGP,Contained()->~AimAngle(10), GetFMData(FM_Damage,1)); }
public func Fire1T1() { Fire1(); }
public func Fire1T2() { Fire1(); }

public func LaunchRocket(rid, angle, dmg)
{
  var user = Contained();
  var x,y;
  user->WeaponEnd(x,y);

  //x = x*3/2;
  //y = y*3/2;

  var rocket = CreateObject(rid,x,y+10,GetController(user));
  rocket->Launch(angle, dmg);

  // Effekte
  var ax, ay, xdir, ydir;
  user->WeaponBegin(ax,ay);
  xdir = ax-x;
  ydir = ay-y;

  CreateParticle("Thrust",ax,ay,xdir/2,ydir/2,80,RGBa(255,200,200,0),0,0);

  for(var i = 0; i < 10; ++i)
  {
    var rand = RandomX(-7,+7);
    CreateParticle("Smoke2",ax+Sin(angle,rand),ay-Cos(angle,rand),
                   RandomX(0,xdir),RandomX(0,ydir),
                   RandomX(80,100),RGBa(255,255,255,128),0,0);
  }

  Sound("H2RL_Launch.ogg");
}


// Soundeffekte

public func OnReload()
{
  //Sound("H2RL_Reload.ogg");
}
