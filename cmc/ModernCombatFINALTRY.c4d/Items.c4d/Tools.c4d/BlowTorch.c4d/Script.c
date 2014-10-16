/*-- Schweißbrenner --*/

#strict 2

local charge;

public func IsDrawable()		{return true;}		//Wird sichtbar getragen
public func HandSize()			{return 1000;}
public func HandX()			{return 4500;}

public func MaxEnergy()			{return 100;}
public func IsEquipment()		{return true;}
public func NoArenaRemove()		{return true;}
public func IsBlowTorch()		{return true;}
public func CustomQuickInventoryMenu()	{return QINV_ObjectMenu;}

public func StartSound()		{return Sound("BWTH_FireStart.ogg");}
public func StopSound()			{return Sound("BWTH_FireEnd.ogg");}
public func InUseSound(int iLoop)	{return Sound("BWTH_Fire.ogg", false, this, 100, 0, iLoop);}
public func StartRepairSound()		{return Sound("BWTH_RepairStart.ogg");}
public func StopRepairSound()		{return Sound("BWTH_RepairEnd.ogg");}
public func RepairingSound(int iLoop)	{return Sound("BWTH_Repair.ogg", false, this, 100, 0, iLoop);}
public func DamageSound()		{return Sound("SharpnelImpact*.ogg");}

local living_dmg_cooldown;


/* Initialisierung */

public func Initialize()
{
  charge = MaxEnergy(); //Schweißbrenner geladen
  living_dmg_cooldown = 4;
  AddEffect("ReparationBars", this, 1, 3, this);
}

/* Reparatureffekt */

public func FxReparationBarsStart(object target, int nr)
{
  EffectVar(0, target, nr) = [];
  EffectVar(1, target, nr) = false;
}

public func FxReparationBarsTimer(object target, int nr)
{
  var pCont = Contained(); var iPlr = GetOwner(pCont);

  if(!pCont || !pCont->~IsClonk())
  {
    if(EffectVar(1, target, nr))
    {
      for(var obj in EffectVar(0, target, nr))
        if(obj)
          RemoveObject(obj);
      EffectVar(1, target, nr) = false;
    }
    return false;
  }
  else if(Contents(0, pCont) != this) //Nur bei Anwahl
  {
    if(EffectVar(1, target, nr))
    {
      for(var obj in EffectVar(0, target, nr))
        if(obj)
          obj->Update(0, true);

      EffectVar(1, target, nr) = false;
    }

    return false;
  }

  EffectVar(1, target, nr) = true;

  for(var bar in EffectVar(0, target, nr))
  {
    if(!bar)
      continue;

    var actTarget = GetActionTarget(0, bar);

    if(!actTarget || Hostile(GetOwner(actTarget), iPlr) || !(actTarget->~IsRepairable()))
    {
      RemoveObject(bar);
      continue;
    }

    var dmg = GetDamage(actTarget);
    var max_dmg = actTarget->~MaxDamage();
    var percent = dmg * 100 / max_dmg;
    var deactivate = false;
    if(!percent)
      deactivate = true;

    percent = 100 - percent;

    bar->Update(percent, deactivate);
  }

  for(var obj in FindObjects(Find_Func("IsRepairable"), Find_Not(Find_Hostile(iPlr))))
  {
    if(FindObject2(Find_ID(SBAR), Find_ActionTarget(obj), Find_Owner(iPlr), Find_Func("HasBarType", BAR_Repairbar))) //Hat schon einen Balken?
      continue;

    var bar = CreateObject(SBAR, 0, 0, iPlr);
    bar->Set(obj, RGB(80,190,255), BAR_Repairbar, 0, 0, SM12);
    EffectVar(0, target, nr)[GetLength(EffectVar(0, target, nr))] = bar;
  }
}

public func FxReparationBarsStop(object target, int nr)
{
  for(var obj in EffectVar(0, target, nr))
    if(obj)
      RemoveObject(obj);

  return true;
}

/* Bereitschaft */

public func GetUser()
{
  return Contained();
}

public func Ready()
{
  return charge >= 10;
}

/* Tankregeneration */

