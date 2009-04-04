/*--- HEV-Schutzanzug ---*/

#strict
#include GEAR

static const HEVS_MaxCharge = 100;
static const HEVS_BoostTime = 20;

local iCharge,pHUD,iDmg,pLight,pLight2,pUser,iBoostTime;

public func DoCharge(int iChange)
{
  iCharge = BoundBy(iCharge+iChange,0,HEVS_MaxCharge);
  UpdateHUD();
}

public func GetCharge()
{
  return(iCharge);
}

public func IsCharged()
{
  return(iCharge >= HEVS_MaxCharge);
}

/* Status */

public func GetGearType()	{ return(GEAR_Armor); }
protected func Hit()		{ Sound("ClonkHit*"); }
public func IsEquipment()	{ return(true); }

/* Callbacks */

public func GearEffect(object pClonk)
{
  pUser = pClonk;
  var iPortraitCount = 3; 
  var strGraphics = "Armor";
  SetGraphics("Weared",pClonk,GetID());
  SetPortrait("random", pClonk, GetID(), 0, 0);
  pHUD = CreateObject(VHUD,0,0,GetOwner());
  pHUD->Set(this(),pClonk);
  UpdateHUD();
  Sound("Connect");
}

public func GearEffectReset(object pClonk)
{
  pUser = 0;
  SetGraphics("",pClonk);
  SetPortrait(GetPortrait(pClonk, 0, 1), pClonk, GetPortrait(pClonk, 1, 1), 0, 0);
  if(pHUD)
    RemoveObject(pHUD);
  Sound("Connect");
  
  RemoveLight();
}

public func UpdateHUD()
{
  pHUD->Charge(iCharge*100/HEVS_MaxCharge);
  pHUD->Fuel(iCharge,HEVS_MaxCharge,GetName());
  pHUD->MasterFuel(H2BT,iCharge);
}

/* Damage reduction */

public func OnClonkDmg(int iDamage, int iType, object pClonk)
{
  if(!pClonk) return();
  if(iCharge <= 0) return();
  
  iDmg = iDamage;

  if(iType == DMG_Melee)
    return(60);

  if(iType == DMG_Projectile)
    return(100);

  if(iType == DMG_Fire)
    return(100);

  if(iType == DMG_Explosion)
    return(60);

  if(iType == DMG_Energy)
    return(60);

  if(iType == DMG_Bio)
    return(100);
}

public func OnClonkHit(int iDamage, int iType, object pClonk)
{
  if(!pClonk) return();
  if(iCharge <= 0) return();
  Message("%d",0,iDamage);
  if(iDamage <= 0) return();
 
  var v = iDmg;
  
  if(iType == DMG_Melee)
    v = iDmg*60/100;
  else
  if(iType == DMG_Projectile)
    v = iDmg/2;
  else
  if(iType == DMG_Fire)
    v = iDmg/2;
  else
  if(iType == DMG_Explosion)
    v = iDmg*60/100;
  else
  if(iType == DMG_Energy)
    v = iDmg*60/100;
  else
  if(iType == DMG_Bio)
    v = iDmg*20/100;

  DoCharge(-v/2);
}


/* OnBoard-Flashlight ;D */
public func ConDesc()
{
  if(pLight) return("$CtxLightOff$");
  return("$CtxLightOn$");
}

public func ConUseable()
{
  return(true);
  //return(IsDark());
}

private func FxIntUpdateLightTimer()
{
  pLight->ChangeR(pUser->~AimAngle());
}

private func FxIntLightEnergyTimer()
{
  DoCharge(-1);
  if(!iCharge)
  {
    RemoveLight();
    pUser->Sound("WPN2_Switch");
  }
}

