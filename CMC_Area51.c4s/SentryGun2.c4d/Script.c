/*-- Selbstschussanlage --*/

#strict 2
#include CSTR

local cur_Attachment;
local aim_angle;
local target_angle;
local Shooting;
local iPat_Dir;
local fActive;
local GotTarget;
local last_id;
local fAAMode;
local powerMode;
local leftborder;
local rightborder;
local fTurning;

public func MaxDamage()				{return 100;}				//Maximaler Schaden
public func RepairSpeed()			{return 4;}				//Reparatur-Geschwindigkeit
public func GetAttWeapon()			{return cur_Attachment;}		//Waffe
public func SearchDistance()			{return 350 + fAAMode*300;}		//Suchlänge
public func MaxRotLeft()			{return 110 + GetR() - fAAMode*40;}	//Maximaler Winkel links
public func MaxRotRight()			{return 250 + GetR() + fAAMode*40;}	//Maximaler Winkel rechts
public func AimAngle()				{return aim_angle+GetR();}		//Winkel auf Ziel
public func ReadyToFire()			{return 1;}				//Allzeit bereit
public func IsMachine()				{return true;}				//Ist eine Elektrische Anlage
public func IsBulletTarget()			{return !IsDestroyed();}		//Kugelziel wenn nicht zerstört
public func IsAiming()				{return true;}				//Selbstschussanlage immer am Zielen
public func IsThreat()				{return fActive && !IsDestroyed();}	//Status
public func UpdateCharge()			{return 1;}
public func AttractTracer(object pTracer)	{return GetPlayerTeam(GetController(pTracer)) != GetTeam() && !IsDestroyed();}
public func RemoveTracer()			{return IsDestroyed();}			//Tracer entfernen, wenn zerstört
public func BonusPointCondition()		{return fActive;}


/* Initialisierung */

public func Initialize() 
{
  AddEffect("ShowWeapon",this,1,1,this,GetID());
  SetGraphics(0,this,GetID(),3,5,0,0,this);

  return inherited();
}

/* Einstellungen */

func Set(id idWeapon, bool fMode, bool active, int rot, int left, int right)
{
  //Alte Waffe entfernen
  Disarm();

  //Waffe erstellen
  if(idWeapon)
    Arm(idWeapon);
  else
    Arm(MISA);

  //Modus setzen
  fAAMode = fMode;
  if(fAAMode)
  {
    SetAction("Tower");
    SetGraphics(0, this, GetID(), 5, GFXOV_MODE_Action, "Turn");
    var fsin=Sin(rot, 1000), fcos=Cos(rot, 1000);
    SetObjDrawTransform(+fcos, -fsin, fsin*0, fsin, +fcos, -(1000-fcos)*0, this, 5);
  }
  else
  {
    SetAction("Turn");
    SetGraphics(0, this, 0, 5);
  }

  //SSA einschalten
  if(active != fActive)
  {
    if(active)
      TurnOn();
    else
      TurnOff();
  }

  //SSA rotieren
  SetR(rot);
  target_angle = AimAngle();

  //Zielerfassung einschränken
  if(left)
    leftborder = left;
  if(right)
    rightborder = right;
}

/* Schaden */

public func OnDmg(int iDmg, int iType)
{
  if(iType == DMG_Projectile)	return 40;	//Projektile
  if(iType == DMG_Fire)		return 60;	//Feuer
  if(iType == DMG_Explosion)	return;		//Explosionen und Druckwellen
  if(iType == DMG_Energy)	return 50;	//Energiewaffen
  if(iType == DMG_Bio)		return 100;	//Säure und biologische Schadstoffe

  return 50;
}

/* Zerstörung */

public func OnDestruction()
{
  //Waffe entfernen
  Disarm();

  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",3,20,0,0,220,500);
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",4,100,0,0,40,15,RGB(40,20,20));

  //Eventuelles Waffenbewegungsgeräusch stoppen
  Sound("ATBY_CannonRotation.ogg", false, this, 30, 0, -1);
}

public func Destruction()
{
  RemoveEffect("ShowWeapon", this);
}

public func OnRepair()
{
  //Neu bewaffnen
  Arm(last_id);
}

/* Aufrufe */

public func TurnOn()
{
  fActive = true;
}

public func TurnOff()
{
  fActive = false;
  if(GetAttWeapon()) GetAttWeapon()->StopAutoFire();
}

