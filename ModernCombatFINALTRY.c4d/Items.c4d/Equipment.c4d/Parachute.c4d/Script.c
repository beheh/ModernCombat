/*-- Zusammengepackter Fallschirm --*/

#strict


public func Initialize()
{  
	return(1);
}

protected func Activate(caller)
{
	CreateObject(PARA,0,200,-1)->Set(caller);
	RemoveObject();
}

protected func ControlDigDouble(caller)
{
	Activate(caller);
}
