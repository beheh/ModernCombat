/*-- Punktetabelle --*/

//Stellt die standardmäßige Punktevergabe für Aktionen bereit und ist beliebig erweiter- und veränderbar.

#strict 2


global func BonusPoints(string szType, extraData)
{
  //Im Gefecht
  if(szType == "Kill")				return 50;		//IC01	Kill
  if(szType == "Multikill")			return 10;		//IC21	Multikill
  if(szType == "Suicide")			return -30;		//IC07	Selbstmord
  if(szType == "Teamkill")			return -80;		//IC06	Teamkill

  //Assist
  if(szType == "KillAssist")			return 30;		//IC02	Kill Assist
  if(szType == "TracerAssist")			return 30;		//IC22	Kill Assist durch Peilsender
  if(szType == "DriverAssist")			return 30;		//IC23	Kill Assist als Fahrer
  if(szType == "VisualAssist")			return 10;		//IC24	Kill Assist durch Sichtung
  if(szType == "BlindingAssist")		return 10;		//IC19	Kill Assist durch Blendung

  //Teamwork
  if(szType == "Reanimation")			return 50;		//IC04	Reanimation
  if(szType == "Healing")			return extraData/4;	//IC05	Heilung
  if(szType == "Supply")			return extraData/3;	//IC14	Munitionierung
  if(szType == "Protection")			return 10;		//IC16	Projektil abgefangen
  if(szType == "TracerSet")			return 10;		//IC17	Peilsender platziert
  if(szType == "Repair")			return 10;		//IC29	Reparatur

  //Zerstörung
  if(szType == "Defusing")			return 20;		//IC15	Entschärfung
  if(szType == "Damage")			return 10;		//IC18	Beschädigung
  if(szType == "VehicleDestruction")		return 50;		//IC27	Fahrzeug zerstört
  if(szType == "StructureDestruction")		return 50;		//IC03	Struktur zerstört

  //Capture the Flag
  if(szType == "CTFTheft")			return 50;		//IC08	Flagge gestohlen
  if(szType == "CTFRecovery")			return 30;		//IC09	Flagge geborgen

  //Besitznahme/Hold the Flag
  if(szType == "OPConquest")			return 50;		//IC10	Flaggenposten erobert
  if(szType == "OPConquestAssist")		return 10;		//IC11	Hilfe bei Flaggenposteneroberung
  if(szType == "OPNeutralization")		return 10;		//IC13	Flaggenposten neutralisiert
  if(szType == "OPDefense")			return 30;		//IC12	Flaggenposten verteidigt
  if(szType == "Control")			return 10;		//IC28	Team führt

  //Assault/Base Assault
  if(szType == "ASDestruction")			return 50;		//IC25	Zielobjekt zerstört
  if(szType == "ASDestructionAssist")		return 20;		//IC26	Hilfe bei Zielobjektzerstörung
  if(szType == "ASPlacement")			return 20;		//IC30	Sprengladung platziert
  if(szType == "ASPlacementAssist")		return 10;		//IC31	Hilfe bei Sprengladungsplatzierung
  if(szType == "ASDisarming")			return 20;		//IC32	Sprengladung entschärft
  if(szType == "ASDisarmingAssist")		return 10;		//IC20	Hilfe bei Sprengladungsentschärfung
}