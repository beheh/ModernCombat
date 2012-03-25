/*--- Zeichen ---*/

#strict 2


protected func Initialize()
{
  SetVisibility(VIS_Owner);
  return Set();
}

public func Set(int iChar, int dwClr)
{
  if(!iChar)
    iChar = 48;
  if(!dwClr)
    dwClr = RGB(255, 255, 255);

  SetAction(Format("%d",iChar));

  SetClrModulation(dwClr);

  return true;
}
