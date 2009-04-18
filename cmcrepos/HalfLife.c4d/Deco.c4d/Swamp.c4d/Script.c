#strict 2

public func Initialize()
{
  var rnd;
  if(Random(4))
    rnd = Random(4)+1;
  else
    rnd = 4+Random(4)+1;

  if(rnd != 1)
    SetGraphics(Format("%d",rnd));
  else
    SetGraphics();
}