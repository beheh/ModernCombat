/*-- MunitionsHUD --*/

//Nur inkludierende Munitionstypen werden im HUD anzeigen.

#strict 2

#appendto STAM	//Kugeln
#appendto GRAM	//Granaten
#appendto MIAM	//Raketen

public func IsHUDAmmo()	{return true;}