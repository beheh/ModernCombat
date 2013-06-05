/*-- Revolver-Erweiterung für Waffenupgrades --*/

#strict 2
#appendto RVLR

func PermittedAtts()
{
  return AT_Laserpointer | AT_Flashlight;
}

public func FMData1(int data)
{
  if(data == FM_SpreadAdd)	return 110 - (iAttachment == AT_Laserpointer)*4;	//Bei jedem Schuss hinzuzuaddierende Streuung
  if(data == FM_StartSpread)	return 30 - (iAttachment == AT_Laserpointer)*20;	//Bei Auswahl der Waffe gesetzte Streuung
  if(data == FM_MaxSpread)	return 220 - (iAttachment == AT_Laserpointer)*70;	//Maximaler Streuungswert
  if(data == FM_MinSpread)	return 20 - (iAttachment == AT_Laserpointer)*10;	//Kleinstmögliche Streuung

  return _inherited(data);
}