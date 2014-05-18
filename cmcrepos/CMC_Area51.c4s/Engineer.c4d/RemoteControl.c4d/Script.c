/*-- Fernsteuerung --*/

#strict 2

local pTarget;
local fControlling;
local pClonk;
local throwActive;
local lastMovement; //1. (niedrigstes) Bit: Links 2. Bit: Rechts 3. Bit: Hoch 4. Bit: Runter

public func IsDrawable()	{return true;}	//Wird sichtbar getragen
public func HandX()		{return 4000;}
public func HandY()		{return 10;}
public func HandSize()		{return 1000;}

func Activate(object pCaller) {
  
  if(fControlling)
  {
  	Cancel();
  	return true;
  }
  
  if(pTarget)
  {
  	SetComDir(COMD_Stop, pCaller);
		Sound("Bsss.wav", 1, 0, 0, GetOwner(pCaller));
		fControlling = true;
		SetPlrView(GetOwner(pClonk), ViewTarget());
		ObjectSetAction(pCaller, "Heal");
		if(!pTarget->~ActivateEntrance(pCaller, true))
			Cancel();
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
		pTarget->~ControlThrow(pCaller);
		return true;
	}
	
	var pGun = FindObject2(Find_AtPoint(), Find_Or(Find_Func("IsGunEmplacement"), Find_Func("IsMAVStation")));

	if(pTarget && pGun)
	{
		var pPrevRemote = LocalN("pRemoteControl", pTarget);
		if(pPrevRemote)
		{
			pPrevRemote->Cancel();
			LocalN("pRemoteControl", pTarget) = 0;
			LocalN("pTarget", pPrevRemote) = 0;
		}
	}
	if(pGun)
	{
		pTarget = pGun;
		LocalN("pRemoteControl", pTarget) = this;
	}
	return true;
}

func Cancel()
{
	fControlling = false;
	if(pClonk)
		SetPlrView(GetOwner(pClonk), pClonk);
	if(GetAction(pClonk) == "Heal")
		ObjectSetAction(pClonk, "Walk");
	
}

func ViewTarget()
{
	if(pTarget)
	{
		var pMAV;
		if(pTarget->~IsMAVStation() && (pMAV = LocalN("pMAV", pTarget)) && !pMAV->IsDestroyed())
			return pMAV;
		else
			return pTarget;
	}
}

func Check()
{
	if(!fControlling)
		return;

	if(WildcardMatch(GetAction(pClonk), "Walk*"))
		return ObjectSetAction(pClonk, "Heal");
	else if(GetAction(pClonk) != "Heal")
		return Cancel();
}

func RelayControl(object pCaller, string szControl)
{
	if(!fControlling)
		return;
		
	if(pTarget)
	{
		SetPlrView(GetOwner(pClonk), ViewTarget());
		if(pTarget->~IsGunEmplacement() && WildcardMatch(szControl, "*Double"))
			return true;
		pTarget->~eval(Format("%s(%d)", szControl, pCaller));
		return true;
	}
}

func ControlUpdate(object pCaller, int comdir, bool dig, bool throw) 
{
	if(fControlling)
	{
		if(WildcardMatch(GetAction(pClonk), "Walk*"))
			ObjectSetAction(pClonk, "Heal");
		else if(GetAction(pClonk) != "Heal")
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
