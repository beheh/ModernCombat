/*-- Waffenscript-Erweiterung für Waffenupgrades --*/

#strict 2
#appendto WPN2

local iAttachment, pLaser, pBeam;

static const AT_NoAttachment		= 0;	//Kein Waffenaufsatz
static const AT_ExtendedMag		= 1;	//Erweitertes Magazin
static const AT_Bayonet			= 2;	//Bajonett
static const AT_Laserpointer		= 4;	//Laserpointer
static const AT_Silencer		= 8;	//Schalldämpfer
static const AT_Foregrip		= 16;	//Frontgriff
static const AT_GrenadeLauncher		= 32;	//Granatwerfer
static const AT_TracerDart		= 64;	//Peilsender

func PermittedAtts()
{
  return AT_NoAttachment;
}

func GetAttachment()
{
  return iAttachment;
}

/* Waffenaufsatz festlegen */

func SetAttachment(int iValue)
{
  if(iValue && !(PermittedAtts() & iValue)) return -1;

  if(GetEffect("LaserDot", this)) RemoveEffect("LaserDot", this);
  SetFireMode(1);

  var iTemp = iAttachment;
  iAttachment = iValue;
  if(iAttachment == AT_Laserpointer) AddEffect("LaserDot", this, 1, 1, this);
  
  SetGraphics(0,0,AttachmentIcon(iAttachment),2,GFXOV_MODE_Picture);    
  SetObjDrawTransform(500,0,10000,0,500,10000, 0, 2);
  
  if(Contained()) Contained()->~UpdateCharge(true);
  
  return iTemp;
}

/* Laserpointer */

func FxLaserDotTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  //Nutzer festlegen
  var user = this->~GetUser();
  var x, y, z;
  if(!user || !user->~IsClonk() || !user->WeaponAt(x, y, z) || Contents(0, user) != this)
  {
    if(pBeam) RemoveObject(pBeam);
    if(pLaser) RemoveObject(pLaser);
    return;
  }

  if(iAttachment == AT_Laserpointer)
  {
    var iAngle = EffectVar(1, user, GetEffect("ShowWeapon", user));
    var fStart = false;
    if(!pLaser)
      pLaser = CreateObject(LRDT,0,0,GetOwner(this));

    var xPos, yPos;
    user->WeaponEnd(xPos,yPos);
    xPos += GetX();
    yPos += GetY();

    var x = GetX(), y = GetY(), xdir = Sin(iAngle, 30), ydir = Cos(iAngle, -30);
    var gravity = GetGravity();

    SetGravity(0);
    if (!SimFlight(x, y, xdir, ydir))
      pLaser->Stop();
    else 
      if(!pLaser->Active())
        fStart = true;
    SetGravity(gravity);

    xdir = Sin(iAngle, 3000);
    ydir = Cos(iAngle, -3000);

    var pEnemy;

    if(pLaser->Active() || fStart)
      pEnemy = FindObject2(Find_OnLine(0, 0, x - xPos, y - yPos), Find_Hostile(GetOwner(this)), Find_NoContainer(), Find_Or(Find_OCF(OCF_Alive), Find_Func("IsBulletTarget", GetID(), this, this), Find_Func("IsCMCVehicle")), Sort_Distance(0, 0));
    else
      pEnemy = FindObject2(Find_OnLine(0, 0, xdir, ydir), Find_Hostile(GetOwner(this)), Find_NoContainer(), Find_Or(Find_OCF(OCF_Alive), Find_Func("IsBulletTarget", GetID(), this, this), Find_Func("IsCMCVehicle")), Sort_Distance(0, 0));

    //Feinderkennung
    if(pEnemy)
    {
      if(!pLaser->Active())
        fStart = true;

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
        x = Sin(iAngle, (yOff - yPos) * 1000 / (-Cos(iAngle, 1000))) + xPos;
        y = -Cos(iAngle, (yOff - yPos) * 1000 / (-Cos(iAngle, 1000))) + yPos;
      }
      else
        if(Inside(yPos, Min(y, yOff), Max(y, yOff)))
        {
          x = Sin(iAngle, (xOff - xPos) * 1000 / (Sin(iAngle, 1000))) + xPos;
          y = -Cos(iAngle, (xOff - xPos) * 1000 / (Sin(iAngle, 1000))) + yPos;
        }
      else
        if((Angle(xPos, yPos, xOff, yOff) >= 180 && Angle(xPos, yPos, xOff, yOff) < iAngle) || (Angle(xPos, yPos, xOff, yOff) <= 180 && Angle(xPos, yPos, xOff, yOff) > iAngle))
        {
          x = Sin(iAngle, (yOff - yPos) * 1000 / (-Cos(iAngle, 1000))) + xPos;
          y = -Cos(iAngle, (yOff - yPos) * 1000 / (-Cos(iAngle, 1000))) + yPos;
        }
      else
      {
        x = Sin(iAngle, (xOff - xPos) * 1000 / (Sin(iAngle, 1000))) + xPos;
        y = -Cos(iAngle, (xOff - xPos) * 1000 / (Sin(iAngle, 1000))) + yPos;
      }

  }

  //Laser zeichnen
  if(!pBeam)  
    pBeam = CreateObject(LRBM, 0, 0, GetOwner(this));
  else
    pBeam->SetPosition(xPos, yPos);

  //Sichtbarkeit nur für Besitzer und Verbündete
  pBeam->SetVisibility(VIS_Owner | VIS_Allies);

  //Laser passend strecken
  if(pEnemy || pLaser->Active())
    pBeam->SetObjDrawTransform(100 * Distance(xPos, yPos, x, y), 0, -450 * Distance(xPos, yPos, x, y), 0, 1000, 0);
  else
    pBeam->SetObjDrawTransform(100 * Distance(xPos, yPos, xPos + xdir/3, yPos + ydir/3), 0, -450 * Distance(xPos, yPos, xPos + xdir/3, yPos + ydir/3), 0, 1000, 0);

  pBeam->SetR(iAngle+90);
  SetPosition(x, y, pLaser);
  if(fStart) pLaser->Start();
  }
}

