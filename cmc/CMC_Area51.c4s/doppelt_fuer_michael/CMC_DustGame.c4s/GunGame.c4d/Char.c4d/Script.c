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
  
  var action = Format("%d", iChar);
  //Bei -1 Unendlichkeitszeichen anzeigen
  if(iChar == -1)
  	action = "Infinite";
  
  //Bei keiner Farbangabe: Weiﬂ
  if(!dwClr)
    dwClr = RGB(255, 255, 255);

  //Konfiguration setzen
  SetVisibility(VIS_Owner);
  SetAction(action);
  SetClrModulation(dwClr);

  return true;
}
