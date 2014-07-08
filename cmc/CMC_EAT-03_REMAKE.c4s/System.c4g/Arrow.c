/*-- Neues Script --*/

#strict 2

#appendto _AR2

global func SetArrow(iX, iY, int iPlr) 
{
  RemoveArrow(iPlr);
  CreateObject(_AR2, iX, iY, iPlr);
  
  return true;
}
  
global func RemoveArrow(int iPlr) 
{
	var arrow = FindObject2(Find_ID(_AR2), Find_Owner(iPlr));
  if(arrow) RemoveObject(arrow);
  
  return true;
}
