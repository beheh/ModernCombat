/*-- Punktetabelle --*/

//Stellt die standardliche Punktevergabe für Aktionen bereit und ist beliebig erweiter- und veränderbar.

#strict 2


global func KillPoints()			{return 50;}	//Kill
global func AssistPoints()			{return 30;}	//Kill Assist
global func SuicidePoints()			{return -30;}	//Selbstmord
global func TeamkillPoints()			{return -80;}	//Teamkill
global func ReanimationPoints()			{return 50;}	//Reanimation
global func DragninPoints()			{return 10;}	//Dragnininjektion an Verbündeten

//Zerstörungspunkte werden aus dem Wert des gegnerischen Gebäudes/Multiplikator errechnet
//global func DestructionPoints(int iValue){return iValue/2;}

global func BonusPoints(string szType, extraData)
{
  if(szType == "Healing")			return extraData/4;	//Heilung
  if(szType == "Restocking")			return extraData/3;	//Munitionierung
  if(szType == "Protection")			return 10;		//Schildabwehr/Projektilabwehr
  if(szType == "TracerSet")			return 10;		//Peilsenderplazierung
  if(szType == "TracerAssist")			return 20;		//Kill Assist durch Peilsender
  if(szType == "VisualAssist")			return 10;		//Kill Assist durch Blendung/Sichtung
  if(szType == "TechnicalTask")			return 20;		//Entschärfung
  if(szType == "VehicleDamage")			return 10;		//Beschädigung
  if(szType == "Destruction")			return 50;		//Fahrzeug-/Strukturzerstörung
  if(szType == "CTFSteal")			return 50;		//Flaggendiebstahl
  if(szType == "CTFRegain")			return 30;		//Flaggenrückeroberung
  if(szType == "OPNeutralize")			return 10;		//Flaggenpostenneutralisierung
  if(szType == "OPConquer")			return 50;		//Flaggenposteneroberung
  if(szType == "OPDefend")			return 30;		//Flaggenpostenrückeroberung
  if(szType == "OPAssist")			return 10;		//Hilfe bei Flaggenposteneroberung/-neutralisierung
  if(szType == "ASDestruction")			return 50;		//Zielobjektzerstörung
  if(szType == "ASDestructionAssist")		return 20;		//Hilfe bei Zielobjektzerstörung
  if(szType == "ASTeamDestruction")		return -50;		//Team-Zielobjektzerstörung
  if(szType == "ASTeamDestructionAssist")	return -20;		//Hilfe bei Team-Zielobjektzerstörung
  //if("IncinerateClonk")			return 20;		//Anzündung gegnerischer Clonks
  //if("DestroyHomebase")			return 100;		//Zerstörung gegnerischer Heimatbasis
}