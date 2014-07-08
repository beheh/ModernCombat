/*-- Neues Script --*/

#strict 2

#appendto HZCK

public func HitExclude(object pByObj)
{
	//Sprengfallen werden trotzdem gezündet
	if(GetID(pByObj) == BBTP)
		return;

	return GetEffect("Spawnprotection", this);
}
