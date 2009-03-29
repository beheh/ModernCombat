/*-- Item-Seil --*/

#strict

local target;

public func Set(object pTarget)
{
  target = pTarget;
  pTarget->SetPosition(GetX(),GetY());
  SetAction("Rope",pTarget);
  AddEffect("IntItemRope",pTarget,10,1,this(),0,+1,0);
}

public func FxIntItemRopeStop(object pTarget, int iEffectNumber, int iReason)
{
  //if(iReason < 3) return();

  target = CreateObject(TIM1,0,EffectVar(0,pTarget,iEffectNumber),-1);//Dummy-Objekt
  SetAction("Rope",target);
  AddEffect("IntItemRope",target,10,1,this(),0,-2,EffectVar(0,pTarget,iEffectNumber));
}

public func FxIntItemRopeStart(object pTarget, int iEffectNumber, tmp, speed, ypos)
{
  EffectVar(0,pTarget,iEffectNumber) = ypos;//Aktuelles Y-Offset.
  EffectVar(1,pTarget,iEffectNumber) = speed;//Abseilgeschwindigkeit.
  EffectVar(2,pTarget,iEffectNumber) = GetCategory(pTarget);//Kategorie.
  pTarget->SetCategory(C4D_StaticBack);
}

public func FxIntItemRopeTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  if(Stuck(pTarget))
  {
    EffectVar(0,pTarget,iEffectNumber) -= EffectVar(1,pTarget,iEffectNumber);
    target->SetPosition(GetX(),GetY()+EffectVar(0,pTarget,iEffectNumber));
    target->SetCategory(EffectVar(2,pTarget,iEffectNumber));
    return(-1);
  }
  
  if((GetID(target) == TIM1) && (EffectVar(0,pTarget,iEffectNumber) <= 10))
  {
    SetAction("Idle");
    RemoveObject(target);
    RemoveObject();
    return(-1);
  }

  EffectVar(0,pTarget,iEffectNumber) += EffectVar(1,pTarget,iEffectNumber);
  target->SetPosition(GetX(),GetY()+EffectVar(0,pTarget,iEffectNumber));
}