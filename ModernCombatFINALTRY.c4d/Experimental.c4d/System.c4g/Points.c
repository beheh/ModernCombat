/*-- Anzahl der vergebenen Punkte f�r bestimmte Aktionen --*/

#strict 2

//Alle Mengen der Punkte k�nnen beliebig eingestellt werden

global func KillPoints()		{return 50;}	//Punkte pro Kill
global func AssistPoints()		{return 30;}	//Punkte pro Assist-Kill
global func SuicidePoints()		{return -30;}	//Punkte pro Selbstmord
global func TeamkillPoints()		{return -80;}	//Punkte pro Teamkill
global func ReanimationPoints()		{return 50;}	//Punkte f�r Reanimation


//DestructionPoints wird aus Wert des gegnerischen Geb�udes/Multiplikator errechnet
//global func DestructionPoints(int iValue){ return iValue/2; }

//Bonuspunkte f�r bestimmte Aktionen. Benutzung:
//Zerst�rtes Objekt muss Callback liefern + Typ der Belohnung als String
//Anschlie�end in die Punkteverwertung diese Funktion mit Typ f�ttern

global func BonusPoints(string szType, extraData)
{
  if(szType == "Restocking") return extraData/3;		//Munitionswert/3 Punkte
  if(szType == "Healing") return extraData/2;			//Geheilte Menge/2 Punkte
  if(szType == "OPNeutralize") return 40;			//Punkte f�r Flaggenneutralisierung
  if(szType == "OPConquer") return 30;				//Punkte f�r Flaggeneroberung
  if(szType == "OPSavekill") return 30;				//Flaggenverteidigung
  if(szType == "OPAssist") return 20;				//Hilfe bei Eroberung/Neutralisierung
  if(szType == "CTFSteal") return 50;				//Flagge stehlen
  if(szType == "CTFRegain") return 50;				//Flagge zur�ckgeholt
  //if("IncinerateClonk") return 20;				//Bonuspunkte f�r das anz�nden gegnerischer Clonks...
  //if("DestroyHomebase") return 100;				//Bonuspunkte, falls man Geb�ude mit Flagge zerst�rt...
}
