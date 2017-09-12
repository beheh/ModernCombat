/*-- Grundfahrzeug --*/

#strict 2

local fDestroyed;
local iLastAttacker;
local aDealers;

public func OnDamage()			{}					//Beim Erhalten von Schaden
public func OnDestruction()		{}					//Bei der Zerstörung des Fahrzeugs
public func MaxDamage()			{return 100;}				//Maximalschaden
public func BonusPointCondition()	{return true;}				//Ob bei der Zerstörung Punkte vergeben werden

public func GetLastAttacker()		{return iLastAttacker;}			//Letzer Angreifer
public func IsDestroyed()		{return fDestroyed;}			//Zerstört
public func IsCMCVehicle()		{return true;}				//Ist ein CMC Fahrzeug
public func IsRepairable()		{return true;}				//Reparierbar
public func RepairSpeed()		{return 0;}				//Standard-Reparaturgeschwindigkeit
public func RemoveTracer()		{return GetOwner() == NO_OWNER;}	//Tracer entfernen, wenn neutral


/* Initialisierung */

public func Initialize()
{
  fDestroyed = false;
  iLastAttacker = NO_OWNER;
  aDealers = CreateArray();

  //Neutrale Fahrzeuge sind weiß
  if(GetOwner() == NO_OWNER)
  {
    SetColorDw(RGB(255,255,255));
  }
  else
  {
    SetColorDw(GetPlrColorDw(GetOwner()));
  }

  AddEffect("VehicleNoOwner", this, 50, 38, this);

  return true;
}

/* Reparatur */

local iRejectRepairCounter;

public func RejectRepair()
{
  if(--iRejectRepairCounter < 0)
    iRejectRepairCounter = RepairSpeed();

  return iRejectRepairCounter;
}

/* Unbenutztes Fahrzeug */

global func FxVehicleNoOwnerStart(object pTarget, int iEffectNumber)
{
  EffectVar(0, pTarget, iEffectNumber) = 10;
}

global func FxVehicleNoOwnerTimer(object pTarget, int iEffectNumber, int iTime)
{
  if(GetOwner(pTarget) == NO_OWNER) return FX_OK;
  var pClonk = FindObject2(Find_Allied(GetOwner(pTarget)), Find_Or(Find_Container(pTarget), Find_Distance(50, AbsX(GetX(pTarget)), AbsY(GetY(pTarget)))),  Find_Func("IsClonk"), Find_OCF(OCF_Alive));
  if(!pClonk)
  {
    EffectVar(0, pTarget, iEffectNumber)--;
  }
  else
  {
    EffectVar(0, pTarget, iEffectNumber) = 10;
  }
  if(EffectVar(0, pTarget, iEffectNumber) > 0) return FX_OK;

  var iPrevious = GetOwner(pTarget);
  SetController(NO_OWNER, pTarget);
  SetOwnerFade(NO_OWNER, pTarget);

  pTarget->~OnOwnerLoss(iPrevious);
  return FX_OK;
}

/* Schaden */

public func Damage(int iChange)
{
  if(GetDamage() > MaxDamage() && !IsDestroyed())
    Destroyed();
  OnDamage(iChange);
}

public func OnDmg(int iDmg, int iType)
{
  return 50;	//Standardwert
}

public func OnHit(int iDmg, int iType, object pBy)
{
  var iPlr = GetController(pBy);
  if(!IsDestroyed())
  {
    iLastAttacker = iPlr;
    if(Hostile(iPlr, GetOwner()))
    {
      if(!aDealers)
        aDealers = CreateArray();
      if(!aDealers[iPlr])
        aDealers[iPlr] = 0;
      aDealers[iPlr] += iDmg;
      while(aDealers[iPlr] >= 50)
      {
        //Punkte bei Belohnungssystem (Beschädigung)
        DoPlayerPoints(BonusPoints("Damage"), RWDS_BattlePoints, iLastAttacker, GetCursor(iLastAttacker), IC18);
        aDealers[iPlr] -= 50;
      }
    }
  }
  return true;
}

/* Treffbar */