public func Timer()
{
  //Nicht laden solange überhitzt
  if(GetEffect("Overheat", this))
    return;

  //Akku um einen Punkt aufladen
  if(!GetEffect("RepairObjects", this))
    if(charge > 0)
      charge = BoundBy(charge+3,0,MaxEnergy());
    else
    {
      Sound("BWTH_Charge.ogg",0,0,25);
      charge = BoundBy(charge+20,0,MaxEnergy());
    }

  return true;
}

/* Reparieren starten/beenden */

public func ControlUpdate(object pByObj, int comdir, bool dig, bool throw)
{
  if(throw && !GetEffect("RepairObjects", this))
    return ControlThrow(pByObj);
  else if(!throw && GetEffect("RepairObjects", this))
    return RemoveEffect("RepairObjects", this);
}

public func ControlThrow(pByObject)
{
  //Hinweis bei fehlendem Inhalt
  if(charge <= 0)
  {
    PlayerMessage(GetOwner(pByObject), "$NotCharged$", Contained(this), GSAM);
    Sound("WPN2_Empty.ogg");
    
    //Nicht starten wenn nicht geladen
    return true;
  }

  //Feuerbereitschaft?
  if(!(Contained()->~ReadyToFire()) || GetAction(Contained()) == "Crawl")
    return true;

  //Effekt (de)aktivieren
  var effect = GetEffect("RepairObjects", this);
  if(!effect)
  {
    AddEffect("RepairObjects", this, 101, 3, this, 0, pByObject);
  }
  else
  {
    RemoveEffect("RepairObjects", this);
  }
  return true;
}

public func Activate(pClonk)
{
  //Hinweis bei fehlendem Inhalt
  if(charge <= 0)
  {
    PlayerMessage(GetOwner(pClonk), "$NotCharged$", Contained(this),GSAM);
    Sound("WPN2_Empty.ogg");
    
    //Nicht starten wenn nicht geladen
    return false;
  }

  //Feuerbereitschaft?
  if(!(Contained()->~ReadyToFire()) || GetAction(pClonk) == "Crawl")
    return false;

  //Effekt (de)aktivieren
  var effect = GetEffect("RepairObjects", this);
  if(!effect)
  {
    AddEffect("RepairObjects", this, 101, 3, this, 0, pClonk);
  }
  else
  {
    RemoveEffect("RepairObjects", this);
  }
}

/* Reparatureffekt */

public func FxRepairObjectsStart(object target, int nr, temp, object pClonk)
{
  EffectVar(0, target, nr) = pClonk;
  EffectVar(1, target, nr) = pClonk->~IsRiding();
}

public func FxRepairObjectsTimer(object target, int nr, int time)
{
  if(charge <= 0)
  {
    //Überhitzen
    AddEffect("Overheat", this, 1, 100);
    Sound("BWTH_Overheat.ogg");
    return -1;
  }

  var clonk = EffectVar(0, target, nr);
  if(!clonk) return -1;

  if(clonk->~IsRiding() && !EffectVar(1, target, nr))
  {
    EffectVar(1, target, nr) = true;
    return -1;
  }

  EffectVar(1, target, nr) = clonk->~IsRiding();

  if(Contained() != clonk || !(clonk->~ReadyToFire()) || GetAction(clonk) == "Crawl")
    return -1;

  if(!(Contents(0, clonk) == this) || !(target->Use(clonk)))
    return -1;

  if(time < 4)
  {
    if(!used)
      StartSound();
    else
      StartRepairSound();

    temp_used = used;
    return true;
  }
  else if(time == 6)
  {
    if(!used)
      InUseSound(1);
    else
      RepairingSound(1);
  }

  if(used == temp_used)
    return true;

  temp_used = used;

  if(!used)
  {
    RepairingSound(-1);
    StopRepairSound();
    InUseSound(1);
  }
  else
  {
    InUseSound(-1);
    StopSound();
    RepairingSound(1);
  }
}

public func FxRepairObjectsStop(object target, int nr)
{
  InUseSound(-1);
  RepairingSound(-1);
  if(!used)
    StopSound();
  else
    StopRepairSound();

  return true;
}

local idle_energy_cnt, used, temp_used;
local iRepaired; //Teampunkte

