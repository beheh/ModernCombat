/*-- Schrotflinten-Erweiterung f�r Waffenupgrades --*/

#strict 2
#appendto PPGN

func Initialize()
{
   iPermittedAtts = AT_ExtendedMag | AT_Bayonet | AT_Laserpointer | AT_Foregrip;
   return _inherited(...);
}

public func FMData1(int data)
{
  if(data == FM_AmmoLoad)	
    return _inherited(data) + (iAttachment == AT_ExtendedMag)*4;	//Magazingr��e

  if(data == FM_SpreadAdd)	return 150 - (iAttachment == AT_Foregrip)*5;	//Bei jedem Schuss hinzuzuaddierende Streuung
  if(data == FM_StartSpread)	return 320 - (iAttachment == AT_Foregrip)*10;	//Bei Auswahl der Waffe gesetzte Streuung
  if(data == FM_MaxSpread)	return 570 - (iAttachment == AT_Foregrip)*10;	//Maximaler Streuungswert

  return _inherited(data);
}

public func GetMCData(int data)
{
  if(data == MC_Damage)		
    return _inherited(data) + (iAttachment == AT_Bayonet)*10;	//Schaden eines Kolbenschlages
  
  return _inherited(data);
}
