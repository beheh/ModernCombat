/*-- Dragnin --*/

#strict 2

public func IsDrawable()		{return true;}			//Wird sichtbar getragen
public func HandSize()			{return 1000;}
public func HandX()			{return 4500;}
public func HandY()			{}

public func IsEquipment()		{return true;}
public func NoArenaRemove()		{return true;}
public func LimitationCount()		{return 3;}
public func CustomQuickInventoryMenu()	{return QINV_ObjectMenu;}


/* Initialisierung */

protected func Initialize()
{
  CheckLimitation();
}

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

    if(!Hostile(GetOwner(),GetOwner(obj)) && GetOwner() != GetOwner(obj))
    {
      //Punkte bei Belohnungssystem (Heilung)
      DoPlayerPoints(BonusPoints("Healing", HealAmount()), RWDS_TeamPoints, GetOwner(caller), caller, IC05);
      //Achievement-Fortschritt (I'll fix you up!)
      DoAchievementProgress(BonusPoints("Dragnin"), AC02, GetOwner(caller));
    }

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
  ScreenRGB(pByObj,RGBa(0,230,255,190),50,5,50, SR4K_LayerMedicament);
  Sound("DGNN_Use.ogg");
  RemoveObject();
  return 1;
}

public func ControlDigDouble(object pByObj)
{
  return Activate(pByObj);
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

/* Aufnahme */

protected func Entrance()
{
  //Rotation zurücksetzen
  SetR();
}

/* Dragnineffekt */

func HealRate()		{return 2;}
func HealAmount()	{return 40;}

func FxDragninHealStart(object pTarget, int iEffectNumber, int iTemp, int iHealAmount, int iHealRate)
{
  if(iTemp)
    if(GetPhysical("Walk", 2, pTarget) <= GetPhysical("Walk", 1, 0, GetID(pTarget))*5/10)
      return;

  EffectVar(0,pTarget,iEffectNumber) = iHealAmount;	//Heilung insgesamt
  EffectVar(1,pTarget,iEffectNumber) = iHealRate;	//Frames per HP

  //Lähmung
  SetPhysical("Walk", GetPhysical("Walk", 2, pTarget)/3, 3, pTarget);
  SetPhysical("Jump", GetPhysical("Jump", 2, pTarget)/3, 3, pTarget);
  SetPhysical("Scale", GetPhysical("Scale", 2, pTarget)/3, 3, pTarget);
  SetPhysical("Hangle", GetPhysical("Hangle", 2, pTarget)/3, 3, pTarget);
  SetPhysical("Swim", GetPhysical("Swim", 2, pTarget)/3, 3, pTarget);
  SetPhysical("Dig", GetPhysical("Dig", 2, pTarget)/3, 3, pTarget);
  ScreenRGB(pTarget,RGBa(0, 230, 255, 190), 80, 3,false, SR4K_LayerMedicament, 200);
}

func FxDragninHealTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  //Effekt
  pTarget->CreateParticle("ShockWave",0,0,Random(10),Random(10),5*GetObjHeight(pTarget)+25+Sin(iEffectTime*5,35),RGB(0,230,255),pTarget);
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
  //Bildschirm Effekt
  if(!(iEffectTime % 20))
  {
    ScreenRGB(pTarget,RGBa(0, 230, 255, 190), 80, 3,false, SR4K_LayerMedicament, 200);
  }
}

func FxDragninHealDamage(target, no, dmg, dmgtype)
{
  //Bei Schaden abbrechen
  if(dmg < 0) RemoveEffect(0,target,no);

  return dmg;
}

public func FxDragninHealStop(object pTarget, no, reason, temp)
{
  if(temp) return;

  //Lähmung rückgängig machen
  ResetPhysical(pTarget, "Walk");
  ResetPhysical(pTarget, "Jump");
  ResetPhysical(pTarget, "Scale");
  ResetPhysical(pTarget, "Hangle");
  ResetPhysical(pTarget, "Swim");
  ResetPhysical(pTarget, "Dig");
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

/* KI-Behandlung */

protected func AI_IdleInventory(object pClonk)	{return AI_Inventory(pClonk);}

protected func AI_Inventory(object pClonk)
{
  //Benutzen, wenn der Clonk weniger als 2/3 Leben hat
  if(!pClonk->~IsHealing() && GetEnergy(pClonk) < GetPhysical("Energy", PHYS_Current, pClonk) * 2/3 / 1000)
  {
    ShiftContents(pClonk, 0, GetID());
    //Benutzen (verzögert einsetzen)
    ScheduleCall(this, "Activate", 1, 0, pClonk);
    return 2;
  }
  return 1;
}