/*-- Schweißbrenner --*/

#strict 2

local charge;

public func HandSize()			{return 1000;}
public func HandX()			{return 4500;}
public func IsDrawable()		{return true;}
public func MaxEnergy()			{return 100;}
func IsEquipment()			{return true;}
public func NoArenaRemove()		{return true;}

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
  AddEffect("ReparationBars", this, 101, 8, this);
}

/* Reparatureffekt */

public func FxReparationBarsStart(object target, int nr)
{
  EffectVar(0, target, nr) = [];
  EffectVar(1, target, nr) = false;
}

public func FxReparationBarsTimer(object target, int nr)
{
  if(!Contained() || !(Contained()->GetOCF() & OCF_Alive))
  {
    if(GetLength(EffectVar(0, target, nr)) > 0)
    {
      for(var obj in EffectVar(0, target, nr))
      {
        if(obj)
          RemoveObject(obj);
      }
      SetLength(EffectVar(0, target, nr), 0);
    }
    return true;
  }

  if(Contents(0, Contained()) != this) //Nur bei Anwahl
  {
    if(EffectVar(1, target, nr))
    {
      for(var obj in FindObjects(Find_Func("IsBar"), Find_Owner(GetOwner(Contained()))))
        if(obj->GetBarType() == BAR_Repairbar)
          obj->Update(0, true);

      EffectVar(1, target, nr) = false;
    }
    return false;
  }

  EffectVar(1, target, nr) = true;

  for(var obj in FindObjects(Find_Func("IsRepairable")))
  {
    var bar;
    if(bar = FindObject2(Find_ID(SBAR), Find_ActionTarget(obj)))
    {
      if(Hostile(GetOwner(obj), GetOwner(Contained())))
      {
        RemoveObject(bar);
        continue;
      }
      var dmg = GetDamage(obj);
      var max_dmg = obj->~MaxDamage();
      var percent = dmg * 100 / max_dmg;
      var deactivate = false;
      if(!percent)
        deactivate = true;

      percent = 100 - percent;

      bar->Update(percent, deactivate);
    }
    else if(!Hostile(GetOwner(obj), GetOwner(Contained())))
    {
      bar = CreateObject(SBAR, 0, 0, GetOwner(Contained()));
      bar->Set(obj, RGB(255,0,0), BAR_Repairbar, true, "", IC15);

      var dmg = GetDamage(obj);
      var max_dmg = obj->~MaxDamage();
      var percent = dmg * 100 / max_dmg;
      var deactivate = false;
      if(!percent)
        deactivate = true;

      percent = 100 - percent;

      bar->Update(percent, deactivate);

      EffectVar(0, target, nr)[GetLength(EffectVar(0, target, nr))] = bar;
    }
  }
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
  //Akku um einen Punkt aufladen
  if(!GetEffect("RepairObjects", this))
    charge = BoundBy(charge+3,0,MaxEnergy());

  return true;
}

/* Reparieren starten/beenden */

public func ControlThrow(pByObject)
{
  //Hinweis bei fehlendem Inhalt
  if(charge <= 0)
  {
    PlayerMessage(GetOwner(pByObject), "$NotCharged$", Contained(this),GSAM);
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
  if(!(Contained()->~ReadyToFire()) || GetAction(clonk) == "Crawl")
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

public func FxRepairObjectsStart(object target, int nr, temp, object pClonk)
{
  EffectVar(0, target, nr) = pClonk;
}

public func FxRepairObjectsTimer(object target, int nr, int time)
{
  if(charge <= 0)
    return -1;

  var clonk = EffectVar(0, target, nr);
  if(Contained() != clonk)
    return -1;

  if(!(clonk->~ReadyToFire()) || GetAction(clonk) == "Crawl")
    return -1;

  if(!(Contents(0, clonk) == this))
    return -1;

  if(!(target->Use(clonk)))
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
local iRepaired; // Teampoints

public func Use(caller)
{
  if(living_dmg_cooldown)
    living_dmg_cooldown--;

  used = false;

  //Objekte suchen, die repariert werden können
  var obj = caller->FindObject2(Find_Or(Find_And(Find_Func("IsRepairable"), Find_Or(Find_Func("GetDamage"), Find_Hostile(GetOwner(caller)))), Find_And(Find_OCF(OCF_Alive), Find_Hostile(GetOwner(caller)), Find_NoContainer())), Find_AtPoint());
  if(obj)
  {
    if(Hostile(GetOwner(obj), GetOwner(caller)))
    {
      if(obj->~IsRepairable()) //Fahrzeuge
      {
        //Fahrzeug beschädigen
        DoDmg(5,DMG_Fire,obj);

        used = true;
        charge = BoundBy(charge - 1, 0, MaxEnergy());
      }
      else //Lebewesen
      {
        if(!living_dmg_cooldown)
        {
          //Lebewesen verletzen
          DoDmg(10,DMG_Fire,obj);
          living_dmg_cooldown = 8;
        }

        if(!Random(7))
          DamageSound();

        used = true;
        charge = BoundBy(charge - 1, 0, MaxEnergy());
      }
    }
    else
    {
      //Fahrzeug reparieren
      DoDamage(-2, obj);

      if(GetDamage(obj) == 0)
        obj->~IsFullyRepaired();

      if(GetPlayerTeam(GetOwner(obj)) == GetPlayerTeam(GetOwner(Contained())) && iRepaired++ >= 40)
      {
        //Punkte bei Belohnungssystem (Reparatur)
        DoPlayerPoints(10, RWDS_TeamPoints, GetOwner(Contained()), Contained(), IC15);
        //DoPlayerPoints(Repair(), RWDS_TeamPoints, GetOwner(Contained()), Contained(), IC15);
        iRepaired = 0;
      }

      used = true;
      charge = BoundBy(charge - 1, 0, MaxEnergy());
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
    /*RepairingSound(-1);
    InUseSound(1);*/
    idle_energy_cnt++;
    if(idle_energy_cnt >= 6)
    {
      charge = BoundBy(charge - 1, 0, MaxEnergy());
      idle_energy_cnt = 0;
    }
  }
  else
  {
    //Effekte
    if(!Random(2))
      Sparks(8+Random(4), RGB(100,100,250), RandomX(-5, 5), RandomX(-5,5));

    /*InUseSound(-1);
    RepairingSound(1);*/
    //Sound("Repair.ogg",false,this,50,0,+1); 
  }

  return true;
}

public func RejectEntrance(object pObj)
{
  if(GetOCF(pObj) & OCF_Living)
    return ContentsCount(GetID(),pObj);
}

/* HUD */

func CustomHUD()	{return true;}

func UpdateHUD(object pHUD)
{
  var color = RGB(255, 255, 255);
  pHUD->Ammo(charge, MaxEnergy(), GetName(), true, color);
  //pHUD->Recharge(charge, MaxEnergy());
}

/* Allgemein */

protected func Hit()
{
  Sound("WPN2_Hit*.ogg");
}