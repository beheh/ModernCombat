/*--- MAV ---*/

#strict 2
#include CVHC

local cur_Attachment;
local iXDir;
local iYDir;
local iXTendency;
local iYTendency;
local iBank;
local iBankMax;
local iSpeed;
local fIsAiming;
local iAimAngle;
local iPat_Dir;
local crosshair;
local pLaser;
local pBeam;
local iC4Count;
local pItem;
local iItemType; //0: Standard, 1: MTP, 2: EHP, 3: Schweiﬂbrenner, 4: Schild, 5: Sprengschutz

public func AttractTracer(object pTracer)	{return GetPlayerTeam(GetController(pTracer)) != GetTeam() && !fDestroyed;}
public func IsBulletTarget()			{return !fDestroyed;}
public func IsDestroyed()			{return fDestroyed;}
public func AimAngle()				{return iAimAngle;}	//Winkel auf Ziel
public func ReadyToFire()			{return 1;}
public func IsMachine()				{return true;}
public func IsThreat()				{return !IsDestroyed();}
public func UpdateCharge()			{return 1;}
public func GetAttWeapon()			{return cur_Attachment;}
public func IsAiming() 				{return fIsAiming;}
public func GetLaser()				{return pLaser;}
public func Sgn(int x)				{if (x < 0) return x / x * -1; return x / x;}
public func IsMAV()						{return true;}
public func MaxDamage()				{return 40;}
public func IsRepairable()			{return !fDestroyed;}
public func IsMeleeTarget(object attacker)	{return !fDestroyed && Hostile(GetOwner(this), GetOwner(attacker));}
public func MeleeHit(object pWeapon)				{return DoDmg(MaxDamage()+1, DMG_Melee, this, 0, GetOwner(pWeapon));}
public func SensorDistance()	{return 190;}
public func IsActive() 				{return GetAction(this) == "Flying";}
public func TeamSupportRange()			{return 80;}
public func MaxRotLeft()
{
  return 120;
}

public func MaxRotRight()
{
  return 240;
}


/* Initialisierung */

protected func Initialize()
{
  //Standardwerte setzen
  iSpeed = 40;
  iAimAngle = 180;
  iBankMax = 13;

  //Ausr¸sten
  Arm(LRDR);

  Sound("MAVE_Engine.ogg", 0, 0, 70, 0, +1);

  SetAction("Wait");
}

/* Positionsbestimmung */

public func WeaponAt(&x, &y, &r)
{
  x = Sin(GetR()-180, 7000);
  y = -Cos(GetR()-250, 7000);
  r = iAimAngle+630+GetR();

  return 1;
}

public func WeaponBegin(&x, &y)
{
  var number = GetEffect("ShowWeapon", this);
  if(!number)
    return;
  x = EffectVar(2, this, number)/1000;
  y = EffectVar(3, this, number)/1000;
}

public func WeaponEnd(&x, &y)
{
  var number = GetEffect("ShowWeapon", this);
  if(!number)
   return;
  x = EffectVar(4, this, number)/1000;
  y = EffectVar(5, this, number)/1000;
}

/* Timer */

