/*-- Sentry Gun --*/

#strict 2

local cur_Attachment;
local aim_angle;
local target_angle;
local Shooting;
local iPat_Dir;
local Active;
local GotTarget;
local Damaged;
local Repairing;
local autorepair;
local last_id;

public func GetAttWeapon()	{return(cur_Attachment);}	//Waffe
public func MaxRotLeft()	{return(110+GetR());}		//Maximaler Winkel links
public func MaxRotRight()	{return(250+GetR());}		//Maximaler Winkel rechts
public func SearchLength()	{return(250);}			//Suchlänge
public func AimAngle()		{return(aim_angle+GetR());}	//Winkel auf Ziel
public func ReadyToFire()	{return(1);}			//Allzeit bereit
public func IsMachine()		{return(true);}			//Ist eine Elektrische Anlage
public func IsBulletTarget()	{return(true);}			//Kugelziel
public func IsAiming()		{return(true);}			// Die Sentry Gun "zielt" immer
public func IsThreat()		{return(Active);}
public func UpdateCharge()	{return(1);}


/* Reperatur */

public func StartRepair()
{
  ClearScheduleCall(this, "StartRepair");
  if(!Repairing && !WildcardMatch(GetAction(), "*Repair*"))
  {
   Repairing = true;
   SetAction("RepairStart");
   RemoveEffect("ShowWeapon",this);
  }
}

