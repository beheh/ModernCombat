/*-- Pack --*/

#strict 2

public func IsPack()				{return true;}
public func IsEquipment()			{return GetID() != PACK;}
public func NoArenaRemove()			{return true;}

public func StartPoints()			{return 100;}
public func MaxPoints()				{return 100;}

public func RefillTime()			{return 0;}
public func CanRefill()				{return true;}

public func TeamSupportTime()			{return 30;}
public func TeamSupportRange()			{return 80;}

public func PackLight()				{return true;}

public func DestroyEmptyPack()			{return true;}

public func IsDrawable()			{return false;}

public func OnRefill(int iChange)		{}
public func DoTeamSupport(array aClonks)	{}


/* Initialisierung */

public func Initialize()
{
  AddEffect("IntPack", this, 1, RefillTime(), this);
  if(TeamSupportTime())
    AddEffect("IntTeamSupport", this, 1, TeamSupportTime(), this);
  if(PackLight())
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
  //Auf Wert iValue setzen und Änderung zurückgeben
  var current = GetPackPoints();
  GetPackPoints() = BoundBy(iValue, 0, MaxPoints());
  return GetPackPoints() - current;
}

/* Auffüll-Effekt */

protected func FxIntPackTimer()
{
  //Nicht im Freien
  if(!Contained()) return;
  //Weitere Checks
  if(!CanRefill())
    return false;
  DoPackPoints(1);
}

/* Lichteffekt */

protected func FxIntPackLightTimer()
{
  if(!Contained())
    CreateParticle("FapLight", 1, -2, 0, 0, 25, Fill2Color(), this);
  else
    if(Contents(0, Contained()) == this && IsDrawable() && WildcardMatch(GetAction(Contained()), "*Armed*"))
      CreateParticle("FapLight", GetDir(Contained()), -2, 0, 0, 25, Fill2Color(), this);
}

/* HUD */

public func CustomHUD()		{return true;}
public func ColorEmpty()	{return RGB(255, 0, 0);}
public func ColorLow()		{return RGB(255, 150, 0);}
public func MinValue()		{return MaxPoints()/4;}
public func MinValue2()		{return MaxPoints()/2;}

public func UpdateHUD(object pHUD)
{
  var color = ColorEmpty()*(Inside(GetPackPoints(), 0, MinValue()));
  if(!color)
    color = ColorLow()*(Inside(GetPackPoints(), MinValue()+1, MinValue2()));
  pHUD->~Charge(GetPackPoints(), MaxPoints());
  pHUD->~Ammo(GetPackPoints(), MaxPoints(), GetName(), true, color);
}

/* Team-Support */

protected func FxIntTeamSupportTimer()
{
  var objs;

  //Gar keine Punkte?
  if(!GetPackPoints()) return;
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
    objs = FindObjects(Find_Distance(TeamSupportRange()), Find_OCF(OCF_CrewMember), Find_NoContainer(), Find_Allied(GetOwner(Contained())), Find_Exclude(Contained()));
  }

  //Unterstützung starten
  DoTeamSupport(objs);
}

/* Zusammenlegen */

public func JoinPack(object pInto, object pMsgObject)
{
  //Natürlich nur dieselben Packs
  if(GetID(pInto) != GetID())
    return false;
  //Anderes Pack auffüllen
  var iChange = pInto->~DoPackPoints(GetPackPoints());
  //Und hier abziehen
  DoPackPoints(-iChange);
  //Mitteilung ausgeben
  if(iChange)
  {
    if(pMsgObject)
      PlayerMessage(GetOwner(pMsgObject), "$Refilled$", pMsgObject, GetID(), iChange);
    Sound("Merge.ogg",0,pInto,0,GetOwner(pInto)+1);
    Sound("PackGrab*.ogg");
  }
  //Wir sind leer?
  if(!GetPackPoints() && DestroyEmptyPack())
    RemoveObject();
  //Success!
  return true;
}

public func RejectEntrance(object pInto)
{
  //Hat schon eins: Zusammenlegen
  var other = FindContents(GetID(), pInto);
  if(other && !pInto->~NoPackMerge())
    JoinPack(other, pInto);
  //Clonks können nur eins tragen
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