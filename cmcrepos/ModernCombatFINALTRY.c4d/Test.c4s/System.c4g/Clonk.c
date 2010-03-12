#strict
#appendto CLNK


protected func Initialize()
{
  _inherited();
  if(IsClonk() && (GetOwner() != NO_OWNER) && (GetPlayerType(GetOwner()) != C4PT_Script))
    AddEffect("FakeDeath",this(),10,0,this());
}

public func CalcPain(){}//Kein Schmerz-System.


global func FxFakeDeathDamage(object pTarget, int iEffectNumber, int iDmgEngy, int iCause)
{
  if(!ObjectCount(NOFD) && !IsFakeDeath(pTarget))
  {
    if(GetEnergy(pTarget) <= -iDmgEngy/1000)
    {
      FakeDeath(pTarget);
      return(0);
    }
  }
  
  return(iDmgEngy);
}




func Death()
{
  if(IsFakeDeath())
  {
    SetPhase(5);//animation unterdrücken (GetPhase() blah)
    Sound("Death");
  }
  else
  {
    Sound("ClonkDie*");
  }
  
  FadeOut(this());

  RemoveObject();
  //return(_inherited(...));//Haha ... tolle Funktion.
}


global func FakeDeath(object pTarget)
{
  if(!pTarget) pTarget = this();
  if(!pTarget) return(false);
  if(!pTarget->IsClonk()) return(false);
  
  var fake = CreateObject(FKDT,0,0,GetOwner(pTarget));
  fake->Set(pTarget);

  SetComDir(COMD_Stop,pTarget);
  pTarget->Sound("ClonkDie*");
  if(!ObjectSetAction(pTarget,"Death",0,0,1))
    pTarget->SetAction("Dead");
    
  return(true);
}

global func StopFakeDeath(object pTarget)
{
  if(!pTarget) pTarget = this();
  if(GetID(pTarget) == FKDT)
    pTarget = pTarget->GetClonk();
  if(!pTarget) return(false);
  if(!pTarget->IsClonk()) return(false);
  
  Sound("ClonkCough*",0,pTarget);
  ObjectSetAction(pTarget,"FlatUp",0,0,1);
  RemoveObject(pTarget->Contained(),true);
    
  return(true);
}


global func IsFakeDeath(object pTarget)
{
  if(!pTarget) pTarget = this();
  if(!pTarget) return(false);
  //if(!pTarget->~IsClonk()) return(false);
  
  return(GetID(pTarget->Contained()) == FKDT);
}
