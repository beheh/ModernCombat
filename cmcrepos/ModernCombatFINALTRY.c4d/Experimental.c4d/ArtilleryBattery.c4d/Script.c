/*-- Artilleriebatterie --*/

#strict

local pCannon;
local bRotate,bDirection;
local iCooldown;
local byObj;
local Damaged;
local Repairing;
local autorepair;

public func IsMachine()		{return(true);}
public func MaxDamage()		{return(200);}


/* Initalisierung */

func Initialize()
{
  pCannon=CreateObject(CNON,0,32,-1);
  iCooldown = 0;
  autorepair = 36*30;
  SetR(0, pCannon); //Findet Michael ja sch�ner als SetR(RandomX(-44,44),pCannon); =)
  return(1);
}

/* Check */

func Rotation()
{
  if(Damaged)
  {
    if(FrameCounter()%30 < 3)
      CreateParticle("PSpark",18,-9,0,0,50,RGB(255,255,0),this());
    return();
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
  Sound("CatapultSet");
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
  Sound("C4EX_Ignition.ogg");
  ScheduleCall(this(),"Shoot",70,10);
  Schedule(Format("EventInfo4K(0, \"$ArtilleryLaunch$\", ATBY)", GetPlrColorDw(GetOwner(byObj)), GetPlayerName(GetOwner(byObj))), 70);
}

/* Schuss */

public func Shoot()
{
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

/* Reperatur */

public func StartRepair()
{
  ClearScheduleCall(this, "StartRepair");
  if(!Repairing && !WildcardMatch(GetAction(), "*Repair*"))
  {
   Repairing = true;
   SetAction("RepairStart");
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
   pTarget->SetAction("RepairEnd");
  }
  return 0;
}

/* Schaden */

public func Damage()
{
  if(GetDamage() > MaxDamage() && !Damaged)
  {
   Destroyed();
  }
}

public func Destroyed()
{
  //Waffe entfernen
  if(pCannon) pCannon->RemoveObject();

  //Status setzen
  SetAction("Destroyed");
  Damaged = 1;
  iCooldown = 0;
  RemoveEffect("ShowWeapon",this); 

  //Reparatur anordnen
  AutoRepair();

  //Punkte bei Belohnungssystem
  if(GetKiller(this) != -1)
    if((GetOwner() != -1 && Hostile(GetOwner(), GetKiller())) || GetOwner() == -1 && !GetTeam(this))
		  DoPlayerPoints(BonusPoints("Destruction"), RWDS_BattlePoints, GetKiller(this), GetCursor(GetKiller(this)), IC03);

  //Effekte
  CastParticles("MetalSplinter",6,150,0,-10,40,150);
  CastParticles("Smoke3",10,25,0,0,50,200);
  Sound("StructuralDamage*.ogg");

  //Explosion
  CreateObject(ROCK,0,0)->Explode(20);
}

public func OnDmg(int iDmg, int iType)
{
	if(iType == DMG_Explosion)	return(0); //Maximale Wirkung von Sprengstoff
	return(80); //Default
}