public func Repair()
{ 
  //Jetzt gepanzert
  DoDamage(-GetDamage());
  if(!GetEffect("IntRepair")) AddEffect("IntRepair",this,50,5,this);
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

public func StopRepair()
{
  Repairing = false;
  Damaged = 0;
  DoDamage(-GetDamage());
  Arm(last_id);
  AddEffect("ShowWeapon",this,1,1,this,GetID());
  SetGraphics(0,this,GetID(),3,5,0,0,this);
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

/* Zerstörung */

public func Destroyed()
{
  last_id = GetID(Contents());
  RemoveObject(Contents());
  SetAction("Destroyed");
  Damaged = 1;
  RemoveEffect("ShowWeapon",this); 
  AutoRepair();
  CreateObject(ROCK,0,0)->Explode(20);
}

/* Schaden */

public func OnDmg(int iDmg, int iType)
{
	if(iType == DMG_Explosion)	return(0); //Maximale Wirkung von Sprengstoff
	return(80); //Default
}

/* Aufrufe */

public func TurnOn()
{
  Active = true;
}

public func TurnOff()
{
  Active = false;
  if(GetAttWeapon()) GetAttWeapon()->StopAutoFire();
}

public func Arm(id idWeapon)
{
  //Crash-Vermeidung
  if(!idWeapon) return;
  if(!GetName(0, idWeapon)) return;

  //Ausrüsten mit idWeapon
  var pWeapon = CreateObject(idWeapon, 0, 0, GetOwner());
  Enter(this(),pWeapon);
  //Ordnern
  SetObjectOrder(this(), pWeapon, 1);
  aim_angle = 180;
  iPat_Dir = -1+ Random(2)*2;
  cur_Attachment = pWeapon;
  LocalN("controller", pWeapon) = this();
  Reload();
}

/* Script */

public func Initialize() 
{
  AddEffect("ShowWeapon",this(),1,1,this(),GetID());
  SetGraphics(0,this(),GetID(),3,5,0,0,this());
}

public func WeaponAt(&x, &y, &r)
{
  x = Sin(GetR()-180,7000);
  y = -Cos(GetR()-180,7000);
  r = aim_angle+270+GetR();
  return(1);
}

public func WeaponBegin(&x, &y)
{
  var number = GetEffect("ShowWeapon",this());
  if(!number)
   return(0);
  x = EffectVar(2, this(), number)/1000;
  y = EffectVar(3, this(), number)/1000;
}

public func WeaponEnd(&x, &y)
{
  var number = GetEffect("ShowWeapon",this());
  if(!number)
   return(0);
  x = EffectVar(4, this(), number)/1000;
  y = EffectVar(5, this(), number)/1000;
}

public func GetWeaponR()
{
  var number = GetEffect("ShowWeapon",this());
  if(!number)
   return(0);
  return(EffectVar(1, this(), number));
}

public func Activity()
{
  var iHeight, iWidth, iAngle;
  //Wuah, haben wir eine Waffe?
  if(! GetAttWeapon()) return;
  //Sind wir im Eimer?
  if(EMPShocked()) return;
  if(Damaged) return;
  //Sind wir aktiv?
  if(!Active) return;
  if(Repairing) return;
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
    else
      var rgb = RGB(255,255,255);
    CreateParticle("NoGravSpark",Sin(GetR()+45,5),-Cos(GetR()+45,5)-10,0,0,5*15,rgb,cur_Attachment);
  }
  
  /* Patroullie fahren */
  
  // alle 5 Frames
  if(!(GetActTime()%5)) {
	  //Zu weit links?
	  if( AimAngle() <= MaxRotLeft() )
	  {
	    //Wir fahren zurück
	    iPat_Dir = 1;
	    target_angle = MaxRotRight();
	    GotTarget = 0;
	  }
	  //Oder zu weit rechts?
	  else if( AimAngle() >= MaxRotRight() )
	  {
	    //Hinfahren
	    iPat_Dir = -1;
	    target_angle = MaxRotLeft();
	    GotTarget = 0;
	  }
	  if(!GotTarget) {
  		aim_angle += iPat_Dir*3;
    }
  }
  //Das Fahren selber ;)
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
		target_angle = Angle(GetX(), GetY() + 7, GotTarget->GetX(), GotTarget->GetY());
		if(Abs(AimAngle() - target_angle) < 15)
		{
		    Shooting = true;
		    if(!GetAttWeapon()->IsShooting())
			    GetAttWeapon()->ControlThrow(this());
	  	    if(GetAmmo(GetAttWeapon()->GetFMData(FM_AmmoID), GetAttWeapon()) < GetAttWeapon()->GetFMData(FM_AmmoUsage))
				Reload();
		}
		else
		{
	    Shooting = false;
   		GetAttWeapon()->StopAutoFire();
		}
		
		if(!CheckTarget(GotTarget,this()))
			GotTarget = 0;
		else if(!PathFree(GetX(),GetY()+7,GotTarget->GetX(), GotTarget->GetY()))
			GotTarget = 0;
		else if(ObjectDistance(GotTarget,this()) > SearchLength())
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

	var w,h;
  
	var Targets = FindTargets(this(), SearchLength()); //FindObjects(Find_Distance(SearchLength()),Find_PathFree(),Find_NoContainer());
  for(pAim in Targets)
  {
    if(GetOwner() != NO_OWNER)
      if(pAim->GetOwner() == GetOwner() || !Hostile(pAim->GetOwner(), GetOwner()))
        continue;

		if(!CheckTarget(pAim,this()))
			continue;


    //Winkel zum Ziel
    target_angle = Angle(GetX(), GetY() + 7, pAim->GetX(), pAim->GetY());

		target_angle = Normalize(target_angle, 0);
    if(target_angle < MaxRotLeft() || target_angle > MaxRotRight())
    	continue;

		return pAim;


    break;
  }
  
  if(Shooting && ! pAim)
  {
    Shooting = false;
    GetAttWeapon()->StopAutoFire();
  }
  
  if(!pAim)
    GotTarget = true;
}

private func Reload()
{
  // Munitionsart
  var AmmoID = GetAttWeapon()->~GetFMData(FM_AmmoID);
  // Erzeugen
  Local(0, CreateContents(AmmoID)) = GetAttWeapon()->~GetFMData(FM_AmmoLoad);
  // Waffe soll nachladen
  GetAttWeapon()->~Reloaded(this());
  GetAttWeapon()->~Recharge();
  GetAttWeapon()->~StopAutoFire();
}

public func MaxDamage()
{
  return(100);
}

public func Damage()
{
  if(GetDamage() > MaxDamage() && !Damaged)
  {
   Destroyed();
  }
}

/* EMP */

public func EMPShock()
{
  EMPShockEffect(20*35);
  if(GetAttWeapon())
   GetAttWeapon()->StopAutoFire();
  return(1);
}

/* Konsolensteuerung */

public func ConsoleControl(int i)
{
    if(i == 1)
    {
      if(Active) return "$TurnOff$";
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
        if(Active) TurnOff();
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

public func RejectContainedSerialize(object foo) { return !false; } // weg mit den alten Waffen

public func Serialize(array& extra)
{
	extra[GetLength(extra)] = Format("SetTeam(%d)", GetTeam());
	if (Active)
		extra[GetLength(extra)] = "TurnOn()";
	if (cur_Attachment) {
		extra[GetLength(extra)] = Format("Arm(%i)", GetID(cur_Attachment));
		extra[GetLength(extra)] = Format("LocalN(\"aim_angle\")=%d", aim_angle);
		extra[GetLength(extra)] = Format("LocalN(\"iPat_Dir\")=%d", iPat_Dir);
	}
}
