/*-- Laterne --*/

#strict 2
#include BULB

local unstucking;

public func IgnoreTracer()	{return true;}


private func Adjust()
{
  if(!base)
  {
    RemoveObject(cable);
    RemoveObject();
  }
  if(!dir)
    return;

  //In Material zurückpendeln
  if(Stuck())
  {
    if(!unstucking)
    {
      unstucking = true;
      dir = -dir;
      speed = -dir*5;
    }
  }
  else
    unstucking = false;

  //Position anpassen
  UpdatePos();

  phase += speed;
  speed += dir;

  if((speed > 0 && dir < 0) || (speed < 0 && dir > 0))
    if(speed%5)
      speed += dir;

  if(Abs(phase) >= 900)
  {
    speed = 0;
    phase = phase/Abs(phase)*899;
  }

  if((phase >= 0 && speed >= 0 && dir > 0) || (phase <= 0 && speed <= 0 && dir < 0))
  {
    dir *= -1;
    if(Abs(speed) < 15)
    {
      speed+=dir;
      phase+=dir;
      if(Abs(speed) < 7 && Abs(phase) < 5)
      {
        dir = 0;
        speed = 0;
        speed = 0;
      }
    }
  }
  SetR(Angle(GetX(),GetY(),base->GetX(),base->GetY()));

  //Kabel anpassen
  cable->Update();
}

public func IsBulletTarget(id idBullet, object pBullet, object pShooter)
{
  if(pBullet && pBullet->~IsBullet())
    if(!unstucking)
    {
      speed += pBullet->GetXDir();
      speed = BoundBy(speed,-55,55);
      dir = BoundBy(-speed,-1,1);
    }
  return;
}

public func Damage()
{
  RemoveObject(cable);
  RemoveObject(base);
  Explode(5);
  return;
}