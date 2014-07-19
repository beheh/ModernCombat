 /*--- MAV ---*/

#strict 2
#include CVHC

local cur_Attachment;
local iXDir, iYDir;
local iXTendency, iYTendency;
local iBank;
local iBankMax;
local iSpeed;
local iPat_Dir;
local pMAVStation;
local iXDest, iYDest;
local fOutbound; //Auf dem Hinweg

public func AttractTracer(object pTracer)	{return GetPlayerTeam(GetController(pTracer)) != GetTeam() && !fDestroyed;}
public func IsDestroyed()			{return fDestroyed;}
public func ReadyToFire()			{return 1;}
public func IsMachine()				{return true;}
public func UpdateCharge()			{return 1;}
public func GetAttWeapon()			{return cur_Attachment;}
public func IsMAV()				{return true;}
public func IsCollector()	{return true;}
public func MaxDamage()				{return 60;}
public func IsRepairable()			{return !fDestroyed;}
public func IsMeleeTarget(object attacker)	{return !fDestroyed && Hostile(GetOwner(this), GetOwner(attacker));}
public func MeleeHit(object pWeapon)		{return DoDmg(MaxDamage()+1, DMG_Melee, this, 0, GetOwner(pWeapon));}
public func SensorDistance()			{return 190;}
public func IsActive()				{return GetAction(this) == "Flying";}
public func TeamSupportRange()			{return 80;}
public func IsBorderTarget()			{return true;}
//public func GetRealCursor()			{return IsActive() && pMAVStation->GetUser();}
public func DenyWeaponPathFreeCheck()		{return true;}
public func HasMoveOrder()	{return (iXDest >= 0 && iYDest >= 0);}
public func DeleteMoveOrder()	{iXDest = -1; iYDest = -1;}

public func Full()
{
	if(iXDest < 0 || iYDest < 0)
		pMAVStation->NextWaypoint(this);
}


public func MoveTo(int iX, int iY)
{
	if(iX < 0 || iY < 0)
		return;
		
	if(Distance(GetX(), GetY(), iX, iY) < 4)
	{
		iXDest = -1;
		iYDest = -1;
		iXTendency = 0;
		iYTendency = 0;
		pMAVStation->NextWaypoint(this);
		return;
	}
		
	iXDest = iX;
	iYDest = iY;

	if(!GetEffect("Flying", this))
		Start(pMAVStation);
	
	var iXDiff = iX - GetX();
	var iYDiff = iY - GetY();
	
	iXTendency = BoundBy(Sgn(iXDiff)*(iXDiff*iXDiff)/Abs(iYDiff), -iSpeed, iSpeed);
	iYTendency = BoundBy(Sgn(iYDiff)*(iYDiff*iYDiff)/Abs(iXDiff), -iSpeed, iSpeed);
	
	//Bremsweg = iXDir * iXDir/20
	if((iXDir * iXDir)/20 >= Abs(iXDiff))
		iXTendency = 0;
	if((iYDir * iYDir)/20 >= Abs(iYDiff))
		iYTendency = 0;
}

public func MaxRotLeft()
{
  return 120;
}

public func MaxRotRight()
{
  return 240;
}

public func IsBulletTarget()
{
  if(fDestroyed)
    return;
  else
    return DefaultBulletTarget(...);
}


/* Initialisierung */

protected func Initialize()
{
  //Standardwerte setzen
  iXDest = -1;
  iYDest = -1;
  iSpeed = 40;
  iBankMax = 13;

  Sound("MAVE_Engine.ogg", 0, 0, 70, 0, +1);

  SetAction("Wait");
}

/* Positionsbestimmung */
/*
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
*/
/* Aktivität */

local water_damage;

