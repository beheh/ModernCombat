/*-- Punktetabelle --*/

//Stellt die standardm��ige Punktevergabe f�r Aktionen bereit und ist beliebig erweiter- und ver�nderbar.

#strict 2


global func BonusPoints(string szType, extraData)
{
  //Im Gefecht
  if(szType == "Kill")				return 50;		//Kill
  if(szType == "KillSeries")			return 10;		//Kill-Serie
  if(szType == "Suicide")			return -30;		//Selbstmord
  if(szType == "Teamkill")			return -80;		//Teamkill

  //Assist
  if(szType == "Assist")			return 30;		//Kill Assist
  if(szType == "TracerAssist")			return 30;		//Kill Assist durch Peilsender
  if(szType == "VisualAssist")			return 10;		//Kill Assist durch Blendung/Sichtung

  //Teamwork
  if(szType == "Reanimation")			return 50;		//Reanimation
  if(szType == "Dragnin")			return 10;		//Dragnininjektion an Verb�ndeten
  if(szType == "Healing")			return extraData/4;	//Heilung
  if(szType == "Restocking")			return extraData/3;	//Munitionierung
  if(szType == "Protection")			return 10;		//Schildabwehr/Projektilabwehr
  if(szType == "TracerSet")			return 10;		//Peilsenderplazierung

  //Allgemein
  if(szType == "TechnicalTask")			return 20;		//Entsch�rfung
  if(szType == "VehicleDamage")			return 10;		//Besch�digung
  if(szType == "Destruction")			return 50;		//Fahrzeug-/Strukturzerst�rung
  if(szType == "Repair")			return 10;		//Reparatur

  //Capture the Flag
  if(szType == "CTFSteal")			return 50;		//Flaggendiebstahl
  if(szType == "CTFRegain")			return 30;		//Flaggenr�ckeroberung

  //Besitznahme
  if(szType == "OPNeutralize")			return 10;		//Flaggenpostenneutralisierung
  if(szType == "OPConquer")			return 50;		//Flaggenposteneroberung
  if(szType == "OPDefend")			return 30;		//Flaggenpostenr�ckeroberung
  if(szType == "OPAssist")			return 10;		//Hilfe bei Flaggenposteneroberung/-neutralisierung

  //Assault/Base Assault
  if(szType == "ASDestruction")			return 50;		//Zielobjektzerst�rung
  if(szType == "ASDestructionAssist")		return 20;		//Hilfe bei Zielobjektzerst�rung
  if(szType == "ASTeamDestruction")		return -50;		//Team-Zielobjektzerst�rung
  if(szType == "ASTeamDestructionAssist")	return -20;		//Hilfe bei Team-Zielobjektzerst�rung
  if(szType == "ASTargetDefused")		return 20;		//Sprengladungsentsch�rfung
  if(szType == "ASTargetDefusedAssist")		return 10;		//Hilfe bei Sprengladungsentsch�rfung
  if(szType == "ASTargetArmed")			return 20;		//Sprengladungsplazierung
  if(szType == "ASTargetArmedAssist")		return 10;		//Hilfe bei Sprengladungsplazierung
}