public func FxFlyingTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
	if(GetAction() != "Flying") 
	{
		RemoveEffect("Flying", this);
		return;
	}

  //Flugverhalten
  if(iXDir < iXTendency)
    iXDir+= 1 - (fIsAiming && !(iEffectTime % 3));
  if(iXDir > iXTendency)
    iXDir-= 1 - (fIsAiming && !(iEffectTime % 3));
  if(iYDir < iYTendency)
    iYDir+= 1 - (fIsAiming && !(iEffectTime % 3));
  if(iYDir > iYTendency)
    iYDir-= 1 - (fIsAiming && !(iEffectTime % 3));

  if(GetY() <= GetDefCoreVal("Offset", "DefCore", MAVE, 1) * -1 && iYTendency < 0)
  {
    iYTendency = 0;
    iYDir = 0;
  }

  //C4 verlangsamt den Flug
  SetXDir(iXDir / (iC4Count+1));

  if(iYDir<=0)
    SetYDir(iYDir / (iC4Count+1) - 2);
  else
    SetYDir(iYDir * (iC4Count+1) - 2);

  //Je nach Flugrichtung drehen
  if(iXTendency == 0)
    iBank -= iBank / Abs(iBank);
  else
    if(Abs(iBank) < iBankMax)
      iBank += iXTendency / Abs(iXTendency);

    SetObjDrawTransform(1000, -iBank*20, 0, iBank*20, 1000, 0, this);

  //Blinklicht (alle 30 Frames)
  if(!(iEffectTime % 30))
  {
    if(GetTeam())
      var rgb = GetTeamColor(GetTeam());
    else if(GetOwner())
      var rgb = GetPlrColorDw(GetOwner());
    else
      var rgb = RGB(255, 255, 255);
    CreateParticle("FlashLight", 0, 4, 0, 0 , 45, rgb, this);
  }

  //Nachladen, C4 pr¸fen und SensorChecks (alle 5 Frames)
  if(!(iEffectTime % 5))
  {
    if((GetAmmo(GetAttWeapon()->GetFMData(FM_AmmoID), GetAttWeapon()) < GetAttWeapon()->GetFMData(FM_AmmoUsage)) && !GetAttWeapon()->IsReloading())
      Reload();

    //Vorhandenes C4 z‰hlen
    iC4Count = 0;

    for(var pC4 in FindObjects(Find_Distance(50, 0, 0), Find_Func("IsC4Explosive")))
    {
      if(LocalN("pStickTo",pC4) != this)
        continue;
      iC4Count++;
    }
    
    //Sicherheitspr¸fung
    if(!pItem || Contained(pItem) != this)
    {
    	pItem = 0;
    	iItemType = 0;
    }
    
    if (iItemType == 0) Sense();
    if (iItemType == 2 !(iEffectTime % 20)) EHP();
  }

  //Schadensverhalten
  DamageChecks();

  //Waffensteuerung
  if(fIsAiming)
  {
    //Waffe vorhanden?
    if(!GetAttWeapon()) return;
    //Funktionst¸chtig?
    if(EMPShocked()) return;
    if(IsDestroyed()) return;

    cur_Attachment->SetTeam(GetTeam());

    //‹berdrehung nach links und rechts verhindern
    if(AimAngle() <= MaxRotLeft() && iPat_Dir < 0)
    {
      iPat_Dir = 0;
    }
    else if(AimAngle() >= MaxRotRight() && iPat_Dir > 0)
    {
      iPat_Dir = 0;
    }

    iAimAngle += iPat_Dir;

    //Fadenkreuz platzieren
    if(crosshair)
    {
      if(AimAngle()+GetR() <= 360)
        crosshair->SetAngle(AimAngle()-GetR()+360);
      else
        crosshair->SetAngle(AimAngle()-GetR());
    }

    if(!pLaser)
      pLaser = CreateObject(LRDT,0,0,GetOwner(this));

    var xPos = GetX(), yPos = GetY(), x = GetX(), y = GetY(), xdir = Sin(AimAngle(), 30), ydir = Cos(AimAngle(), -30);
    var gravity = GetGravity();

    SetGravity(0);
    if (!SimFlight(x, y, xdir, ydir))
      pLaser->Stop();
    else 
      if(!pLaser->Active())
        pLaser->Start();
    SetGravity(gravity);

    xdir = Sin(AimAngle(), 3000);
    ydir = Cos(AimAngle(), -3000);

    var pEnemy;

    if(pLaser->Active())
      pEnemy = FindObject2(Find_OnLine(0, 0, x - xPos, y - yPos), Find_Hostile(GetOwner(this)), Find_NoContainer(), Find_Or(Find_OCF(OCF_Alive), Find_Func("IsBulletTarget"), Find_Func("IsCMCVehicle")), Sort_Distance(0, 0));
    else
      pEnemy = FindObject2(Find_OnLine(0, 0, xdir, ydir), Find_Hostile(GetOwner(this)), Find_NoContainer(), Find_Or(Find_OCF(OCF_Alive), Find_Func("IsBulletTarget"), Find_Func("IsCMCVehicle")), Sort_Distance(0, 0));

    //Feinderkennung
    if(pEnemy)
    {
      if(!pLaser->Active())
        pLaser->Start();

      x = GetX(pEnemy);
      y = GetY(pEnemy);

      var xOff, yOff;

      if(xPos > x)
        xOff = GetDefCoreVal("Width", "DefCore", GetID(pEnemy)) + GetDefCoreVal("Offset", "DefCore", GetID(pEnemy), 0) + x;
      else
        xOff = GetDefCoreVal("Offset", "DefCore", GetID(pEnemy), 0) + x;

      if(yPos > y)
        yOff = GetDefCoreVal("Height", "DefCore", GetID(pEnemy)) + GetDefCoreVal("Offset", "DefCore", GetID(pEnemy), 1) + y;
      else
        yOff = GetDefCoreVal("Offset", "DefCore", GetID(pEnemy), 1) + y;

      if(Inside(xPos, Min(x, xOff), Max(x, xOff)))
      {
        x = Sin(AimAngle(), (yOff - yPos) * 1000 / (-Cos(AimAngle(), 1000))) + xPos;
        y = -Cos(AimAngle(), (yOff - yPos) * 1000 / (-Cos(AimAngle(), 1000))) + yPos;
      }
      else
        if(Inside(yPos, Min(y, yOff), Max(y, yOff)))
        {
          x = Sin(AimAngle(), (xOff - xPos) * 1000 / (Sin(AimAngle(), 1000))) + xPos;
          y = -Cos(AimAngle(), (xOff - xPos) * 1000 / (Sin(AimAngle(), 1000))) + yPos;
        }
      else
        if((Angle(xPos, yPos, xOff, yOff) >= 180 && Angle(xPos, yPos, xOff, yOff) < AimAngle()) || (Angle(xPos, yPos, xOff, yOff) <= 180 && Angle(xPos, yPos, xOff, yOff) > AimAngle()))
        {
          x = Sin(AimAngle(), (yOff - yPos) * 1000 / (-Cos(AimAngle(), 1000))) + xPos;
          y = -Cos(AimAngle(), (yOff - yPos) * 1000 / (-Cos(AimAngle(), 1000))) + yPos;
        }
      else
      {
        x = Sin(AimAngle(), (xOff - xPos) * 1000 / (Sin(AimAngle(), 1000))) + xPos;
        y = -Cos(AimAngle(), (xOff - xPos) * 1000 / (Sin(AimAngle(), 1000))) + yPos;
      }

  }

  //Laser zeichnen
  if(!pBeam)  
    pBeam = CreateObject(LRBM, 0, 0, GetOwner(this));
  else
    pBeam->SetPosition(xPos, yPos);

  //Sichtbarkeit nur f¸r Besitzer und Verb¸ndete
  pBeam->SetVisibility(VIS_Owner | VIS_Allies);

  //Laser passend strecken
  if(pEnemy || pLaser->Active())
    pBeam->SetObjDrawTransform(100 * Distance(xPos, yPos, x, y), 0, -458 * Distance(xPos, yPos, x, y), 0, 1000, 0);
  else
    pBeam->SetObjDrawTransform(100 * Distance(xPos, yPos, xPos + xdir/3, yPos + ydir/3), 0, -458 * Distance(xPos, yPos, xPos + xdir/3, yPos + ydir/3), 0, 1000, 0);
  
  pBeam->SetR(AimAngle()+90);
  SetPosition(x, y, pLaser);
  
  //Find_OnLine(int x, int y, int x2, int y2)
   /*
   
   if (x > 0)
    x = 10000 * (LandscapeWidth() - posX) / x;
   else
    x = -10000 * posX / x;
   
   if (y > 0)
     y = 10000 * posY / y;
   else
     y = -10000 * (LandscapeHeight() - posY) / y;
   
   if (posX + Sin(AimAngle(), y) > LandscapeWidth() - 1)
   {
     x = posX + Sin(AimAngle(), x);
     y = posY - Cos(AimAngle(), x);
   }
   else
   {
      x = posX + Sin(AimAngle(), y);
     y = posY - Cos(AimAngle(), y);
   }
   if (AimAngle()<225)
  {
    PathFree2(posX, posY, x, y);
    SetPosition(posX, posY, pLaser);
  }
  else
  {
    PathFree2(x, y, posX, posY);
    SetPosition(x, y, pLaser);
  }
  */
  //Log(Format("angle: %d, xPrev %d, yPrev %d, xNew %d, yNew %d, Material %s", iAimAngle, x, y, posX, posY,MaterialName(GetMaterial(posX, posY))));
   //SetPosition(posX, posY, pLaser);
  }

  //Objekte zum Rammen suchen
  if((Abs(iXDir) + Abs(iYDir) >= 30) && !GetEffect("MeleeCooldown", this))
  {
    var target = FindObject2(Find_AtPoint(0, 0), Find_Hostile(GetOwner(this)), Find_NoContainer(), Find_OCF(OCF_Alive));
    if(target)
    {
      //Ziel am kriechen?
      if(WildcardMatch(GetAction(target),"*Crawl*"))
      {
        //Erhˆhten Schaden verursachen
        DoDmg(30, DMG_Melee, target, 0, GetController(this)+1, GetID());
        //Ziel zum Aufstehen zwingen
        ObjectSetAction(target, "KneelUp");
      }
      else
      {
        //Schaden verursachen
        DoDmg(20, DMG_Melee, target, 0, GetController(this)+1, GetID());

        //Ziel schleudern
        var pwr = 18, angle = 45, dir = iXDir / Abs(iXDir);
        if(GetProcedure(target) != "SWIM")
        {
          if(!dir)
             dir--;
          SetXDir(Sin(angle * dir, pwr), target, 10);
          SetYDir(-Cos(angle * dir, pwr), target, 10);
          ObjectSetAction(target, "Tumble");
        }
      }
      //Auch der MAV nimmt Schaden
      DoDmg(10, DMG_Melee, this, 0, GetController(this)+1, GetID());
      AddEffect("MeleeCooldown", this, 1, 30);

      //Sound
      Sound("WPN2_Punch*.ogg");
      Sound("BKHK_RotorHit*.ogg");
    }
  }
}

