/*-- NUR OPEN BETA --*/

#strict 2

#appendto CDBT

func Use(caller)
{
  //Hinweis bei fehlender Spannung
  if(charge <= 9)
  {
    PlayerMessage(GetOwner(caller), "$NotCharged$", Contained(this),ENAM);
    Sound("WPN2_Empty.ogg");
  }

  //Nicht schocken wenn nicht bereit
  if(!Ready()) return false;

  //Nicht in Gebäuden
  if(Contained(caller)) return false;

  //Richtung feststellen
  var dir = +1;
  if(GetDir(GetUser()) == DIR_Left)
    dir = -1;

  //Patienten suchen
  var obj;
  if(obj = FindObject2(Find_InRect(-10,-10,20,20),Find_ID(FKDT),				//Schwerverletzter?
						  Find_Allied(GetOwner(caller)),		//Verbündet?
						  Find_NoContainer(),				//Im Freien?
						  Find_Not(Find_Func("RejectReanimation"))))	//Verweigert die Reanimation nicht?
  {
    obj = obj->GetClonk();

    //Patient wiederbeleben
    StopFakeDeath(obj);
    //Energieschub
    DoEnergy(29, obj);
    //Restliche Energie mit Heilungseffekt übergeben
    AddEffect("ShockPaddlesHeal",obj,20,1,0,GetID(),HealAmount(),HealRate());

    //Effekte
    Sound("CDBT_Shock.ogg");
    Sound("ClonkCough*.ogg", 0, obj);
    obj->Sparks(10,RGB(250,150,0), (GetDir(Contained())*2-1)*HandX()*2/1000);
    obj->Sparks(5,RGB(100,100,250), (GetDir(Contained())*2-1)*HandX()*2/1000);
    if(GetEffectData(EFSM_BulletEffects) >1)  obj->AddLightFlash(40+Random(20),0,0,RGB(0,140,255));

    //Eventnachricht: Spieler reanimiert Spieler
    EventInfo4K(0,Format("$MsgReanimation$",GetTaggedPlayerName(GetOwner(caller)), GetTaggedPlayerName(GetOwner(obj))),FKDT);

    //Achievement-Fortschritt (Shock Therapist)
    DoAchievementProgress(1, AC04, GetOwner(caller));

    //Punkte bei Belohnungssystem (Reanimation)
    DoPlayerPoints(BonusPoints("Reanimation"), RWDS_TeamPoints, GetOwner(caller), caller, IC04);

    //Energie entladen
    charge = BoundBy(charge-20,0,MaxEnergy());

    return 1;
  }

  obj=0;

  //Keine Patienten, dann eben Feinde suchen
  if(FindObject2(Find_InRect(-10,-10,20,20),Find_OCF(OCF_Alive),	//Am Leben?
  					Find_NoContainer(),		//Im Freien?
					Find_Hostile(GetOwner(caller)),	//Feindlich?
  					Find_Exclude(caller)))		//Nicht der Schocker selbst?
  {
    obj = FindObjects(Find_InRect(-10,-10,20,20),Find_OCF(OCF_Alive),Find_NoContainer(),Find_Exclude(caller));
    for(var target in obj)
    {
      if(target && !target->~HitExclude(this))
      {
        //Schaden durch elektrischen Schlag (und Schleudern)
        DoDmg(30+Random(10),DMG_Energy,target,0,GetController(caller)+1,GetID());
        if(Hostile(GetOwner(GetUser()), GetOwner(target)))
          if(!GetAlive(target) || IsFakeDeath(target))
            //Achievement-Fortschritt (Don't tase me Bro)
            DoAchievementProgress(1, AC05, GetOwner(GetUser()));
        Fling(target,2*dir,-2);
        if(!target)
        target = this;

        //Effekte
        Sound("CDBT_Shock.ogg");
        target->Sparks(5,RGB(250,150,0));
        target->Sparks(10,RGB(100,100,250));
        target->AddLightFlash(40+Random(20),0,0,RGB(0,140,255));

        //Energie entladen
        charge = BoundBy(charge-20,0,MaxEnergy());
        return true;
      }
    }
  }
  else
  {
    //Effekte
    Sound("CDBT_ShockFail.ogg");
    Sparks(5,RGB(250,150,0), (GetDir(Contained())*2-1)*HandX()*2/1000);
    Sparks(10,RGB(100,100,250), (GetDir(Contained())*2-1)*HandX()*2/1000);
    AddLightFlash(40+Random(20),0,0,RGB(0,140,255));

    //Energie entladen (Schock ins Leere kostet weniger Energiepunkte)
    charge = BoundBy(charge-10,0,MaxEnergy());
    return true;
  }
}
