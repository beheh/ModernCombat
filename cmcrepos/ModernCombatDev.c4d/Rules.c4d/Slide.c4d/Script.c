/*-- Rutschen --*/

#strict 2

public func IsChooseable(){ return 1; }

protected func Activate(iByPlayer)
{
  MessageWindow(GetDesc(), iByPlayer);
  return 1;
}

/*-- Append an Clonk --*/

#appendto CLNK

local speed;      //Geschwindigkeit vor dem Aufprall
local speedfall;  //8tel der Geschwindigkeit beim Aufprall

public func UpdateCharge()
{
  //Nur, wenn Regel aktiviert
  if(!FindObject(SLDE)) return 0;
  if(GetAction() != "FlatUp")
    speed = GetXDir(this,70);
  if(GetAction() == "FlatUp")
    if(GetPhase(this)==1)
      speedfall = speed / 7;
  if(GetAction() == "FlatUp")
    Slipper();
    
  _inherited();
}

private func Slipper() 
{
  if(speed != 0)
   {SetXDir(speed,this,70);
    speed -= speedfall;}
  return 1;
}
