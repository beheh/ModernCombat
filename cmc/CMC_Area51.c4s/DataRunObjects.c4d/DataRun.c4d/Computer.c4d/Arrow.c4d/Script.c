/*-- Info-Arrow --*/

#strict 2

public func MaxRange()	{return 1600;}
public func NoWarp()	{return true;}
public func IsHUD()	{return true;}

//Informationsgrafik und Pfeilgrafik müssen getrennt sein, ansonsten würde die Information mitdrehen (-> 180° Drehung)

/* Initialisierung */

func Initialize()
{
  SetVisibility(VIS_Owner);
  return true;
}

public func Set(string szAction, object pOwner)
{
	SetAction("Show", pOwner);
	//Zusatzobjekt erstellen welches sich an dieses Objekt attached (im vordefinierten Abstand)
	
	return true;
}

public func SetPos(int iAngle, int iRange)
{
	var xoff = -Sin(iAngle, iRange, 1000);
  var yoff = +Cos(iAngle, iRange, 1000);
  SetVertex(0, 0, xoff);
  SetVertex(0, 1, yoff);
  
  SetR(iAngle/1000);

  return true;
}
