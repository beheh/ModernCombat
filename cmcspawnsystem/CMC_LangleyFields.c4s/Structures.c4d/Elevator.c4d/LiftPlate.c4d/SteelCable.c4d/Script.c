/*-- Stahlseil --*/

#strict 2

public func Set(object pTarget, bool fBack, int iColor)
{
  SetAction("Rope", pTarget);
  if(!iColor)
    iColor = RGB(75, 75, 75);

  SetClrModulation(iColor, this);

  if(!fBack)
    SetCategory(GetCategory()|C4D_Foreground);
  return true;
}