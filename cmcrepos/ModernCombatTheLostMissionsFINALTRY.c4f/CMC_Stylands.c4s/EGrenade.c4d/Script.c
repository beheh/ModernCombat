/*-- Treibsandgranate --*/

#strict
#include NADE

public func FuseTime(){return(2*35);}
public func Color(){return(RGB(255,128,0));}//orange
public func ContainedDamage(){return(0);}

func HitSound()
{
  Sound("SGRN_Hit*");
}

public func Fused()
{
  DoCircle("Earth-Smooth",30,0);

  Sound("EGRN_Blast");
  CreateParticle("Blast",0,0,0,0,10*30,RGB(255,0,0));
  var r;
  for(r = 0; r <= 360; r+= 10)
    CreateParticle("Smoke2",Sin(r,30+5),-Cos(r,30+5),0,0,10*10,RGB(255,128,0));
    
  RemoveObject();
}

func DoCircle(string szMat, int iROut, int iRIn)
{
  var r;
  var x = GetX();
  var y = GetY();
  for(r = 0; r <= 360; r+= 10)
  { 
    DrawMaterialQuad(szMat,x + Sin(r, iRIn+iROut),
                           y - Cos(r, iRIn+iROut),
                           
                           x + Sin(r+10, iRIn+iROut),
                           y - Cos(r+10, iRIn+iROut),
                           
                           x + Sin(r+10, iRIn),
                           y - Cos(r+10, iRIn),
                           
                           x + Sin(r, iRIn),
                           y - Cos(r, iRIn));                                                            
  }
}