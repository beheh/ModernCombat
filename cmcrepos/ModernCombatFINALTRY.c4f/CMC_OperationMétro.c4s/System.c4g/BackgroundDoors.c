/*-- Hintergrundtürsteuerung --*/

//Hintergrundtüren nicht sofort aufeinanderfolgend betretbar.

#strict 2
#appendto BDOR


protected func ActivateEntrance(pObj)
{
  if(GetEffect("NoDoorEntrance", pObj) && !Contained(pObj))
    return false;
  else
    AddEffect("NoDoorEntrance", pObj, 1, 30, pObj);

  if(lock) return false;
  return inherited(pObj);
}