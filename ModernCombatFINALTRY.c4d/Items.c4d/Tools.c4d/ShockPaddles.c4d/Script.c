/*-- Defibrillator --*/

#strict 2

local charge;

public func HandSize()			{return 1000;}
public func HandX()			{return 4500;}
public func IsDrawable()		{return true;}
public func MaxEnergy()			{return 30;}
public func IsEquipment()		{return true;}
public func NoArenaRemove()		{return true;}
public func IsSchockPaddles()		{return true;}
public func CustomQuickInventoryMenu()	{return QINV_ObjectMenu;}


/* Initialisierung */

public func Initialize()
{
  charge = MaxEnergy(); //Defibrillator geladen
}

public func GetUser()
{
  return Contained();
}

/* Akkuregeneration */

public func Timer()
{
  //Hinweis bei voller Ladung sowie nachladen
  if(GetUser())
    if(charge == 9)
    {
      ScheduleCall(0, "Beep", 15);
      SetAction("Reload");
    }
  //Akku um einen Punkt aufladen
  charge = BoundBy(charge+1,0,MaxEnergy());

  return true;
}

/* Shocken */

public func ControlThrow(pByObject)
{
  if(Use(pByObject))
  {
    ScheduleCall(0, "Beep", 20);
    SetAction("Reload");
  }
  return true;
}

public func Activate(pClonk)
{
  if(Use(pClonk))
  {
    ScheduleCall(0, "Beep", 20);
    return SetAction("Reload");
  }
}

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

  //Nicht in Geb�uden
  if(Contained(caller)) return false;

  //Richtung feststellen
  var dir = +1;
  if(GetDir(GetUser()) == DIR_Left)
    dir = -1;

  //Patienten suchen
  var obj;
  if(obj = FindObject2(Find_InRect(-10,-10,20,20),Find_ID(FKDT),			//Schwerverletzter?
						  Find_Allied(GetOwner(caller)),	//Verb�ndet?
						  Find_NoContainer()))			//Im Freien?
  {
    //Reanimation abgelehnt?
    if(!obj->~RejectReanimation())
    {
      obj = obj->GetClonk();

      //Patient wiederbeleben
      StopFakeDeath(obj);
      //Energieschub
      DoEnergy(20, obj);
       //Restliche Energie mit Heilungseffekt �bergeben
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
      DoPlayerPoints(ReanimationPoints(), RWDS_TeamPoints, GetOwner(caller), caller, IC04);

      //Energie entladen
      charge = BoundBy(charge-20,0,MaxEnergy());

      return 1;
    }
  }

  obj=0;

  //Keine Patienten, dann eben Feinde suchen
  if(FindObject2(Find_InRect(-10,-10,20,20),Find_OCF(OCF_Alive),	//Am Leben?
  					Find_NoContainer(),		//Im Freien?
  					Find_Exclude(caller)))		//Nicht der Schocker selbst?
  {
    obj = FindObjects(Find_InRect(-10,-10,20,20),Find_OCF(OCF_Alive),Find_NoContainer(),Find_Exclude(caller));
    for(var target in obj)
    {
      if(target && CheckEnemy(GetUser(),target))
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

/* Selbstheilungseffekt durch Wiederbelebung */

func HealRate()		{return 2;}
func HealAmount()	{return 100;}

func FxShockPaddlesHealStart(object pTarget, int iEffectNumber, int iTemp, int iHealAmount, int iHealRate)
{
  //Heilungsrate und -menge festlegen
  EffectVar(0,pTarget,iEffectNumber) = iHealAmount;	//Heilung pro Frame
  EffectVar(1,pTarget,iEffectNumber) = iHealRate;	//Zeitdauer der Heilung
}

func FxShockPaddlesHealTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  //Heilungseffekt
  if(!(iEffectTime % EffectVar(1, pTarget, iEffectNumber)))
  {
    DoEnergy(1, pTarget);
    EffectVar(0,pTarget,iEffectNumber)--;
  }
  //Schluss wenn komplett geheilt
  if(GetEnergy(pTarget) >= GetPhysical("Energy",0,pTarget)/1000)
    return -1;
  //oder der Effekt aufgebraucht
  if(!EffectVar(0,pTarget,iEffectNumber))
    return -1;
}

func FxShockPaddlesHealDamage(object pTarget, int iEffectNumber, int iChange)
{
  //Wenn durch Au�eneinwirkung verletzt, Effekt abbrechen
  if(iChange >= 0) return iChange;
  RemoveEffect("ShockPaddlesHeal", pTarget);
  return iChange;
}

/* Bereitschaft */

func Ready()
{
  //Nicht wenn am Ausw�hlen
  if(GetEffect("IntSelection", this))
    return false;

  //Nicht wenn am Nachladen
  if(GetAction() == "Reload")
    return false;

  //Nicht bei zu wenig Spannung
  if(charge >= 10)
    return true;
}

public func Beep()
{
  //Erst nach Ablauf des letzten Beeps
  if(GetEffect("IntWait", this)) return;

  if(charge <= 9) return;

  Sound("CDBT_Ready.ogg");

  //Einen Moment lang nicht mehr beepen
  AddEffect("IntWait", this, 1, 20, this);
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
  var color = RGB(255, 0, 0)*(Inside(charge, 0, 9));
  pHUD->Charge(charge,MaxEnergy());
  pHUD->Ammo(charge, MaxEnergy(), GetName(), true, color);
  if(GetEffect("IntSelection", this) != 0)
    pHUD->Recharge(GetEffect("IntSelection", this, 0, 6), GetEffect("IntSelection", this, 0, 3)-1);
  if(GetAction() == "Reload")
    pHUD->Recharge(GetActTime(), 34);
}

/* Allgemein */

protected func Hit()
{
  Sound("WPN2_Hit*.ogg");
}

protected func Selection()
{
  AddEffect("IntSelection", this, 1, 20, this);
  Sound("CDBT_Charge.ogg");
}

protected func FxIntSelectionStop() {
  Beep();
}

/* KI Funktion */

public func AI_Inventory(object pClonk)
{
  return true;
}

public func HasBotSupport(object pBot, object pTarget)
{
  if(!(GetOCF(pTarget) & OCF_Alive))
    return false;

  if(charge <= 10)
    return false;

  if(GetY(pBot)-20 > GetY(pTarget))
    return false;

  return true;
}

public func BotControl(object pBot, object pTarget, int iLevel, bool fAggroFire)
{
  if(!pBot || !pTarget || !fAggroFire || GetAction() == "Reload")
    return false;

  if(!GetCommand(pBot))
    SetCommand(pBot, "MoveTo", pTarget);

  if(Distance(GetX(), GetY(), GetX(pTarget), GetY(pTarget)) < 20) //Nah genug?
    ControlThrow(pBot);

  return true;
}
