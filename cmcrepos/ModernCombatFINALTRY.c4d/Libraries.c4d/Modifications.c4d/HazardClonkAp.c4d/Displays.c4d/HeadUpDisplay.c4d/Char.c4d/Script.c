/*--- Zeichen ---*/

#strict 2


protected func Initialize()
{
  return Set();
}

public func Set(int iChar, int dwClr)
{
  if(!iChar)
    iChar = 48;
  if(!dwClr)
    dwClr = RGB(255, 255, 255);

  SetVisibility(VIS_Owner);
  SetAction(Format("%d",iChar));

  SetClrModulation(dwClr);

  return true;
}
