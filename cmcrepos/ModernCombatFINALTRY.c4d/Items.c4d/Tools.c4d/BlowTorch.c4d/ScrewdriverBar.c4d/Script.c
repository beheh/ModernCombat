/*-- Reparaturbalken --*/

#strict 2

local obj;
local iBarCount;
local fActive;

local iBarType;

// Standard-Balkentypen:
static const BAR_Repairbar = 1;
static const BAR_Energybar = 2;
static const BAR_Ammobar = 3;

static const BAR_IconLayer = 2;
static const BAR_RowLayer = 1;

public func GetBarType()
{
	return iBarType;
}
public func IsBar()
{
	return true;
}
public func BarActive()
{
	return fActive;
}
public func RejectEntrance()
{
	return true;
}
public func HasBarType(int barType) { return (iBarType == barType); }

global func GetBarCount(object target, int iOwner, object exclude)
{	
	if(!target) { target = this; }
	
	if(!exclude)
		return GetLength(FindObjects(Find_Func("IsBar"), Find_Func("BarActive"), Find_ActionTarget(target), Find_Owner(iOwner)));
	else
		return GetLength(FindObjects(Find_Func("IsBar"), Find_Func("BarActive"), Find_ActionTarget(target), Find_Owner(iOwner), Find_Exclude(exclude)));
}


/* Initialisierung */

protected func Initialize()
{
	fActive = false;
	SetVisibility(VIS_None);

	SetGraphics("Row", this, GetID(), BAR_RowLayer, 1);
}

/* Einstellung */

public func Set(object target, int color, int iType, bool fIcon, string szIcon, id idSrcDef, int iXAdjust, int iYAdjust)
{
	SetVisibility(VIS_None);

	obj = target;
	iBarCount = GetBarCount(obj, GetOwner());
	iBarType = iType;

	SetVertex(0, 0, GetVertex(0, 0, target));

	//Balken setzen
	SetVertex(0, 1, GetVertex(0, 1, target) - GetObjHeight(target) / 2 - (iBarCount * 10));

	//Und festsetzen
	SetAction("Attach", target);

	//Färben
	SetClrModulation(color, this, BAR_RowLayer);

	// Icon zur Identifizierung?
	if(fIcon)
	{
		if(!iXAdjust)
			iXAdjust = -30000; // Adjust Werte an Grafiken der Maße 25x25px angepasst, bei anderen Maßen werden evtl. andere Werte benötigt.
		
		SetGraphics(szIcon, this, idSrcDef, BAR_IconLayer, 1);
		SetObjDrawTransform(1000, 0, iXAdjust, 0, 1000, iYAdjust, this, BAR_IconLayer);
	}
	
	fActive = false;
	
	return true;
}

public func SetBarCount(int iCount)
{
	iBarCount = iCount;
}

local tPercent; 

public func Update(int percent, bool fDeactivate)
{
	//Anpassen (falls andere Anzeigen vorhanden)
	SetVertex(0, 1, GetVertex(0, 1, obj) - GetObjHeight(obj) / 2 - (GetBarCount(obj, GetOwner(), this) * 10));

	if(fDeactivate && fActive)
	{
		SetVisibility(VIS_None);
		fActive = false;
		return true;
	}
	else if(!fActive && !fDeactivate)
	{
		fActive = true;
		SetVisibility(VIS_Owner);
	}
	
	//Prozentbalken anpassen
	if(percent != tPercent)
	{
		SetObjDrawTransform(10 * percent, 0, -160 * (100 - percent), 0, 1000, 0, 0, BAR_RowLayer);
		tPercent = percent;
	}
	
	return true;
}

public func AttachTargetLost()
{
	return RemoveObject();
}
