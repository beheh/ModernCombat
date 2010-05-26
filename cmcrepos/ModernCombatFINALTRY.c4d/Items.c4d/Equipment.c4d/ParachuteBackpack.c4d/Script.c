/*-- Fallschirmrucksack --*/

#strict

public func IsEquipment()	{return true;}
public func NoArenaRemove()	{return(true);}


/* Initalisierung */

public func Initialize()
{  
  return(1);
}

/* Aktivierung */

protected func Activate(caller)
{
  //Fallschirm erstellen
  CreateObject(PARA,0,200,-1)->Set(caller);

  RemoveObject();
}

protected func ControlDigDouble(caller)
{
  Activate(caller);
}