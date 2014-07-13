/*-- Neues Script --*/

#strict 2

#appendto LORY

protected func RejectCollect(id idObj,object pObj)
{
  if(ContentsCount() < MaxContents())
  {
    if(idObj != RSCE)
      Sound("Clonk"); 

    return false;
  }
  if(Contained(pObj)) return Message("$TxtLorryisfull$", this);
  if(Abs(GetXDir(pObj))>6) SetYDir(-5,pObj);
  
  if(idObj != RSCE)
    Sound("WoodHit*");

  return true;
}

