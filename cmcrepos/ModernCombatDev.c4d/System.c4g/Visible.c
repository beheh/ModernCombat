#strict 2

global func CheckVisibility(object Obj1, object Obj2)
{
	if(!Obj2)
		Obj2 = this;
	
  //Geblendet?
  if(GetEffect("IntFlashbang",Obj1))
    return false;
	
	return inherited(Obj1,Obj2,...);
}