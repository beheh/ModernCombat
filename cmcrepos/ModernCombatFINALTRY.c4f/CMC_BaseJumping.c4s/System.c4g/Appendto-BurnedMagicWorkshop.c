/*-- Verbrannte Zauberwerkstatt --*/

//Verbrannte Zauberwerkstätte können als unzerstörbare Landschaftsobjekte benutzt werden.

#strict 2
#appendto BMWS

protected func Incineration()	{Extinguish();}
public func OnDmg()		{return 0;}
protected func Construction()	{return 0;}	//Kein Fundament

/* Initialisierung */

protected func Initialize()
{
  //In den Hintergrund verschieben
  SetCategory(C4D_StaticBack);
  SetCategory(C4D_Background);
  return 1;
}