func FxLaserDotStop()
{
  if(pBeam) RemoveObject(pBeam);
  if(pLaser) RemoveObject(pLaser);
}

global func SALaunchBullet(int iX, int iY, int iOwner, int iAngle, int iSpeed, int iDist, int iDmg, int iRemoveTime, id idType, bool fNoTrail)
{
  //ID des Projektils identifizieren
  var ammoid = idType;
  //Standard nutzen wenn nicht gefunden
  if(!ammoid) ammoid = SHTX;

  var iSize = 2;
  var iTrail = iDmg*10;
  if(fNoTrail) iTrail = -1;
  
  var iAttachment = 0;
  if(this)
    iAttachment = LocalN("iAttachment", this);

  //Projektil erstellen
  var ammo = CreateObject(ammoid, iX, iY, iOwner);
  ammo->CustomLaunch(iAngle, iSpeed, iDist, iSize, iTrail, iDmg, iRemoveTime, iAttachment);

  return ammo;
}

/*----- Werfen -----*/

public func ControlThrow(caller)
{
  //Nutzer ist Schütze
  SetUser(caller);

  var fWait, meleeattacked;
  if(Contained())
    for (var obj in FindObjects(Find_Container(Contained())))
    {
      if(!obj->~IsWeapon2())
        continue;
      if(GetEffect("StrikeRecharge", obj))
      {
        fWait = true;
        break;
      }
    }

  if(!fWait && GetMCData(MC_CanStrike) && (caller->~ReadyToFire() || caller->~ReadyToAttack()) && !caller->~IsAiming() && (GetFMData(FM_Aim) == 0 || GetUser()->~IsAiming() || GetUser()->~AimOverride()))
  {
    var dir = GetDir(GetUser());
    //Ziele finden
    var obj = FindObjects(Find_InRect(-15+10*dir,-10,20,20), Find_Or(Find_OCF(OCF_Alive), Find_Func("IsMeleeTarget", this)), Find_NoContainer(), Find_Exclude(caller));
    for(var target in obj)
    {
      if(GetOCF(target) & OCF_Alive)
      {
        //Ziel feindlich?
        if(target && (Hostile(GetOwner(GetUser()), GetOwner(target)) || GetOwner(target) == NO_OWNER)) //Hier bewusst kein CheckEnemy, da wir auf FF-Checks verzichten
        {
          //Ziel am kriechen?
          if(target->~IsCrawling())
          {
            //Erhöhten Schaden verursachen
            DoDmg(GetMCData(MC_Damage)*3/2,DMG_Melee,target,0,GetController(GetUser())+1,GetID());
            //Ziel zum Aufstehen zwingen
            ObjectSetAction(target, "KneelUp");
          }
          else
          {
            //Schaden verursachen
            DoDmg(GetMCData(MC_Damage),DMG_Melee,target,0,GetController(GetUser())+1,GetID());
            SetCommand(GetUser(), "");
            SetComDir(COMD_None, GetUser());
  
            //Ziel schleudern
            var pwr = GetMCData(MC_Power), angle = GetMCData(MC_Angle);
            if(GetProcedure(target) != "SWIM")
            {
              if(!dir)
                dir--;
              SetXDir(Sin(angle*dir,pwr),target,10);
              SetYDir(-Cos(angle*dir,pwr),target,10);
              ObjectSetAction(target, "Tumble");
            }
          }

          //Achievement-Fortschritt (Fist of Fury)
          if(GetOCF(target) & OCF_CrewMember)
            DoAchievementProgress(1, AC36, GetOwner(GetUser())); 

          if(GetOwner(target) != NO_OWNER && Hostile(GetOwner(target), GetController(GetUser())))
            if(!GetAlive(target) || IsFakeDeath(target))
            {
              //Achievement-Fortschritt (Eat this!)
              DoAchievementProgress(1, AC14, GetOwner(GetUser()));
              //Ribbon-Fortschritt (The Tuna)
              AttemptAwardRibbon(RB07, GetOwner(GetUser()), GetOwner(target));
            }
          meleeattacked = true;
        }
      }
      else
      {
        target->~MeleeHit(this);
        meleeattacked = true;

        //Achievement-Fortschritt (Fly Swatter)
        if(target && target->~IsMAV() && target->~IsDestroyed() && (Abs(GetXDir(target)) + Abs(GetYDir(target)) >= 25))
          DoAchievementProgress(1, AC42, GetOwner(GetUser()));
      }
    }
  }
  if (meleeattacked)
  {
    //Soundeffekte
    Sound("ClonkMelee*.ogg", 0, this);
    Sound("WPN2_Punch*.ogg", 0, this);
    if(iAttachment == AT_Bayonet)
      Sound("BKHK_RotorHit*.ogg", 0, this);

    //Cooldown
    AddEffect("StrikeRecharge", this, 1, 1, this);
  }

  //Automatischen Schuss beenden, wenn erneut Werfen gedrückt
  if(IsRecharging())
  {
    if(!GetPlrCoreJumpAndRunControl(GetController(caller)))
    {
      StopAutoFire();
      if(GetFMData(FM_Auto) && !fWait && !meleeattacked) OnFireStop(firemode);
    }
    return 1;
  }

  //Kein Feuer erlauben wenn Kolbenschlag ausgeführt
  if(meleeattacked || fWait)
    return 1;

  //Unterstützt der Schussmodus das zielen aber es wird nicht gezielt?
  if(GetFMData(FM_Aim)>0 && !(GetUser()->~IsAiming()) && !(GetUser()->~AimOverride()))
  {
    //Bereit zum Zielen?
    if(GetUser()->~ReadyToAim())
      //Zielen starten
      GetUser()->StartAiming();

    //Nachladen?
    var ammoid = GetFMData(FM_AmmoID);
    var ammousage = GetFMData(FM_AmmoUsage);
    //Nachladen wenn möglich sofern Munition verbraucht
    if(!CheckAmmo(ammoid,ammousage,this))
      if(CheckAmmo(ammoid,ammousage,GetUser()))
      {
        Reload();
      }
      //Nicht genügend Munition
      else
      {
        //Unmöglichkeit des Nachladens angeben
        PlayerMessage(GetOwner(caller), "$NotEnoughAmmo$", caller, ammoid);
      }
    return 1;
  }

  //Feuern! Fehlgeschlagen?
  if(!Fire())
  {
    var ammoid = GetFMData(FM_AmmoID);
    var ammousage = GetFMData(FM_AmmoUsage);
    //Nachladen wenn möglich sofern Munition verbraucht
    if(!CheckAmmo(ammoid,ammousage,this))
      if(CheckAmmo(ammoid,ammousage,GetUser()))
      {
        Reload();
      }
      //Nicht genügend Munition
      else
      {
        //Unmöglichkeit des Nachladens angeben
        PlayerMessage(GetOwner(caller), "$NotEnoughAmmo$", caller, ammoid);
        Sound("WPN2_Empty.ogg");
      }
  }
  else
  {
    if(GetFMData(FM_BurstAmount) > 1 && !GetEffect("BurstFire", this))
    {
      var rechargetime = GetFMData(FM_BurstRecharge);
      if(rechargetime)
        AddEffect("BurstFire", this, 1, rechargetime, this, 0, GetFMData(FM_BurstAmount));
      else
      {
        for(var i = GetFMData(FM_BurstAmount); i > 0; i--)
          if(!Fire())
            return 1;
      }
    }
  }

  return 1;
}
