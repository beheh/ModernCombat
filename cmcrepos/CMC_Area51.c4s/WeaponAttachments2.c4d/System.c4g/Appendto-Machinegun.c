/*-- Maschinengewehr-Erweiterung für Waffenupgrades --*/

#strict 2
#appendto MNGN

func Initialize()
{
   iPermittedAtts = AT_Bayonet | AT_Laserpointer | AT_Foregrip;
   return _inherited(...);
}

public func FMData1(int data)
{

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