/* Feindbewegung suchen */

protected func Sense()
{

  //Zu markierende Gefahren suchen
  for (var pObj in FindObjects(Find_Distance(SensorDistance()),			//In Reichweite
  		Find_Exclude(this),						//Selber ausschlieﬂen
  		Find_NoContainer(),						//Im Freien
  		Find_Or(Find_OCF(OCF_Alive), Find_Func("IsDetectable"))))	//Lebewesen oder als identifizierbar markiert
  {
    //Feindlich
    if(!Hostile(GetController(), GetController(pObj))) continue;

    //Beep.
    Beep();

    //Nicht markieren wenn schon der Fall
    if(FindObject2(Find_ID(SM08), Find_Action("Attach"), Find_ActionTarget(pObj), Find_Allied(GetOwner())))
      continue;

    //Ansonsten markieren
    CreateObject(SM08, GetX(pObj), GetY(pObj), GetOwner())->Set(pObj, this);

    //Achievement-Fortschritt (Intelligence)
    DoAchievementProgress(1, AC21, GetOwner());
  }
  return 1;
}

/* Beep */

public func Beep()
{
  //Erst nach Ablauf des letzten Beeps
  if(GetEffect("IntWait", this))
    return;

  //Effekte
  CreateParticle("PSpark", 0, 0, 0, 0, 60, GetPlrColorDw(GetOwner()), this);

  //Kreissymbol erstellen
  CreateObject(SM09,0,0,GetOwner())->Set(this);

  //Einen Moment lang nicht mehr beepen
  AddEffect("IntWait", this, 1, 50, this);
}


