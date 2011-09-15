/*-- Soundeffekte --*/

//Stellt das Echosystem zur Verfügung.

#strict 2


/* Echo */

global func Echo(string szSound)
{
  var iLevel = 100;
  var obj = this;
  while(Contained(obj)) obj = Contained(obj);
  if(GetMaterial(AbsX(GetX(obj)), AbsY(GetY(obj))) != -1) iLevel /= 3;
  SoundLevel(szSound, 0, this);
  if(iLevel) Sound(szSound, false, this, BoundBy(iLevel, 0, 100), 0, 0, true, 1000);
  return true;
}