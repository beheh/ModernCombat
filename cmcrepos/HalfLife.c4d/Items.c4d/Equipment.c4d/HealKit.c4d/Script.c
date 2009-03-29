#strict

public func HealAmount() { return(35); }

public func RejectEntrance(object pObj)
{
  if(pObj->GetAlive() && (GetEnergy(pObj) < GetPhysical("Energy",0,pObj)/1000))
  {
    AddEffect("H2HK_Heal",pObj,200,1,0,GetID(),HealAmount());
    Sound("H2HK_Heal.ogg");
    RemoveObject();
  }
  return(1);
}

public func Hit()
{
  Sound("WoodHit*");
}

func FxH2HK_HealStart(object pTarget, int iEffectNumber, int iTemp, int iHealAmount)
{
  if(iTemp)
    return();

  EffectVar(0,pTarget,iEffectNumber) = iHealAmount;
  ScreenRGB(pTarget,RGBa(255,255,0,192),0,0,iHealAmount,true);
}

func FxH2HK_HealTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  /*var width= GetDefCoreVal("Width",0,GetID(pTarget)),
      height = GetDefCoreVal("Height",0,GetID(pTarget));
  width -= width/3;
  height -= height/3;
  width /= 2;
  height /= 2;
  CreateParticle("PxSpark",GetX(pTarget)+RandomX(-width, width),GetY(pTarget)+RandomX(-height,height),0,-10,Random(25)+10,RGBa(255,0,0,50));*/
  
  DoEnergy(1,pTarget);
  EffectVar(0,pTarget,iEffectNumber)--;
  
  if(GetEnergy(pTarget) >= GetPhysical("Energy",0,pTarget)/1000)
    return(-1);

  if(!EffectVar(0,pTarget,iEffectNumber)) 
    return(-1);
}

func FxH2HK_HealStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  if(fTemp) return();
}