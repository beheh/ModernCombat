/*-- Seilhalterung --*/

#strict 2


/* Initalisierung */

func Initialize()
{
}

public func RopeAttachmentDestroyed(object pRopeAttachment)
{
	//Hier kann das Teil zum Wrack werden.
	//Aus Demonstrationszwecken....:
	SetGraphics(0, 0, BHWK);
}

public func SetGraphic(int iGraphic)
{
	//Hier wird die Grafik gesetzt. Blub.
}
