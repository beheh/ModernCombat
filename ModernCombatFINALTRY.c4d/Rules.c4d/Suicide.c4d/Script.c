/*-- Selbstmord --*/

#strict

public func IsChooseable()	{return(1);}	//Kann mittels des Spielzielauswählers ausgewählt werden


private func Initialized()
{
  //Verschwinden wenn Kein FakeDeath im Spiel ist
  if(FindObject(NOFD))
   RemoveObject();
}

protected func Activate(iPlr)
{
  MessageWindow(GetDesc(),iPlr);
}