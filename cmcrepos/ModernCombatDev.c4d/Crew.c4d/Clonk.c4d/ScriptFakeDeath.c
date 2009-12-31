global func FxDmgCheckDamage(object pTarget, int iEffectNumber, int iDmgEngy, int iCause)
{
  if(iDmgEngy > 0) return iDmgEngy;
  
  var type;
  //Explosion
  if(iCause == 1 || iCause == 33)
    type = DMG_Explosion;
  //Feuer
  else if(iCause == 2 || iCause == 35)
    type = DMG_Fire;
  //Säure (Bio)
  else if(iCause == 38)
    type = DMG_Bio;
  //Melee = Clonk zu Clonk Kampf
  else if(iCause == 34 || iCause == 40)
    type = DMG_Melee;
    
  //Ziel sagen, welcher Schadenstyp gemacht wurde
  if(type)
  {
    pTarget->~LastDamageType(type);
    
    var red = pTarget->~OnDmg(iDmgEngy/100,type); 
    //reduzierten Schaden berechnen
    var dmg;
    dmg = (100-red)*iDmgEngy;
    iDmgEngy = dmg/100;
  }
  
  //Hier setzt CMC an:
  if(pTarget->~IsClonk() && !ObjectCount(NOFD) && !IsFakeDeath(pTarget))
  {
    if(type)
      pTarget->HurtSounds(iDmgEngy,type);

    if(GetEnergy(pTarget) <= -iDmgEngy/1000)
    {
      FakeDeath(pTarget);
      return 0;//Nain... der - ist - doch - tot.... echt mal!
    }
  }
  
  return iDmgEngy;
}

/* Faketot */
func FakeDeathHP(){return 20;}

func Death()
{
  if(IsFakeDeath())
  {
    SetPhase(5);//animation unterdrücken (GetPhase() blah)
    Sound("Death");
  }
  else
  {
    Sound("ClonkDie*.ogg");
  }
  
  FadeOut(this);
  
  return _inherited(...);//Haha ... tolle Funktion.
}

public func FxFakeDeathStart(pTarget,iEffectNumber)
{
  SetComDir(COMD_Stop,pTarget);
  pTarget->Sound("ClonkDie*.ogg");
  if(!ObjectSetAction(pTarget,"Death",0,0,1))
    pTarget->SetAction("Dead");
  DoEnergy(FakeDeathHP(),pTarget);//O.o 
  
  var pHelper = pTarget->CreateObject(FKDT,0,0,GetController(pTarget));
  pHelper->Set(pTarget);
  EffectVar(1,pTarget,iEffectNumber) = pHelper;
  
  EffectCall(pTarget,iEffectNumber,"Unpause");
}

public func FxFakeDeathTimer(pTarget,iEffectNumber,iEffectTime)
{
  DoEnergy(EffectVar(0,pTarget,iEffectNumber),pTarget);//Sei Schmerz dein Lehrmeister!
}

public func FxFakeDeathPause(object pTarget, int iEffectNumber)
{
  EffectVar(0,pTarget,iEffectNumber) = 0;
}

public func FxFakeDeathUnpause(object pTarget, int iEffectNumber)
{
  EffectVar(0,pTarget,iEffectNumber) = -1;
}

public func FxFakeDeathStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  if(EffectVar(1,pTarget,iEffectNumber))
    RemoveObject(EffectVar(1,pTarget,iEffectNumber));
  
  if(iReason == 4)
    FadeOut(pTarget);//tot :O
    
  if(iReason == 0)
    ObjectSetAction(pTarget,"FlatUp",0,0,1);
    
  return 1;
}

global func PauseFakeDeath(bool fPause, object pTarget)
{
  if(!pTarget) pTarget = this;
  if(GetID(pTarget) == FKDT)
    pTarget = pTarget->Contents();
  if(!pTarget) return false;
  if(!pTarget->IsClonk()) return false;
  
  var effect = GetEffect("FakeDeath",pTarget);
  if(effect)
  {
    if(fPause)
      EffectCall(pTarget,effect,"Pause");
    else
      EffectCall(pTarget,effect,"Unpause");
  }
    
  return effect;
}

global func FakeDeath(object pTarget)
{
  if(!pTarget) pTarget = this;
  if(!pTarget) return false;
  if(!pTarget->IsClonk()) return false;
  
  var effect = GetEffect("FakeDeath",pTarget);
  if(!effect)
    effect = AddEffect("FakeDeath",pTarget,100,35,pTarget,GetID(pTarget));
    
  return effect;
}

global func StopFakeDeath(object pTarget)
{
  if(!pTarget) pTarget = this;
  if(GetID(pTarget) == FKDT)
    pTarget = pTarget->GetClonk();
  if(!pTarget) return false;
  if(!pTarget->IsClonk()) return false;
  
  Sound("ClonkCough*.ogg",0,pTarget);
  
  while(GetEffect("FakeDeath",pTarget))
    RemoveEffect("FakeDeath",pTarget);
    
  return true;
}

global func IsFakeDeath(object pTarget)
{
  if(!pTarget) pTarget = this;
  if(!pTarget) return false;
  if(!pTarget->~IsClonk()) return false;
  
  return GetEffect("FakeDeath",pTarget);
}
