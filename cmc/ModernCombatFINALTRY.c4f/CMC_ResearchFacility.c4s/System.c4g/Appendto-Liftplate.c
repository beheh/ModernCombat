/*-- Große Schwebeplatte --*/

//Aufzüge fahren Passagiere nicht automatisch an.

#strict
#appendto LFTP


/* Automatische Bewegung */

private func Automatic()
{
  //Keine Automatik wenn ein Clonk mitfährt
  if(Passenger()) return(0);
  //Ansonsten eigenständige Bewegung beginnen
  if(SelfTravel()) return(1);
  return(0);
}