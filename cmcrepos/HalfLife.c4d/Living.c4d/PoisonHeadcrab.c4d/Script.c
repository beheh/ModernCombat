/*--- giftige Headcrab---*/

#strict
#include HCRB


/* Überladungen */

private func ExecAttackJump()
{
  Jump();
  var target = GetActionTarget();
  if(target)
  {
    SetXDir(BoundBy(GetX(target) - GetX(), -50, +50));
    SetYDir(BoundBy(GetY(target) - GetY(), -50, -20));
    SetActionTargets();
  }
  else
  {
    var jump = GetPhysical("Jump", 0) * 100 / GetPhysical("Jump", 0,0, GetID());
    SetXDir(22 * (GetDir()*2-1) * jump / 100);
    SetYDir(-28 * jump / 100);
    if(!GetEffect("IntAttackDelay", this())) AddEffect("IntAttackDelay", this(), 1, 120);
  }
  SetAction("AttackJump");
  JumpSound();
}

public func FxAttackingTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  var victim = FindObject2(Find_AtPoint(),Find_OCF(OCF_Prey|OCF_Alive),Find_NoContainer(),Find_Not(Find_Faction(Faction())),Find_Exclude(this()),Sort_Distance());
  if(victim)
  {
    BiteSound();
    Fling(victim, BoundBy(GetXDir(),-2,+2), BoundBy(GetYDir(),-2,+2));
    DoDmg(4,DMG_Melee,victim);
    if(victim)
    {
      if(!GetEffect("PCRB_Poison",victim) && !victim->~IsMachine() && (victim->~OnDmg(1,DMG_Bio) < 100))
      {
        AddEffect("PCRB_Poison",victim,125,20,0,GetID(),GetController(),60);
      }
      else
      {
        //var effect = GetEffect("PCRB_Poison",victim);
        //EffectVar(0, victim, effect) = 60;//O_o
        ScreenRGB(pTarget,RGBa(100,255,0,220),0,0,45,false);
      }
    }
    SetYDir(0);
    if(!GetEffect("IntAttackDelay", this())) AddEffect("IntAttackDelay", this(), 1, 45);
    return(-1);
  }
}

private func Alert(object pTarget)
{
  if(!pTarget) return();
  _inherited(pTarget);

  AddEffect("IntAttackDelay", this(), 1, 45);
}


/* Gifteffekt */

public func FxPCRB_PoisonStart(object pTarget, int iEffectNumber, int iTemp, controller, max)
{
  if(iTemp)
    return();

  //Lähmung
  pTarget->Paralyze(max*GetEffect(0,pTarget,iEffectNumber,3));
  
  //Gift
  EffectVar(0, pTarget, iEffectNumber) = max;//HP die maximal abgezogen werden...
  EffectVar(1, pTarget, iEffectNumber) = GetEnergy(pTarget);
  EffectVar(2, pTarget, iEffectNumber) = controller;
  ScreenRGB(pTarget,RGBa(100,255,0,192),0,0,70,false);
  
  return(1);
}

public func FxPCRB_PoisonTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  if(EffectVar(1, pTarget, iEffectNumber) < GetEnergy(pTarget))
    return(-1);

  EffectVar(1, pTarget, iEffectNumber) = GetEnergy(pTarget);
  pTarget->DoDmg(1, DMG_Bio, pTarget, 0, EffectVar(2,pTarget,iEffectNumber), PCRB);
  EffectVar(0, pTarget, iEffectNumber)--;
  if(EffectVar(0, pTarget, iEffectNumber) <= 0)
    return(-1);
}


/* Geräusche */

public func JumpSound(){Sound("PCRB_Jump*.ogg");}
public func AttackSound(){Sound("PCRB_Attack*.ogg");}
public func AlertSound(){Sound("PCRB_Alert*.ogg");}
public func HurtSound(){Sound("PCRB_Hurt*.ogg");}
public func BiteSound(){Sound("PCRB_Bite*.ogg");}