public func FxFlyingTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  //Bei Wasserkontakt Schaden nehmen
  if(GBackLiquid(0, 0) && (water_damage = !water_damage))
    DoDamage(1);

  if(GetAction() != "Flying") 
    return;

  SetController(GetOwner());

	MoveTo(iXDest, iYDest);

  //Flugverhalten
  /*
  if(iXDir < iXTendency)
    iXDir+= 1 - (fIsAiming && !(iEffectTime % 3));
  if(iXDir > iXTendency)
    iXDir-= 1 - (fIsAiming && !(iEffectTime % 3));
  if(iYDir < iYTendency)
    iYDir+= 1 - (fIsAiming && !(iEffectTime % 3));
  if(iYDir > iYTendency)
    iYDir-= 1 - (fIsAiming && !(iEffectTime % 3)) + (iYDir>iSpeed);*/
  if(iXDir < iXTendency)
    iXDir+= 1;
  if(iXDir > iXTendency)
    iXDir-= 1;
  if(iYDir < iYTendency)
    iYDir+= 1;
  if(iYDir > iYTendency)
    iYDir-= 1;

  if(GetY() <= GetDefCoreVal("Offset", "DefCore", WMAV, 1) * -1 && iYTendency <= 0)
  {
    iYTendency = 0;
    iYDir = BoundBy((GetY() + GetDefCoreVal("Offset", "DefCore", WMAV, 1))*-1, 1, 10);
  }

  if(!iYTendency && !iYDir)
    iYDir += Sin(iEffectTime * 8, 2);
  
  //Widerstand im Wasser
  var resistance = 0;
  if(GBackLiquid())
    resistance = 2;

  SetXDir(iXDir - (resistance) * 10 * iXDir / iSpeed);

  if(iYDir<=0)
    SetYDir(iYDir - (resistance) * 10 * iYDir / iSpeed - 2);
  else
    SetYDir(iYDir + (resistance) * 10 * iYDir / iSpeed - 2);

  //Je nach Flugrichtung drehen
  if(iXTendency == 0)
    iBank -= iBank / Abs(iBank);
  else
    if(Abs(iBank) < iBankMax)
      iBank += iXTendency / Abs(iXTendency);

    SetObjDrawTransform(1000, -iBank*20, 0, iBank*20, 1000, 0, this);

  //Partikeleffekte
  for(var i = 0; i < 5; i++)
  {
    var iXParticle = RandomX(-4, 4) + GetXDir() * 3 / 4;
    var iYParticle = RandomX(-9, 9) + 10 - (iYTendency - iYDir) * 3 / 4;

    //Im Wasser: Luftblasen
    if(GBackLiquid(0, 0))
    {
      var bubble = CreateObject(FXU1, 6 - GetXDir() / 10,  - 8*(iYTendency > 0) + 2*Sgn(iBank-Sgn(iBank)) - iYParticle / 6, -1);
      if(bubble)
      {
        bubble->SetXDir(iXParticle);
        bubble->SetYDir(iYParticle);
      }
      bubble = CreateObject(FXU1, -7 - GetXDir() / 10,  - 8*(iYTendency > 0) - 2*Sgn(iBank-Sgn(iBank)) - iYParticle / 6, -1);
      if(bubble)
      {
        bubble->SetXDir(iXParticle);
        bubble->SetYDir(iYParticle);
      }
      //Objekte sparen...
      break;
    }
    else
    {
      CreateParticle("PSpark", 6 - GetXDir() / 10, - 8*(iYTendency > 0) + 2*Sgn(iBank-Sgn(iBank)) - iYParticle / 6, iXParticle, iYParticle, 15, RGBa(200, 200, 255, 35));
      CreateParticle("PSpark", -7 - GetXDir() / 10, - 8*(iYTendency > 0) - 2*Sgn(iBank-Sgn(iBank)) - iYParticle / 6, iXParticle, iYParticle, 15, RGBa(200, 200, 255, 35));
    }
  }

  //Nachladen und Sensorchecks (alle 5 Frames)
  if(!(iEffectTime % 5))
  {
    
    //Blinklicht (alle 30 Frames)
    if(!(iEffectTime % 30))
    {
      //Farbe ermitteln
      if(GetTeam())
        var rgb = GetTeamColor(GetTeam());
      else
        if(GetOwner())
          var rgb = GetPlrColorDw(GetOwner());
        else
          var rgb = RGB(255, 255, 255);
      CreateParticle("FlashLight", 0, 4, 0, 0 , 45, rgb, this);
    }
  }

  //Namensanzeige für Verbündete
  for(var first = true, iFlags, i = 0; i < GetPlayerCount(); i++)
  {
    var iPlr = GetPlayerByIndex(i);
    if(pMAVStation && !Hostile(GetOwner(), iPlr) && GetOwner(this) != iPlr)
    {
      if(first)
        first = false;
      else
        iFlags = MSG_Multiple;
      var szStr = Format("@%s (%s)", GetName(pMAVStation->GetUser()), GetPlayerName(GetOwner()));
      CustomMessage(szStr, cur_Attachment, iPlr, 0, 0, SetRGBaValue(GetPlrColorDw(GetOwner()), 128), 0, 0, iFlags);
    }
  }

  //Schadensverhalten
  DamageChecks();
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
  if(iDmg > 0 && !fDestroyed && GetAction() == "Flying") Sound("WarningDamage.ogg",0,0,0,GetOwner()+1);

  if(iType == DMG_Melee)	return 0;	//Melee
  if(iType == DMG_Fire)		return 40;	//Feuer
  if(iType == DMG_Energy)	return 40;	//Energiewaffen
  if(iType == DMG_Projectile)	return 55;	//Kugeln
  if(iType == DMG_Explosion)	return 0;	//Explosionen

  return 50;
}

