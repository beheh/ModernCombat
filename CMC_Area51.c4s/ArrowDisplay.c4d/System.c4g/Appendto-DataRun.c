/*-- Neues Script --*/

#strict 2

#appendto GDAR

public func IsMarkerTarget(object pBy)
{
	return GetOCF(GetComputer2()) & OCF_CrewMember;
}

public func TVAR_MarkerColor(object pBy) { return GetColorDw(GetComputer2()); } //Farbe des Pfeils
public func TVAR_MarkerIcon(object pBy) { return 0; }	//Angezeigtes Icon (String)
public func TVAR_MarkerTarget(object pBy) { return GetComputer2(); } //Verweis auf anderes Objekt zum zeigen (0 = this)

public func TVAR_MarkerVisibility(object pBy)
{
	return TVAR_Not([TVAR_VIS_Objects, GetComputer2()]);
}

public func GetComputer2()
{
	if(GetComputer())
		return GetComputer();

  for(var obj in FindObjects(Find_OCF(OCF_CrewMember)))
    if(GetEffect("DataRunComputer", obj))
    	return obj;
   
  return false;
}
