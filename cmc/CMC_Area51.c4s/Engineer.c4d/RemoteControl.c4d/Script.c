/*-- Fernsteuerung --*/

#strict 2

local pTarget;
local fControlling;
local pClonk;
local throwActive;
local lastMovement; //1. (niedrigstes) Bit: Links 2. Bit: Rechts 3. Bit: Hoch 4. Bit: Runter
local tempView;

public func IsDrawable()	{return true;}	//Wird sichtbar getragen
public func HandX()		{return 4000;}
public func HandY()		{return 10;}
public func HandSize()		{return 1000;}
public func IsControlling() {return fControlling;}

func Activate(object pCaller)
{
  if(fControlling)
  {
  	Cancel();
  	if(pTarget && pTarget->~IsPatrolBoatMotor())
  		pTarget->Grabbed(this, false);
  	return true;
  }
  
  if(pTarget)
  {
  	SetComDir(COMD_Stop, pCaller);
		if(	((pTarget->~IsGunEmplacement() || pTarget->~IsMAVStation()) && !pTarget->~ActivateEntrance(pCaller, true)) || 
				((pTarget->~IsArtilleryBattery() || pTarget->~IsPatrolBoatMotor()) && FindObject2(Find_Action("Push"), Find_ActionTarget(pTarget))) ||
				(pTarget->~IsWeaponRack() && LocalN("pController", pTarget))
			)
		{
			Cancel();
			PlayerMessage(GetOwner(pCaller), "Target in use!", this);
			return true;
		}
		
		if(pTarget->~IsWeaponRack() && Hostile(GetOwner(), GetOwner(LocalN("heli", pTarget))))
		{
			RemoveTarget();
			PlayerMessage(GetOwner(pCaller), "No target!", this);
			return true;
		}
		
		if(pTarget->~IsPatrolBoatMotor())
  		pTarget->Grabbed(this, true);
  		
  	if(pTarget->~IsWeaponRack())
  		pTarget->SetGunner(pCaller);

  	fControlling = true;
  	Sound("Bsss.wav", 1, 0, 0, GetOwner(pCaller));
  	ObjectSetAction(pCaller, "Remote");
  	SetPlrView(GetOwner(pClonk), ViewTarget());
  }
  else
		PlayerMessage(GetOwner(pCaller), "No target!", this);
		
  return true;
}

func ControlThrow(object pCaller)
{
	if(pTarget && fControlling)
	{
		SetPlrView(GetOwner(pClonk), ViewTarget());
		
		if(pTarget->~IsWeaponRack() && !LocalN("heli", pTarget)->GetPilot())
		{
			LocalN("heli", pTarget)->RefuseLaunch(pCaller);
			return true;
		}
		
		pTarget->~ControlThrow(pCaller);
		return true;
	}

	var pGun = FindObject2(Find_AtPoint(), Find_Or(
																									Find_Func("IsGunEmplacement"),
																									Find_Func("IsMAVStation"),
																									Find_Func("IsArtilleryBattery"),
																									Find_Func("IsConsole"),
																									Find_Func("IsPatrolBoatMotor"),
																									Find_Func("IsWeaponRack"),
																									Find_Func("IsLiftPlate")
																								)
	);
						
	if(pGun && pGun->~IsWeaponRack() && Hostile(GetOwner(), GetOwner(LocalN("heli", pGun))))
		return true;

	if(pGun)
	{
		if(pTarget)
			LocalN("pRemoteControl", pTarget) = 0;
		tempView = 0;

		var pPrevRemote = LocalN("pRemoteControl", pGun);
		if(pPrevRemote)
			pPrevRemote->RemoveTarget();
			
		LocalN("pRemoteControl", pGun) = this;
		PlayerMessage(GetOwner(pCaller), Format("%s hacked!", GetName(pGun)), this);
		pTarget = pGun;
	}
	return true;
}

func Cancel()
{
	fControlling = false;
	if(pTarget && pTarget->~IsWeaponRack() && LocalN("pController", pTarget) == pClonk)
	{
		pTarget->SetGunner(this);
	}
	
	if(pClonk)
		SetPlrView(GetOwner(pClonk), pClonk);

	if(GetAction(pClonk) == "Remote")
		ObjectSetAction(pClonk, "Walk");
}

func RemoveTarget()
{
	Cancel();
	if(pTarget)
		LocalN("pRemoteControl", pTarget) = 0;
	pTarget = 0;
	if(Contained()->Contents() == this)
		PlayerMessage(GetOwner(), "Target lost!", this);
}

func ViewTarget()
{
	if(pTarget)
	{	
		var pMAV;
		if(pTarget->~IsMAVStation() && (pMAV = LocalN("pMAV", pTarget)) && !pMAV->IsDestroyed())
			return pMAV;
		
		if(tempView)
			return tempView;
		
		return pTarget;
	}
}


func Check()
{
	if(!fControlling || !pTarget)
		return;

	if(GetAction(pClonk) != "Remote" || (pTarget->~IsWeaponRack() && Hostile(GetOwner(), GetOwner(LocalN("heli", pTarget)))))
	{
		Cancel();
		return;
	}

	SetPlrView(GetOwner(), ViewTarget());
}

