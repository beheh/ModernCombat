/*-- Rauchgranate --*/

#strict
#include NADE

local time;//:X

public func FuseTime(){return(2*35);}
public func Color(){return(RGB(128,255,0));}//grün
public func ContainedDamage(){return(20);}

func HitSound()
{
  Sound("SGRN_Hit*");
}

public func Fused()
{
  Sound("SGRN_Fused");
  ScheduleCall(this(),"Smoke",14,10);//4 Sekunden lang. :)
  FadeOut();
}

func Smoke()
{
  CastParticles("Smoke3",2,5,0,0,30,100,RGBa(255,255,255,130),RGBa(255,255,255,200));
 
  var dir = +1;
  if(time%2) dir = -1;//Bei ungeraden Zahlen...

  var gas = CreateObject(SM4K,0,0,GetController());
  gas->SetXDir((GetXDir()/3*2) + (dir*(time*(40+Random(20))/10)));
  gas->SetYDir(GetYDir()/3*2 - (time*(10+Random(30))/10));
  
   time++;
}