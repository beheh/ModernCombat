/*-- Neues Script --*/

#strict 2

#appendto BWTH

local pStation;

public func SetStation(object pObj) { pStation = pObj; }
public func GetStation() { return pStation; } 

public func Use(caller)
{
  //Eventuellen Cooldown verringern
  if(living_dmg_cooldown)
    living_dmg_cooldown--;

  used = false;

  //Entschärfbare Objekte suchen
  var obj = caller->FindObject2(Find_Func("IsDefusable"),		//Entschärfbar?
  			Find_Hostile(GetOwner(caller)),			//Feindlich?
  			Find_AtRect(-10,-10,20,20));
  if(obj)
  {
    if(obj->~RTDefuse(caller))
      //Punkte bei Belohnungssystem (Entschärfung)
      DoPlayerPoints(BonusPoints("TechnicalTask"), RWDS_TeamPoints, GetOwner(caller), caller, IC15);

    used = true;
  }
  else
  {
    //Reparierbare Objekte suchen
    obj = FindObject2(Find_Or(Find_And(Find_Func("IsRepairable"),	//Reparierbar?
    					Find_Or(Find_Func("GetDamage"),		//Beschädigt?
    					Find_Hostile(GetOwner(caller)))),	//Feindlich?
    					Find_And(Find_OCF(OCF_Alive),
    					Find_Hostile(GetOwner(caller)),
    					Find_NoContainer()),			//Nicht verschachtelt?
    					Find_Func("IsFakeRepairable")),		//Konsolen?
    					Find_AtRect(-10,-10,20,20));
    
    if(pStation)
    	obj = FindObject2(Find_Func("GetDamage"), Find_Or(Find_ID(AHBS), Find_Func("IsRepairable"), Find_Func("IsFakeRepairable")), Find_Not(Find_Hostile(GetOwner(caller))), Find_AtRect(-10, -10, 20, 20));
    
    if(obj)
    {	
      if(Hostile(GetOwner(obj), GetOwner(caller)))
      {
        if(obj->~IsRepairable())
        {
          //Feindliche Fahrzeuge beschädigen
          DoDmg(5, DMG_Fire, obj);

          used = true;
          charge = BoundBy(charge-1, 0, MaxEnergy());
        }
        else if(obj->~IsFakeRepairable())
        {
          //Feindliche Konsolen beschädigen
      	  obj = obj->GetRealRepairableObject();
          DoDmg(5, DMG_Fire, obj);

          used = true;
          charge = BoundBy(charge-1, 0, MaxEnergy());
        }
        else
        {
          if(!living_dmg_cooldown)
          {
            //Feindliche Lebewesen verletzen
            DoDmg(12,DMG_Fire,obj);
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
        //Konsolen reparieren
        if(obj->~IsFakeRepairable())
          obj = obj->GetRealRepairableObject();

        //Fahrzeug reparieren
        DoDamage(-2-(2*(!!pStation)), obj);
				
        //Callback
        if(GetDamage(obj) == 0)
          obj->~IsFullyRepaired();
        else
          obj->~OnRepairing(this);

        if(!Hostile(GetOwner(obj), GetOwner(Contained())) && GetOwner(obj) != GetOwner(Contained()) && iRepaired++ >= 40)
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

public func Timer()
{
  //Nicht laden solange überhitzt
  if(GetEffect("Overheat", this))
    return;

  //Akku um einen Punkt aufladen
  if(!GetEffect("RepairObjects", this))
  	if(!pStation || pStation == Contained())
    	charge = BoundBy(charge+3,0,MaxEnergy());

  return true;
}


