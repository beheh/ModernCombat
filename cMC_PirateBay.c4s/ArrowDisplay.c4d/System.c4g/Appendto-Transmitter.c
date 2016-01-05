/*-- Appendto-Transmitter  --*/

#strict 2

#appendto TRMR

public func IsMarkerTarget(object pBy)
{
	if(!FindObject(GDAR))
		return;
	
	return GetOCF(FindObject(GDAR)->GetComputer2()) & OCF_CrewMember;
}


public func TVAR_MaxMarkerDistance() { return 0; }  //Maximale Entfernung, damit ein Marker erscheint (0 = unendlich)
public func TVAR_MaxMarkerCount() { return 0; }  //Maximale Anzahl an Marker (0 = unendlich)

public func TVAR_MarkerColor(object pBy) { return RGB(255, 255, 255); } //Farbe des Pfeils
public func TVAR_MarkerIcon(object pBy) { return "Transmitter"; }	//Angezeigtes Icon (String)
public func TVAR_MarkerTarget(object pBy) { return 0; } //Verweis auf anderes Objekt zum zeigen (0 = this)

public func TVAR_MarkerVisibility(object pBy)
{
	return [TVAR_VIS_Objects, FindObject(GDAR)->GetComputer2()];
}