public func Arm(id idWeapon)
{
  //Crash-Vermeidung
  if(!idWeapon) return;
  if(!GetName(0, idWeapon)) return;

  //Ausrüsten mit idWeapon
  var pWeapon = CreateObject(idWeapon, 0, 0, GetOwner());
  Enter(this,pWeapon);

  //Ordnen
  SetObjectOrder(this, pWeapon, 1);
  aim_angle = 180;
  iPat_Dir = -1+ Random(2)*2;
  cur_Attachment = pWeapon;
  LocalN("controller", pWeapon) = this;
  Reload();
  
  //Effekt
  AddEffect("ShowWeapon", this, 20, 1, this);
}

public func Disarm()
{
  while(Contents())
  {
   last_id = GetID(Contents());
   RemoveObject(Contents());
  }
  RemoveEffect("ShowWeapon", this);
}

/* Positionsbestimmung */

public func WeaponAt(&x, &y, &r)
{
  x = Sin(GetR()-180,7000);
  y = -Cos(GetR()-180,7000);
  r = aim_angle+270+GetR();
  return 1;
}

public func WeaponBegin(&x, &y)
{
  var number = GetEffect("ShowWeapon",this);
  if(!number)
   return;
  x = EffectVar(2, this, number)/1000;
  y = EffectVar(3, this, number)/1000;
}

public func WeaponEnd(&x, &y)
{
  var number = GetEffect("ShowWeapon",this);
  if(!number)
   return;
  x = EffectVar(4, this, number)/1000;
  y = EffectVar(5, this, number)/1000;
}

public func GetWeaponR()
{
  var number = GetEffect("ShowWeapon",this);
  if(!number)
   return;
  return EffectVar(1, this, number);
}

/* Zielerfassung und Suchbewegung */

