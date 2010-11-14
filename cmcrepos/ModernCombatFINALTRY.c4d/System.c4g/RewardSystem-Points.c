/*-- Punktetabelle --*/

//Stellt die standardliche Punktevergabe für Aktionen bereit und ist beliebig erweiter- und veränderbar.

#strict 2


global func KillPoints()		{return 50;}	//Kill
global func AssistPoints()		{return 30;}	//Assist-Kill
global func SuicidePoints()		{return -30;}	//Selbstmord
global func TeamkillPoints()		{return -80;}	//Teamkill
global func ReanimationPoints()		{return 50;}	//Reanimation
global func DragninPoints()		{return 10;}	//Dragnininjektion an Verbündeten

//DestructionPoints wird aus Wert des gegnerischen Gebäudes/Multiplikator errechnet
//global func DestructionPoints(int iValue){return iValue/2;}

global func BonusPoints(string szType, extraData)
{
  if(szType == "Restocking")	return extraData/3;	//Munitionierung
  if(szType == "Healing")	return extraData/4;	//Heilung
  if(szType == "OPNeutralize")	return 10;		//Flaggenneutralisierung
  if(szType == "OPConquer")	return 50;		//Flaggeneroberung
  if(szType == "OPDefend")	return 30;		//Flaggenrückeroberung
  if(szType == "OPAssist")	return 10;		//Hilfe bei Eroberung/Neutralisierung
  if(szType == "CTFSteal")	return 50;		//Flagge stehlen
  if(szType == "CTFRegain")	return 30;		//Flagge zurückgeholt
  if(szType == "TechnicalTask")	return 20;		//Sprengsatz entschärft
  if(szType == "Protection")	return 10;		//Schildabwehr / Abwehr mit Täuschkörper
  if(szType == "Destruction")	return 50;		//Fahrzeug/Strukturzerstörung
  if(szType == "TracerSet")	return 10;		//Peilsendermarkierung
  if(szType == "TracerAssist")	return 20;		//Assist-Kill mit Peilsender
  if(szType == "VehicleDamage")  return 10;		//Fahrzeugschädigung

  //if("IncinerateClonk")	return 20;		//Anzündung gegnerischer Clonks
  //if("DestroyHomebase")	return 100;		//Zerstörung gegnerischer Heimatbasis
}
