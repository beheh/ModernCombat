/*-- Soundeffekte --*/

//Stellt das Echosystem zur Verfügung.

#strict 2


/* Echo */

global func Echo(string szSound, bool fNewObject)
{
  var iLevel = 100;
  var obj = this;
  while(Contained(obj)) obj = Contained(obj);
  if(GetMaterial(AbsX(GetX(obj)), AbsY(GetY(obj))) != -1) iLevel /= 3;

  //Echo objektunabhängig: Platzhalter erstellen
  if(fNewObject)
  {
    obj = CreateObject(TIM1,AbsX(GetX(obj)),AbsY(GetY(obj)));
    Schedule("RemoveObject()",1050,1,obj);
  }

  //Der eigentliche Sound
  SoundLevel(szSound, 0, obj);
  if(iLevel) Sound(szSound, false, obj, BoundBy(iLevel, 0, 100), 0, 0, true, 1000);

  return true;
}

/* Echo für Loop-Sounds */

global func EchoLoop(string szSound, int iLoopCount)
{
  var iLevel = 100;
  var obj = this;
  while(Contained(obj)) obj = Contained(obj);
  if(GetMaterial(AbsX(GetX(obj)), AbsY(GetY(obj))) != -1) iLevel /= 3;

  //Der eigentliche Sound
  //SoundLevel(szSound, 0, obj);
  if(iLevel) Sound(szSound, false, obj, BoundBy(iLevel, 0, 100), 0, iLoopCount, false, 1000);

  return true;
}