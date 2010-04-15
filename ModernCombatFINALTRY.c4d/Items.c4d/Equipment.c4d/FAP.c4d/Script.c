/*-- Erste Hilfe Pack --*/

#strict 2

local healpoints;

public func HandSize()   	{return 1000;}
public func HandX()     	{return 4000;}
public func HandY()     	{return 10;}
public func IsDrawable() 	{return true;} 

public func MaxHealPoints()	{return 150;}
public func StartHealPoints()	{return 150;}
public func IsEquipment()	{return true;}
public func NoArenaRemove()	{return(true);}


/* Initialisierung */

protected func Initialize()
{
  //Punkteregeneration
  AddEffect("FAPRegenerate",this(),251,50,this(),GetID());
  //Gruppenheilung
  AddEffect("FAPGroupheal",this(),252,20,this(),GetID());
  //Lichteffekt
  AddEffect("FAPLight",this(),250,1,this(),GetID());
  healpoints = StartHealPoints();
}

/* Selbst heilen */

protected func Activate(caller)
{
  //Zerstören wenn leer
  if(!GetHealPoints())
  {
   Sound("FAPK_Hit*.ogg");
   CastParticles("Paper", RandomX(4,8), 40, 0, 0, 4*5, 7*5, RGBa(180,180,180,0), RGBa(240,240,240,150));
   return RemoveObject();
  }
  //Bereits am Heilen? Dann Stoppen
  if(GetEffect("FAPHeal",this()))
  {
   RemoveEffect("FAPHeal",this());
   return 1;
  }
  //Bereits anderweitig am heilen?
  if(GetEffect("*Heal*",caller))
  {
    PlayerMessage(GetOwner(caller), "$AlreadyHealing$",caller);
    return 1;
  }
  //Nicht verwundet?
  if(GetEnergy(caller) == GetPhysical("Energy",0, caller)/1000)
  {
   PlayerMessage(GetOwner(caller), "$NotWounded$",caller);
   return 1;
  }
  //Brennende Clonks löschen
  if(OnFire(caller))
  {
    caller->Sound("Extinguish");
    DoHealPoints(-30);
    Extinguish(caller);
  }
  
  //Clonk anhalten
  SetComDir(COMD_Stop, caller);

  //Heilungseffekt auflegen
  AddEffect("FAPHeal",this(),250,2,this(),GetID(),caller);

  return 1;
}

func Departure(object pClonk)
{
  //Heilungseffekt bei Verlassen des Patienten entfernen
  if(GetEffect("FAPHeal", this()))
   RemoveEffect("FAPHeal",this());
}

