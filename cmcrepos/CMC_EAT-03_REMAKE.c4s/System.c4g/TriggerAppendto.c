/*-- Triggerappendto --*/

#strict 2

#appendto *

// Diese Datei lässt es zu, dass alle Objekte die Trigger unterstützen.

public func FindTriggerType(int iTriggerType) 
{ 
	var i, effect;
	while(effect = GetEffect("CMCBindTrigger", this, i))
	{
		if(iTriggerType == EffectVar(0, this, effect))
			return effect;
	
		++i;
	}
}

public func CallTrigger(int iType, par1, par2, par3, par4, par5, par6, par7)
{
	var effect;
	if(!(effect = FindTriggerType(iType)))
		return false; 
		
	EffectCall(this, effect, "CallFunction", par1, par2, par3, par4, par5, par6, par7);
	
	return true;
}

public func Collection2(object pObj)
{
	if(FindTriggerType(TRIGGER_Collection))
		CallTrigger(TRIGGER_Collection, pObj);
	
	return _inherited(pObj, ...);
}

public func Entrance(object pContainer)
{
	if(FindTriggerType(TRIGGER_Entrance))
		CallTrigger(TRIGGER_Entrance, pContainer);
	
	return _inherited(pContainer, ...);
}

public func Destruction()
{
	if(FindTriggerType(TRIGGER_Removed))
		CallTrigger(TRIGGER_Removed);
		
	return _inherited(...);
}

public func Departure(object pContainer)
{
	if(FindTriggerType(TRIGGER_Departure))
		CallTrigger(TRIGGER_Departure, pContainer);
		
	return _inherited(pContainer, ...);
}

public func ControlLeft(object pCaller)
{
	if(FindTriggerType(TRIGGER_Control))
		CallTrigger(TRIGGER_Control, pCaller, "Left");
		
	return _inherited(pCaller, ...);
}

public func ControlRight(object pCaller)
{
	if(FindTriggerType(TRIGGER_Control))
		CallTrigger(TRIGGER_Control, pCaller, "Right");
		
	return _inherited(pCaller, ...);
}

public func ControlStop(object pCaller)
{
	if(FindTriggerType(TRIGGER_Control))
		CallTrigger(TRIGGER_Control, pCaller, "Stop");
		
	return _inherited(pCaller, ...);
}

public func ControlThrow(object pCaller)
{
	if(FindTriggerType(TRIGGER_Control))
		CallTrigger(TRIGGER_Control, pCaller, "Throw");
		
	return _inherited(pCaller, ...);
}

public func ControlDig(object pCaller)
{
	if(FindTriggerType(TRIGGER_Control))
		CallTrigger(TRIGGER_Control, pCaller, "Dig");
		
	return _inherited(pCaller, ...);
}

public func ControlUp(object pCaller)
{
	if(FindTriggerType(TRIGGER_Control))
		CallTrigger(TRIGGER_Control, pCaller, "Up");
		
	return _inherited(pCaller, ...);
}

public func ContainedLeft(object pCaller)
{
	if(FindTriggerType(TRIGGER_Control))
		CallTrigger(TRIGGER_Control, pCaller, "CLeft");
		
	return _inherited(pCaller, ...);
}

public func ContainedRight(object pCaller)
{
	if(FindTriggerType(TRIGGER_Control))
		CallTrigger(TRIGGER_Control, pCaller, "CRight");
		
	return _inherited(pCaller, ...);
}

public func ContainedStop(object pCaller)
{
	if(FindTriggerType(TRIGGER_Control))
		CallTrigger(TRIGGER_Control, pCaller, "CStop");
		
	return _inherited(pCaller, ...);
}

public func ContainedThrow(object pCaller)
{
	if(FindTriggerType(TRIGGER_Control))
		CallTrigger(TRIGGER_Control, pCaller, "CThrow");
		
	return _inherited(pCaller, ...);
}

public func ContainedDig(object pCaller)
{
	if(FindTriggerType(TRIGGER_Control))
		CallTrigger(TRIGGER_Control, pCaller, "CDig");
		
	return _inherited(pCaller, ...);
}

public func ContainedUp(object pCaller)
{
	if(FindTriggerType(TRIGGER_Control))
		CallTrigger(TRIGGER_Control, pCaller, "CUp");
		
	return _inherited(pCaller, ...);
}

public func OnHit(int iChange, int iType, object pFrom)
{
	if(FindTriggerType(TRIGGER_Damage))
		CallTrigger(iChange, iType, pFrom);
	
	return _inherited(iChange, iType, pFrom, ...);
}

public func Activate(object pByObj)
{
	if(FindTriggerType(TRIGGER_Activate))
		CallTrigger(pByObj);
	
	return _inherited(pByObj, ...);
}

public func Grabbed(object pByObj, bool fGrab)
{
	if(FindTriggerType(TRIGGER_Grabbed))
		CallTrigger(TRIGGER_Grabbed, pByObj, fGrab);
	
	return _inherited(pByObj, fGrab, ...);
}

public func Death()
{
	if(FindTriggerType(TRIGGER_Death))
		CallTrigger(TRIGGER_Death);
	
	return _inherited(...);
}
