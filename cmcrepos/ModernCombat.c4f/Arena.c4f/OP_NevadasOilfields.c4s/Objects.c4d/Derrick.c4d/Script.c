/* Bohrturm */

#strict

local fontaine, onfire;


func Initialize()
{
  SetAction("Timer");
  fontaine = CreateObject(OFFT,0,-24,-1);
  fontaine->Set(this());
  return(1);
}
 
 func Oel()
{
  SetPosition(GetX(),GetY()-24,fontaine);
}
  
func Damage() {
  onfire = 1;
  }

func CheckFire() {
  if(!Random(20))
    onfire = 0;
  }

func Burning() { return(onfire); }