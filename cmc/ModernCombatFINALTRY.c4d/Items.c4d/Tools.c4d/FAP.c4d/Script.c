/*-- EHP --*/

#strict 2
#include PACK

public func IsDrawable()	{return true;}	//Wird sichtbar getragen
public func HandX()		{return 4000;}
public func HandY()		{return 10;}
public func HandSize()		{return 1000;}

public func StartPoints()	{return 150;}
public func MaxPoints()		{return 150;}
public func RefillTime()	{return 30;}
public func TeamSupportTime()	{return 20;}

public func MinValue()		{return 39;}
public func MinValue2()		{return 79;}

local iHealed;


/* Auffüllen */

public func CanRefill()
{
  //Nur wenn Träger ein Sanitäter ist und Pack momentan nicht benutzt wird
  return Contained()->~IsMedic() && !GetEffect("FAPHeal", this);
}

/* Selbstheilung */

public func Activate(object pCaller)
{
  //Leeres Pack zerstören
  if(!GetPackPoints() && !pCaller->~IsMedic())
  {
    Sound("FAPK_Hit.ogg", false, this);
    CastParticles("Paper", RandomX(4, 8), 40, 0, 0, 20, 35, RGB(180, 180, 180), RGBa(240, 240, 240, 150));
    RemoveObject();
    return true;
  }
  //Heilt bereits: Stoppen
  if(GetEffect("FAPHeal", this))
  {
    RemoveEffect("FAPHeal", this);
    return true;
  }
  //Wird schon geheilt
  if(GetEffect("*Heal*", pCaller))
  {
    PlayerMessage(GetOwner(pCaller), "$AlreadyHealing$", pCaller);
    return true;
  }
  //Ist nicht verwundet
  if(GetEnergy(pCaller) == GetPhysical("Energy", PHYS_Current, pCaller) / 1000)
  {
    PlayerMessage(GetOwner(pCaller), "$NotWounded$", pCaller);
    return true;
  }
  //Falsche Aktion?
  if(!WildcardMatch(GetAction(pCaller), "*Walk*"))
  {
    PlayerMessage(GetOwner(pCaller), "$CantHeal$", pCaller);
    return true;
  }
  //Clonk anhalten
  SetComDir(COMD_Stop, pCaller);
  //Heilen
  AddEffect("FAPHeal", this, 250, 2, this);
  return true;
}

/* Dragnin entnehmen */

public func ControlThrow(object pCaller)
{
  //Sanitäter können mit [Werfen] Dragnin entpacken
  if(pCaller->~IsMedic())
  {
    //Clonk hat schon eine Dragninspritze: Geht nicht
    if(ContentsCount(DGNN, pCaller))
      return PlayerMessage(GetOwner(pCaller), "$NoSpace$", pCaller);

    //Falsche Aktion?
    if(!WildcardMatch(GetAction(pCaller), "*Walk*") && !WildcardMatch(GetAction(pCaller), "*Swim*") && !WildcardMatch(GetAction(pCaller), "*Crawl*") && !WildcardMatch(GetAction(pCaller), "*Jump*"))
    {
      PlayerMessage(GetOwner(pCaller), "$CantTake$", pCaller);
      return true;
    }

    if(GetPackPoints() >= 40)
    {
      DoPackPoints(-40);
      CreateContents(DGNN, pCaller);
      Sound("FAPK_Dragnin.ogg");
      ShiftContents(pCaller, 0, DGNN);
    }
    else
      PlayerMessage(GetOwner(pCaller), "$NotEnoughPoints$", pCaller);
  }
  return true;
}

/* Team-Heilung */