public func Activity()
{
  //Bereitschaft prüfen: Waffe vorhanden/kein EMP-Einfluss/nicht zerstört/ist eingeschaltet/nicht am reparieren
  if(!GetAttWeapon())	return;
  if(EMPShocked())	return;
  if(IsDestroyed())	return;
  if(!fActive)		return;
  if(IsRepairing())	return;

  //Wenn nicht schon gesetzt: Turn-Action
  if(!fAAMode)
  {
    if(GetAction() != "Turn")
      SetAction("Turn");
  }
  else if(GetAction() != "Tower")
    SetAction("Tower");

  //Besitzer aktualisieren
  cur_Attachment->SetTeam(GetTeam());

  //Alle 30 Frames blinken
  if(!(GetActTime()%30))
  {
    if(GetTeam())
      var rgb = GetTeamColor(GetTeam());
    else if(GetOwner())
      var rgb = GetPlrColorDw(GetOwner());
    else
      var rgb = RGB(255,255,255);
    CreateParticle("FlashLight",0,4,0,0,3*15,rgb,this);
  }

  //Im Standard-Modus entsprechende Patrouille fahren
  if(!fAAMode)
  {
    //Rotation feststellen
    if(target_angle < MaxRotLeft() || target_angle > MaxRotRight())
      target_angle = AimAngle();
    //Alle 80 Frames eine zufällige Bewegung nach links oder rechts
    if(!GotTarget && !(GetActTime()%80))
    {
      if(target_angle == MaxRotRight())
        target_angle -= 50;
      else
        if(target_angle == MaxRotLeft())
          target_angle += 50;
        else
          target_angle = BoundBy(AimAngle() + 50 - 100*Random(2), MaxRotLeft(), MaxRotRight());
    }

    //Drehung festlegen
    var prevAngle = aim_angle;
    aim_angle += BoundBy(target_angle-AimAngle(),-1,1);
    if(prevAngle != aim_angle)
    {
      //Sounds für Waffenbewegung
      if(!fTurning)
      {
        Sound("ATBY_CannonStart.ogg", false, this, 30);
        Sound("ATBY_CannonRotation.ogg", false, this, 30, 0, 1);
      }
      fTurning = true;
    }
    else
    {
      if(fTurning)
      {
        Sound("ATBY_CannonStop.ogg", false, this, 30);
        Sound("ATBY_CannonRotation.ogg", false, this, 30, 0, -1);
      }
      fTurning = false;
    }

    //Kein Ziel: Eventuelles Feuer einstellen und alle 3 Frames neue Ziele suchen
    if(!GotTarget)
    {
      if(Shooting)
      {
        Shooting = false;
          GetAttWeapon()->StopAutoFire();
      }
      if(!(GetActTime()%3))
        GotTarget = Search();

      if(GotTarget)
        Sound("BBTP_Alarm.ogg", false, this, 50);
    }
    //Ansonsten Ziel verfolgen und Feuer eröffnen
    else
    {
      target_angle = Angle(GetX(), GetY() + 7, GetX(GotTarget), GetY(GotTarget));

      if((MaxRotRight() >= 360) && (target_angle < MaxRotRight()-360))
        target_angle += 360;

      //Wenn Ziel im Visier, entsprechend das Feuer eröffnen/einstellen
      if(Abs(AimAngle() - target_angle) < 15)
      {
        Shooting = true;
        if(!GetAttWeapon()->IsShooting())
          GetAttWeapon()->Fire(this);
        if(GetAmmo(GetAttWeapon()->GetFMData(FM_AmmoID), GetAttWeapon()) < GetAttWeapon()->GetFMData(FM_AmmoUsage))
          Reload();
      }
      else
      {
        Shooting = false;
        GetAttWeapon()->StopAutoFire();
      }

      if(!CheckTarget(GotTarget,this))
        GotTarget = 0;
      else if(!PathFree(GetX(),GetY()+7,GetX(GotTarget), GetY(GotTarget)))
        GotTarget = 0;
      else if(ObjectDistance(GotTarget,this) > SearchDistance())
        GotTarget = 0;
      else
      {
        //Winkel zum Ziel
        var search_angle = Angle(GetX(), GetY() + 7, GetX(GotTarget), GetY(GotTarget));

        search_angle = Normalize(search_angle, 0);
        if(MaxRotRight() < 360 && (search_angle < MaxRotLeft() || search_angle > MaxRotRight()))
          GotTarget = 0;
        else if(MaxRotRight() >= 360 && (search_angle < MaxRotLeft() && search_angle > MaxRotRight()-360))
          GotTarget = 0;
      }
    }
  }
  //Ansonsten AA-Patrouille fahren
  else
  {
    powerMode--;

    //Keine Ziele: Prioritäten zurücksetzen
    if(!ValidTarget(GotTarget))
    {
      GotTarget = 0;
    }

    var fHasTarget = GotTarget;

    //Ziele suchen
    if(powerMode > 0 || !(GetActTime()%2))
      GotTarget = SearchAA();

    //Ziel vorhanden: Verfolgen und Feuer eröffnen
    if(GotTarget)
    {
      //Signalgeräusch bei neuer Zielerfassung
      if(GotTarget != fHasTarget)
        Sound("BBTP_Alarm.ogg", false, this, 50);

      powerMode = 100;

      //Rotation berechnen
      target_angle = Angle(GetX(), GetY() + 7, GetX(GotTarget), GetY(GotTarget));

      if((MaxRotRight() >= 360) && (target_angle < MaxRotRight()-360))
        target_angle += 360;

      var prevAngle = aim_angle;
      aim_angle += BoundBy(target_angle-AimAngle(),-5,5);
      if(prevAngle != aim_angle)
      {
        if(!fTurning)
        {
          Sound("ATBY_CannonStart.ogg", false, this, 30);
          Sound("ATBY_CannonRotation.ogg", false, this, 30, 0, 1);
        }
        fTurning = true;
      }
      else
      {
        if(fTurning)
        {
          Sound("ATBY_CannonStop.ogg", false, this, 30);
          Sound("ATBY_CannonRotation.ogg", false, this, 30, 0, -1);
        }
        fTurning = false;
      }

      //Wenn Ziel im Visier, entsprechend das Feuer eröffnen/einstellen
      if(Abs(AimAngle() - target_angle) < 15)
      {
        Shooting = true;
        if(!GetAttWeapon()->IsShooting())
          GetAttWeapon()->Fire(this);
        if(GetAmmo(GetAttWeapon()->GetFMData(FM_AmmoID), GetAttWeapon()) < GetAttWeapon()->GetFMData(FM_AmmoUsage))
          Reload();
      }
      else
      {
        Shooting = false;
        GetAttWeapon()->StopAutoFire();
      }
    }
    //Ansonsten zufällige Bewegung ausführen
    else
    {
      //Alle 80 Frames eine zufällige Bewegung nach links oder rechts
      if(!(GetActTime()%80))
      {
        if(target_angle == MaxRotRight())
          target_angle -= 50;
        else
          if(target_angle == MaxRotLeft())
            target_angle += 50;
          else
            target_angle = BoundBy(AimAngle() + 50 - 100*Random(2), MaxRotLeft(), MaxRotRight());
      }
      var prevAngle = aim_angle;
      aim_angle += BoundBy(target_angle-AimAngle(),-3,3);
      if(prevAngle != aim_angle)
      {
        if(!fTurning)
        {
          Sound("ATBY_CannonStart.ogg", false, this, 30);
          Sound("ATBY_CannonRotation.ogg", false, this, 30, 0, 1);
        }
        fTurning = true;
      }
      else
      {
        if(fTurning)
        {
          Sound("ATBY_CannonStop.ogg", false, this, 30);
          Sound("ATBY_CannonRotation.ogg", false, this, 30, 0, -1);
        }
        fTurning = false;
      }

      //Eventuelles Feuer einstellen
      if(Shooting)
      {
        Shooting = false;
        GetAttWeapon()->StopAutoFire();
      }
    }
  }
}

