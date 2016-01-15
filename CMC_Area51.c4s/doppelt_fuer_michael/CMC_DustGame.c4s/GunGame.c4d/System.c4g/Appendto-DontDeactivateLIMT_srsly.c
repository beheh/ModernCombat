/*-- Neues Script --*/

#strict 2

#appendto BTBG

/* Steuerung */

public func ControlThrow(object caller)
{
  if(GetPlrDownDouble(GetOwner(caller)))
    return _inherited(...);

  //Wird nicht getragen: Werfen gesperrt
  if(!Contained() || !GetPackPoints()) return true;

  //Sprengfalle erstellen und legen
  SetOwner(GetOwner(Contained()));
  var trap = CreateContents(BBTP, this, 1);
  trap->ControlThrow(this);

  //Plazierung erfolgreich?
  if(LocalN("bActive", trap)) {
    //In GunGame ggf. nicht abziehen
    if(!GetEffect("IntKeepObject", this))
    	DoPackPoints(-1);
  }
  else
    RemoveObject(trap);

  //Verschwinden sobald aufgebraucht
  if(!GetPackPoints())
    RemoveObject(this);

  return true;
}