/* Schaden */

private func DamageChecks()
{
  //Schadensverhalten
  if(GetDamage() >= MaxDamage() / 2 && !GBackLiquid(0, 0))
    Smoke(0, 0, Random(7));
  if(GetDamage() >= MaxDamage() * 3 / 4 && !GBackLiquid(0, 0))
    CreateParticle("Blast", 0, 3, 0, 3, Random(35), RGB(255, 255, 255), this);
}

public func OnDmg(int iDmg, int iType)
{
  if(iType == DMG_Melee)	return 0 + 50 * (iItemType == 4);	//Melee
  if(iType == DMG_Fire)		return 40 + 24 * (iItemType == 4);	//Feuer
  if(iType == DMG_Energy)	return 40 + 24 * (iItemType == 4);	//Energiewaffen

  return 50 + 25 * (iItemType == 4);
}

/* Zerstˆrung */

public func OnDestruction()
{
		//Item rauswerfen
		if(pItem)
			Exit(pItem);
		pItem = 0;
		iItemType = 0;

	  //Vorhandenes C4 entfernen
    for(var pC4 in FindObjects(Find_Distance(50, 0, 0), Find_Func("IsC4Explosive")))
    {
      if(LocalN("pStickTo",pC4) != this)
        continue;
      AddEffect("C4Timer", pC4, 1, 4, 0, GetID(this));
    }

  //Explosion
  FakeExplode(4, GetController()+1, this);

  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter",3,100,0,0,0,80,100);

  //Deaktivieren
  Wait();
  SetAction("Destroyed");

  //Verschwinden
  FadeOut();
}

