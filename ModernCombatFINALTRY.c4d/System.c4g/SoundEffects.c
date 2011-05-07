/* Soundeffekte */

#strict 2

/* Echo */

global func Echo(string szSound) {
  var obj = this;
  while(Contained(obj)) obj = Contained(obj);
  if(GetMaterial(AbsX(GetX(obj)), AbsY(GetY(obj))) != -1) return;
  SoundLevel(szSound, 0, this);
  Sound(szSound, false, this, 100, 0, 0, true, 1000);
  return true;
}