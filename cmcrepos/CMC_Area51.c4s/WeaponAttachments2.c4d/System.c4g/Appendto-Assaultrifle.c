/*-- Sturmgewehr-Erweiterung für Waffenupgrades --*/

#strict 2
#appendto ASTR

public func FMData1(int data)
{
  if(data == FM_AmmoLoad)	
    return _inherited(data) + (iAttachment == AT_ExtendedMag)*3;	//Magazingröße

  return _inherited(data);
}

public func GetMCData(int data)
{
  if(data == MC_Damage)		
    return _inherited(data) + (iAttachment == AT_Bayonet)*10;	//Schaden eines Kolbenschlages
  
  return _inherited(data);
}