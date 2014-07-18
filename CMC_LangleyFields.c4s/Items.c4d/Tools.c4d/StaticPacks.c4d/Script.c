/*-- Pack-Erweiterung --*/

#strict 2

local fPlaced, iCOwner;

public func ControlUp(object pCaller)
{
  //Wird angefassen
  if(GetActionTarget(0, pCaller) == this && GetProcedure(pCaller) == "PUSH")
  {
    //Ins Inventar legen
    Enter(pCaller);
    return true;
  }
  
  return false;
}

/* Team-Support */

protected func FxIntTeamSupportTimer()
{
  var objs;

  //Gar keine Punkte?
  if(!this->~GetPackPoints()) return;
  //Contained
  if(!fPlaced)
  {
		//Liegt irgendwo?
		if(!Contained()) return;
		//Träger lebt nicht
		if(!GetAlive(Contained())) return;
		//Träger ist kein Clonk
		if(!Contained()->~IsClonk()) return;
		//Träger simuliert Todeskampf
		if(IsFakeDeath(Contained())) return;
		//Träger verschachtelt
		if(Contained(Contained()))
		{
		  //Egal ob ausgewählt
		  objs = FindObjects(Find_OCF(OCF_CrewMember), Find_Container(Contained(Contained())), Find_Exclude(Contained()));
		}
		//Träger im Freien
		else
		{
		  //Ist nicht ausgewählt
		  if(Contents(0, Contained()) != this) return;
		  objs = FindObjects(Find_Distance(this->~TeamSupportRange()), Find_OCF(OCF_CrewMember), Find_NoContainer(), Find_Allied(GetOwner(Contained())), Find_Exclude(Contained()));
		}
  }
  else
  {
    //Liegt nicht im Freien?
    if(Contained()) return;
    
    objs = FindObjects(Find_Distance(this->~TeamSupportRange()), Find_OCF(OCF_CrewMember), Find_NoContainer(), Find_Allied(iCOwner));
  }

  //Unterstützung starten
  this->~DoTeamSupport(objs);
}

public func DoPackPoints()
{
  var r = _inherited(...);
  
  //Pack leer? Zerlegen
  if(!this->~GetPackPoints())
  {
    Sound("FAPK_Hit.ogg", false, this);
    CastParticles("Paper", RandomX(4, 8), 40, 0, 0, 20, 35, RGB(180, 180, 180), RGBa(240, 240, 240, 150));
    RemoveObject();
    return true;
  }
  
  return r;
}

/* Platzieren */

public func ControlThrow(object pCaller)
{
  if(!pCaller) return;
  if(Contained(pCaller)) return PlayerMessage(GetOwner(pCaller), "$ErrInFree$", pCaller);
  
  //Clonk muss zum Aufstellen laufen
  if(!WildcardMatch(GetAction(pCaller), "*Walk*")) return;

  //Nach draußen
  Exit();

  iCOwner = GetOwner(pCaller);
  
  //Pack anfassen
  pCaller->SetAction("Push", this);
  
  //Effekte
  
  //Animation und Effekt setzen
  SetAction("Con");
  AddEffect("IntCon", this, 1, 1, this);

  return true;
}

protected func FxIntConTimer(object target, int number, iEffectTime)
{
  //Keine Arbeiter: wieder zusammenfallen
  if(!FindObject2(Find_ActionTarget(this),Find_Action("Push")) && GetActTime() > 5)
  {
    Collapse();
    return -1;
  }

  //Aufbaugeräusche
  if(!(iEffectTime%30))
  {
    Sound("WPN2_Hit*.ogg",0,0,50);
  }
}

private func ConComplete()
{
  if(GetEffect("IntCon", this))
    RemoveEffect("IntCon", this);

  var pTemp;
  while(pTemp = FindObject2(Find_ActionTarget(this), Find_Action("Push")))
    ObjectSetAction(pTemp, "Walk");

  //Aufstellen
  SetAction("Standing");
  fPlaced = true;

  CheckLimitation();

  Sound("Connect");
  
  return true;
}

private func Collapse()
{
  //Ins Inventar legen
  Enter(FindObject2(Find_ActionTarget(this), Find_Action("Push"), Find_Owner(iCOwner)));

  return true;
}

/* Abbau */

protected func FxIntDesTimer(object target, int number, iEffectTime)
{
  //Keine Arbeiter: Bestehen bleiben
  if(!FindObject2(Find_ActionTarget(this),Find_Action("Push")) && GetActTime() > 5)
  {
    if(GetEffect("IntDes", this))
      RemoveEffect("IntDes", this);

    //Wieder richtig stellen
    SetAction("Standing");
  }

  //Abbaugeräusche
  if(!(iEffectTime%30))
  {
    Sound("WPN2_Hit*.ogg",0,0,50);
  }
  
  return true;
}

private func DesComplete()
{
  if(GetEffect("IntDes", this))
    RemoveEffect("IntDes", this);

  var pTemp;
  while(pTemp = FindObject2(Find_ActionTarget(this), Find_Action("Push")))
    ObjectSetAction(pTemp, "Walk");

  //Ins Inventar legen
  var obj;
  if(obj = FindObject2(Find_ActionTarget(this), Find_Action("Push"), Find_Owner(iCOwner)))
    pTemp = obj;
  Enter(pTemp);
  
  fPlaced = false;

  Sound("RSHL_Hide.ogg");

  return;
}
