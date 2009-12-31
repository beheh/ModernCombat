/*-- Panzerfaust (PzF) --*/

#strict 2

#include WEPN

// Anzeige in der Hand
public func HandSize() { return 800; }
public func HandX()    { return 3000; }
public func HandY()    { return 0;  }

public func BarrelXOffset() { return 5000; }		
public func BarrelYOffset() { return 8500; }		

func OnReload()
{
  Sound("PZFW_Reload");
}

func OnSelect()
{
   Sound("PZFW_Charge");
}

public func FMData1(int data)
{
  if(data == FM_Name)     return "$Missile$";
  if(data == FM_Icon)     return PZFM;	
  if(data == FM_AmmoID)   return MIAM;
  if(data == FM_AmmoLoad) return 2;//1 Schuss verbraucht 2 Raketenmunition.
  if(data == FM_AmmoUsage)return 2;

  if(data == FM_Reload)   return 160; 
  if(data == FM_Recharge) return 1;

  if(data == FM_Aim)      return 1;

  if(data == FM_Damage)   return 100;

  return Default(data);
}

public func BotData1(int data)
{
  if(data == BOT_Range)    return 700;
  if(data == BOT_DmgType)  return DMG_Explosion;
  if(data == BOT_Power)    return BOT_Power_LongLoad;

  return Default(data);
}

public func Fire1()
{
  LaunchRocket(PZFM,Contained()->~AimAngle(20), GetFMData(FM_Damage, 1)/2);
}

//Wuuuschh!
public func LaunchRocket(rid, angle, dmg)
{
  var user = Contained();
  var x,y;
  user->WeaponEnd(x,y);
  // not too near to clonk
  x = x*3/2;
  y = y*3/2;

  var rocket = CreateObject(rid,x,y,GetController(user));
  rocket->Launch(angle, dmg);

  // Effekte
  var ax, ay, xdir, ydir;
  user->WeaponBegin(ax,ay);
  xdir = ax-x;
  ydir = ay-y;

  /*CreateParticle("Thrust",ax,ay,xdir/2,ydir/2,80,RGBa(255,200,200,0),0,0);

  for(var i=0; i<20; ++i)
  {
    var rand = RandomX(-10,+10);
    CreateParticle("Smoke2",ax+Sin(angle,rand),ay-Cos(angle,rand),
                   RandomX(0,2*xdir),RandomX(0,2*ydir),
                   RandomX(80,140),RGBa(220,200,180,0),0,0);
  }*/

  // Sound
  Sound("PZFW_Fire*");
  
  ShakeViewPort(20, Contained());
  
  BackFire();
}

func BackFire()
{
  var user = GetUser();
  if(!user) return ;
  
  var x,y,angle;

  angle = user->AimAngle()-180;
  
  //if(user->~IsClonk())//Höchstwarscheinlich also ein HazardClonk. :>
    //user->WeaponEnd(x,y);
  
  x += Sin(angle,20);
  y += -Cos(angle,20);

  for(var obj in FindObjects(Find_Exclude(GetUser()), Find_Not(Find_Func("IsLight"),Find_Func("IsHUD")), Find_Distance (20, x, y),Find_NoContainer(),Find_Category(C4D_Living | C4D_Object | C4D_Vehicle)))
  {
    //Beschädigen
    var dist = ObjectDistance(this,obj);
    DoDmg (dist, DMG_Fire, obj); 
    
    //Beschleunigen
    var xdir = Sin(angle,dist/5);
    var ydir = -Cos(angle,dist/5);
    
    SetXDir (GetXDir(obj)+xdir, obj);
    SetYDir (GetYDir(obj)+ydir, obj);
  }

  //Effektgehasche
  AddEffect ("IntBackFire",this,200,2,this); 
}

public func FxIntBackFireStart(object pTarget, int iEffectNumber, int iTemp)
{
  var user = pTarget->GetUser();
  if(!user) return -1;
  
  var x,y;
  var angle = user->AimAngle()-180;
  
  x = -Sin(angle,17);
  y = Cos(angle,17);
  
  var ax, ay, xdir, ydir;
  user->WeaponBegin(ax,ay);
  xdir = ax-x;
  ydir = ay-y;

  for(var i=0; i<10; ++i)
  {
    var rand = RandomX(-10,+10);
    user->CreateParticle("Smoke2",x+Sin(angle,rand),y-Cos(angle,rand),
                   RandomX(0,2*xdir),RandomX(0,2*ydir),
                   RandomX(80,140),RGBa(220,200,180,0),0,0);
  }

  return 1;
}

public func FxIntBackFireTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  if(iEffectTime >= 10)
    return -1;
    
  var user = pTarget->GetUser();
  if(!user) return -1;

  var x,y,angle;
  angle = user->AimAngle()-180;
  
  if(user->~IsClonk())//Höchstwarscheinlich also ein HazardClonk. :>
    user->WeaponEnd(x,y);
  
  x = Sin(angle,30);
  y = -Cos(angle,30);
  
  user->CreateParticle("JetFlame", x, y-3, x, y, RandomX(150,300), RGB(255,30+Random(110),0,iEffectTime*255/10));
  
  return 0;
}


