/*-- Selbstschussanlage --*/

#strict 2
#include CSTR

local cur_Attachment;
local aim_angle;
local target_angle;
local Shooting;
local iPat_Dir;
local fActive;
local GotTarget;
local last_id;

public func GetAttWeapon()			{return cur_Attachment;}		//Waffe
public func MaxRotLeft()			{return 110+GetR();}			//Maximaler Winkel links
public func MaxRotRight()			{return 250+GetR();}			//Maximaler Winkel rechts
public func SearchLength()			{return 300;}				//Suchlänge
public func AimAngle()				{return aim_angle+GetR();}		//Winkel auf Ziel
public func ReadyToFire()			{return 1;}				//Allzeit bereit
public func IsMachine()				{return true;}				//Ist eine Elektrische Anlage
public func IsBulletTarget()			{return !IsDestroyed();}		//Kugelziel wenn nicht zerstört
public func IsAiming()				{return true;}				//Selbstschussanlage immer am Zielen
public func IsThreat()				{return fActive && !IsDestroyed();}	//Status
public func UpdateCharge()			{return 1;}
public func AttractTracer(object pTracer)	{return GetPlayerTeam(GetController(pTracer)) != GetTeam() && !IsDestroyed();}
public func RemoveTracer()			{return IsDestroyed();}			//Tracer entfernen, wenn zerstört


/* Initialisierung */

public func Initialize() 
{
  AddEffect("ShowWeapon",this,1,1,this,GetID());
  SetGraphics(0,this,GetID(),3,5,0,0,this);
}

/* Zerstörung */

public func OnDestruction()
{
  //Waffe entfernen
  Disarm();

  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",3,20,0,0,220,500);
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",4,100,0,0,40,15,RGB(40,20,20));
}

public func Destruction()
{
  RemoveEffect("ShowWeapon", this);
}

public func OnRepair()
{
  Arm(last_id);
}

/* Bonus-Punkte */

public func BonusPointCondition()
{
  return fActive;
}

/* Schaden */

public func OnDmg(int iDmg, int iType)
{
  if(iType == DMG_Projectile)	return 40;	//Projektile
  if(iType == DMG_Fire)		return 60;	//Feuer
  if(iType == DMG_Explosion)	return;		//Explosionen und Druckwellen
  if(iType == DMG_Energy)	return 50;	//Energiewaffen
  if(iType == DMG_Bio)		return 100;	//Säure und biologische Schadstoffe
  return 50;
}

/* Aufrufe */

public func TurnOn()
{
  fActive = true;
}

public func TurnOff()
{
  fActive = false;
  if(GetAttWeapon()) GetAttWeapon()->StopAutoFire();
}

public func Arm(id idWeapon)
{
  //Crash-Vermeidung
  if(!idWeapon) return;
  if(!GetName(0, idWeapon)) return;

  //Ausrüsten mit idWeapon
  var pWeapon = CreateObject(idWeapon, 0, 0, GetOwner());
  Enter(this,pWeapon);
   
  //Ordnern
  SetObjectOrder(this, pWeapon, 1);
  aim_angle = 180;
  iPat_Dir = -1+ Random(2)*2;
  cur_Attachment = pWeapon;
  LocalN("controller", pWeapon) = this;
  Reload();
  
  //Effekt
  AddEffect("ShowWeapon", this, 20, 1, this);
}

public func Disarm()
{
  while(Contents())
  {
   last_id = GetID(Contents());
   RemoveObject(Contents());
  }
  RemoveEffect("ShowWeapon", this);
}

/* Positionsbestimmung */

public func WeaponAt(&x, &y, &r)
{
  x = Sin(GetR()-180,7000);
  y = -Cos(GetR()-180,7000);
  r = aim_angle+270+GetR();
  return 1;
}

public func WeaponBegin(&x, &y)
{
  var number = GetEffect("ShowWeapon",this);
  if(!number)
   return;
  x = EffectVar(2, this, number)/1000;
  y = EffectVar(3, this, number)/1000;
}

public func WeaponEnd(&x, &y)
{
  var number = GetEffect("ShowWeapon",this);
  if(!number)
   return;
  x = EffectVar(4, this, number)/1000;
  y = EffectVar(5, this, number)/1000;
}

public func GetWeaponR()
{
  var number = GetEffect("ShowWeapon",this);
  if(!number)
   return;
  return EffectVar(1, this, number);
}

