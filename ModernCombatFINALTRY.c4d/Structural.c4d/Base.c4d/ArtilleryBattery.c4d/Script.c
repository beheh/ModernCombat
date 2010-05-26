/*-- Artilleriebatterie --*/

#strict 2
#include CSTR

local pCannon;
local bRotate, bDirection;
local iCooldown;
local byObj;

public func IsMachine()		{return(true);}
public func IsBulletTarget()	{if(!Random(3)) return(true);}
public func MaxDamage()		{return(200);}


/* Initalisierung */

func Initialize()
{
  //Kanone erstellen
  pCannon = CreateObject(CNON,0,32,-1);
  iCooldown = 0;
  SetR(0, pCannon);

  return inherited();
}

/* Check */

func Rotation()
{
  //Statusleuchte
  if(IsDestroyed())
  {
    if(FrameCounter()%30 < 3)
      CreateParticle("PSpark",18,-9,0,0,50,RGB(255,255,0),this());
    return;
  }
    
  if(iCooldown <= 0)
      CreateParticle("PSpark",18,-9,0,0,40,RGB(0,255,0,100),this());
  else
  {
    iCooldown -= 3;
      CreateParticle("PSpark",18,-9,0,0,40,RGB(255,0,0,100),this());
  }

  if(!bRotate) return(0);

  if(GetR(pCannon)> 80) {bRotate=0; SetR(GetR(pCannon)-1,pCannon); Sound("CannonStop"); return(0);}
  if(GetR(pCannon)<-80) {bRotate=0; SetR(GetR(pCannon)+1,pCannon); Sound("CannonStop"); return(0);}

  if(bDirection==0) {Sound("CannonRotation"); SetR(GetR(pCannon)+1,pCannon);}
  if(bDirection==1) {Sound("CannonRotation"); SetR(GetR(pCannon)-1,pCannon);}

}

/* Steuerung */

func ControlRight(pByObj)
{
  //Zerstört?
  if(IsDestroyed())
    return(PlayerMessage(GetOwner(pByObj),"$Destroyed$", this()));
    
  bRotate=1;
  bDirection=0;
}

func ControlLeft(pByObj)
{
  //Zerstört?
  if(IsDestroyed())
    return(PlayerMessage(GetOwner(pByObj),"$Destroyed$", this()));
    
  bRotate=1;
  bDirection=1;
}

func ControlDown(pByObj)
{
  //Zerstört?
  if(IsDestroyed())
    return(PlayerMessage(GetOwner(pByObj),"$Destroyed$", this()));
    
  bRotate=0;
  Sound("CannonStop");
}

func ControlUp(pByObj)
{
  //Zerstört?
  if(IsDestroyed())
    return(PlayerMessage(GetOwner(pByObj),"$Destroyed$", this()));
    
  ControlDown();
}

func ControlDig(object pByObj)
{
  //Zerstört?
  if(IsDestroyed())
    return(PlayerMessage(GetOwner(pByObj),"$Destroyed$", this()));

  //Zielpunkt berechnen
  var iX = -AbsX()+Sin(GetR(pCannon),34), iY = -AbsY()-Cos(GetR(pCannon),34)-3, iXDir = Sin(GetR(pCannon),150), iYDir = -Cos(GetR(pCannon),150);
  if(!SimFlight(iX,iY,iXDir,iYDir,50,50,500,10))
    return(1);

  //Icon setzen
  var target = CreateObject(ARCR,AbsX(iX),AbsY(iY),GetOwner(pByObj));
  SetVisibility(VIS_Owner,target);

  //Sicht zentrieren
  SetPlrView(GetOwner(pByObj),target);
  Sound("CatapultSet");
}

func ControlThrow(object pByObj)
{
  //Zerstört?
  if(IsDestroyed())
    return(PlayerMessage(GetOwner(pByObj),"$Destroyed$", this()));

  //Nicht bereit?
  if(iCooldown > 0) return(PlayerMessage(GetOwner(pByObj),"$Reloading$",this())); 

  //-20 Sekunden Feuersalve / 60 Sekunden Cooldown
  iCooldown=80*35;
  byObj = pByObj;
  SetOwner(GetOwner(pByObj));
  Sound("RadioConfirm*.ogg");
  ScheduleCall(this(),"Shoot",70,10);
  Schedule(Format("EventInfo4K(0, \"$ArtilleryLaunch$\", ATBY, 0, 0, 0, \"RadioConfirm*.ogg\")", GetPlrColorDw(GetOwner(byObj)), GetPlayerName(GetOwner(byObj))), 70);
}

/* Schuss */

public func Shoot()
{
  //Zerstört?
  if(IsDestroyed()) return;
	
  var iX=Sin(GetR(pCannon),34);
  var iY=-Cos(GetR(pCannon),34)-3;
   
  var pProjectile = CreateObject(ABLT,iX,iY,GetOwner(byObj));
  SetXDir( Sin(GetR(pCannon),RandomX(135,165)),pProjectile,10);
  SetYDir(-Cos(GetR(pCannon),RandomX(135,165)),pProjectile,10);

  //Effekte
  Sound("ATBY_Fire*.ogg");
  ObjectSetAction(pCannon,"Backdraft");
  CreateParticle("LightFlash",iX,iY,0,0,500,RGBa(255,255,255,32));
  for(var i = 0; i < 14; i++)
    CreateParticle("Smoke",iX,iY+RandomX(-20,20),0,0,RandomX(50,100),RGB(96,96,96));
  MuzzleFlash(RandomX(30,75),this(),iX,iY,GetR(pCannon));
}

/* Schaden */

public func OnDestruction()
{
  //Waffe entfernen
  if(pCannon) pCannon->RemoveObject();
  iCooldown = 0;
  RemoveEffect("ShowWeapon",this); 
}

public func OnDmg(int iDmg, int iType)
{
  if(iType == DMG_Explosion)	return(0);	//Maximale Wirkung von Sprengstoff
  return(80);					//Default
}

/* Reperatur */

public func OnRepair()
{
  //Neue Kanone
  pCannon = CreateObject(CNON,0,32,-1);
}