/*-- Zusammengepackter Fallschirm --*/

#strict


func Initialize()
{  
  return(1);
}

func Activate(caller)
{
  CreateObject(PARA,0,200,-1)->Set(caller);
  RemoveObject();
}

func ControlDigDouble(caller)
{
	Activate(caller);
}
