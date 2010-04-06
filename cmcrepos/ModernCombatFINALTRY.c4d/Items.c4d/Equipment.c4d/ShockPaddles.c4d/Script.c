/*-- Defibrillator --*/

#strict

local charge;

public func HandSize()	{return(1000);}
public func HandX()	{return(4500);}
public func HandY()	{return(0);}
public func IsDrawable(){return(true);}
public func MaxEnergy()	{return(30);}

func IsEquipment(){return(true);}


/* Initalisierung */

public func Initialize()
{
  charge = MaxEnergy(); //Defibrillator geladen
}

public func GetUser()
{
  return(Contained());
}

/* Akkuregeneration */

public func Timer()
{
  //Hinweis bei voller Ladung sowie nachladen
  if(GetUser())
   if(charge == 9)
   {
    Sound("CDBT_Reload*.ogg");
    SetAction("Reload");
   }
  //Akku um einen Punkt aufladen
  charge = BoundBy(charge+1,0,MaxEnergy());

  return(true);
}

/* Shocken */

public func ControlThrow(pByObject)
{
  Use(pByObject);
  return(true);
}

public func Activate(pClonk)
{
  return(Use(pClonk));
}

func Use(caller)
{
  //Hinweis bei fehlender Spannung
  if(charge <= 9)
   PlayerMessage(GetOwner(caller), "$NotCharged$", Contained(this),ENAM);

  //Nicht in Geb�uden
  if(Contained(caller)) return(false);

  //Nicht schocken wenn nicht bereit
  if(!Ready()) return(false);

  //Richtung feststellen
  var dir = +1;
  if(GetDir(GetUser()) == DIR_Left())
   dir = -1;

  //Patienten suchen
  var obj;
  if(obj = FindObject2(Find_InRect(-10,-10,20,20),Find_ID(FKDT),			//Schwerverletzter?
						  Find_Allied(GetOwner(caller)),	//Verb�ndet?
						  Find_NoContainer()))			//Im Freien?
  {
    obj = obj->GetClonk();
 
    //Patient wiederbeleben
    obj->StopFakeDeath();
    //Energieschub
    obj->DoEnergy(20);
    //Restliche Energie mit Heilungseffekt �bergeben
    obj->AddEffect("ShockPaddlesHeal",obj,20,1,0,GetID(),HealAmount(),HealRate());

    //Effekte
    Sound("CDBT_Shock.ogg");
    obj->Sound("ClonkCough*.ogg");
    obj->Sparks(10,RGB(250,150,0));
    obj->Sparks(5,RGB(100,100,250));
    obj->AddLightFlash(40+Random(20),0,0,RGB(0,140,255));
 
    //Eventnachricht: Spieler reanimiert Spieler
    EventInfo4K(0,Format("$MsgReanimation$",GetTaggedPlayerName(GetOwner(obj)),GetTaggedPlayerName(GetOwner(caller))),FKDT);

    //Energie entladen
    charge = BoundBy(charge-20,0,MaxEnergy());

    return(1);
  }

  obj=0;

  //Keine Patienten, dann eben Feinde suchen
  if(FindObject2(Find_InRect(-10,-10,20,20),Find_OCF(OCF_Alive),	//Am Leben?
					    Find_NoContainer(),		//Im Freien?
					    Find_Exclude(caller)))	//Nicht der Schocker selbst?
  {
   obj = FindObjects(Find_InRect(-10,-10,20,20),Find_OCF(OCF_Alive),Find_NoContainer(),Find_Exclude(caller));
   for(var target in obj)
   {
    if(target && CheckEnemy(GetUser(),target))
    {
     //Schaden durch elektrischen Schlag (und Schleudern)
     DoDmg(30+Random(10),DMG_Energy,target);
     Fling(target,2*dir,-2);
     if(!target)//K�nnte ja jetzt weg sein.
     target = this();

     //Effekte
     Sound("CDBT_Shock.ogg");
     target->Sparks(5,RGB(250,150,0));
     target->Sparks(10,RGB(100,100,250));
     target->AddLightFlash(40+Random(20),0,0,RGB(0,140,255));

     //Energie entladen
     charge = BoundBy(charge-20,0,MaxEnergy());
    }
   }
  }
  else
  {
   //Effekte
   Sound("CDBT_ShockFail.ogg");
   Sparks(5,RGB(250,150,0));
   Sparks(10,RGB(100,100,250));
   AddLightFlash(40+Random(20),0,0,RGB(0,140,255));

   //Energie entladen
   charge = BoundBy(charge-10,0,MaxEnergy());//Schock ins Leere kostet weniger Energiepunkte
  }
  //Nachladen
  SetAction("Reload");

  return(true);
}

/* Selbstheilungseffekt durch Wiederbelebung */

func HealRate()		{ return(2);  }
func HealAmount()	{ return(100); }

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
   return(-1);
  //oder der Effekt aufgebraucht
  if(!EffectVar(0,pTarget,iEffectNumber))
   return(-1);
}

func FxShockPaddlesHealDamage(object pTarget, int iEffectNumber, int iChange)
{
  //Wenn durch Au�eneinwirkung verletzt, Effekt abbrechen
  if(iChange >= 0) return(iChange);
  RemoveEffect("ShockPaddlesHeal", pTarget);
  return(iChange);
}

/* Bereitschaft */

func Ready()
{
  //Nicht wenn am Nachladen
  if(GetAction() eq "Reload")
   return(false);

  //Nicht bei zu wenig Spannung
  if(charge >= 10)
   return(true);

  return(false);
}

public func RejectEntrance(object pObj)
{
  if(GetOCF(pObj) & OCF_Living)
  {
   if(ContentsCount(GetID(),pObj))
    return(true);
  }
  return(false);
}

/* HUD */

func CustomHUD(){return(true);}

func UpdateHUD(object pHUD)
{
  pHUD->Charge(charge,MaxEnergy());
  pHUD->Ammo(charge, MaxEnergy(), GetName(), true);
}

/* Sounds */

protected func Hit()
{
  Sound("WPN2_Hit*.ogg");
}

protected func Selection()
{
  Sound("CDBT_Charge.ogg");
}