public func DefaultBulletTarget(id idBullet, object pBullet, object pShooter)
{
  //Nicht treffbar wenn neutral
  if(GetOwner() == NO_OWNER)
    return false;
  //Wir schließen hier Verbündete gezielt aus
  //Damit müssen wir den Sonderfall nicht in den EnemyChecks einbauen
  //C4 darf speziell immer, wenn jemandem zugehörig
  if(!idBullet || !idBullet->~IgnoreEnemyCheck())
    if(pBullet && ((GetOwner(pBullet) != NO_OWNER && !Hostile(GetOwner(pBullet), GetOwner())) || ((GetTeam(pBullet) == GetTeam()) && (GetTeam() != 0))))
      return false;
  return true;
}

/* Zerstörung */

public func Destroyed()
{
  //Status setzen
  fDestroyed = true;

  if(iLastAttacker != NO_OWNER && GetOwner(this) != NO_OWNER && Hostile(GetOwner(), iLastAttacker))
  {
    //Achievement-Fortschritt (Junkdealer)
    DoAchievementProgress(1, AC23, iLastAttacker);
    //Punkte bei Belohnungssystem (Fahrzeug zerstört)
    if(BonusPointCondition())
      DoPlayerPoints(BonusPoints("VehicleDestruction"), RWDS_BattlePoints, iLastAttacker, GetCursor(iLastAttacker), IC27);
  }

  //Sound
  Sound("Blast2", false, this);

  //Callback
  OnDestruction();

  //Letzen Angreifer zurücksetzen
  iLastAttacker = NO_OWNER;
}

/* Umfärbung */

global func SetOwnerFade(int iPlr, object pTarget, int iDuration)
{
  var clrOld = 0;
  var clrNew = RGB(255,255,255);
  if(!pTarget) pTarget = this;
  if(!pTarget) return 0;
  if(GetOwner(pTarget) == iPlr) return true;
    if(GetOwner(pTarget) == NO_OWNER && GetColorDw(pTarget) == 0) SetColorDw(RGB(0,0,255), pTarget);	
      clrOld = GetColorDw(pTarget);
  if(iPlr != NO_OWNER) clrNew = GetPlrColorDw(iPlr);
  SetColorDwFade(clrNew, pTarget, iDuration);
  SetOwner(iPlr, pTarget);
  SetColorDw(clrOld, pTarget);
  return true;
}

global func SetColorDwFade(int clrNew, object pTarget, int iDuration)
{
  if(!iDuration) iDuration = 50;
  if(!pTarget) pTarget = this;
  if(!pTarget) return 0;

  var clrOld = GetColorDw(pTarget);
  RemoveEffect("UpdateOwnerColor", pTarget);
  SetColorDw(clrOld, pTarget);
  AddEffect("UpdateOwnerColor", pTarget, 50, 1, pTarget, 0, clrOld, clrNew, iDuration);
  return true;
}

global func FxUpdateOwnerColorStart(pTarget, iNo, iTemp, clrOld, clrNew, iDuration)
{
  if(clrOld == clrNew) return -1;
  EffectVar(0, pTarget, iNo) = clrOld;
  EffectVar(1, pTarget, iNo) = clrNew;
  EffectVar(2, pTarget, iNo) = iDuration;
  return true;
}
    
global func FxUpdateOwnerColorTimer(pTarget, iNo, iTime)
{
  var clrOld = EffectVar(0, pTarget, iNo);
  var clrNew = EffectVar(1, pTarget, iNo);
  var iDuration = EffectVar(2, pTarget, iNo);
  var color = RGBa(Interpolate2(GetRGBaValue(clrOld, 1), GetRGBaValue(clrNew, 1), iTime, iDuration), Interpolate2(GetRGBaValue(clrOld, 2), GetRGBaValue(clrNew, 2), iTime, iDuration), Interpolate2(GetRGBaValue(clrOld, 3), GetRGBaValue(clrNew, 3), iTime, iDuration));
  SetColorDw(color, pTarget);
  if(iTime >= iDuration) return -1;
}

global func FxUpdateOwnerColorStop(pTarget, iNo, iReason, fTemp)
{
  SetColorDw(EffectVar(1, pTarget, iNo), pTarget);
}