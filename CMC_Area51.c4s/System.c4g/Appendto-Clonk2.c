/*-- Clonk-Appendto für die Waffenplattform --*/

#strict
#appendto CLNK


private func Riding()
{
  // Richtung an die des Pferdes anpassen
  if(GetActionTarget()->~VaryingDir())
  	SetDir(GetActionTarget()->~VaryingDir()-2);
  else
  	SetDir(GetDir(GetActionTarget()));
  // Pferd steht still: Clonk soll auch still sitzen
  if(GetActionTarget()->~IsStill())
  {
    if(GetAction() ne "RideStill")
      SetAction("RideStill");
  }
  // Pferd steht nicht still: Clonk soll auch nicht still sitzen
  else
    if(GetAction() ne "Ride")
      SetAction("Ride");
  return(1);
}
