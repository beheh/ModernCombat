/*-- Dragnin --*/

#strict 2

public func HandSize()		{return 1000;}
public func HandX()		{return 4500;}
public func HandY()		{}
public func IsDrawable()	{return true;}
public func IsEquipment()	{return true;}
public func NoArenaRemove()	{return true;}


/* Stich setzen */

public func ControlThrow(pByObject)
{
  Sting(pByObject);
  return true;
}

func Sting(caller)
{
  //Patienten suchen
  var obj;
  if(obj = FindObject2(Find_InRect(-10,-10,20,20),Find_OCF(OCF_CrewMember),Find_Exclude(caller),Find_Allied(GetOwner(caller)),Find_NoContainer()))
  {
   //Bereits anderweitig am heilen?
   if(GetEffect("*Heal*",obj))
   {
     PlayerMessage(GetOwner(caller), "$AlreadyHealing$",caller);
     return 1;
   }
   //Nicht verwundet?
   if(GetEnergy(obj) == GetPhysical("Energy",0, obj)/1000)
   {
     PlayerMessage(GetOwner(caller), "$NotWounded$",caller);
     return 1;
   }

   //Heileffekt geben
   AddEffect("DragninHeal",obj,20,1,0,GetID(),HealAmount(),HealRate());
   Sound("DGNN_Use.ogg");

   //Punkte bei Belohnungssystem
   DoPlayerPoints(DragninPoints(), RWDS_TeamPoints, GetOwner(caller), caller, IC05);
   DoAchievementProgress(DragninPoints(), AC02, GetOwner(caller));

   RemoveObject();
  }
  else
  {
   //Ins Leere stechen
   Sound("GrenadeThrow*.ogg");
  }
  return 1;
}

/* Aktivierung */

func Activate(object pByObj)
{
  //Bereits anderweitig am heilen?
  if(GetEffect("*Heal*",pByObj))
  {
    PlayerMessage(GetOwner(pByObj), "$AlreadyHealing$",pByObj);
    return 1;
  }
  //Nicht verwundet?
  if(GetEnergy(pByObj) == GetPhysical("Energy",0, pByObj)/1000)
  {
    PlayerMessage(GetOwner(pByObj), "$NotWounded$",pByObj);
    return 1;
  }

  //Heileffekt geben
  AddEffect("DragninHeal",pByObj,20,1,0,GetID(),HealAmount(),HealRate());
  Sound("DGNN_Use.ogg");
  RemoveObject();
  return 1;
}

public func RejectEntrance(object pObj)
{
  if(GetOCF(pObj) & OCF_Living)
  {
   if(ContentsCount(GetID(),pObj))
    return true;
  }
  return false;
}

/* Dragnineffekt */

func HealRate()		{return 2;}
func HealAmount()	{return 40;}

func FxDragninHealStart(object pTarget, int iEffectNumber, int iTemp, int iHealAmount, int iHealRate)
{
  if(iTemp)
    if(GetPhysical("Walk", 2, pTarget) <= GetPhysical("Walk", 1, 0, GetID(pTarget))*5/10)
      return;

  EffectVar(0,pTarget,iEffectNumber) = iHealAmount; //Heilung insgesammt
  EffectVar(1,pTarget,iEffectNumber) = iHealRate; //Frames per HP

  //L�hmung
  SetPhysical("Walk", GetPhysical("Walk", 2, pTarget)/3, 3, pTarget);
  SetPhysical("Jump", GetPhysical("Jump", 2, pTarget)/3, 3, pTarget);
  SetPhysical("Scale", GetPhysical("Scale", 2, pTarget)/3, 3, pTarget);
  SetPhysical("Hangle", GetPhysical("Hangle", 2, pTarget)/3, 3, pTarget);
  SetPhysical("Swim", GetPhysical("Swim", 2, pTarget)/3, 3, pTarget);
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
    return -1;
  }
  //Schon leer?
  if(!EffectVar(0,pTarget,iEffectNumber))
  {
    return -1;
  }
  //FakeDeath?
  if(Contained(pTarget) && GetID(Contained(pTarget)) == FKDT)
  {
    return -1;
  }
}

func FxDragninHealDamage(target, no, dmg, dmgtype)
{
  //Bei Schaden abbrechen
  if(dmg < 0)
    RemoveEffect(0,target,no);
  else
    return dmg;
}

public func FxDragninHealStop(object pTarget, no, reason, temp)
{
  //L�hmung r�ckg�ngig machen
  ResetPhysical(pTarget, "Walk");
  ResetPhysical(pTarget, "Jump");
  ResetPhysical(pTarget, "Scale");
  ResetPhysical(pTarget, "Hangle");
  ResetPhysical(pTarget, "Swim");
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
