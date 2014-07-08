#strict 2

#appendto 1HUD

global func CompassForbidden(int iPlr) { return GetPlrExtraData(iPlr, "CMC_ForbidCompass"); }

protected func Initialize()
{ 
  if(!CMPS_fNoCompass && !CompassForbidden(GetOwner()))
  	CreateObject(CMPS, 0, 0, GetOwner());
	
	return _inherited();
}

public func CreateCompass() { return CreateObject(CMPS, 0, 0, GetOwner()); }	
