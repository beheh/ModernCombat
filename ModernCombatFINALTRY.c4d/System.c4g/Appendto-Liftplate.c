/*-- Gro�e Schwebeplatte --*/

//Aufz�ge fahren Passagiere nicht automatisch an.

#strict
#appendto LFTP


/* Automatische Bewegung */

private func Automatic()
{
  //Keine Automatik wenn ein Clonk mitf�hrt
  if(Passenger()) return(0);
  //Ansonsten eigenst�ndige Bewegung beginnen
  if(SelfTravel()) return(1);
  return(0);
}