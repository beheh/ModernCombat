/*-- Computer --*/

#strict 2

public func ThrowAway() {
	 //Auswerfen sofern verschachtelt
  var dir = +1, rot = 0;
  if(GetDir(Contained()) == DIR_Right)
  {
    dir = -1;
    rot = 180;
  }

  if(Contained())
    Exit(0, 0, 0, rot, dir,-3, RandomX(-8,8));
  else {
  	SetR(rot);
  	SetXDir(dir*10);
  	SetYDir(-30);
  	SetRDir(RandomX(-80, 80));
  }

  Sound("AT4R_ThrowAway.ogg");

  //Verschwinden
  FadeOut();
}

/* Allgemein */

protected func Hit()
{
  Sound("AmmoBoxHit*.ogg");
  Sound("CrateImpact*.ogg");
}
