/*-- Kein FakeDeath --*/

#strict 2

public func IsChooseable()	{return 1;}	//Kann mittels des Spielzielausw�hlers ausgew�hlt werden
public func RejectRespawnTimer() {return true;} //Keine Spawnverzoegerung

protected func Activate(iByPlayer)
{
  MessageWindow(GetDesc(), iByPlayer);
  return 1;
}
