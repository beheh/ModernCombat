/*-- Computer --*/

#strict 2

public func ThrowAway() {
	 //Auswerfen sofern verschachtelt
  if(Contained())
  {
    var dir = +1, rot = 0;
    if(GetDir(Contained()) == DIR_Right)
    {
      dir = -1;
      rot = 180;
    }

    Exit(0, 0, 0, rot, dir,-3, RandomX(-8,8));

    Sound("AT4R_ThrowAway.ogg");
  }

  //Verschwinden
  FadeOut();
}

/* Allgemein */

protected func Hit()
{
  Sound("AmmoBoxHit*.ogg");
  Sound("CrateImpact*.ogg");
}
