/*-- Maschinenpistolen-Erweiterung f�r Waffenupgrades --*/

#strict 2
#appendto SMGN

public func FMData1(int data)
{
  if(data == FM_AmmoLoad)	return 30 + (iAttachment == AT_ExtendedMag)*5;	//Magazingr��e

  return _inherited(data);
}
