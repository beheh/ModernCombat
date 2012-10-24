#strict 2

#appendto SHRK

public func Initialize()
{
	if(!Random(36))
		SetGraphics(0, this, SKSN);
	
	return inherited();
}
