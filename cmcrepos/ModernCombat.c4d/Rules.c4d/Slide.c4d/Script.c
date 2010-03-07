/*-- Rutschen --*/

#strict
#appendto CLNK


local speed;      //Geschwindigkeit vor dem Aufprall
local speedfall;  //8tel der Geschwindigkeit beim Aufprall

/*-- Timer --*/

public func UpdateCharge()
{
  //Nur, wenn Regel aktiviert
  if(!FindObject(SLDE)) return 0;
  if(GetAction() ne "FlatUp")
    speed = GetXDir(this(),70);
  if(GetAction() eq "FlatUp")
    if(GetPhase(this())==1)
      speedfall = speed / 7;
  if(GetAction() eq "FlatUp")
    Slipper();
    
  _inherited();
}

private func Slipper() 
{
  if(speed != 0)
   {SetXDir(speed,this(),70);
    speed -= speedfall;}
  return(1);
}

public func IsChooseable()
{
  return(1);
}