public func OnHit(int iDmg, int iType, object pBy)
{
  _inherited();

  //Negativer Schaden erzeugt keinen Rückstoß
  if(iDmg < 0)
    return true;

  iXDir-= Sin(Angle(GetX(), GetY(), GetX(pBy), GetY(pBy)), BoundBy(iDmg*3/2, 10, 40));
  iYDir+= Cos(Angle(GetX(), GetY(), GetX(pBy), GetY(pBy)), BoundBy(iDmg*3/2, 10, 40));
  return true;
}

/* Zerstörung */

public func BorderDestruction()
{
  return OnDestruction();
}

public func OnDestruction()
{

  //Explosion
  FakeExplode(4, GetController()+1, this);

  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter",3,100,0,0,0,80,100);

  //Sound
  Sound("MISL_ShotDown.ogg");

  //Deaktivieren
  Wait();
  ChangeDef(BDMV);
  this->~Initialize();
}

/* MAV-Funktionen */

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

public func Start(object pStation)
{
  iXDir = GetXDir();
  iYDir = GetYDir();

  pMAVStation = pStation;

  SetAction("Flying");
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
  }

  iXTendency = 0;
  iYTendency = 0;
  iXDir = 0;
  iYDir = 0;

  Sound("MAVE_Engine.ogg", 0, 0, 70, 0, -1);
  Sound("BWTH_Repair.ogg", false, this, 100, 0, -1);

  CustomMessage("", cur_Attachment, NO_OWNER);
}

/* Steuerung */

public func ControlLeft(pByObj)
{
  if(GetActionTarget(0, pByObj) == this) return false;

  if(GetAction() == "Wait")
    return true;

  if(iXTendency > 0)
    iXTendency = 0;
  else
    iXTendency = -iSpeed;

  return true;
}

public func ControlLeftDouble(pByObj)
{
  if(GetActionTarget(0, pByObj) == this) return false;

  if(GetAction() == "Wait")
    return true;

  iXTendency = -iSpeed;

  return true;
}

public func ControlLeftReleased(pByObj)
{
  if(GetActionTarget(0, pByObj) == this) return false;

  if(iXTendency < 0) iXTendency = 0;
  return true;
}

public func ControlRight(pByObj)
{
  if(GetActionTarget(0, pByObj) == this) return false;

  if(GetAction() == "Wait")
    return true;

  if(iXTendency < 0)
    iXTendency = 0;
  else
    iXTendency = iSpeed;

  return true;
}

public func ControlRightDouble(pByObj)
{
  if(GetActionTarget(0, pByObj) == this) return false;

  if(GetAction() == "Wait")
    return true;

  iXTendency = iSpeed;

  return true;
}

public func ControlRightReleased(pByObj)
{
  if(GetActionTarget(0, pByObj) == this) return false;

  if(iXTendency > 0) iXTendency = 0;
  return true;
}

public func ControlDown(pByObj)
{
  if(GetActionTarget(0, pByObj) == this) return false;

  if(GetAction() == "Wait")
    return true;

  if(iYTendency < 0)
    iYTendency = 0;
  else
    iYTendency = iSpeed;

  return true;
}

public func ControlDownReleased(pByObj)
{
  if(GetActionTarget(0, pByObj) == this) return false;

  if(iYTendency > 0) iYTendency = 0;
  return true;
}

public func ControlDownDouble(pByObj)
{
  if(GetActionTarget(0, pByObj) == this) return false;

  if(GetAction() == "Wait")
    return true;

  iYTendency = iSpeed;

  return true;
}

public func ControlUp(object pByObj)
{
  if(GetActionTarget(0, pByObj) == this) return false;

  if(GetAction() == "Wait")
    return true;

  if(iYTendency > 0)
    iYTendency = 0;
  else
    iYTendency = -iSpeed;

  return true;
}

