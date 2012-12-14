/*-- Ziel --*/

#strict 2


/* Initalisierung */

local fActive, pArtillery, pClonk;

public func Construction(object pByObj)
{
  pArtillery = pByObj;
  return true;
}

public func Set(object pObj)
{
  pClonk = pObj;
  return true;
}

public func Check()
{
  if(GetProcedure(pClonk) != "PUSH" || GetActionTarget(0, pClonk) != pArtillery)
    return RemoveObject();

  if(GetPlrView(GetOwner()) != this)
  {
    SetPlrViewRange(0);
    SetVisibility(VIS_None);
    fActive = false;
  }
  else if(!fActive)
  {
    SetPlrViewRange(200);
    SetVisibility(VIS_Owner);
    fActive = true;
  }

  return true;
}