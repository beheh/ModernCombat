/*-- Nordlichter --*/

//Nordlichter werden unabhängig von Tag und Nacht erstellt.

#strict 2
#appendto NRTS


private func NorthernLights()
{
  //Nordlichter
  if(GetEffectData(EFSM_Enlight) > 0)
  {
    if(!Random(10)) 
      PlaceNorthernLight();
  }
}