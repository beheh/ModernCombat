/*-- Neues Script --*/

#strict 2
#appendto _AR2

global func SetArrow(iX,iY) {
  RemoveArrow();
  CreateObject(_AR2,iX,iY,-1);
}
  
global func RemoveArrow() {
	var arrow = FindObject(_AR2);
  if(arrow) RemoveObject(arrow);
}
