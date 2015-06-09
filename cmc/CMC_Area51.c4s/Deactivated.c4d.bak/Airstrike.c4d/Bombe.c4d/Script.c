/*-- Bombe --*/

#strict
#include TBUL

func Init(int dir)
{
  SetYDir(RandomX(7,10),this(),1);
  SetXDir(dir*RandomX(5,7),this(),1);
  SetR(dir*-20,this());
  //SetXDir(Sin(GetWind(0,0,1)/3,10),this(),1);
  Sound("Artillery*.ogg");
  return(1);
}
