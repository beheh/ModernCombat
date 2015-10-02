/*-- Zielen vor anfassbaren Objekten --*/

//Erleichtert das Zielen per [Dreifachstopp] vor anfassbaren Objekten, sofern der Spieler es für sich in den Einstellungen aktiviert hat.

#strict 2
#appendto *


/* Steuerung */

public func ControlDown(object pObj)
{
  if(!(GetOCF() & OCF_Grab) || !pObj)
    return(_inherited(pObj, ...));

  //Bei Dreifachstop loslassen und Zielen
  if(pObj && pObj->~BetterAiming() && GetEffect("IntWasGrabbed", pObj) && Contents(0, pObj))
  {
    pObj->SetAction("Walk");
    if(pObj->~ReadyToSquatAim())
      pObj->~StartSquatAiming();

    this->~Grabbed(pObj, false);
    return;
  }

  return(_inherited(pObj, ...));
}

public func ControlUpdate(object pObj, int comdir, bool dig, bool throw)
{
  if(!pObj || (comdir != 5) || !(GetOCF() & OCF_Grab))
    return(_inherited(pObj, comdir, dig, throw, ...));

  //Bei Dreifachstop loslassen und Zielen
  if(pObj && pObj->~BetterAiming() && GetEffect("IntWasGrabbed", pObj) && Contents(0, pObj))
  {
    pObj->SetAction("Walk");
    if(pObj->~ReadyToSquatAim())
      pObj->~StartSquatAiming();

    this->~Grabbed(pObj, false);
    return;
  }

  return(_inherited(pObj, comdir, dig, throw, ...));
}

public func Grabbed(object pByObject, bool fGrab)
{
  if(pByObject && fGrab && !this()->~NoFastAiming())
    AddEffect("IntWasGrabbed", pByObject, 100, 10);

  return _inherited(pByObject, fGrab, ...);
}