public func ConUse(object pHazard)
{
  if(!pLight)
  {
    pUser = pHazard;
  
    if(!iCharge)
    {
      Click();
      ScheduleCall(this(),"Click",10);
      return();
    }
    
    pLight = AddLightCone(1000,RGBa(220,255,225,90),pUser);//Woah. Tolles LED-Licht. ;)
    if(!pLight) return();
    pLight->ChangeSizeXY(2000,7000);
    pLight->ChangeOffset(0,0,true);

    pLight2 = AddLight(160,RGBa(220,255,225,90),pUser);
    if(!pLight2) return();
    
    AddEffect("IntUpdateLight",this(),10,1,this());
    AddEffect("IntLightEnergy",this(),10,35*3,this());
  }
  else
  {
    RemoveLight();
  }

  pHazard->Sound("WPN2_Switch");
}

public func Click()
{
  pUser->Sound("WPN2_Switch");
}

private func RemoveLight()
{
  if(pLight) RemoveObject(pLight);
  if(pLight2) RemoveObject(pLight2);
  RemoveEffect("IntUpdateLight",this());
  RemoveEffect("IntLightEnergy",this());
}


/* Booster */
public func ControlRightDouble(object pCon)
{
  if(!(pCon->~HasGear(0,GetID()))) return(0);
  
  if(!CheckStart(pCon)) return(0);

  if(pCon->GetDir() == DIR_Right)
    return(FlyForward(pCon));
  else
    return(0);
}

public func ControlLeftDouble(object pCon)
{
  if(!(pCon->~HasGear(0,GetID()))) return(0);
  
  if(!CheckStart(pCon)) return(0);

  if(pCon->GetDir() == DIR_Left)
    return(FlyForward(pCon));
  else
    return(0);
}

public func FlyForward(object pClonk)
{
  if(iCharge < (HEVS_BoostTime)/2) return(0);
  
  var ydir = pClonk->GetYDir();
  pClonk->Jump();
  pClonk->SetXDir(pClonk->GetXDir()/2 + 50*(pClonk->GetDir()*2-1));
  pClonk->SetYDir(ydir/2 - 20);

  pClonk->~JumpStart();
  StartToFly(pClonk);
  
  return(1);
}

private func StartToFly(object pClonk)
{
  AddEffect("JetpackDelay", pClonk, 170, 3, this());
  AddEffect("Thrust", pClonk, 170, 1, this());
  iBoostTime = HEVS_BoostTime;
}

public func CheckStart(object pCon)
{
  if(!ReadyToFly(pCon)) return(0);
  return(1);
}

public func ReadyToFly(object pClonk)
{
  if(pClonk->GetAction() eq "Walk"
  || pClonk->GetAction() eq "WalkArmed")
  {
    if(GetEffect("JetpackDelay", pClonk))
      return(false);
    return(true);
  }
  return(false);
}


public func FxThrustStart(object pTarget, int iEffectNumber)
{
  pTarget->Sound("HEVS_Boost.ogg");
}

public func FxThrustTimer(object pTarget, int iEffectNumber, int iTime)
{
  if(!(iTime % 2))//statt 20 nur 10 ;)
    DoCharge(-1);

  var dist = Distance(0,0,GetXDir(pTarget),GetYDir(pTarget));
  var angle = Angle(0,0,GetXDir(pTarget),GetYDir(pTarget));
  var maxx = +Sin(angle,dist/10);
  var maxy = -Cos(angle,dist/10);
  var ptrdist = 66;

  for(var i=0; i<dist; i+=ptrdist)
  {
    var x = -maxx*i/dist+GetX(pTarget)-GetDir(pTarget)*20+10;
    var y = -maxy*i/dist+GetY(pTarget);

    CreateParticle("Smoke3",x,y,GetXDir(pTarget)/3,GetYDir(pTarget)/3,50+(iTime*150/HEVS_BoostTime),RGBa(255,255,255,127+(iTime*127/HEVS_BoostTime)),0,0);
  }
  
  if(iTime >= HEVS_BoostTime)
    return(-1);
}

public func FxJetpackDelayTimer(object pTarget, int iEffectNumber)
{
  iBoostTime--;
  if(iBoostTime <= 0) return(-1);
} 

public func FxJetpackDelayStop(object pTarget, int iEffectNumber){}