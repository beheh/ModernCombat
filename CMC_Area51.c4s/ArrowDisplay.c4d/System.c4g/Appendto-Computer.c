/*-- Appendto-Computer --*/

#strict 2

#appendto COMP

public func IsMarkerTarget(object pBy) { return FindObject(GDAR); }
public func TVAR_MaxMarkerDistance() { return 0; }  //Maximale Entfernung, damit ein Marker erscheint (0 = unendlich)
public func TVAR_MaxMarkerCount() { return 0; }  //Maximale Anzahl an Marker (0 = unendlich)

public func TVAR_MarkerColor(object pBy) { return RGB(0, 0, 255); }
public func TVAR_MarkerIcon(object pBy) { return 0; }

public func TVAR_MarkerVisibility(object pBy)
{
	return [TVAR_VIS_All];
}