//C4 soll verschwinden, wenn das MAV kaputt geht
public func FxC4TimerStop (object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
	DecoExplode(4, pTarget);
}

//EHP

public func EHP()
{
	pItem->DoPackPoints(1);

	var aClonks = FindObjects(Find_Distance(TeamSupportRange()), Find_OCF(OCF_CrewMember), Find_NoContainer(), Find_Allied(GetOwner(Contained())));
  var a = [];
  //Zuerst die mit vollem Leben aussortieren
  for (var pClonk in aClonks)
    if (GetEnergy(pClonk) < GetPhysical("Energy", PHYS_Current, pClonk) / 1000)
      a[GetLength(a)] = pClonk;
  aClonks = a;
  //Keiner mehr ¸brig
  if (!GetLength(aClonks))
    return;
  //Je mehr geheilt werden, desto schw‰cher
  var heal = Max(2, 8 - 2 * GetLength(aClonks));

  for (var pClonk in aClonks)
  {
    DoEnergy(heal, pClonk);
    //Achievement-Fortschritt (I'll fix you up!)
    DoAchievementProgress(heal, AC02, GetOwner(Contained()));
    LocalN("iHealed", pItem) += heal;
    CreateParticle("ShockWave", GetX(pClonk) - GetX(), GetY(pClonk) - GetY(), 0, 0, 5 * (5  + GetObjHeight(pClonk)), RGB(0, 230, 255), pClonk);
    CreateParticle("ShockWave", GetX(pClonk) - GetX(), GetY(pClonk) - GetY(), 0, 0, 5 * (10 + GetObjHeight(pClonk)), RGB(0, 230, 255), pClonk);
    CreateParticle("ShockWave", GetX(pClonk) - GetX(), GetY(pClonk) - GetY(), 0, 0, 5 * (15 + GetObjHeight(pClonk)), RGB(0, 230, 255), pClonk);
    ScreenRGB(pClonk, RGBa(0, 230, 255, 190), 80, 3, false, SR4K_LayerMedicament, 200);
    pItem->DoPackPoints(heal / -2);
    Sound("FAPK_Healing*.ogg");
  }
  while (LocalN("iHealed", pItem) >= 40)
  {
    LocalN("iHealed", pItem) -= 40;
    //Punkte bei Belohnungssystem (Heilung)
    DoPlayerPoints(BonusPoints("Healing", 40), RWDS_TeamPoints, GetOwner(Contained()), Contained(), IC05);
  }
}

/* Aktionen */

public func FxWaitTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
	if(GetAction() != "Wait") 
	{
		RemoveEffect("Wait", this);
		return;
	}
  DamageChecks();
}