public func Search(int iX, int iWidth, int iHeight)
{
  var pAim;

  /*DrawParticleLine("PSpark", 0, 0, -70 + Sin(aim_angle, SearchDistance()), SearchDistance(), 10, 80, RGB(255, 0, 0));
  DrawParticleLine("PSpark", 0, 0, 70 + Sin(aim_angle, SearchDistance()), SearchDistance(), 10, 80, RGB(255, 0, 0));*/ 

  var Targets = FindTargets(this, SearchDistance());
  var pTarget = 0;
  for(pAim in Targets)
  {
    if(GetOwner() != NO_OWNER)
      if(GetOwner(pAim) == GetOwner() || !Hostile(GetOwner(pAim), GetOwner()))
        continue;

    if(!CheckTarget(pAim,this))
      continue;

    //Winkel zum Ziel
    var search_angle = Angle(GetX(), GetY() + 7, GetX(pAim), GetY(pAim));

    search_angle = Normalize(search_angle, 0);
    if(MaxRotRight() < 360 && (search_angle < MaxRotLeft() || search_angle > MaxRotRight()))
      continue;
    else if(MaxRotRight() >= 360 && (search_angle < MaxRotLeft() && search_angle > MaxRotRight()-360))
      continue;

    var pTarget = pAim;
    break;
  }

  if(Shooting && !pTarget)
  {
    Shooting = false;
    GetAttWeapon()->StopAutoFire();
  }

  if(!pTarget)
    GotTarget = true;

  return pTarget;
}

public func GetPriority(object pSuspect)
{
  var priority = 0;

  if(pSuspect->~IsHelicopter())
  {
    if(!pSuspect->~GetPassengerCount())
      priority = 1;
    else
      priority = 2;
  }
  if(pSuspect->~IsMAV())
    priority = 3;
  if(GetOCF(pSuspect) & OCF_Alive)
    priority = 4;
  if(pSuspect->~IsRifleGrenade() || pSuspect->~IsRocket())
  {
    if(pSuspect->~IsRocket() && pSuspect->~IsDamaged())
      priority = 6;
    else
      priority = 8;
  }
  if(pSuspect->~IsArtilleryShell())
    priority = 7;

  return priority;
}

public func SearchAA()
{
  var pAim;
  var Targets = FindAATargets(SearchDistance());
  var pTarget = GotTarget;
  var iAngleDist = 360;
  var curPrio = 0;
  if(pTarget)
  {
    iAngleDist = Abs(target_angle - AimAngle());
    //Log("%d, %d, %d", iAngleDist, target_angle, AimAngle());
    curPrio = GetPriority(pTarget);
  }
  
  for(pAim in Targets)
  {
    if(pTarget == pAim)
      continue;

    var priority = GetPriority(pAim);

    if(priority > curPrio)
    {
      pTarget = pAim;
      curPrio = priority;
      iAngleDist = iCheckDist;
    }
    else if (priority == curPrio)
    {
      var iCheckAngle = Angle(GetX(), GetY() + 7, GetX(pAim), GetY(pAim));

      if((MaxRotRight() >= 360) && (iCheckAngle < MaxRotRight()-360))
        iCheckAngle += 360;

      var iCheckDist = Abs(iCheckAngle - AimAngle());

      if(iCheckDist < iAngleDist)
      {
        pTarget = pAim;
        curPrio = priority;
        iAngleDist = iCheckDist;
      }
    }
  }

  return pTarget;
}