func RelayControl(object pCaller, string szControl)
{

	if(!fControlling)
		return;
		
	if(pTarget)
	{
		if(!pTarget->~IsGunEmplacement())
			tempView = 0;

		SetPlrView(GetOwner(pClonk), ViewTarget());
		if((pTarget->~IsGunEmplacement() || pTarget->~IsArtilleryBattery() || pTarget->~IsPatrolBoatMotor() || pTarget->~IsLiftPlate()) && WildcardMatch(szControl, "*Double"))
			return true;

		if(pTarget->~IsConsole())
			if(szControl == "ControlDigSingle")
			{
				var target = LocalN("target", pTarget);
				SetPlrView(GetOwner(pCaller),target);
				tempView = target;

  			//Menü erstellen
  			CreateMenu(GetID(target), pCaller, target, 0, Format("Control: %s", GetName(target)), 0, 1);
  			for(var i = 1, desc ; desc = target->~ConsoleControl(i, pCaller, this) ; i++)
  			{
    			AddMenuItem(desc, Format("ConsoleControlled(%d, %d, %d)", i, ObjectNumber(pCaller), ObjectNumber(this)), GetID(), pCaller, 0, 0, "Control");
    			Sound("Acknowledge.ogg", 0, pCaller, 100, GetOwner(pCaller)+1);
  			}
				szControl = "ControlDig";
			}
			else
				return true;
				
		if(pTarget->~IsLiftPlate() && szControl == "ControlDown")
			szControl = "ControlDownSingle";
		
		pTarget->~eval(Format("%s(Object(%d))", szControl, ObjectNumber(pCaller)));
		return true;
	}
}

func ControlUpdate(object pCaller, int comdir, bool dig, bool throw) 
{
	if(fControlling)
	{
		if(WildcardMatch(GetAction(pClonk), "Walk*"))
			ObjectSetAction(pClonk, "Remote");
		else if(GetAction(pClonk) != "Remote")
			return Cancel();
		pTarget->~ControlUpdate(pCaller, comdir, dig, throw);
		//1. (niedrigstes) Bit: Links 2. Bit: Rechts 3. Bit: Hoch 4. Bit: Runter
		
		var newMovement = 0;
		if(comdir == COMD_Left || comdir == COMD_UpLeft|| comdir == COMD_DownLeft)
			newMovement += 1;
		if(comdir == COMD_Right || comdir == COMD_UpRight|| comdir == COMD_DownRight)
			newMovement +=2;
		if(comdir == COMD_Up || comdir == COMD_UpLeft || comdir == COMD_UpRight)
			newMovement +=4;
		if(comdir == COMD_Down || comdir == COMD_DownLeft || comdir == COMD_DownRight)
			newMovement +=8;

		if(lastMovement == newMovement)
			return lastMovement = newMovement;
		
		if(lastMovement & 1 && !(newMovement & 1))
			pTarget->~ControlLeftReleased(pCaller);
			
		if(lastMovement & 2 && !(newMovement & 2))
			pTarget->~ControlRightReleased(pCaller);
			
		if(lastMovement & 4 && !(newMovement & 4))
			pTarget->~ControlUpReleased(pCaller);
			
		if(lastMovement & 8 && !(newMovement & 8))
			pTarget->~ControlDownReleased(pCaller);
			
		lastMovement = newMovement;
	}
}

func ControlLeft(object pCaller)
{
	return RelayControl(pCaller, "ControlLeft");
}

func ControlLeftDouble(object pCaller)
{

	return RelayControl(pCaller, "ControlLeftDouble");
}

func ControlLeftReleased(object pCaller)
{
	return RelayControl(pCaller, "ControlLeftReleased");
}

func ControlRight(object pCaller)
{
	//lastMovement = "Right";
	return RelayControl(pCaller, "ControlRight");
}

func ControlRightDouble(object pCaller)
{
	//lastMovement = "Right";
	return RelayControl(pCaller, "ControlRightDouble");
}

func ControlRightReleased(object pCaller)
{
	return RelayControl(pCaller, "ControlRightReleased");
}

func ControlUp(object pCaller)
{
	//lastMovement = "Up";
	return RelayControl(pCaller, "ControlUp");
}

func ControlUpDouble(object pCaller)
{
	//lastMovement = "Up";
	return RelayControl(pCaller, "ControlUpDouble");
}

func ControlUpReleased(object pCaller)
{
	return RelayControl(pCaller, "ControlUpReleased");
}

func ControlDown(object pCaller)
{
	//lastMovement = "Down";
	return RelayControl(pCaller, "ControlDown");
}

func ControlDownDouble(object pCaller)
{
	//lastMovement = "Down";
	return RelayControl(pCaller, "ControlDownDouble");
}

func ControlDownReleased(object pCaller)
{
	return RelayControl(pCaller, "ControlDownReleased");
}

func ControlDig(object pCaller)
{
	return RelayControl(pCaller, "ControlDigSingle");
}

func ControlDigDouble(object pCaller)
{
	return Activate(pCaller);
}

func Entrance(object pContainer)
{
	lastMovement = 0;
	pClonk = pContainer;
	SetOwner(GetOwner(pClonk));
}

func Departure()
{
	pClonk = 0;
	Cancel();
}

protected func Destruction()
{
	Cancel();
	if(pTarget)
		LocalN("pRemoteControl", pTarget) = 0;
}

func Selection()
{
	if(pTarget)
		PlayerMessage(GetOwner(), GetName(pTarget), this);
}
