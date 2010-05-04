/*-- Artilleriebatterie --*/

#strict

local pCannon;
local bRotate,bDirection;
local iCooldown;
local byObj;
local Damaged;
local Repairing;
local autorepair;

public func IsMachine()		{return(true);} //Maschine
//public func IsBulletTarget()   {if(!Random(3)) return(true);} //Ziel


/* Initalisierung */

func Initialize()
{
  pCannon=CreateObject(CNON,0,32,-1);
  iCooldown = 0;
  autorepair = 1;
  SetR(RandomX(-44,44),pCannon);
  return(1);
}

public func MaxDamage()
{
  return(200);
}

/* Check */

func Rotation()
{
  if(Damaged)
  {
    if(FrameCounter()%30 < 3)
      CreateParticle("PSpark",19,-7,0,0,50,RGB(255,255,0),this());
    return();
  }
    
  if(iCooldown <= 0)
      CreateParticle("PSpark",19,-7,0,0,40,RGB(0,255,0,100),this());
  else
  {
    iCooldown -= 3;
      CreateParticle("PSpark",19,-7,0,0,40,RGB(255,0,0,100),this());
  }

  if(!bRotate) return(0);

  if(GetR(pCannon)> 80) {bRotate=0; SetR(GetR(pCannon)-1,pCannon); Sound("CannonStop"); return(0);}
  if(GetR(pCannon)<-80) {bRotate=0; SetR(GetR(pCannon)+1,pCannon); Sound("CannonStop"); return(0);}

  if(bDirection==0) {Sound("CannonRotation"); SetR(GetR(pCannon)+1,pCannon);}
  if(bDirection==1) {Sound("CannonRotation"); SetR(GetR(pCannon)-1,pCannon);}

}

/* Kontrolle */

func ControlRight(pByObj)
{
  //Abfrage
  if(Damaged)
    return(PlayerMessage(GetOwner(pByObj),"$Destroyed$", this()));
    
  bRotate=1;
  bDirection=0;
}

func ControlLeft(pByObj)
{
  //Abfrage
  if(Damaged)
    return(PlayerMessage(GetOwner(pByObj),"$Destroyed$", this()));
    
  bRotate=1;
  bDirection=1;
}

func ControlDown(pByObj)
{
  //Abfrage
  if(Damaged)
    return(PlayerMessage(GetOwner(pByObj),"$Destroyed$", this()));
    
  bRotate=0;
  Sound("CannonStop");
}

func ControlUp(pByObj)
{
  //Abfrage
  if(Damaged)
    return(PlayerMessage(GetOwner(pByObj),"$Destroyed$", this()));
    
  ControlDown();
}

func ControlDig(object pByObj)
{
  //Abfrage
  if(Damaged)
    return(PlayerMessage(GetOwner(pByObj),"$Destroyed$", this()));
    
  var iX = -AbsX()+Sin(GetR(pCannon),34), iY = -AbsY()-Cos(GetR(pCannon),34)-3, iXDir = Sin(GetR(pCannon),150), iYDir = -Cos(GetR(pCannon),150);
  if(!SimFlight(iX,iY,iXDir,iYDir,50,50,500,10))
    return(1);
  var target = CreateObject(ARCR,AbsX(iX),AbsY(iY),GetOwner(pByObj));
  SetVisibility(VIS_Owner,target);
  SetPlrView(GetOwner(pByObj),target);
  Sound("Info.ogg");
}

func ControlThrow(object pByObj)
{
  //Abfrage
  if(Damaged)
    return(PlayerMessage(GetOwner(pByObj),"$Destroyed$", this()));
    
  if(iCooldown > 0) return(PlayerMessage(GetOwner(pByObj),"$Reloading$",this())); 

  iCooldown=80*35; //-20 Sekunden Feuersalve... 60 Sek Cooldown
  byObj = pByObj;
  SetOwner(GetOwner(pByObj));
  Sound("Info.ogg");
  ScheduleCall(this(),"Shoot",70,10);
}

/* Schuss */

public func Shoot()
{
  var iX=Sin(GetR(pCannon),34);
  var iY=-Cos(GetR(pCannon),34)-3;
   
  var pProjectile=CreateObject(ABLT,iX,iY,GetOwner(byObj));
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


//------------------------------------------------------------------------------
///* Reperatur */---------------------------------------------------------------
//---------------------------------------(Ich brauch das für die Ordnung :I)----

public func StartRepair()
{
  ClearScheduleCall(this, "StartRepair");
  Log("%v , %v",!Repairing,!WildcardMatch(GetAction(), "*Repair*"));
  if(!Repairing && !WildcardMatch(GetAction(), "*Repair*"))
  {
   Repairing = true;
   SetAction("StartRepair");
  }
}

public func Repair()
{ 
  //Jetzt gepanzert
  DoDamage(-GetDamage());
  if(!GetEffect("IntRepair")) AddEffect("IntRepair",this,50,5,this);
}

public func StopRepair()
{
  Repairing = false;
  Damaged = 0;
  DoDamage(-GetDamage());
  pCannon = CreateObject(CNON,0,32,-1);
  SetAction("Idle");
}

public func AutoRepair()
{
  if(autorepair)
    ScheduleCall(this,"StartRepair",autorepair+RandomX(-50,+50));
}

public func SetAutoRepair(int iAuto)
{
  autorepair = iAuto;
}

/* Reparatureffekt */

public func FxIntRepairStart(object pTarget, int iEffectNumber, int iTemp)
{
  Sound("Repair.ogg",false,this,50,0,+1); 
  return 1;
}

public func FxIntRepairTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  if(iEffectTime >= 35*20)
   return -1;

  if(!Random(2))
   Sparks(2+Random(5), RGB(187, 214, 224), RandomX(-GetDefWidth()/2,+GetDefWidth()/2), RandomX(-GetDefHeight()/2,+GetDefHeight()/2));

  return 0;
}

public func FxIntRepairStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  Sound("Repair.ogg",false,this,0,0,-1); 
  if(!iReason)
  {
   pTarget->SetAction("RepairStop");
  }
  return 0;
}

/* Schaden */

public func Damage()
{
  return;
  if(GetDamage() > MaxDamage());
  RemoveObject(pCannon);
  SetAction("Destroyed");
  Damaged = 1;
  iCooldown = 0;
  AutoRepair();
}

public func OnDmg(int iDmg, int iType)
{
	if(iType == DMG_Explosion)	return(0); //Maximale Wirkung von Sprengstoff
	return(80); //Default
}
