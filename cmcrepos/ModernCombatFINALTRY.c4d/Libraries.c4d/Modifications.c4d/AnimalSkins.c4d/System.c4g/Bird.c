#strict 2

#appendto BIRD

public func Initialize()
{
	if(!Random(20))
		SetGraphics(0, this, BDSN);
	
	return inherited();
}
