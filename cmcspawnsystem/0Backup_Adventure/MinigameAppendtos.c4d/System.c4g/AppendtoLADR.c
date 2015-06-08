/*-- Neues Script --*/

#strict 2

#appendto LADR

local n_climbable;
local horizontal;

//Ja, Leitern kann man auch hochklettern.. :D
public func IsClimbable() { return !n_climbable; }

public func ColorLadderTo(int dwColor)
{
	for(var i = 0; i <= length; i++)
		SetClrModulation(dwColor, this, i);
	
	return true;
}
