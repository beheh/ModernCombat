/*-- Kein FakeDeath --*/

#strict 2

public func IsChooseable()	{return 1;}	//Kann mittels des Spielzielauswählers ausgewählt werden
public func RejectRespawnTimer() {return true;} //Keine Spawnverzoegerung

protected func Activate(iByPlayer)
{
  MessageWindow(GetDesc(), iByPlayer);
  return 1;
}
