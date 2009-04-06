/*-- Schlauchboot (gepackt) --*/

#strict

/* Initialisierung */

public func HandSize()   {return(800);}
public func HandX()      {return(3000);}
public func HandY()      {return(-3000);}
public func IsDrawable() {return(true);}

func IsEquipment(){return(true);}

protected func Activate(object pCaller)
{
  if(pCaller)
    if(GetProcedure(pCaller) ne "WALK")
      return(1);

  CreateObject(INFL,0,0,GetOwner(pCaller));
  RemoveObject();
  return(1);
}

func Deselection(object pClonk)//Hack f�r Clonks die nicht RejectShift unterst�tzen. :P
{
  ScheduleCall(this(),"SelectMe",1);
}

protected func SelectMe()
{
  ShiftContents(Contained(),0,GetID());
}

public func RejectShift(){return(true);}