public func Use(caller)
{
  //Eventuellen Cooldown verringern
  if(living_dmg_cooldown)
    living_dmg_cooldown--;

  used = false;

  //Angreifbare Objekte suchen
  var obj = caller->FindObject2(Find_Func("IsMeleeTarget", this),	//Angreifbar?
  			Find_AtRect(-10,-10,20,20));
  if(obj)
  {
    //Objekt beschädigen
    DoDmg(3, DMG_Fire, obj);

    used = true;
    charge = BoundBy(charge-1, 0, MaxEnergy());
  }
  //Entschärfbare Objekte suchen
  var obj = caller->FindObject2(Find_Func("IsDefusable"),		//Entschärfbar?
  			Find_Hostile(GetOwner(caller)),			//Feindlich?
  			Find_NoContainer(),				//Nicht verschachtelt?
  			Find_AtRect(-10,-10,20,20));
  if(obj)
  {
    if(obj->~RTDefuse(caller))
      //Punkte bei Belohnungssystem (Entschärfung)
      DoPlayerPoints(BonusPoints("TechnicalTask"), RWDS_TeamPoints, GetOwner(caller), caller, IC15);

    used = true;
    charge = BoundBy(charge-1, 0, MaxEnergy());
  }
  else
  {
    //Reparierbare Objekte suchen
    obj = caller->FindObject2(Find_Or(Find_And(Find_Func("IsRepairable"),				//Reparierbar?
    						Find_Or(
    						Find_Func("GetDamage"),					//Beschädigt?
    						Find_Hostile(GetOwner(caller)))),			//Feindlich?
    						Find_And(
    						Find_OCF(OCF_Alive),
    						Find_Hostile(GetOwner(caller)),
    						Find_NoContainer()),					//Nicht verschachtelt?
    						Find_Func("IsFakeRepairable", GetOwner(caller))),	//Konsolen?
    						Find_AtRect(-10,-10,20,20),
    						Find_Not(Find_Func("HitExclude", this)));
    if(obj)
    {
      //Konsolen reparieren / beschädigen
      if(obj->~IsFakeRepairable())
        obj = obj->GetRealRepairableObject();

      if(Hostile(GetOwner(obj), GetOwner(caller)) || HostileTeam(caller->~GetTeam(),obj->~GetTeam()))
      {
        if(obj->~IsRepairable())
          if(!obj->~IsNotAttackable())
          {
            //Feindliche Fahrzeuge beschädigen
            DoDmg(5, DMG_Fire, obj);

            used = true;
            charge = BoundBy(charge-1, 0, MaxEnergy());
          }
          else
          {}
          else
          {
            if(!living_dmg_cooldown)
            {
              //Feindliche Lebewesen verletzen
              DoDmg(12,DMG_Fire,obj);

              if(!GetAlive(obj) || IsFakeDeath(obj))
                //Achievement-Fortschritt (I'll fix you up?)
                DoAchievementProgress(1, AC32, GetOwner(GetUser()));

              living_dmg_cooldown = 7;
            }

          if(!Random(7))
            DamageSound();

          used = true;
          charge = BoundBy(charge - 1, 0, MaxEnergy());
        }
      }
      else
      {
        if(!obj->~RejectRepair())
        {
          //Fahrzeug reparieren
          DoDamage(-2, obj);

          if(!Hostile(GetOwner(obj), GetOwner(Contained())) && GetOwner(obj) != GetOwner(Contained()) && GetOwner(obj) != NO_OWNER)
          {
            //Achievement-Fortschritt (Wicked Engineer)
            DoAchievementProgress(2, AC33, GetOwner(Contained()));
          }

          //Callback
          if(GetDamage(obj) == 0)
            obj->~IsFullyRepaired();
          else
            obj->~OnRepairing(this);
        }

        if(!Hostile(GetOwner(obj), GetOwner(Contained())) && GetOwner(obj) != GetOwner(Contained()) && iRepaired++ >= 50)
        {
          //Punkte bei Belohnungssystem (Reparatur)
          DoPlayerPoints(BonusPoints("Repair"), RWDS_TeamPoints, GetOwner(Contained()), Contained(), IC15);
          iRepaired = 0;
        }

        used = true;
        charge = BoundBy(charge - 1, 0, MaxEnergy());
      }
    }
  }
  //Effekte
  var d = GetDir(Contained())-(!GetDir(Contained()));
  CreateParticle("RepairFlame", 10*d, -4, 5*d, Random(2)-2, 80, RGB(0,100,250));
  if(GetEffectData(EFSM_BulletEffects) >1)
    if(!Random(2))
      AddLightFlash(80, 10*d, -4, RGB(0,140,255));

  if(!used)
  {
    idle_energy_cnt++;
    if(idle_energy_cnt >= 2)
    {
      //Munitionsverbrauch im Leerlauf
      charge = BoundBy(charge - 1, 0, MaxEnergy());
      idle_energy_cnt = 0;
    }
  }
  else
  {
    //Effekte
    if(!Random(2))
      Sparks(8+Random(4), RGB(100,100,250), RandomX(-5, 5), RandomX(-5,5));
  }

  return true;
}

