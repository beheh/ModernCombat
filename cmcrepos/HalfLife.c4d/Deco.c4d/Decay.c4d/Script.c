#strict 2

public func Initialize()
{
  var rnd = Random(4)+1;
  if(rnd != 1)
    SetGraphics(Format("%d",rnd));
  else
    SetGraphics();
}