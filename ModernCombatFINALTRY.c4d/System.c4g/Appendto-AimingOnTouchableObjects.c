/*-- Zielen vor anfassbaren Objekten --*/

//Erleichtert das Zielen per [Dreifachstopp] vor anfassbaren Objekten.

#strict 2

#appendto *


/* Steuerung */

public func ControlDown(object pObj)
{
  if(!(GetOCF() & OCF_Grab))
    return(_inherited(...));

  //bei Dreifachstop loslassen und Zielen
  if(pObj && GetEffect("IntWasGrabbed") && Contents(0, pObj))
  {
	pObj->SetAction("Walk");
    if(pObj->~ReadyToSquatAim())
      pObj->~StartSquatAiming();
    return(1);
  }

  return(_inherited(...));
}

public func ControlUpdate(object pObj, int comdir, bool dig, bool throw)
{
  if((comdir != 5) || !(GetOCF() & OCF_Grab))
    return(_inherited(pObj, comdir, dig, throw));

  //bei Dreifachstop loslassen und Zielen
  if(pObj && GetEffect("IntWasGrabbed") && Contents(0, pObj))
  {
	pObj->SetAction("Walk");
    if(pObj->~ReadyToSquatAim())
      pObj->~StartSquatAiming();
    return;
  }

  return(_inherited(pObj, comdir, dig, throw));
}

public func Grabbed(object pByObject, bool fGrab)
{
  if(fGrab)
    AddEffect("IntWasGrabbed", 0, 100, 10);

  return _inherited(...);
}

public func IntWasGrabbedStart()	{return 1;}