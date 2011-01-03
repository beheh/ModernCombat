/*-- Pack --*/

#strict 2

public func IsPack()            {return true;}
public func IsEquipment()       {return true;}
public func NoArenaRemove()     {return true;}

public func StartPoints()       {return 100;}
public func MaxPoints()         {return 100;}

public func RefillTime()        {return 0;}
public func CanRefill()         {return true;}

public func TeamSupportTime()   {return 30;}
public func TeamSupportRange()  {return 80;}

public func PackLight()         {return true;}

public func DestroyEmptyPack()  {return true;}

public func IsDrawable()        {return false;}


public func OnRefill(int iChange)           {}
public func DoTeamSupport(array aClonks)    {}

/* Initialisierung */

public func Initialize()
{
  AddEffect("IntPack", this, 1, RefillTime(), this);
  if (TeamSupportTime())
    AddEffect("IntTeamSupport", this, 1, TeamSupportTime(), this);
  if (PackLight())
    AddEffect("IntPackLight", this, 1, 1, this);
  SetPackPoints(StartPoints());
}

/* Calls */

public func & GetPackPoints()
{
  return EffectVar(0, this, GetEffect("IntPack", this));
}

public func DoPackPoints(int iChange)
{
  var change = SetPackPoints(iChange + GetPackPoints());
  OnRefill(change);
  return change;
}

public func SetPackPoints(int iValue)
{
  //Auf Wert iValue setzen und �nderung zur�ckgeben
  var current = GetPackPoints();
  GetPackPoints() = BoundBy(iValue, 0, MaxPoints());
  return GetPackPoints() - current;
}

/* Auff�ll-Effekt */

protected func FxIntPackTimer()
{
  //Nicht im Freien
  if (!Contained()) return;
  //Weitere Checks
  if (!CanRefill())
    return false;
  var iChange = DoPackPoints(1);
}

/* Lichteffekt */

protected func FxIntPackLightTimer()
{
  if (!Contained())
    CreateParticle("FapLight", 1, -2, 0, 0, 25, Fill2Color(), this);
  else
    if (Contents(0, Contained()) == this && IsDrawable() && WildcardMatch(GetAction(Contained()), "*Armed*"))
      CreateParticle("FapLight", GetDir(Contained()), -2, 0, 0, 25, Fill2Color(), this);
}

/* HUD */

public func CustomHUD()     {return true;}

public func UpdateHUD(object pHUD)
{
  pHUD->~Charge(GetPackPoints(), MaxPoints());
  pHUD->~Ammo(GetPackPoints(), MaxPoints(), GetName(), true);
}

/* Team-Support */

protected func FxIntTeamSupportTimer()
{
  //Gar keine Punkte?
  if (!GetPackPoints()) return;
  //Liegt irgendwo?
  if (!Contained()) return;
  //Tr�ger lebt nicht
  if (!GetAlive(Contained())) return;
  //Tr�ger ist kein Clonk
  if (!Contained()->~IsClonk()) return;
  //Tr�ger ist nicht drau�en
  if (Contained(Contained())) return;
  //Tr�ger simuliert Todeskampf
  if (IsFakeDeath(Contained())) return;
  //Ist nicht ausgew�hlt
  if (Contents(0, Contained()) != this) return;

  //Na also, das w�r geschafft. Wer mag haben?
  var objs = FindObjects(Find_Distance(TeamSupportRange()), Find_OCF(OCF_CrewMember), Find_NoContainer(), Find_Allied(GetOwner(Contained())), Find_Exclude(Contained()));
  DoTeamSupport(objs);
}

/* Zusammenlegen */

public func JoinPack(object pInto, object pMsgObject)
{
  //Nat�rlich nur dieselben Packs
  if (GetID(pInto) != GetID())
    return false;
  //Anderes Pack auff�llen
  var iChange = pInto->~DoPackPoints(GetPackPoints());
  //Und hier abziehen
  DoPackPoints(-iChange);
  //Mitteilung ausgeben
  if (iChange && pMsgObject)
    PlayerMessage(GetOwner(pMsgObject), "$Refilled$", pMsgObject, GetID(), iChange);
  //Wir sind leer?
  if (!GetPackPoints() && DestroyEmptyPack())
    RemoveObject();
  //Success!
  return true;
}

public func RejectEntrance(object pInto)
{
  //Hat schon eins: Zusammenlegen
  var other = FindContents(GetID(), pInto);
  if (other)
    JoinPack(other, pInto);
  //Clonks k�nnen nur eins tragen
  return other && (GetOCF(pInto) & OCF_CrewMember);
}

/* Sonstiges */

protected func CalcValue()
{
  return Interpolate2(0, Value(GetID()), GetPackPoints(), MaxPoints());
}

public func Fill2Color()
{
  return HSL(Interpolate2(-10, 90, GetPackPoints(), MaxPoints()), 255, 127);
}