public func Start()
{
	iXDir = GetXDir();
	iYDir = GetYDir();

  SetAction("Flying");
  SetPhase(iItemType, this);
  if(!GetEffect("Flying", this))
  	AddEffect("Flying", this, 1, 1, this);
  Sound("MAVE_Engine.ogg", 0, 0, 70, 0, +1);
}

public func Wait()
{
  if(GetAction() != "Destroyed")
  {
    SetAction("Wait");
    if(!GetEffect("Wait", this))
  		AddEffect("Wait", this, 1, 1, this);
  	SetPhase(iItemType, this);
  }

  iXTendency = 0;
  iYTendency = 0;
  iXDir = 0;
  iYDir = 0;
  
  if(pLaser) RemoveObject(pLaser);

  Sound("MAVE_Engine.ogg", 0, 0, 70, 0, -1);

  EndAim();
}

/* Bewaffnung */

public func Arm(id idWeapon)
{
  //Waffe erstellen
  var pLaser = CreateObject(idWeapon, 0, 0, GetOwner(this));
  Enter(this, pLaser);

  //Und konfigurieren
  SetObjectOrder(this, pLaser, 1);
  cur_Attachment = pLaser;
  LocalN("controller", pLaser) = this;
}

private func Reload()
{
  //Munitionsart bestimmen
  var AmmoID = GetAttWeapon()->~GetFMData(FM_AmmoID);
  //Munition erzeugen
  Local(0, CreateContents(AmmoID)) = GetAttWeapon()->~GetFMData(FM_AmmoLoad);
  //Feuer einstellen und nachladen
  GetAttWeapon()->~StopAutoFire();
  GetAttWeapon()->~Reload();
}

private func InitAim()
{
  iXTendency = 0;
  iYTendency = 0;

  //Fadenkreuz entfernen falls vorhanden
  if(crosshair)
    RemoveObject(crosshair);

  //Besitzer setzen
  crosshair = CreateObject(HCRH, 0, 0, GetOwner(this));
  crosshair->Init(this);

  if (pLaser)
    pLaser->SetOwner(GetOwner(this));

  if(AimAngle()+GetR() >= 360)
    crosshair->SetAngle(AimAngle()+GetR()-360);
  else
    crosshair->SetAngle(AimAngle()+GetR());
}

public func EndAim()
{
  if(crosshair)
    RemoveObject(crosshair);

  if (pLaser)
    pLaser->Stop();

  if(pBeam)
    RemoveObject(pBeam);

  fIsAiming = false;
  iPat_Dir = 0;
}

/* Steuerung */

public func ControlLeft(pByObj)
{
	if(GetActionTarget(0, pByObj) == this) return false;

  if(GetAction() == "Wait")
    return true;

  if(fIsAiming)
  {
    iPat_Dir = 1;
    return true;
  }

  if(iXTendency > 0)
    iXTendency = 0;
  else
    iXTendency = -iSpeed;

  return true;
}

public func ControlLeftDouble(pByObj)
{
	if(GetActionTarget(0, pByObj) == this) return false;

  if(fIsAiming || GetAction() == "Wait")
    return true;

  iXTendency = -iSpeed;

  return true;
}

public func ControlLeftReleased(pByObj)
{
	if(GetActionTarget(0, pByObj) == this) return false;

  iXTendency = 0;
  return true;
}

public func ControlRight(pByObj)
{
	if(GetActionTarget(0, pByObj) == this) return false;

  if(GetAction() == "Wait")
    return true;

  if(fIsAiming)
  {
    iPat_Dir = -1;
    return true;
  }

  if(iXTendency < 0)
    iXTendency = 0;
  else
    iXTendency = iSpeed;

  return true;
}

public func ControlRightDouble(pByObj)
{
	if(GetActionTarget(0, pByObj) == this) return false;

  if(fIsAiming || GetAction() == "Wait")
    return true;

  iXTendency = iSpeed;

  return true;
}

public func ControlRightReleased(pByObj)
{
	if(GetActionTarget(0, pByObj) == this) return false;

  iXTendency = 0;
  return true;
}

