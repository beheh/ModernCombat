/*-- Gro�e Schwebeplatte --*/

//Aufz�ge fahren Passagiere nicht automatisch an und nutzen weniger Lichter.

#strict
#appendto LFTP


/* Initialisierung */

protected func Initialize()
{
  AddEffect("LightEffects", this(), 1, 5, this());
  SetAction("Travel");
  UpdateTransferZone();
  return(1);
}

/* Automatische Bewegung */

private func Automatic()
{
  //Keine Automatik wenn ein Clonk mitf�hrt
  if(Passenger()) return(0);
  //Ansonsten eigenst�ndige Bewegung beginnen
  if(SelfTravel()) return(1);
  return(0);
}