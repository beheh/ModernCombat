/*-- Schrotflinten-Erweiterung für Waffenupgrades --*/

#strict 2
#appendto PPGN

func PermittedAtts()
{
  return AT_ExtendedMag | AT_Bayonet | AT_Laserpointer | AT_Foregrip;
}

public func FMData1(int data)
{
  if(data == FM_AmmoLoad)
    return _inherited(data) + (iAttachment == AT_ExtendedMag)*8;		//Magazingröße

  if(data == FM_SpreadAdd)	return 150 - (iAttachment == AT_Foregrip)*3;	//Bei jedem Schuss hinzuzuaddierende Streuung
  if(data == FM_StartSpread)	return 320 - (iAttachment == AT_Foregrip)*70;	//Bei Auswahl der Waffe gesetzte Streuung
  if(data == FM_MaxSpread)	return 570 - (iAttachment == AT_Foregrip)*70;	//Maximaler Streuungswert
  if(data == FM_MinSpread)	return 120 - (iAttachment == AT_Foregrip)*20;	//Minimal mögliche Streuung

  return _inherited(data);
}

public func GetMCData(int data)
{
  if(data == MC_Damage)		
    return _inherited(data) + (iAttachment == AT_Bayonet)*6;	//Schaden eines Kolbenschlages
  
  return _inherited(data);
}