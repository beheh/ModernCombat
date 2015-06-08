/*-- NUR OPEN BETA --*/

#strict 2

#appendto WPN2

public func ControlThrow(caller)
{
  //Nutzer ist Schütze
  SetUser(caller);

  var fWait, meleeattacked;
  if(Contained())
    for (var obj in FindObjects(Find_Container(Contained())))
    {
      if(!obj->~IsWeapon2())
        continue;
      if(GetEffect("StrikeRecharge", obj))
      {
        fWait = true;
        break;
      }
    }

  if(!fWait && GetMCData(MC_CanStrike) && (caller->~ReadyToFire() || caller->~ReadyToAttack()) && !caller->~IsAiming() && (GetFMData(FM_Aim) == 0 || GetUser()->~IsAiming() || GetUser()->~AimOverride()))
  {
    var dir = GetDir(GetUser());
    //Ziele finden
    var obj = FindObjects(Find_InRect(-15+10*dir,-10,20,20), Find_Or(Find_OCF(OCF_Alive), Find_Func("IsMeleeTarget", this)), Find_NoContainer(), Find_Exclude(caller));
    for(var target in obj)
    {
    	if(target->~HitExclude(this))
    		continue;
    
      if(GetOCF(target) & OCF_Alive)
      {
        //Ziel feindlich?
        if(target && (Hostile(GetOwner(GetUser()), GetOwner(target)) || GetOwner(target) == NO_OWNER)) //Hier bewusst kein CheckEnemy, da wir auf FF-Checks verzichten
        {
          //Ziel am kriechen?
          if(target->~IsCrawling())
          {
            //Erhöhten Schaden verursachen
            DoDmg(GetMCData(MC_Damage)*3/2,DMG_Melee,target,0,GetController(GetUser())+1,GetID());
            //Ziel zum Aufstehen zwingen
            ObjectSetAction(target, "KneelUp");
          }
          else
          {
            //Schaden verursachen
            DoDmg(GetMCData(MC_Damage),DMG_Melee,target,0,GetController(GetUser())+1,GetID());
            SetCommand(GetUser(), "");
            SetComDir(COMD_None, GetUser());
  
            //Ziel schleudern
            var pwr = GetMCData(MC_Power), angle = GetMCData(MC_Angle);
            if(GetProcedure(target) != "SWIM")
            {
              if(!dir)
                dir--;
              SetXDir(Sin(angle*dir,pwr),target,10);
              SetYDir(-Cos(angle*dir,pwr),target,10);
              ObjectSetAction(target, "Tumble");
            }
          }

          //Achievement-Fortschritt (Fist of Fury)
          if(GetOCF(target) & OCF_CrewMember)
            DoAchievementProgress(1, AC36, GetOwner(GetUser())); 

          if(GetOwner(target) != NO_OWNER && Hostile(GetOwner(target), GetController(GetUser())))
            if(!GetAlive(target) || IsFakeDeath(target))
            {
              //Achievement-Fortschritt (Eat this!)
              DoAchievementProgress(1, AC14, GetOwner(GetUser()));
              //Ribbon-Fortschritt (The Tuna)
              AttemptAwardRibbon(RB07, GetOwner(GetUser()), GetOwner(target));
            }
          meleeattacked = true;
        }
      }
      else
      {
        target->~MeleeHit(this);
        meleeattacked = true;

        //Achievement-Fortschritt (Fly Swatter)
        if(target && target->~IsMAV() && target->~IsDestroyed() && (Abs(GetXDir(target)) + Abs(GetYDir(target)) >= 25))
          DoAchievementProgress(1, AC42, GetOwner(GetUser()));
      }
    }
  }
  if (meleeattacked)
  {
    //Soundeffekte
    Sound("ClonkMelee*.ogg", 0, this);
    Sound("WPN2_Punch*.ogg", 0, this);
    //Cooldown
    AddEffect("StrikeRecharge", this, 1, 1, this);
  }

  //Automatischen Schuss beenden, wenn erneut Werfen gedrückt
  if(IsRecharging())
  {
    if(!GetPlrCoreJumpAndRunControl(GetController(caller)))
    {
      StopAutoFire();
      if(GetFMData(FM_Auto) && !fWait && !meleeattacked) OnFireStop(firemode);
    }
    return 1;
  }

  //Kein Feuer erlauben wenn Kolbenschlag ausgeführt
  if(meleeattacked || fWait)
    return 1;

  //Unterstützt der Schussmodus das zielen aber es wird nicht gezielt?
  if(GetFMData(FM_Aim)>0 && !(GetUser()->~IsAiming()) && !(GetUser()->~AimOverride()))
  {
    //Bereit zum Zielen?
    if(GetUser()->~ReadyToAim())
      //Zielen starten
      GetUser()->StartAiming();

    //Nachladen?
    var ammoid = GetFMData(FM_AmmoID);
    var ammousage = GetFMData(FM_AmmoUsage);
    //Nachladen wenn möglich sofern Munition verbraucht
    if(!CheckAmmo(ammoid,ammousage,this))
      if(CheckAmmo(ammoid,ammousage,GetUser()))
      {
        Reload();
      }
      //Nicht genügend Munition
      else
      {
        //Unmöglichkeit des Nachladens angeben
        PlayerMessage(GetOwner(caller), "$NotEnoughAmmo$", caller, ammoid);
      }
    return 1;
  }

  //Feuern! Fehlgeschlagen?
  if(!Fire())
  {
    var ammoid = GetFMData(FM_AmmoID);
    var ammousage = GetFMData(FM_AmmoUsage);
    //Nachladen wenn möglich sofern Munition verbraucht
    if(!CheckAmmo(ammoid,ammousage,this))
      if(CheckAmmo(ammoid,ammousage,GetUser()))
      {
        Reload();
      }
      //Nicht genügend Munition
      else
      {
        //Unmöglichkeit des Nachladens angeben
        PlayerMessage(GetOwner(caller), "$NotEnoughAmmo$", caller, ammoid);
        Sound("WPN2_Empty.ogg");
      }
  }
  else
  {
    if(GetFMData(FM_BurstAmount) > 1 && !GetEffect("BurstFire", this))
    {
      var rechargetime = GetFMData(FM_BurstRecharge);
      if(rechargetime)
        AddEffect("BurstFire", this, 1, rechargetime, this, 0, GetFMData(FM_BurstAmount));
      else
      {
        for(var i = GetFMData(FM_BurstAmount); i > 0; i--)
          if(!Fire())
            return 1;
      }
    }
  }

  return 1;
}
