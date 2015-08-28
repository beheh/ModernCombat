/*--- Zeichen ---*/

#strict 2


/* Initialisierung */

protected func Initialize()
{
  return Set();
}

/* Konfiguration */

public func Set(int iChar, int dwClr)
{
  //Bei keiner Zeichenangabe: Null
  if(!iChar)
    iChar = 48;
  //Bei keiner Farbangabe: Weiﬂ
  if(!dwClr)
    dwClr = RGB(255, 255, 255);

  //Konfiguration setzen
  SetVisibility(VIS_Owner);
  SetAction(Format("%d",iChar));
  SetClrModulation(dwClr);

  return true;
}