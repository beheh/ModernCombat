/*-- Neues Script --*/

#strict 2

#appendto HZCK

private func Ejection(object pObj) {
	//KeepObject-Handling fuer GunGame
	if(GetAlive(this) && GetEffect("IntKeepObject", pObj)) {
		if(pObj->~IsWeapon2() && pObj->GetFMData(FM_NoAmmoModify) && FindObject(GGNG))
			FindObject(GGNG)->~EquipWeapon(GetOwner(), this, true);
		//Haette auch was wenn man zum Gegner rennen muesste :D
		else if(!pObj->~IsGrenade())
			return Enter(this, pObj);
		else if(FindObject(GGNG))
			FindObject(GGNG)->~EquipWeapon(GetOwner(), this, true, true);
	}

	return _inherited(pObj, ...);
}