func FindAATargets(int maxDist)
{
  var pT;
  var targets = CreateArray();

  var x = GetX();
  var y = GetY();

  var preTargets = FindObjects(
        Find_Distance(maxDist, AbsX(x), AbsY(y)),
        Find_NoContainer(),
        Find_Or(
                Find_Func("IsRifleGrenade"),
                Find_Func("IsRocket"),
                Find_And(Find_Func("IsHelicopter"), Find_Func("IsBulletTarget")),
                Find_Func("IsMAV"),
                Find_Func("IsArtilleryShell"),
                Find_And(Find_OCF(OCF_Alive))),
        Find_Not(Find_Func("IsDestroyed")),
        Sort_Distance()
        );

  for(pT in preTargets)
  {
    if(ValidTarget(pT))
      //In Ergebnismenge aufnehmen
      targets[GetLength(targets)] = pT;
  }

  return targets;
}

func ValidTarget(object pT)
{
  if(!pT) return;

  if((GetOCF(pT) & OCF_Alive) && GetProcedure(pT) != "FLIGHT") return;

  if(!IsBulletTarget()) return;

  if(!CheckEnemy(this, pT, true))	return;

  var ox = GetX(pT);
  var oy = GetY(pT);

  if(leftborder && ox < leftborder)
    return;
  if(rightborder && ox > rightborder)
    return;

  if(Distance(GetX(), GetY(), ox, oy) > SearchDistance())
    return;

  //Winkel zum Ziel
  var target_angle = Angle(GetX(), GetY() + 7, ox, oy);
  target_angle = Normalize(target_angle, 0);
  if(MaxRotRight() < 360 && (target_angle < MaxRotLeft() || target_angle > MaxRotRight()))
    return;
  else if(MaxRotRight() >= 360 && (target_angle < MaxRotLeft() && target_angle > MaxRotRight()-360))
    return;

  //Pfad frei
  if(!PathFree(GetX(), GetY() + 7, ox, oy))
    return;

  //unsichtbare Ziele
  if(!CheckVisibility(pT, this))
    return;

  //Alles in Ordnung
  return true;
}

private func Reload()
{
  //Munitionsart
  var AmmoID = GetAttWeapon()->~GetFMData(FM_AmmoID);
  //Erzeugen
  Local(0, CreateContents(AmmoID)) = GetAttWeapon()->~GetFMData(FM_AmmoLoad);
  //Waffe soll nachladen
  GetAttWeapon()->~Reloaded(this);
  GetAttWeapon()->~StopAutoFire();
}

/* EMP */

public func EMPShock()
{
  EMPShockEffect(20*35);
  if(GetAttWeapon())
   GetAttWeapon()->StopAutoFire();
  return 1;
}

/* Konsolensteuerung */

public func ConsoleControl(int i)
{
  if(i == 1)
  {
    if(fActive) return "$TurnOff$";
    else
    return "$TurnOn$";
  }
  if(i == 2)
    if(GetAction() == "Destroyed")
      return "$Repair$";
}

public func ConsoleControlled(int i)
{
  if(i == 1)
  {
    if(fActive) TurnOff();
    else
    {
      TurnOn();
    }
  }
  if(i == 2)
  {
    if(GetAction() == "Destroyed")
      StartRepair();
  }
}

/* Serialisierung */

public func RejectContainedSerialize(object foo)	{return !false;} //weg mit den alten Waffen

public func Serialize(array& extra)
{
  extra[GetLength(extra)] = Format("SetTeam(%d)", GetTeam());
  if (fActive)
    extra[GetLength(extra)] = "TurnOn()";
  if (cur_Attachment)
  {
    extra[GetLength(extra)] = Format("Arm(%i)", GetID(cur_Attachment));
    extra[GetLength(extra)] = Format("LocalN(\"aim_angle\")=%d", aim_angle);
    extra[GetLength(extra)] = Format("LocalN(\"iPat_Dir\")=%d", iPat_Dir);
  }
}