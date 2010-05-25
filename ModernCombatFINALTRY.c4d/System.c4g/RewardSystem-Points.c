/*-- Punktetabelle --*/

//Stellt Punkte f�r Standardaktionen bereit und ist beliebig erweiterbar.

#strict 2


global func KillPoints()		{return 50;}	//Punkte pro Kill
global func AssistPoints()		{return 30;}	//Punkte pro Assist-Kill
global func SuicidePoints()		{return -30;}	//Punkte pro Selbstmord
global func TeamkillPoints()		{return -80;}	//Punkte pro Teamkill
global func ReanimationPoints()		{return 50;}	//Punkte f�r Reanimation
global func DragninPoints()		{return 10;}	//Punkte f�r Dragnininjektion an Verb�ndeten

//DestructionPoints wird aus Wert des gegnerischen Geb�udes/Multiplikator errechnet
//global func DestructionPoints(int iValue){return iValue/2;}

global func BonusPoints(string szType, extraData)
{
  if(szType == "Restocking")	return extraData/3;	//Munitionswert/3 Punkte
  if(szType == "Healing")	return extraData/2;	//Geheilte Menge/2 Punkte
  if(szType == "OPNeutralize")	return 10;		//Punkte f�r Flaggenneutralisierung
  if(szType == "OPConquer")	return 50;		//Punkte f�r Flaggeneroberung
  if(szType == "OPDefend")	return 30;		//Punkte f�r Flaggenr�ckeroberung
  if(szType == "OPAssist")	return 10;		//Hilfe bei Eroberung/Neutralisierung
  if(szType == "CTFSteal")	return 50;		//Flagge stehlen
  if(szType == "CTFRegain")	return 30;		//Flagge zur�ckgeholt
  if(szType == "TechnicalTask")	return 10;		//Sprengsatz entsch�rft
  if(szType == "Protection")	return 10;		//Punkte f�r Schildabwehr
  if(szType == "Destruction")	return 30;		//Punkte f�r zerst�rte feindliche Struktur
  if(szType == "TracerSet")	return 10;		//Punkte f�r angebrachten Tracer
  if(szType == "TracerAssist")	return 20;		//Punkte f�r Tracerkill

  //if("IncinerateClonk")	return 20;		//Bonuspunkte f�r das anz�nden gegnerischer Clonks
  //if("DestroyHomebase")	return 100;		//Bonuspunkte f�r die Zerst�rung gegnerischer Heimatbasen
}