//Überladungen und WEPN-Hacks! ;D

func OnSelect()
{
  SetGraphics("Unlocked");
}

func OnDeselect()
{
  SetGraphics();
  RemoveEffect("IntReady",this);
  RemoveEffect("IntUnlock",this);
}

func Entrance()
{
  SetGraphics("Unlocked");
  return _inherited(...);
}

func Departure()
{
  OnDeselect();
  return _inherited(...);
}

func OnAimStop()
{
  RemoveEffect("IntReady",this);
  RemoveEffect("IntUnlock",this);
}

public func UnlockTime(){return 35*2;}

private func Shoot(object pCaller)
{
  if(!GetEffect("IntReady",this) && UnlockTime())//Bereit?
  {
    if(GetEffect("IntUnlock",this))//Entsichert noch?
    {
      //Sound("PZFW_Deny",false,0,0,GetController(GetUser())); 
    }
    else//Nein? Entsichern starten!
    {
      AddEffect("IntUnlock",this,1,1,this);
    }
    
    return ;
  }
  
  RemoveEffect("IntReady",this);
  
  return inherited(pCaller,...);
}


func FxIntUnlockStart(object pTarget, int iEffectNumber, int iTemp)
{
  RemoveEffect("IntReady",this);
  var pHUD = CreateObject(1HHD,0,0,GetOwner(GetUser()));
  pHUD->Set(GetUser(),0,RGB(255,255,0),GetUser());
  EffectVar(0, pTarget, iEffectNumber) = pHUD;
}

func FxIntUnlockTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  EffectVar(0, pTarget, iEffectNumber)->Update(iEffectTime,UnlockTime());

  if(iEffectTime >= UnlockTime())
  {
    Sound("PZFW_Ready",false,0,0,GetController(GetUser())); 
    AddEffect("IntReady",this,1,0,this);
    return -1;
  }
  
  if(!(iEffectTime % 17))
    Sound("PZFW_Beep",false,0,0,GetController(GetUser())); 
}

func FxIntUnlockStop(object pTarget, int iEffectNumber, int iEffectTime)
{
  EffectVar(0, pTarget, iEffectNumber)->Remove();
}


func FxIntReadyTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  //TODO: Prüfen?
}









/* Extra-Zeug, was entweder noch nicht geht oder nur Speicherplatz vergeudet. */

/*private func ChangeFireMode(id, i)
{
  // Immer noch genügend Munition da?
  if(!CheckAmmo(GetFMData(FM_AmmoID,i),GetFMData(FM_AmmoUsage,i),GetUser(),this))
  {
    // Nein. Mecker.
    Failure(id, i);
    // Menü will geupdatet werden, wahrscheinlich
    var item=GetMenuSelection(GetUser());
    CloseMenu(GetUser());
    ControlDigDouble(GetUser());
    SelectMenuItem (item, GetUser());
    // Fertig.
    return ; 
  }
  
  CloseMenu(GetUser());
  var last = firemode;
  
  // Schussmodus umstellen
  if(SetFireMode(i))
  {
    if(!GetAmmo(i) && (i != last))
      return Reload(i);
  }
  
  if(last == i)
    return Lock();
}*/


/*Aiming
public func AimAngleUpdate(int iAngle)
{
  //Nain! Geht momentan noch nicht. :C
  var cross,x,y;
  if(GetUser())
  {
    cross = LocalN("crosshair",GetUser());
    //GetUser()->WeaponEnd(x,y);
  }
  
  if(cross)
  {
    cross = ResetCross(cross);
    //SimRocketFlight(x, y, x*3/2-x, y*3/2-y, PZFM, cross);
    SimRocketFlight(0, 0, 10,10, PZFM, cross);
  }
}

//Helperz!
//Zeichnet Flugbah... von M&M kopiert
private func SimRocketFlight(iX, iY, iXDir, iYDir, idID, pCross, iXOffset, iYOffset)
{
  var i = -1, iXOld, iYOld;
  var iFaktor = 100;
  iX *= iFaktor; iY *= iFaktor;
  iYDir *= 5; iXDir *= 5;
  iY -= 4*iFaktor;
  iXOld = iX; iYOld = iY;
  
  while(++i < 200)//maximum
  {
    iX += iXDir;
    iY += iYDir + GetGravity()*i/20;
    if(Distance((iXOld-iX)/iFaktor, (iYOld-iY)/iFaktor)>=10)
    {
      CreateParticle("Aimer", iX/iFaktor-iXOffset, iY/iFaktor-iYOffset, iXDir/500, iYDir/500, 10, RGBa(0, 255, 0), pCross);
      iXOld = iX; iYOld = iY;
    }
    if(GBackSolid(iX/iFaktor-GetX(), iY/iFaktor-GetY())) break;
  }
}

private func ResetCross(object pCross)
{
  if(!pCross)return ;
  
  var pNewCross = pCross->CreateObject(GetID(pCross),0,0,NO_OWNER);
  
  pNewCross->SetAngle(pCross->GetAngle());
  LocalN("crosshair",GetUser()) = pNewCross;
  pNewCross->Init(GetUser());
  
  RemoveObject(pCross);
  return pNewCross;
}
*/
