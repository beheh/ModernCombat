/*-- Artilleriebatterie --*/

#strict 2

#appendto ATBY

local loaded_ammo;
local plr_locked;

public func NewAmmo()
{
	loaded_ammo = 3;
	return true;
}

func ControlThrow(object pByObj)
{
	if(plr_locked)
		return false;

	//Zerstört?
  if(IsDestroyed())
    return PlayerMessage(GetOwner(pByObj),"$Destroyed$", this);

  //Nicht bereit?
  if(iCooldown > 0) return PlayerMessage(GetOwner(pByObj),"$Reloading$",this); 

  //-20 Sekunden Feuersalve / 60 Sekunden Cooldown
  iCooldown = 80*35;
  SetController(GetController(pByObj));
  
  if(FindObject(MARD))
  	ScheduleCall(this, "Shoot", 70, 3);
  else
 		ScheduleCall(this, "Shoot", 70, 10);
  
  ScheduleCall(this, "BeginAttack", 70);

  Sound("Acknowledge.ogg", 0, pByObj, 100, GetOwner(pByObj)+1);
}

public func Shoot()
{
  //Zerstört?
  if(IsDestroyed()) return false;
	if(FindObject(MARD) && !loaded_ammo)
		return false;
	
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
  if(FindObject(MARD))
  {
  	loaded_ammo--;
  	if(!loaded_ammo)
  		FindObject(MARD)->LastProjectile(pProjectile);
  }
  //if(Minigame() == 6)
  //	ScheduleCall(this, "Shoot", 70);
}

public func Damage()
{
	if(FindObject(MARD))
		return false;
		
  return _inherited(...);
}

public func SetMinigame(int iMinigame, value)
{
	/*if(iMinigame == 6)
	{
		DrawMaterialQuad("Wall", GetX()-25, GetY()+11, GetX()-25, GetY()+21, GetX()+25, GetY()+21, GetX()+25, GetY()+11);
		var rdir = value;
		SetRotation(RandomX(40, 100)*rdir);
		plr_locked = true;
		ControlThrow();
		Shoot();
	}*/
}

/* Steuerung */

func ControlRight()
{
  if(plr_locked) return true;
  
  return _inherited(...);
}

func ControlLeft()
{
  if(plr_locked) return true;
  
  return _inherited(...);
}

func ControlDown()
{
  if(plr_locked) return true;
  
  return _inherited(...);
}

func ControlUp()
{
  if(plr_locked) return true;
  
  return _inherited(...);
}

func ControlDig()
{
  if(plr_locked) return true;
  
  return _inherited(...);
}