public func Activity()
{
  //Wuah, haben wir eine Waffe?
  if(!GetAttWeapon()) return;
  //Sind wir im Eimer?
  if(EMPShocked()) return;
  if(IsDestroyed()) return;
  //Sind wir aktiv?
  if(!fActive) return;
  if(IsRepairing()) return;
  //Wenn nicht schon gesetzt: Turn-Action
  if(GetAction() != "Turn")
    SetAction("Turn");
    
  //Owner updaten
  cur_Attachment->SetTeam(GetTeam());

  // alle 30 Frames
  if(!(GetActTime()%30))
  {
    if(GetTeam())
      var rgb = GetTeamColor(GetTeam());
    else if(GetOwner())
      var rgb = GetPlrColorDw(GetOwner());
    else
      var rgb = RGB(255,255,255);
    CreateParticle("FlashLight",0,4,0,0,3*15,rgb,this);
  }
  
  /* Patroullie fahren */
  
  // alle 5 Frames
  if(!(GetActTime()%5))
  {
    //Zu weit links?
    if(AimAngle() <= MaxRotLeft())
    {
      //Wir fahren zurück
      iPat_Dir = 1;
      target_angle = MaxRotRight();
      GotTarget = 0;
    }
    //Oder zu weit rechts?
    else if(AimAngle() >= MaxRotRight())
    {
      //Hinfahren
      iPat_Dir = -1;
      target_angle = MaxRotLeft();
      GotTarget = 0;
    }
    if(!GotTarget)
    {
      aim_angle += iPat_Dir*3;
    }
  }
  if(GotTarget)
  	aim_angle += BoundBy(target_angle-AimAngle(),-1,1);
       
  /* Feinde suchen */
  
  if(!GotTarget)
  {
  	if(Shooting)
  	{
	    Shooting = false;
   		GetAttWeapon()->StopAutoFire();
  	}
 		GotTarget = Search();
	}
	else
	{
		target_angle = Angle(GetX(), GetY() + 7, GetX(GotTarget), GetY(GotTarget));
		if(Abs(AimAngle() - target_angle) < 15)
		{
		    Shooting = true;
		    if(!GetAttWeapon()->IsShooting())
			    GetAttWeapon()->Fire(this);
	  	  if(GetAmmo(GetAttWeapon()->GetFMData(FM_AmmoID), GetAttWeapon()) < GetAttWeapon()->GetFMData(FM_AmmoUsage))
					Reload();
		}
		else
		{
	    Shooting = false;
   		GetAttWeapon()->StopAutoFire();
		}
		
		if(!CheckTarget(GotTarget,this))
			GotTarget = 0;
		else if(!PathFree(GetX(),GetY()+7,GetX(GotTarget), GetY(GotTarget)))
			GotTarget = 0;
		else if(ObjectDistance(GotTarget,this) > SearchLength())
			GotTarget = 0;
	}
	 
  /*
  //Berechnung
  iAngle = aim_angle;
  iWidth =  Sin(iAngle, SearchLength());
  iHeight = -Cos(iAngle, SearchLength());

  if(iWidth < 0)
    Search(iWidth, -iWidth, iHeight);
  else
    Search(0, iWidth, iHeight);
  */
}

public func Search(int iX, int iWidth, int iHeight)
{
  var pAim;
  
  /*DrawParticleLine("PSpark", 0, 0, -70 + Sin(aim_angle, SearchLength()), SearchLength(), 10, 80, RGB(255, 0, 0));
  DrawParticleLine("PSpark", 0, 0, 70 + Sin(aim_angle, SearchLength()), SearchLength(), 10, 80, RGB(255, 0, 0));*/ 

	var Targets = FindTargets(this, SearchLength());
	var pTarget = 0;
  for(pAim in Targets)
  {
		if(GetOwner() != NO_OWNER)
			if(GetOwner(pAim) == GetOwner() || !Hostile(GetOwner(pAim), GetOwner()))
				continue;

		if(!CheckTarget(pAim,this))
			continue;

		//Winkel zum Ziel
		target_angle = Angle(GetX(), GetY() + 7, GetX(pAim), GetY(pAim));

		target_angle = Normalize(target_angle, 0);
		if(target_angle < MaxRotLeft() || target_angle > MaxRotRight())
			continue;

		var pTarget = pAim;
		break;
  }
  
	if(Shooting && !pTarget)
	{
		Shooting = false;
		GetAttWeapon()->StopAutoFire();
	}

	if(!pTarget)
  	GotTarget = true;

  return pTarget;
}

private func Reload()
{
  // Munitionsart
  var AmmoID = GetAttWeapon()->~GetFMData(FM_AmmoID);
  // Erzeugen
  Local(0, CreateContents(AmmoID)) = GetAttWeapon()->~GetFMData(FM_AmmoLoad);
  // Waffe soll nachladen
  GetAttWeapon()->~Reloaded(this);
  GetAttWeapon()->~Recharge();
  GetAttWeapon()->~StopAutoFire();
}

public func MaxDamage()
{
  return 80;
}

/* EMP */

public func EMPShock()
{
  EMPShockEffect(20*35);
  if(GetAttWeapon())
   GetAttWeapon()->StopAutoFire();
  return 1;
}

/* Konsolensteuerung */

public func ConsoleControl(int i)
{
  if(i == 1)
  {
    if(fActive) return "$TurnOff$";
    else
    return "$TurnOn$";
  }
  if(i == 2)
    if(GetAction() == "Destroyed")
      return "$Repair$";
}

public func ConsoleControlled(int i)
{
  if(i == 1)
  {
    if(fActive) TurnOff();
    else
    {
      TurnOn();
    }
  }
  if(i == 2)
  {
    if(GetAction() == "Destroyed")
      StartRepair();
  }
}

/* Serialisierung */

public func RejectContainedSerialize(object foo)	{return !false;} //weg mit den alten Waffen

public func Serialize(array& extra)
{
  extra[GetLength(extra)] = Format("SetTeam(%d)", GetTeam());
  if (fActive)
    extra[GetLength(extra)] = "TurnOn()";
  if (cur_Attachment)
  {
    extra[GetLength(extra)] = Format("Arm(%i)", GetID(cur_Attachment));
    extra[GetLength(extra)] = Format("LocalN(\"aim_angle\")=%d", aim_angle);
    extra[GetLength(extra)] = Format("LocalN(\"iPat_Dir\")=%d", iPat_Dir);
  }
}