/*-- Why not since R1.0 :(( --*/

#strict 2

#appendto C4PA

protected func Check() {
  if(thrown) return;

  //Grund zum Existieren?
  if(!GetPackPoints() && !GetLength(GetC4())) {
		//In GunGame ggf. wieder auffuellen
		if(GetEffect("IntKeepObject", this))
			return SetPackPoints(1);

    thrown = true;

    //Kategorie wechseln
    SetCategory(C4D_Vehicle);

    //Schützen verlassen
    if(Contained())
    {
      var dir = +1;
      if(GetDir(Contained()) == DIR_Right)
        dir = -1;

      Exit(0, 0,0, Random((360)+1), dir,-3, Random(11)-5);

      Sound("AT4R_ThrowAway.ogg");
    }

    //Verschwinden
    FadeOut();
  }
}

public func UpdateHUD(object pHUD) {
	if(GetEffect("IntKeepObject", this)) {
		var clr = 0xFFFFFF;
		if(!GetPackPoints())
			clr = 0xFF0000;

		pHUD->~Ammo(-1, -1, GetName(), true, clr);
	}
	else
  	_inherited(pHUD, ...);
}
