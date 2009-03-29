/*-- ItemKiste --*/

#strict

public func IsBulletTarget() {return(true);}
public func IsCraneGrabable() {return(1);}
public func CanBeLaserRecycled() {return(2);}

func ControlDigDouble(object p)
{
  var speed = 50;
  if(p) speed = 20;
  CastParticles("WoodSplinter", 10, speed, 0,0, 25, 50, RGBa(255,255,0,0), RGBa(255,255,0,0));
  Sound("H2IC_Break*.ogg");
  while(Contents())
    Exit(Contents(),0,0,Random(360),RandomX(-1,+1)+GetXDir(),GetYDir(),RandomX(-6,+6));
  RemoveObject();
}

func Damage()
{
  if(GetDamage() > 20)
    ControlDigDouble();
}

func Hit()
{
  Sound("WoodHit*");
}

func Hit3()
{
  ControlDigDouble();
}