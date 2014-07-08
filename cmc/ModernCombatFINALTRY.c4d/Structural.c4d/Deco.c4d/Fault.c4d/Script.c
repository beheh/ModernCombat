/*-- Riss --*/

#strict 2

private func GraphicsCount()	{return 5;}	//Anzahl Grafiken (abzüglich 1)


/* Initalisierung */

func Initialize()
{
  //Zufällige Grafik setzen
  var random = Random(GraphicsCount());
  SetAction(Format("Fault%d", random+1));

  //Zufällige Rotation
  SetR(RandomX(0,359));
}