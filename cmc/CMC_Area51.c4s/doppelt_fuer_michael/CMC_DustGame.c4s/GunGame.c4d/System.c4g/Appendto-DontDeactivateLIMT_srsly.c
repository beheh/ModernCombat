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
  
  //Hat Cooldown
  if(GetEffect("Cooldown", this))
  	return true;

  //Sprengfalle erstellen und legen
  SetOwner(GetOwner(Contained()));
  var trap = CreateContents(BBTP, this, 1);
  trap->ControlThrow(this);

  //Plazierung erfolgreich?
  if(LocalN("bActive", trap)) {
    //In GunGame ggf. nicht abziehen
    if(!GetEffect("IntKeepObject", this))
    	DoPackPoints(-1);
    //Dafuer gibts 'n Cooldown
    else
    	AddEffect("Cooldown", this, 1, 20);
  }
  else
    RemoveObject(trap);

  //Verschwinden sobald aufgebraucht
  if(!GetPackPoints())
    RemoveObject(this);

  return true;
}

public func UpdateHUD(object pHUD)
{
	if(GetEffect("IntKeepObject", this))
		pHUD->~Ammo(-1, -1, GetName(), true, 0xFFFFFF);
	else
  	_inherited(pHUD, ...);

	//Das hier sollte lieber gleich in PACK rein
  if(GetEffect("Cooldown", this))
  	pHUD->~Recharge(GetEffect("Cooldown", this, 0, 6), GetEffect("Cooldown", this, 0, 3)-1);
}
