/*-- Artilleriebatterie --*/

#strict 2
#include CSTR

local pCannon;
local bRotate, bDirection;
local iCooldown;
local byObj;

public func IsMachine()		{return true;}
public func MaxDamage()		{return 150;}
public func RepairSpeed()	{return 2;}


/* Initialisierung */

func Initialize()
{
  //Kanone erstellen
  pCannon = CreateObject(CNON,0,32,-1);
  iCooldown = 0;
  pCannon->~Attach(this, 4);
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
      CreateParticle("PSpark",18,-9,0,0,50,RGB(255,255,0),this);
    return;
  }

  if(iCooldown <= 0)
    CreateParticle("PSpark",18,-9,0,0,40,RGB(0,255,0,100),this);
  else
  {
    iCooldown -= 3;
      CreateParticle("PSpark",18,-9,0,0,40,RGB(255,0,0,100),this);
  }

  if(!bRotate) return;

  if(!FindObject2(Find_Action("Push"), Find_ActionTarget(this))) {bRotate = 0; Sound("CannonStop"); return;}

  if(GetR(pCannon)> 80) {bRotate=0; SetR(GetR(pCannon)-1,pCannon); Sound("CannonStop"); return;}
  if(GetR(pCannon)<-80) {bRotate=0; SetR(GetR(pCannon)+1,pCannon); Sound("CannonStop"); return;}

  if(bDirection==0) {Sound("CannonRotation"); SetR(GetR(pCannon)+1,pCannon);}
  if(bDirection==1) {Sound("CannonRotation"); SetR(GetR(pCannon)-1,pCannon);}
}

func SetRotation(aRotation)
{
  if(pCannon)
    SetR(aRotation, pCannon);
}

/* Steuerung */

func ControlRight(pByObj)
{
  //Zerstört?
  if(IsDestroyed())
    return PlayerMessage(GetOwner(pByObj),"$Destroyed$", this);

  bRotate=1;
  bDirection=0;
}

func ControlLeft(pByObj)
{
  //Zerstört?
  if(IsDestroyed())
    return PlayerMessage(GetOwner(pByObj),"$Destroyed$", this);

  bRotate=1;
  bDirection=1;
}

func ControlDown(pByObj)
{
  //Zerstört?
  if(IsDestroyed())
    return PlayerMessage(GetOwner(pByObj),"$Destroyed$", this);

  bRotate=0;
  Sound("CannonStop");
}

func ControlUp(pByObj)
{
  //Zerstört?
  if(IsDestroyed())
    return PlayerMessage(GetOwner(pByObj),"$Destroyed$", this);

  ControlDown();
}

func ControlDig(object pByObj)
{
  //Zerstört?
  if(IsDestroyed())
    return PlayerMessage(GetOwner(pByObj),"$Destroyed$", this);

  //Flugbahn berechnen
  var iX = Sin(GetR(pCannon),34), iY = -Cos(GetR(pCannon),34)-3, iXDir = Sin(GetR(pCannon),150), iYDir = -Cos(GetR(pCannon),150);

  if (SimFlight2(iX, iY, iXDir, iYDir))
  {
    var target = CreateObject(ARCR, iX, iY, GetOwner(pByObj));
    SetVisibility(VIS_Owner, target);
    SetPlrView(GetOwner(pByObj), target);
    target->Set(pByObj);
    Sound("CatapultSet");
  }
}

func ControlThrow(object pByObj)
{
  //Zerstört?
  if(IsDestroyed())
    return PlayerMessage(GetOwner(pByObj),"$Destroyed$", this);

  //Nicht bereit?
  if(iCooldown > 0) return PlayerMessage(GetOwner(pByObj),"$Reloading$",this); 

  //-20 Sekunden Feuersalve / 60 Sekunden Cooldown
  iCooldown = 80*35;
  SetController(GetController(pByObj));
  ScheduleCall(this, "Shoot",70,10);
  ScheduleCall(this, "BeginAttack", 70);

  Sound("Acknowledge.ogg", 0, pByObj, 100, GetOwner(pByObj)+1);
}

public func BeginAttack()
{
  //Zerstört?
  if(IsDestroyed()) return;

  //Eventnachricht: Artillerieschlag
  if(GetController() != -1) EventInfo4K(0, Format("$ArtilleryLaunch$", GetPlrColorDw(GetController()), GetPlayerName(GetController())), ATBY, 0, 0, 0, "RadioConfirm*.ogg");
  Sound("StrikeAlert.ogg", false, this, 100);
  return true;
}

/* Schuss */

public func Shoot()
{
  //Zerstört?
  if(IsDestroyed()) return;

  var iX=Sin(GetR(pCannon),34);
  var iY=-Cos(GetR(pCannon),34)-3;

  var pProjectile = CreateObject(ABLT,iX,iY,GetOwner(byObj));
  SetController(GetController(), pProjectile);
  SetXDir( Sin(GetR(pCannon),RandomX(135,165)),pProjectile,10);
  SetYDir(-Cos(GetR(pCannon),RandomX(135,165)),pProjectile,10);

  //Effekte
  ObjectSetAction(pCannon, "Backdraft", this);
  CreateParticle("LightFlash",iX,iY,0,0,500,RGBa(255,255,255,32));
  for(var i = 0; i < 14; i++)
    CreateParticle("Smoke",iX,iY+RandomX(-20,20),0,0,RandomX(50,100),RGB(96,96,96));
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("GunSmoke",4,10,0,0,500,100,RGBa(255,255,255,0));
  MuzzleFlash(RandomX(50,80),this,iX,iY,GetR(pCannon),RGB(255,100,100),5);
  Sound("ATBY_Fire*.ogg");
  Echo("RTLR_Echo.ogg");
}

/* Schaden */

public func OnDestruction()
{
  //Waffe entfernen
  if(pCannon) RemoveObject(pCannon);

  //Cooldown zurücksetzen
  iCooldown = 0;

  RemoveEffect("ShowWeapon", this); 

  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",15,20,0,0,220,500);
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",4,110,0,0,40,100);
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",8,150,0,0,40,15,RGB(40,20,20));
  Sound("StructureHeavyHit*.ogg");
}

public func OnDmg(int iDmg, int iType)
{
  if(iType == DMG_Fire)		return 60;	//Feuer
  if(iType == DMG_Explosion)	return;		//Explosionen und Druckwellen
  if(iType == DMG_Bio)		return 100;	//Säure und biologische Schadstoffe
  return 80;
}

/* Reperatur */

public func OnRepair()
{
  //Neue Kanone
  pCannon = CreateObject(CNON,0, 32, -1);
  pCannon->~Attach(this, 4);
}

/* Normale Entfernung */

public func Destruction()
{
  if(pCannon) RemoveObject(pCannon);
  RemoveEffect("ShowWeapon",this); 
}