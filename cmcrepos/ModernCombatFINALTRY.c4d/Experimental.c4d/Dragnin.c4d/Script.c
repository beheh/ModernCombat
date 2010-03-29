/*-- Dragnin --*/

#strict

public func IsEquipment() {return(true);}


/* Aktivierung */

func Activate(object pByObj)
{
  //Bereits anderweitig am heilen?
  if(GetEffect("*Heal*",pByObj))
  {
    PlayerMessage(GetOwner(pByObj), "$AlreadyHealing$",pByObj);
    return(1);
  }
  //Nicht verwundet?
  if(GetEnergy(pByObj) == GetPhysical("Energy",0, pByObj)/1000)
  {
   PlayerMessage(GetOwner(pByObj), "$NotWounded$",pByObj);
   return(1);
  }

  //Heileffekt geben
  AddEffect("DragninHeal",pByObj,20,1,0,GetID(),HealAmount(),HealRate());
  Sound("DGNN_Use.ogg");
  RemoveObject();
  return(1);
}

/* Dragnineffekt */

func HealRate()		{return(2);}
func HealAmount()	{return(40);}

func FxDragninHealStart(object pTarget, int iEffectNumber, int iTemp, int iHealAmount, int iHealRate)
{
  if(iTemp)
   if(GetPhysical("Walk", 2, pTarget) <= GetPhysical("Walk", 1, 0, GetID(pTarget))*5/10)
    return();

  EffectVar(0,pTarget,iEffectNumber) = iHealAmount; //Heilung insgesammt
  EffectVar(1,pTarget,iEffectNumber) = iHealRate; //Frames per HP

  //Lähmung
  EffectVar(2, pTarget, iEffectNumber) = GetPhysical("Walk", 1, 0, GetID(pTarget))/5;
  EffectVar(3, pTarget, iEffectNumber) = GetPhysical("Jump", 1, 0, GetID(pTarget))/5;
  EffectVar(4, pTarget, iEffectNumber) = GetPhysical("Scale", 1, 0, GetID(pTarget))/5;
  EffectVar(5, pTarget, iEffectNumber) = GetPhysical("Hangle", 1, 0, GetID(pTarget))/5;

  SetPhysical("Walk", GetPhysical("Walk", 0, pTarget)-EffectVar(2, pTarget, iEffectNumber), 2, pTarget);
  SetPhysical("Jump", GetPhysical("Jump", 0, pTarget)-EffectVar(3, pTarget, iEffectNumber), 2, pTarget);
  SetPhysical("Scale", GetPhysical("Scale", 0, pTarget)-EffectVar(4, pTarget, iEffectNumber), 2, pTarget);
  SetPhysical("Hangle", GetPhysical("Hangle", 0, pTarget)-EffectVar(5, pTarget, iEffectNumber), 2, pTarget);
}

func FxDragninHealTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  //Effekt
  pTarget->CreateParticle("ShockWave",0,0,Random(10),Random(10),5*GetObjHeight(pTarget)+25+Sin(iEffectTime*5,35),RGB(220,20,150),pTarget);
  //Heilen
  if(!(iEffectTime % EffectVar(1, pTarget, iEffectNumber)))
  {
   DoEnergy(1, pTarget);
   EffectVar(0,pTarget,iEffectNumber)--;
  }
  //Schon voll geheilt?
  if(GetEnergy(pTarget) >= GetPhysical("Energy",0,pTarget)/1000)
  {
   return(-1);
  }
  //Schon leer?
  if(!EffectVar(0,pTarget,iEffectNumber))
  {
   return(-1);
  }
}

public func FxDragninHealStop(target, no, reason, temp)
{
  //Lähmung rückgängig machen
  SetPhysical("Walk", GetPhysical("Walk", 0, target)+EffectVar(2, target, no), 2, target);
  SetPhysical("Jump", GetPhysical("Jump", 0, target)+EffectVar(3, target, no), 2, target);
  SetPhysical("Scale", GetPhysical("Scale", 0, target)+EffectVar(4, target, no), 2, target);
  SetPhysical("Hangle", GetPhysical("Hangle", 0, target)+EffectVar(5, target, no), 2, target);
}

/* Sounds */

protected func Hit()
{
  Sound("MetalHit*");
}

func Selection()
{
  Sound("DGNN_Charge.ogg");
}