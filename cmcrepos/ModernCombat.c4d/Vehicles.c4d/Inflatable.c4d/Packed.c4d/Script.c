/*-- Schlauchboot (gepackt) --*/

#strict

/* Initialisierung */

public func HandSize()   {return(800);}
public func HandX()      {return(3000);}
public func HandY()      {return(-3000);}
public func IsDrawable() {return(true);}

func IsEquipment(){return(true);}

public func ControlDigDouble(object pCaller)
{
  if(pCaller)
    if(GetProcedure(pCaller) ne "WALK")
      return(1);

  Exit();
  ChangeDef(INFL);
  this->~Initialize();
  SetCategory(GetCategory(0,INFL));
  SetObjDrawTransform();
  return(1);
}

func Deselection(object pClonk)//Hack für Clonks die nicht RejectShift unterstützen. :P
{
  ScheduleCall(this(),"SelectMe",1);
}

protected func SelectMe()
{
  ShiftContents(Contained(),0,GetID());
}

public func RejectShift(){return(true);}

protected func Departure(object p)
{
  RemoveEffect("INFP_Hold",p);
}

protected func Entrance(object pTo)
{
	if(GetOwner(pTo) != NO_OWNER)
		SetOwner(GetOwner(pTo));

	if(Contents(0,Contained()) != this())
		Contents(0,Contained())->~Deselection(Contained());

	ShiftContents(pTo,0,GetID(),0);

  if(!(GetOCF(pTo) & OCF_CrewMember)) return(1);
  if(!GetEffect("INFP_Hold",pTo))
    AddEffect("INFP_Hold",pTo,1,35,0,GetID(),this());
}

public func FxINFP_HoldStart(object pTarget, int iEffectNumber, int iTemp, infp)
{
  if(iTemp)
    if(GetPhysical("Walk", 2, pTarget) <= GetPhysical("Walk", 1, 0, GetID(pTarget))*5/10)
      return();

  // Lähmung
  EffectVar(0, pTarget, iEffectNumber) = GetPhysical("Walk", 1, 0, GetID(pTarget))*3/10;
  EffectVar(1, pTarget, iEffectNumber) = GetPhysical("Jump", 1, 0, GetID(pTarget))*3/10;
  EffectVar(2, pTarget, iEffectNumber) = GetPhysical("Scale", 1, 0, GetID(pTarget))*3/10;
  EffectVar(3, pTarget, iEffectNumber) = GetPhysical("Hangle", 1, 0, GetID(pTarget))*3/10;
  EffectVar(4, pTarget, iEffectNumber) = infp;

  SetPhysical("Walk", GetPhysical("Walk", 0, pTarget)-EffectVar(0, pTarget, iEffectNumber), 2, pTarget);
  SetPhysical("Jump", GetPhysical("Jump", 0, pTarget)-EffectVar(1, pTarget, iEffectNumber), 2, pTarget);
  SetPhysical("Scale", GetPhysical("Scale", 0, pTarget)-EffectVar(2, pTarget, iEffectNumber), 2, pTarget);
  SetPhysical("Hangle", GetPhysical("Hangle", 0, pTarget)-EffectVar(3, pTarget, iEffectNumber), 2, pTarget);
}

public func FxINFP_HoldTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  if(Contained(EffectVar(4,pTarget,iEffectNumber)) != pTarget)
    return(-1);
}

public func FxINFP_HoldStop(target, no, reason, temp)
{
  SetPhysical("Walk", GetPhysical("Walk", 0, target)+EffectVar(0, target, no), 2, target);
  SetPhysical("Jump", GetPhysical("Jump", 0, target)+EffectVar(1, target, no), 2, target);
  SetPhysical("Scale", GetPhysical("Scale", 0, target)+EffectVar(2, target, no), 2, target);
  SetPhysical("Hangle", GetPhysical("Hangle", 0, target)+EffectVar(3, target, no), 2, target);
}