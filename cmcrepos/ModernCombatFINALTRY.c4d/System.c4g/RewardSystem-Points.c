/*-- Punktetabelle --*/

//Stellt die standardm��ige Punktevergabe f�r Aktionen bereit und ist beliebig erweiter- und ver�nderbar.

#strict 2


global func KillPoints()			{return 50;}	//Kill
global func AssistPoints()			{return 30;}	//Kill Assist
global func KillSeries()			{return 10;}	//Kill-Serie
global func SuicidePoints()			{return -30;}	//Selbstmord
global func TeamkillPoints()			{return -80;}	//Teamkill
global func ReanimationPoints()			{return 50;}	//Reanimation
global func DragninPoints()			{return 10;}	//Dragnininjektion an Verb�ndeten

//Zerst�rungspunkte werden aus dem Wert des gegnerischen Geb�udes/Multiplikator errechnet
//global func DestructionPoints(int iValue){return iValue/2;}

global func BonusPoints(string szType, extraData)
{
  if(szType == "Healing")			return extraData/4;	//Heilung
  if(szType == "Restocking")			return extraData/3;	//Munitionierung
  if(szType == "Protection")			return 10;		//Schildabwehr/Projektilabwehr
  if(szType == "TracerSet")			return 10;		//Peilsenderplazierung
  if(szType == "TracerAssist")			return 30;		//Kill Assist durch Peilsender
  if(szType == "VisualAssist")			return 10;		//Kill Assist durch Blendung/Sichtung
  if(szType == "TechnicalTask")			return 20;		//Entsch�rfung
  if(szType == "VehicleDamage")			return 10;		//Besch�digung
  if(szType == "Destruction")			return 50;		//Fahrzeug-/Strukturzerst�rung
  if(szType == "CTFSteal")			return 50;		//Flaggendiebstahl
  if(szType == "CTFRegain")			return 30;		//Flaggenr�ckeroberung
  if(szType == "OPNeutralize")			return 10;		//Flaggenpostenneutralisierung
  if(szType == "OPConquer")			return 50;		//Flaggenposteneroberung
  if(szType == "OPDefend")			return 30;		//Flaggenpostenr�ckeroberung
  if(szType == "OPAssist")			return 10;		//Hilfe bei Flaggenposteneroberung/-neutralisierung
  if(szType == "ASDestruction")			return 50;		//Zielobjektzerst�rung
  if(szType == "ASDestructionAssist")		return 20;		//Hilfe bei Zielobjektzerst�rung
  if(szType == "ASTeamDestruction")		return -50;		//Team-Zielobjektzerst�rung
  if(szType == "ASTeamDestructionAssist")	return -20;		//Hilfe bei Team-Zielobjektzerst�rung
  if(szType == "Repair")			return 10;		//Reparatur
  //if("IncinerateClonk")			return 20;		//Anz�ndung gegnerischer Clonks
  //if("DestroyHomebase")			return 100;		//Zerst�rung gegnerischer Heimatbasis
}