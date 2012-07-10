/*-- Seilhalterung --*/

#strict 2


/* Initalisierung */

func Initialize()
{
}

public func RopeHolderDestroyed(object pRopeHolder)
{
	//Hier kann das Teil zum Wrack werden.
	//Aus Demonstrationszwecken....:
	SetGraphics(0, 0, BHWK);
}