public func RejectEntrance(object pObj)
{
  if(GetOCF(pObj) & OCF_Living)
    return ContentsCount(GetID(),pObj);
}

public func Entrance(object pObj)
{
  SetOwner(GetOwner(pObj));
  return true;
}

/* HUD */

public func CustomHUD()		{return true;}
public func ColorEmpty()	{return RGB(255, 0, 0);}
public func ColorLow()		{return RGB(255, 150, 0);}
public func MinValue()		{return 0;}
public func MinValue2()		{return 49;}

func UpdateHUD(object pHUD)
{
  var color = ColorEmpty()*(Inside(charge, 0, MinValue()));
  if(!color)
    color = ColorLow()*(Inside(charge, MinValue()+1, MinValue2()));
  pHUD->~Ammo(charge, MaxEnergy(), GetName(), true, color);
}

/* Allgemein */

protected func Hit()
{
  Sound("WPN2_Hit*.ogg");
}

protected func Selection()
{
  Sound("BWTH_Charge.ogg");
}

/* KI */

local ai_ignore;

public func HasBotSupport(object pBot, object pTarget)
{
  if(ai_ignore && charge >= 35)
    ai_ignore = false;

  if(ai_ignore)
  return false;

  if(charge <= 10)
  {
    ai_ignore = true;
    return false;
  }

  if(GetY(pBot)-80 > GetY(pTarget))
    return false;

  return true;
}

public func BotControl(object pBot, object pTarget, int iLevel, bool fAggroFire)
{
  if(!pBot || !pTarget || !fAggroFire)
    return false;

  if(!GetCommand(pBot))
    SetCommand(pBot, "MoveTo", pTarget);

  if(FindObject2(Find_Not(Find_Exclude(pBot)), Find_AtRect(-10,-10,20,20))) //Nah genug?
  {
    if(!GetEffect("RepairObjects", this))
      ControlThrow(pBot);
  }
  else if(GetEffect("RepairObjects", this))
    RemoveEffect("RepairObjects", this);

  return true;
}

local ai_counter;

public func AI_IdleEquipment(object pBot)
{
  if(ai_ignore && charge >= 35)
    ai_ignore = false;

  if(ai_ignore)
    return false;

  if(charge <= 10)
  {
    ai_ignore = true;
    return false;
  }

  ai_counter = (ai_counter + 1) % 5;
  if(ai_counter)
    return GetCommand(pBot, 1);

  var object;
  if(!(object = GetCommand(pBot, 1)))
  {
    object = FindObject2(Find_Or(Find_And(Find_Func("IsRepairable"), Find_Func("GetDamage")), Find_Func("IsFakeRepairable", GetOwner(pBot))), Find_Distance(150));
    if(!object)
    {
      if(GetEffect("RepairObjects", this))
        RemoveEffect("RepairObjects", this);

      return false;
    }

    if(Contents(0, pBot) != this)
      ShiftContents(pBot, 0, GetID(), true);

    SetCommand(pBot, "MoveTo", object);
  }

  if(FindObject2(Find_Not(Find_Exclude(object)), Find_AtRect(-10,-10,20,20)))
  {
    if(!GetEffect("RepairObjects", this))
      ControlThrow(pBot);
  }
  else if(GetEffect("RepairObjects", this))
    RemoveEffect("RepairObjects", this);
  
  return true;
}

public func AI_TargetLost()
{
  if(GetEffect("RepairObjects", this))
    RemoveEffect("RepairObjects", this);

  return true;
}