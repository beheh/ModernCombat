/*-- Dragnin --*/

#strict

public func HandSize()		{return(1000);}
public func HandX()		{return(4500);}
public func HandY()		{return(0);}
public func IsDrawable()	{return(true);}
public func IsEquipment()	{return(true);}
public func NoArenaRemove()	{return(true);}


/* Stich setzen */

public func ControlThrow(pByObject)
{
  Sting(pByObject);
  return(true);
}

func Sting(caller)
{
  //Patienten suchen
  var obj;
  if(obj = FindObject2(Find_InRect(-10,-10,20,20),Find_OCF(OCF_Alive),Find_Exclude(caller),Find_Allied(GetOwner(caller)),Find_NoContainer()))
  {
 
   //Bereits anderweitig am heilen?
   if(GetEffect("*Heal*",obj))
   {
    PlayerMessage(GetOwner(caller), "$AlreadyHealing$",caller);
    return(1);
   }
   //Nicht verwundet?
   if(GetEnergy(obj) == GetPhysical("Energy",0, obj)/1000)
   {
    PlayerMessage(GetOwner(caller), "$NotWounded$",caller);
    return(1);
   }

   //Heileffekt geben
   AddEffect("DragninHeal",obj,20,1,0,GetID(),HealAmount(),HealRate());
   Sound("DGNN_Use.ogg");
   RemoveObject();

  }
  else
  {
   //Ins Leere stechen
   Sound("GrenadeThrow*.ogg");
  }
  return(1);
}

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
  EffectVar(2, pTarget, iEffectNumber) = GetPhysical("Walk", 2, pTarget);
  EffectVar(3, pTarget, iEffectNumber) = GetPhysical("Jump", 2, pTarget);
  EffectVar(4, pTarget, iEffectNumber) = GetPhysical("Scale", 2, pTarget);
  EffectVar(5, pTarget, iEffectNumber) = GetPhysical("Hangle", 2, pTarget);

  SetPhysical("Walk", GetPhysical("Walk", 0, pTarget)/3, 2, pTarget);
  SetPhysical("Jump", GetPhysical("Jump", 0, pTarget)/3, 2, pTarget);
  SetPhysical("Scale", GetPhysical("Scale", 0, pTarget)/3, 2, pTarget);
  SetPhysical("Hangle", GetPhysical("Hangle", 0, pTarget)/3, 2, pTarget);
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

func FxDragninHealDamage(target, no, dmg, dmgtype)
{
  //Bei Schaden abbrechen
  if(dmg < 0)
    RemoveEffect(0,target,no);
  else
    return(dmg);
}

public func FxDragninHealStop(target, no, reason, temp)
{
  //Lähmung rückgängig machen
  SetPhysical("Walk", EffectVar(2, target, no), 2, target);
  SetPhysical("Jump", EffectVar(3, target, no), 2, target);
  SetPhysical("Scale", EffectVar(4, target, no), 2, target);
  SetPhysical("Hangle", EffectVar(5, target, no), 2, target);
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