public func DoTeamSupport(array aClonks)
{
  //Können nur Sanitäter
  if(!Contained()->~IsMedic())
    return;
  //Wenn nicht gerade in Gebrauch
  if(GetEffect("FAPHeal", this))
    return;

  var a = [];
  //Zuerst die mit vollem Leben aussortieren
  for (var pClonk in aClonks)
    if(GetEnergy(pClonk) < GetPhysical("Energy", PHYS_Current, pClonk) / 1000)
      a[GetLength(a)] = pClonk;
  aClonks = a;
  //Keiner mehr übrig
  if(!GetLength(aClonks))
    return;
  //Je mehr geheilt werden, desto schwächer
  var heal = Max(2, 8 - 2 * GetLength(aClonks));

  for (var pClonk in aClonks)
  {
    DoEnergy(heal, pClonk);
    //Achievement-Fortschritt (I'll fix you up!)
    DoAchievementProgress(heal, AC02, GetOwner(Contained()));
    iHealed += heal;
    CreateParticle("ShockWave", GetX(pClonk) - GetX(), GetY(pClonk) - GetY(), 0, 0, 5 * (5  + GetObjHeight(pClonk)), RGB(0, 230, 255), pClonk);
    CreateParticle("ShockWave", GetX(pClonk) - GetX(), GetY(pClonk) - GetY(), 0, 0, 5 * (10 + GetObjHeight(pClonk)), RGB(0, 230, 255), pClonk);
    CreateParticle("ShockWave", GetX(pClonk) - GetX(), GetY(pClonk) - GetY(), 0, 0, 5 * (15 + GetObjHeight(pClonk)), RGB(0, 230, 255), pClonk);
    ScreenRGB(pClonk, RGBa(0, 230, 255, 190), 80, 3, false, SR4K_LayerMedicament, 200);
    DoPackPoints(heal / -2);
    Sound("FAPK_Healing*.ogg");
  }
  while (iHealed >= 40)
  {
    iHealed -= 40;
    //Punkte bei Belohnungssystem (Heilung)
    DoPlayerPoints(BonusPoints("Healing", 40), RWDS_TeamPoints, GetOwner(Contained()), Contained(), IC05);
  }
}

/* Heil-Effekt */

protected func FxFAPHealStart(object pTarget, int iEffect, int iTemp)
{
  if(iTemp)
    return;
  //Clonk?
  var pClonk = Contained();
  if(!pClonk || !pClonk->~IsClonk())
    return -1;
  //Heil-Aktion
  ObjectSetAction(pClonk, "Heal");
  Sound("FAPK_HealStart.ogg", false, this);
  ScreenRGB(pClonk, RGBa(0, 230, 255, 190), 80, 3, false, SR4K_LayerMedicament, 200);
}

protected func FxFAPHealTimer(object pTarget, int iEffect, int iTime)
{
  //Clonk?
  var pClonk = Contained();
  if(!pClonk || !pClonk->~IsClonk())
    return -1;

  //Clonk heilt nicht mehr
  if(GetAction(pClonk) != "Heal" || GetComDir(pClonk) != COMD_Stop)
    return -1;

  //Pack nicht mehr ausgewählt
  if(Contents(0, pClonk) != this)
    return -1;

  //Keine Punkte mehr
  if(!GetPackPoints())
    return -1;

  //Vollständig geheilt?
  if(GetEnergy(pClonk) >= GetPhysical("Energy", PHYS_Current, pClonk) / 1000)
    return -1;

  //Effekt
  CreateParticle("ShockWave", GetX(pClonk) - GetX(), GetY(pClonk) - GetY(), Random(10), Random(10), 5 * GetObjHeight(pClonk) + 25 + Sin(iTime * 5, 35), RGB(0, 230, 255), pClonk);

  //Alle 20 Frames
  if(!(iTime % 20))
    ScreenRGB(pClonk, RGBa(0, 230, 255, 190), 80, 3, false, SR4K_LayerMedicament, 200);

  //Alle 40 Frames
  if(!(iTime % 40))
   Sound("FAPK_Healing*.ogg");

  //Alle 6 Frames: Heilen
  if(!(iTime % 6))
  {
    DoEnergy(2, pClonk);
    DoPackPoints(-1);
  }
}

protected func FxFAPHealStop(object pTarget, int iEffect, int iReason, bool fTemp)
{
  if(fTemp)
    return;

  //Clonk?
  var pClonk = Contained();
  if(!pClonk || !pClonk->~IsClonk())
    return;

  Sound("FAPK_HealEnd.ogg");

  //Clonk zurücksetzen
  pClonk->~StopHealing();
}

/* Sonstiges */

protected func Hit()
{
  Sound("FAPK_Hit*.ogg", false, this);
}

protected func Selection()
{
  Sound("FAPK_Charge.ogg", false, this);
}

/* KI-Behandlung */

protected func AI_IdleInventory(object pClonk)	{return AI_Inventory(pClonk);}

protected func AI_Inventory(object pClonk)
{
  //Wenn am heilen, nichts anderes tun
  if(GetEffect("FAPHeal", this))
    return 2;

  //Benutzen, wenn der Clonk weniger als volles Leben hat und genug Punkte da sind
  if(!pClonk->~IsHealing() && GetPackPoints() > 30 && GetEnergy(pClonk) < GetPhysical("Energy", PHYS_Current, pClonk) / 1000)
  {
    ShiftContents(pClonk, 0, GetID());
    //Benutzen (verzögert einsetzen)
    ScheduleCall(this, "Activate", 1, 0, pClonk);
    return 2;
  }
  return 1;
}