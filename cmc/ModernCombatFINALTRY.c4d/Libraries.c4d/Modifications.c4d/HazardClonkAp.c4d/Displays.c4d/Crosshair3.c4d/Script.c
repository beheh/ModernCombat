/*-- Artilleriefadenkreuz --*/

#strict 2


/* Initalisierung */

local fActive, pArtillery, pClonk;

public func Construction(object pByObj)
{
  //Host-Artillerie übernehmen
  pArtillery = pByObj;

  return true;
}

public func Set(object pObj)
{
  //Besitzer-Clonk übernehmen
  pClonk = pObj;

  //Färbung
  var rgb;
  if(pArtillery->~GetCooldown() > 0)
    rgb = RGB(255,0,0);
  else
    rgb = RGB(0,255,0);
  SetClrModulation(rgb);

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

  //Färbung
  var rgb;
  if(pArtillery->~GetCooldown() > 0)
    rgb = RGB(255,0,0);
  else
    rgb = RGB(0,255,0);
  SetClrModulation(rgb);

  //Drehen
  SetR(GetR()+7);

  return true;
}