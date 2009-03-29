/*-- Damit sich die Tore erst nach dem Einstellen öffnen lassen. --*/

#appendto GAT1
#appendto GAT2
#strict

func Ejection(obj)
{
  if(FindObject(CHOS))
	  Enter(this(),obj);
  if (GetAction () eq "DoorOpen") SetAction ("DoorOpen");
  return( _inherited (obj) );
}

func RejectCollect(id idObj, object pObj)
{
  if(FindObject(CHOS))
    return(true);
  return(false);
}

protected func ActivateEntrance(pObj)
{
  if(FindObject(CHOS))
    return(0);
  if(ActIdle()) SetAction("OpenDoor");
  return(1);
}
