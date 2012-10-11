#strict 2

#appendto BIRD

public func Initialize()
{
	if(!Random(25))
		SetGraphics("Barett", this, SKN2);
	
	return inherited();
}