public func RejectShift()
{
  //Wegwurf bei Nutzung verhindern
  return GetEffect("FAPHeal",this());
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

public func Entrance(object pContainer)
{
  for(var obj in FindObjects(Find_Container(pContainer),Find_ID(GetID()),Find_Exclude(this())))
  {
   if(obj->GetHealPoints() < obj->MaxHealPoints())
   {
    var cnt = obj->GetHealPoints();
    var new = obj->DoHealPoints(healpoints);

    if(!DoHealPoints(cnt-new))
     Schedule("RemoveObject",1);
   }
  }
}

public func ControlThrow(object pClonk)
{
  //Sanitäter können mit [Werfen] Dragnin entpacken
  if(pClonk->~IsMedic())
  {
   if(GetHealPoints() >= 40)
   {
    DoHealPoints(-40);
    CreateContents(DGNN,pClonk);
    Sound("FAPK_Dragnin.ogg");
   }
   else
   {
    PlayerMessage(GetOwner(pClonk), "$NotEnoughPoints$",pClonk);
   }
  }
  return true;
}

func Deselection(object pClonk)
{
  if(GetEffect("FAPHeal", this()))
   ScheduleCall(this(),"SelectFAP",1);
}

protected func SelectFAP()
{
  ShiftContents(Contained(),0,GetID());
}

/* Heileffekt */

public func FxFAPHealStart(pTarget, iEffectNumber, iTemp, pClonk)
{
  //Kein Clonk?
  if(!pClonk) return -1;

  EffectVar(0, pTarget, iEffectNumber) = pClonk;
  Sound("FAPK_HealStart.ogg");
  ObjectSetAction(pClonk, "Heal");

  return 1;
}

public func FxFAPHealTimer(pTarget, iEffectNumber, iEffectTime)
{
  var pClonk = EffectVar(0, pTarget, iEffectNumber);
  if(!pClonk) return -1;
  
  //Abbruch
  if((pClonk->GetAction() != "Heal") || (pClonk->GetComDir() != COMD_Stop))
    return -1;

  //Fertig geheilt?
  if(GetEnergy(pClonk) >= GetPhysical("Energy",0,pClonk)/1000)
   return -1;
    
  //Effekte
  //CreateParticle("MSpark",GetX(pClonk)+RandomX(-8, 8),GetY(pClonk)+RandomX(-10, 10),0,-20,RandomX(18,38),RGB(210,20,20));
  pClonk->CreateParticle("ShockWave",0,0,Random(10),Random(10),5*GetObjHeight(pClonk)+25+Sin(iEffectTime*5,35),RGB(210,20,20),pClonk);
  
  if(!(iEffectTime % 40))	//Alle 40 Frames
   Sound("FAPK_Healing*.ogg");
  
  if(!(iEffectTime % 6))	//Alle 6 Frames
  {
   //Eigentliches Heilen.
   DoEnergy(2,pClonk);  
   if(pTarget->DoHealPoints(-1) <= 0)
    return -1;
  }

  return 1;
}

public func FxFAPHealStop(target, no, reason, temp)
{
  var clonk = EffectVar(0, target, no);

  if(!reason && clonk)
    Sound("FAPK_HealEnd.ogg");

  //Clonk zurücksetzen
  clonk->~StopHealing();
}

/* Heilpunkte */

public func GetHealPoints(){return healpoints;}

public func DoHealPoints(int iChange)
{
  healpoints = BoundBy(healpoints+iChange,0,MaxHealPoints());

  return healpoints;
}

/* Regenerierungseffekt */

public func FxFAPRegenerateTimer(pTarget, iEffectNumber, iEffectTime)
{
  if(!Contained()) return 1;
  if(!Contained()->~IsMedic()) return 1;
  if(GetEffect("FAPHeal", this())) return 1;
  if(GetHealPoints() < MaxHealPoints())
   DoHealPoints(1);

  return 1;
}

/* Lämpcheneffekt */

public func FxFAPLightTimer(pTarget, iNo, iTime)
{
  if(GetHealPoints() < 10) return 1;
  if(!Contained())
   CreateParticle("FapLight", 1, -2, 0, 0, 5*5, RGBa(BoundBy(InvertA1(255*GetHealPoints()/150,255)+10,0,255), 255*GetHealPoints()/150),this);
  if(Contents(0,Contained()) == this())
   if(WildcardMatch(GetAction(Contained()), "*Armed*"))
    CreateParticle("FapLight", (GetDir(Contained())*1), -2, 0, 0, 5*5, RGBa(BoundBy(InvertA1(255*GetHealPoints()/150,255)+10,0,255), 255*GetHealPoints()/150),this);
}

/* Gruppenheilung */

public func FxFAPGrouphealTimer(pTarget, iEffectNumber, iEffectTime)
{
  //Kriterien
  if(!Contained())                      return 1;	//Nicht im Freien
  if(!Contained()->~IsMedic())          return 1;	//Nur Sanitäter
  if(GetEffect("FAPHeal", this()))      return 1;	//Ohne Effekt
  if(!GetHealPoints())                  return 1;	//Nur wenn noch Punkte da sind
  if(Contents(0,Contained()) != this()) return 1;	//Nur, falls angewählt
  if(GetID(Contained()) == FKDT)        return 1;	//Im FakeDeath-Objekt?

  //Harte Vorauswahl überlebt? Los geht's.
  var heal = 8; //Merke: 8-2 -> 1 Patient
  var Patients = [];
  for(var patient in FindObjects(Find_OCF(OCF_Alive),			//Patient am Leben?
                                 Find_Distance(80),			//In Reichweite?
                                 Find_NoContainer(),			//Im Freien?
                                 Find_Allied(GetOwner(Contained()))))	//Verbündet?
  if(patient->~IsClonk())						//Patient ein Clonk?
  {
   if(GetEnergy(patient) < GetPhysical("Energy",0, patient)/1000)
   {
    if(patient == Contained()) continue;
    //Bewirkt, dass bei mehr Patienten weniger gut geheilt wird
    heal = Max(heal-2, 2);
    Patients[GetLength(Patients)] = patient;
   }
  }

  for(var clonk in Patients)
  {
   if(!clonk)
    continue;
   else
   {
    DoEnergy(heal, clonk);
    clonk->CreateParticle("ShockWave",0,0,0,0,5*GetObjHeight(clonk)+5*5,RGB(210,20,20),clonk);
    clonk->CreateParticle("ShockWave",0,0,0,0,5*GetObjHeight(clonk)+10*5,RGB(210,20,120),clonk);
    clonk->CreateParticle("ShockWave",0,0,0,0,5*GetObjHeight(clonk)+15*5,RGB(210,20,220),clonk);
    DoHealPoints(-heal/2);
    Sound("FAPK_Healing*.ogg");
   }
  }
}

/* Infos */

protected func CalcValue()
{
  //Wert errechnen
  return healpoints*(GetValue(0,GetID())/(StartHealPoints()/MaxHealPoints()))/MaxHealPoints();
}

/* EHP-HUD */

func CustomHUD(){return true;}

func UpdateHUD(object pHUD)
{
  pHUD->Charge(healpoints,MaxHealPoints());
  pHUD->Ammo(healpoints, MaxHealPoints(), GetName(), true);
}

/* Sounds */

protected func Hit()
{
  Sound("FAPK_Hit*.ogg");
  return 1;
}

protected func Selection()
{
  Sound("FAPK_Charge.ogg");
  return 1;
}