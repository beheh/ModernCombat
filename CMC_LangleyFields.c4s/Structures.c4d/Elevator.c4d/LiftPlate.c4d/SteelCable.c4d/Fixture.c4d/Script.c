/*-- Befestigung --*/

#strict 2

public func Set(object pTarget, int iXOff, int iYOff)
{
	SetAction("Fixture", pTarget);
	SetVertex(0, 0, -iXOff);
	SetVertex(0, 1, -iYOff);
	
	return this;
}
