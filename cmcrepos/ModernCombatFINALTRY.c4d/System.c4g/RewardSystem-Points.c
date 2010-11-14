/*-- Punktetabelle --*/

//Stellt die standardliche Punktevergabe f�r Aktionen bereit und ist beliebig erweiter- und ver�nderbar.

#strict 2


global func KillPoints()		{return 50;}	//Kill
global func AssistPoints()		{return 30;}	//Assist-Kill
global func SuicidePoints()		{return -30;}	//Selbstmord
global func TeamkillPoints()		{return -80;}	//Teamkill
global func ReanimationPoints()		{return 50;}	//Reanimation
global func DragninPoints()		{return 10;}	//Dragnininjektion an Verb�ndeten

//DestructionPoints wird aus Wert des gegnerischen Geb�udes/Multiplikator errechnet
//global func DestructionPoints(int iValue){return iValue/2;}

global func BonusPoints(string szType, extraData)
{
  if(szType == "Restocking")	return extraData/3;	//Munitionierung
  if(szType == "Healing")	return extraData/4;	//Heilung
  if(szType == "OPNeutralize")	return 10;		//Flaggenneutralisierung
  if(szType == "OPConquer")	return 50;		//Flaggeneroberung
  if(szType == "OPDefend")	return 30;		//Flaggenr�ckeroberung
  if(szType == "OPAssist")	return 10;		//Hilfe bei Eroberung/Neutralisierung
  if(szType == "CTFSteal")	return 50;		//Flagge stehlen
  if(szType == "CTFRegain")	return 30;		//Flagge zur�ckgeholt
  if(szType == "TechnicalTask")	return 20;		//Sprengsatz entsch�rft
  if(szType == "Protection")	return 10;		//Schildabwehr / Abwehr mit T�uschk�rper
  if(szType == "Destruction")	return 50;		//Fahrzeug/Strukturzerst�rung
  if(szType == "TracerSet")	return 10;		//Peilsendermarkierung
  if(szType == "TracerAssist")	return 20;		//Assist-Kill mit Peilsender
  if(szType == "VehicleDamage")  return 10;		//Fahrzeugsch�digung

  //if("IncinerateClonk")	return 20;		//Anz�ndung gegnerischer Clonks
  //if("DestroyHomebase")	return 100;		//Zerst�rung gegnerischer Heimatbasis
}
