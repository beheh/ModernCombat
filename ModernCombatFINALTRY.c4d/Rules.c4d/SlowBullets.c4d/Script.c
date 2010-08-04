/*-- langsame Projektile --*/

#strict 2

public func IsChooseable()	{return 1;}	//Kann mittels des Spielzielauswählers ausgewählt werden


protected func Activate(iPlr)
{
  MessageWindow(GetDesc(),iPlr);
}

global func FastBullets()
{
  return !ObjectCount(SBUL);
}