/*-- Erste Hilfe Pack --*/

#strict

local healpoints;

public func HandSize()   	{return(1000);}
public func HandX()     	{return(3500);}
public func HandY()     	{return(0);}
public func IsDrawable() 	{return(true);} 

public func MaxHealPoints()	{return(150);}
public func StartHealPoints()	{return(150);}
func IsEquipment()		{return(true);}


/* Initialisierung */

protected func Initialize()
{
  //Punkteregeneration
  AddEffect("FAPRegenerate",this(),251,50,this(),GetID());
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
   return(RemoveObject());
  }
  //Bereits am Heilen? Dann Stoppen
  if(GetEffect("FAPHeal",this()))
  {
   RemoveEffect("FAPHeal",this());
   return(1);
  }
  //Bereits anderweitig am heilen?
  if(GetEffect("*Heal*",caller))
  {
    PlayerMessage(GetOwner(caller), "$AlreadyHealing$",caller);
    return(1);
  }
  //Nicht verwundet?
  if(GetEnergy(caller) == GetPhysical("Energy",0, caller)/1000)
  {
   PlayerMessage(GetOwner(caller), "$NotWounded$",caller);
   return(1);
  }
  //Brennende Clonks löschen
  if(OnFire(caller))
  {
    caller->Sound("Extinguish");
    DoHealPoints(-30);
    Extinguish(caller);
  }

  //Heilungseffekt auflegen
  AddEffect("FAPHeal",this(),250,2,this(),GetID(),caller);

  return(1);
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
  return(GetEffect("FAPHeal",this()));
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
  if(pClonk->~IsMedic() && GetHealPoints() >= 40)
  {
   DoHealPoints(-40);
   CreateContents(DGNN,pClonk);
   Sound("FAPK_Dragnin.ogg");
  }
  return(true);
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
  if(!pClonk) return(-1);

  EffectVar(0, pTarget, iEffectNumber) = pClonk;
  Sound("FAPK_HealStart.ogg");

  if(pClonk->~IsMedic())
  {
    //Langsam gehen.
    var sub = GetPhysical("Walk", 1, 0, GetID(pClonk))*3/10;
    SetPhysical("Walk", GetPhysical("Walk", 0, pClonk)-sub, 2, pClonk);
    
    EffectVar(1, pTarget, iEffectNumber) = sub;
  }
  else
  {
   ObjectSetAction(pClonk, "Heal");
   EffectVar(1, pTarget, iEffectNumber) = 0;
  }

  return(1);
}

public func FxFAPHealTimer(pTarget, iEffectNumber, iEffectTime)
{
  var pClonk = EffectVar(0, pTarget, iEffectNumber);
  if(!pClonk) return(-1);
  
  //Abbruch
  if(EffectVar(1, pTarget, iEffectNumber))
  {
    if((pClonk->GetProcedure() ne "WALK") || pClonk->~IsCrawling())//Nur gehen ist erlaubt.
      return(-1);
  }
  else
  {
    if((pClonk->GetAction() != "Heal") && (pClonk->GetComDir() != COMD_Stop))
    {
      return(-1);
    }
  }

  //Fertig geheilt?
  if(GetEnergy(pClonk) >= GetPhysical("Energy",0,pClonk)/1000)
   return(-1);
    
  //Effekte
  //CreateParticle("MSpark",GetX(pClonk)+RandomX(-8, 8),GetY(pClonk)+RandomX(-10, 10),0,-20,RandomX(18,38),RGB(210,20,20));
  pClonk->CreateParticle("ShockWave",0,0,Random(10),Random(10),5*GetObjHeight(pClonk)+25+Sin(iEffectTime*5,35),RGB(210,20,20),pClonk);
  
  if(!(iEffectTime % 40))
   Sound("FAPK_Healing*.ogg");
  
  if(!(iEffectTime % 6))
  {
   //Eigentliches Heilen.
   var heal = 1;
   if(pClonk->GetComDir() == COMD_None)
    heal *= 2;
    DoEnergy(+heal,pClonk);
    
    if(!(iEffectTime % 4) || !EffectVar(1, pTarget, iEffectNumber))
     if(pTarget->DoHealPoints(-heal) <= 0)
      return(-1);
  }

  return(1);
}

public func FxFAPHealStop(target, no, reason, temp)
{
  var clonk = EffectVar(0, target, no);

  if(!reason && clonk)
    Sound("FAPK_HealEnd.ogg");

  //Clonk zurücksetzen
  clonk->~StopHealing();
  if(EffectVar(1, target, no))
   SetPhysical("Walk", GetPhysical("Walk", 0, clonk)+EffectVar(1, target, no), 2, clonk);
}

/* Heilpunkte */

public func GetHealPoints(){return(healpoints);}

public func DoHealPoints(int iChange)
{
  Log("%v",iChange);
  healpoints = BoundBy(healpoints+iChange,0,MaxHealPoints());
  Log("%v",BoundBy(healpoints+iChange,0,MaxHealPoints()));

  return(healpoints);
}

/* Regenerierungseffekt */

public func FxFAPRegenerateTimer(pTarget, iEffectNumber, iEffectTime)
{
  if(!Contained()) return(1);
  if(!Contained()->~IsMedic()) return(1);
  if(GetHealPoints() < MaxHealPoints())
   DoHealPoints(1);

  return(1);
}

/* Infos */

protected func CalcValue()
{
  //Wert errechnen
  return(healpoints*(GetValue(0,GetID())/(StartHealPoints()/MaxHealPoints()))/MaxHealPoints());
}

/* EHP-HUD */

func CustomHUD(){return(true);}

func UpdateHUD(object pHUD)
{
  pHUD->Charge(healpoints,MaxHealPoints());
  pHUD->Ammo(healpoints, MaxHealPoints(), GetName(), true);
}

/* Sounds */

protected func Hit()
{
  Sound("FAPK_Hit*.ogg");
  return(1);
}

protected func Selection()
{
  Sound("FAPK_Charge.ogg");
  return(1);
}