#strict
#appendto HZCK

/* HEV-Append */

public func HasHEV()
{
  return(this()->~HasGear(0,HEVS));
}

protected func ControlJetpack(string strControl)
{
	var hev = HasHEV();
	if(!hev) return _inherited(strControl);
	
	var ret = false;
	
	// right double
	if(strControl == "ControlRightDouble")
  {
		ret = hev->ControlRightDouble(this());
	}
	
	// left double
	if(strControl == "ControlLeftDouble")
  {
		ret = hev->ControlLeftDouble(this());
	}
	
	return ret;
}

/* Overloads */

public func ReadyToFire()
{
	if(GetAction() == "JetpackFlight") return true;
 
	return _inherited();
}

public func AimOverride()
{
	if(GetAction() == "JetpackFlight") return true;
	
	return _inherited();
}