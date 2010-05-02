/*-- Geschoss --*/

#strict 2

local sounded;

func Initialize()
{
   sounded = false;
   return(1);
}

func ResetRotation()
{
  SetR(Angle(GetX(),GetY(),GetX()+GetXDir(),GetY()+GetYDir()),this());
    
  if(GetYDir() > 1 && !sounded)
  {
    sounded = true;
    Schedule("Sound(\"Artillery*.ogg\")",20);
  }
}

func Hit()
{
    Explode(40+RandomX(0,20));
}