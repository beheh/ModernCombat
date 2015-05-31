/*-- Zielen vor anfassbaren Objekten erleichtern --*/

#strict 2

#appendto *

public func ControlDown(object pObj)
{
  if(!(GetOCF() & OCF_Grab))
    return(_inherited(...));

  //bei Dreifachstop loslassen und Zielen
  if(pObj && GetPlrDownDouble(GetOwner(pObj)) && Contents(0, pObj))
  {
	pObj->SetAction("Walk");
    if(pObj->~ReadyToSquatAim())
      pObj->~StartSquatAiming(); 
    return(1);
  }

  return(_inherited(...));	
}