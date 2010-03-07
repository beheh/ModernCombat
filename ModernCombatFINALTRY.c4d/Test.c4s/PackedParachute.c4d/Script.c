/*-- Zusammengepackter Fallschirm --*/

#strict


func Initialize()
{  
  return(1);
}

func ControlDigDouble(caller)
{
  CreateObject(PARA,0,200,-1)->Set(caller);
  RemoveObject();
}