public func ControlDown(pByObj)
{
	if(GetActionTarget(0, pByObj) == this) return false;

  if(GetAction() == "Wait")
    return true;

  if(fIsAiming)
  {
    iPat_Dir = 0;
    return true;
  }

  if(iYTendency < 0)
    iYTendency = 0;
  else
    iYTendency = iSpeed;

  return true;
}

public func ControlDownDouble(pByObj)
{
	if(GetActionTarget(0, pByObj) == this) return false;

  if(fIsAiming || GetAction() == "Wait")
    return true;

  iYTendency = iSpeed;

  return true;
}

public func ControlUp(object pByObj)
{
	if(GetActionTarget(0, pByObj) == this) return false;

  if(fIsAiming || GetAction() == "Wait")
    return true;

  if(iYTendency > 0)
    iYTendency = 0;
  else
    iYTendency = -iSpeed;

  return true;
}

public func ControlUpDouble(pByObj)
{
	if(GetActionTarget(0, pByObj) == this) return false;

  if(fIsAiming || GetAction() == "Wait")
    return true;

  iYTendency = -iSpeed;

  return true;
}

public func ControlThrow(pByObj)
{
	if(GetActionTarget(0, pByObj) == this)
	{
		if(GetAction() == "Destroyed") return false;
	
		var pTemp = Contents(0, pByObj);
		if(!pTemp || Hostile(GetOwner(this), GetOwner(pByObj)))
			return false;

		var iTemp = 0;
		if(GetID(pTemp) == AMPK) iTemp = 1;
		if(GetID(pTemp) == FAPK) iTemp = 2;
		if(GetID(pTemp) == BWTH) iTemp = 3;
		if(GetID(pTemp) == RSHL) iTemp = 4;
		if(GetID(pTemp) == BBTP) iTemp = 5;
		
		if(!iTemp)
		{
			PlayerMessage(GetOwner(pByObj), "$InvalidItem$", this);
			return false;
		}
		
		Enter(this, pTemp);
		if(pItem)
			if(!Collect(pItem, pByObj))
				Exit(pItem);
		pItem = pTemp;
		iItemType = iTemp;
		
		ShiftContents();
		
		SetPhase(iItemType, this);
		
		return true;
	}

  if(GetAction() == "Wait")
  {
    Start();
    return true;
  }

  if(!fIsAiming)
  {
    InitAim();
    fIsAiming = true;
  }
  else
  {
    if(pLaser && pLaser->Active())
      GetAttWeapon()->ControlThrow(this);
    else
    {
      PlayerMessage(GetOwner(pByObj), "$MarkRequired$", this);
      if(!GetEffect("NoTargetCooldown"))
      {
        Sound("JetNoTarget.wav",0,0,0,GetOwner(pByObj));
        AddEffect("NoTargetCooldown", this, 1, 150);
      }
    }
  }
}

public func ControlDig(pByObj)
{
	if(pItem && GetActionTarget(0, pByObj) == this)
	{
		if(!Collect(pItem, pByObj))
			Exit(pItem);
		pItem = 0;
		iItemType = 0;
		
		SetPhase(iItemType, this);
	}
}

public func FxNoTargetCooldownTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  RemoveEffect("NoTargetCooldown", this);
}

public func FxMeleeCooldownTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  RemoveEffect("MeleeCooldown", this);
}

/* Aufschlag */

protected func Hit()
{
  //Effekte
  Sparks(Random(2)+2,RGB(255,255,Random(5)+255));
  Sound("HeavyHit*.ogg");
}

protected func Hit3()
{
  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter",2,40,0,0,0,50,60);
  Sparks(Random(2)+2,RGB(255,255,Random(5)+255));
  Sound("HeavyHit*.ogg");

  DoDmg(10);
}

/* Kontaktfunktionen */

public func ContactLeft()
{
	iXDir = 0;
  if(iXTendency < 0)
    iXTendency = 0;
}

public func ContactRight()
{
	iXDir = 0;
  if(iXTendency > 0)
    iXTendency = 0;
}

public func ContactTop()
{
	iYDir = 0;
  if(iYTendency < 0)
    iYTendency = 0;
}

public func ContactBottom()
{
	iYDir = 0;
  if(iYTendency > 0)
    iYTendency = 0;
}
