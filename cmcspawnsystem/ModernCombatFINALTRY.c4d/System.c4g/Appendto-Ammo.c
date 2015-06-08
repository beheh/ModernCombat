/*-- Projektilmunition --*/

//Hazardprojektile (STAM) werden zu CMC Munition (ABOX) umgewandelt.

#strict 2

#appendto STAM

public func AmmoPackID()	{return(ABOX);}