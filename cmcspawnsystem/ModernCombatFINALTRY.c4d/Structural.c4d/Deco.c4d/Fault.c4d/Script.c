/*-- Riss --*/

#strict 2

private func GraphicsCount()	{return 5;}	//Anzahl Grafiken (abz�glich 1)


/* Initalisierung */

func Initialize()
{
  //Zuf�llige Grafik setzen
  var random = Random(GraphicsCount());
  SetAction(Format("Fault%d", random+1));

  //Zuf�llige Rotation
  SetR(RandomX(0,359));
}