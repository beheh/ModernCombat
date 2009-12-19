/* Bohrturm */

#strict
local fontaine;


func Initialize()
{
  SetAction("Timer");
  fontaine=CreateObject(OFFT,0,-24,-1);
  return(1);
}
 
 func Oel()
{
  SetPosition(GetX(),GetY()-24,fontaine);
}
  
  func Destruction()
{
  RemoveObject(fontaine);
  return(1);
}
  
  func Incineration()
{
  RemoveObject(fontaine);
  return(1);
}