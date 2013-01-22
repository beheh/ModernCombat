/*-- Sturmgewehr-Erweiterung für Waffenupgrades --*/

#strict 2
#appendto ASTR

local iPermittedAtts = AT_ExtendedMag | AT_Bayonet | AT_Laser | AT_Silencer | AT_Foregrip | AT_Grenades;

public func FMData1(int data)
{
  if(data == FM_AmmoLoad)	
    return _inherited(data) + (iAttachment == AT_ExtendedMag)*3;	//Magazingröße

  if(data == FM_SpreadAdd)	return 50 - (iAttachment == AT_Foregrip)*5;	//Bei jedem Schuss hinzuzuaddierende Streuung
  if(data == FM_StartSpread)	return 100 - (iAttachment == AT_Foregrip)*10;	//Bei Auswahl der Waffe gesetzte Streuung
  if(data == FM_MaxSpread)	return 450 - (iAttachment == AT_Foregrip)*10;	//Maximaler Streuungswert

  return _inherited(data);
}

public func GetMCData(int data)
{
  if(data == MC_Damage)		
    return _inherited(data) + (iAttachment == AT_Bayonet)*10;	//Schaden eines Kolbenschlages
  
  return _inherited(data);
}

public func FMData2(int data)
{
  if(iAttachment != AT_Grenades) return 0;
  return _inherited(data);
}
