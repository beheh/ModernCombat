/*-- Maschinenpistolen-Erweiterung für Waffenupgrades --*/

#strict 2
#appendto SMGN

public func FMData1(int data)
{
  if(data == FM_AmmoLoad)	return 30 + (iAttachment == AT_ExtendedMag)*5;	//Magazingröße

  return _inherited(data);
}
