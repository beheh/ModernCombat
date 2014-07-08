/*-- Neues Script --*/

#strict 2

#appendto BWTH

public func Use(caller)
{
  //Eventuellen Cooldown verringern
  if(living_dmg_cooldown)
    living_dmg_cooldown--;

  used = false;
  
  var obj = caller->FindObject2(Find_AtRect(-10,-10,20,20), Find_OCF(OCF_Construct));
  if(obj)
  {
  	DoCon(1, obj);
		used = true;
		charge = BoundBy(charge-1, 0, MaxEnergy());
	}
  //Angreifbare Objekte suchen
  obj = caller->FindObject2(Find_Func("IsMeleeTarget", this),	//Angreifbar?
  			Find_AtRect(-10,-10,20,20));
  if(obj && !used)
  {
    //Objekt beschädigen
    DoDmg(3, DMG_Fire, obj);

    used = true;
    charge = BoundBy(charge-1, 0, MaxEnergy());
  }
  //Entschärfbare Objekte suchen
  obj = caller->FindObject2(Find_Func("IsDefusable"),		//Entschärfbar?
  			Find_Hostile(GetOwner(caller)),			//Feindlich?
  			Find_NoContainer(),				//Nicht verschachtelt?
  			Find_AtRect(-10,-10,20,20));
  if(obj)
  {
  	if(!used)
    {
			if(obj->~RTDefuse(caller))
				//Punkte bei Belohnungssystem (Entschärfung)
				DoPlayerPoints(BonusPoints("TechnicalTask"), RWDS_TeamPoints, GetOwner(caller), caller, IC15);

			used = true;
			charge = BoundBy(charge-1, 0, MaxEnergy());
		}
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

      if(Hostile(GetOwner(obj), GetOwner(caller)))
      {
        if(obj->~IsRepairable())
        {
          //Feindliche Fahrzeuge beschädigen
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
