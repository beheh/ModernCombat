/*-- Fernsteuerung --*/

#strict 2

local pTarget;
local fControlling;
local pClonk;

func Activate(object pCaller) {
  
  if(fControlling)
  {
  	fControlling = false;
  	return;
  }
  
  var pGun = FindObject2(Find_AtPoint(), Find_Or(Find_Func("IsGunEmplacement"), Find_Func("IsMAVStation")));

	if(pTarget)
	{
		var pPrevRemote = LocalN("pRemoteControl", pTarget);
		if(pPrevRemote)
		{
			pPrevRemote->Cancel();
			LocalN("pRemoteControl", pTarget) = 0;
			LocalN("pTarget", pPrevRemote) = 0;
		}
	}
	pTarget = pGun;
	if(pTarget)
		LocalN("pRemoteControl", pTarget) = this;
  return true;
}

func ControlThrow(object pCaller)
{
	if(pTarget)
	{
		if(fControlling)
		{
			SetPlrView(GetOwner(pClonk), ViewTarget());
			pTarget->~ControlThrow(pCaller);
			return true;
		}
	
		SetComDir(COMD_Stop, pCaller);
		Sound("Bsss.wav", 1, 0, 0, GetOwner(pCaller));
		fControlling = true;
		SetPlrView(GetOwner(pClonk), ViewTarget());
		if(!pTarget->~ActivateEntrance(pCaller, true))
			Cancel();
	}
	else
	{
		PlayerMessage(GetOwner(pCaller), "No target!", this);
	}
	
	return true;
}

func Cancel()
{
	fControlling = false;
	if(pClonk)
		SetPlrView(GetOwner(pClonk), pClonk);
	
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

func RelayControl(object pCaller, string szControl)
{
	if(!fControlling)
		return;
		
	if(pTarget)
	{
		SetPlrView(GetOwner(pClonk), ViewTarget());
		ObjectCall(pTarget, szControl);
		return true;
	}
}

func ControlUpdate(object pCaller, int comdir, bool dig, bool throw) 
{
	return true;
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
	return RelayControl(pCaller, "ControlRight");
}

func ControlRightDouble(object pCaller)
{
	return RelayControl(pCaller, "ControlRightDouble");
}

func ControlRightReleased(object pCaller)
{
	return RelayControl(pCaller, "ControlRightReleased");
}

func ControlUp(object pCaller)
{
	return RelayControl(pCaller, "ControlUp");
}

func ControlUpDouble(object pCaller)
{
	return RelayControl(pCaller, "ControlUpDouble");
}

func ControlUpReleased(object pCaller)
{
	return RelayControl(pCaller, "ControlUpReleased");
}

func ControlDown(object pCaller)
{
	return RelayControl(pCaller, "ControlDown");
}

func ControlDownDouble(object pCaller)
{
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

func Entrance(object pContainer)
{
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