public func ControlUpReleased(pByObj)
{
  if(GetActionTarget(0, pByObj) == this) return false;

  if(iYTendency < 0) iYTendency = 0;
  return true;
}

public func ControlUpDouble(pByObj)
{
  if(GetActionTarget(0, pByObj) == this) return false;

  if(GetAction() == "Wait")
    return true;

  iYTendency = -iSpeed;

  return true;
}

public func ControlThrow(pByObj)
{

  //MAV aktivieren
  if(GetAction() == "Wait")
  {
    Start(pMAVStation);
    return true;
  }

}

/* Cooldown-Effekte */

public func FxNoTargetCooldownTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  RemoveEffect("NoTargetCooldown", this);
}

public func FxMeleeCooldownTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  RemoveEffect("MeleeCooldown", this);
}


/* Kontaktfunktionen */

private func ContactHitCheck(int x, int y, int x2)
{
  if((Abs(GetXDir()) + Abs(GetYDir()) >= 25) && !GetEffect("MeleeCooldown", this))
  {
    var target = FindObject2(Find_AtPoint(x, y), Find_Func("IsMeleeTarget", this), Find_NoContainer(), Find_Exclude(this), Find_Not(Find_Func("HitExclude", this)));

    //Unten gibt es 2 Vertices, an denen man anstoßen kann; daher müssen im Zweifelsfall beide geprüft werden (x2)
    if(!target)
      target = FindObject2(Find_AtPoint(x2, y), Find_Func("IsMeleeTarget", this), Find_NoContainer(), Find_Exclude(this), Find_Not(Find_Func("HitExclude", this)));

    if(target && (Hostile(GetOwner(this), GetOwner(target)) || GetOwner(target) == NO_OWNER))
    {
      DoDmg(20, DMG_Melee, target, 0, GetController(this)+1, GetID());
      AddEffect("MeleeCooldown", this, 1, 30);

      //Gesonderter Schaden bei feindlichen MAVs
      if(target && target->~IsMAV())
      {
        target->DoDmg(20, DMG_Melee, this, 0, GetController(target)+1, GetID(target));
        AddEffect("MeleeCooldown", target, 1, 30);
      }

      //Effekte
      Sound("VehicleHit*.ogg");
      Sound("WPN2_Punch*.ogg");
      if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter",2,40,0,0,0,50,60);
      Sparks(Random(2)+2,RGB(255,255,Random(5)+255));

      return true;
    }
  }
}

public func ContactLeft()
{
  if(ContactHitCheck(GetDefCoreVal("VertexX", "DefCore", GetID(), 3), GetDefCoreVal("VertexY", "DefCore", GetID(), 3)))
    return true;

  if(GetXDir()<=-30)
    Collision(GetXDir());

  iXDir = 0;
  if(iXTendency < 0)
    iXTendency = 0;
}

public func ContactRight()
{
  if(ContactHitCheck(GetDefCoreVal("VertexX", "DefCore", GetID(), 4), GetDefCoreVal("VertexY", "DefCore", GetID(), 4)))
    return true;

  if(GetXDir()>=30)
    Collision(GetXDir());

  iXDir = 0;
  if(iXTendency > 0)
    iXTendency = 0;
}

public func ContactTop()
{
  if(ContactHitCheck(GetDefCoreVal("VertexX", "DefCore", GetID(), 5), GetDefCoreVal("VertexY", "DefCore", GetID(), 5)))
    return true;

  if(GetYDir()<=-30)
    Collision(GetYDir());

  iYDir = 0;
  if(iYTendency < 0)
    iYTendency = 0;
}

public func ContactBottom()
{
  if(ContactHitCheck(GetDefCoreVal("VertexX", "DefCore", GetID(), 1), GetDefCoreVal("VertexY", "DefCore", GetID(), 1), GetDefCoreVal("VertexX", "DefCore", GetID(), 2)))
    return true;

  if(GetYDir()>=30)
    Collision(GetYDir());

  iYDir = 0;
  if(iYTendency > 0)
    iYTendency = 0;
}

/* Aufschlag */

protected func Collision(int iCollSpeed)
{
  iCollSpeed = Abs(iCollSpeed);

  //Effekte
  Sparks(Random(2)+2,RGB(255,255,Random(5)+255));
  Sound("VehicleHit*.ogg");
  DoDmg(iCollSpeed/4);
}

protected func Hit2()
{
  //Effekte
  Sound("VehicleHit*.ogg");
}
