/*-- Schrotflinten-Erweiterung für Waffenupgrades --*/

#strict 2
#appendto PPGN

func Initialize()
{
   iPermittedAtts = AT_ExtendedMag | AT_Bayonet | AT_Laserpointer;
   return _inherited(...);
}

public func FMData1(int data)
{
  if(data == FM_AmmoLoad)	
    return _inherited(data) + (iAttachment == AT_ExtendedMag)*4;	//Magazingröße

  return _inherited(data);
}

public func GetMCData(int data)
{
  if(data == MC_Damage)		
    return _inherited(data) + (iAttachment == AT_Bayonet)*10;	//Schaden eines Kolbenschlages
  
  return _inherited(data);
}
