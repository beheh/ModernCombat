/*-- Sprengfalle --*/

#strict 2

local bActive, bReady, iDir, controller, laser;

public func IsDrawable()	{return true;}
public func IsReloading()	{return false;}
public func CanAim()		{return !bActive;}
public func IsShooting()	{return false;}
public func IsRecharging()	{return false;}
public func IsMine()		{return true;}
public func Color()		{return RGB(200,200,200);}
public func IsBulletTarget()	{return !Random(6);}
public func HandX()		{return 5000;}
public func HandY()		{return;}
public func HandSize()		{return 1000;}
public func HandBarrel()	{return;}
public func BarrelXOffset()	{return -850;}
public func BarrelYOffset()	{return;}
public func IsEquipment()	{return true;}
public func NoArenaRemove()	{return true;}


/* Initalisierung */

func Initialize()
{
  //inaktiv erstellen
  SetAction("Defused"); 
  return 1;
}

/* Steuerung */

public func ControlThrow(object caller)
{
  //Wird nicht getragen: Werfen gesperrt
  if(!Contained()) return;

  //Träger = Besitzer
  controller = GetOwner(caller);
  SetController(controller);
  SetOwner(controller);

  if(!Contained(caller))
  {
    caller->~CheckArmed();
    if(Contained()->~ReadyToFire())
    {
      Throw();
      return true;
    }
  }
  return _inherited(...);
}

public func Throw()
{
  //Winkel übernehmen
  var user = Contained();
  iDir = user->AimAngle();

  //Clonk verlassen
  if(GetAction(user) == "Crawl")
    if(GetDir(user) == DIR_Left)
      Exit(0,-10,5);
    else
      Exit(0,10,5);
  else
    Exit(0,0,10);

  //Effekte
  Sound("BBTP_Activate.ogg", 0, 0, 50);
  CreateParticle("PSpark",0,0,0,0,60,GetPlrColorDw(GetOwner()),this);

  //Aktivierung
  bActive=true;
  ScheduleCall(0,"FinFuse", 2*36);

  //Grafik setzen
  SetAction("Fused");
  if(Inside(iDir,-180,-130)) SetPhase(3);
  if(Inside(iDir,-129,-78))  SetPhase(0);
  if(Inside(iDir,-77 ,-27))  SetPhase(1);
  if(Inside(iDir,-26 ,25))   SetPhase(2);
  if(Inside(iDir,26  ,76))   SetPhase(3);
  if(Inside(iDir,77  ,128))  SetPhase(0);
  if(Inside(iDir,129 ,179))  SetPhase(1);
  
  var nextmine = user->~GrabGrenade(GetID());
  user->~ResetShowWeapon();
  if(user->~IsAiming())
    if(!nextmine) user->StopAiming();
}

private func FinFuse()
{
  if(!bActive) return;
  SetClrModulation(RGBa(255,255,255,80));
  CreateParticle("PSpark",0,0,0,0,60,GetPlrColorDw(GetOwner()),this);
  laser = CreateObject(LASR,0,0,controller);
  laser -> Set(iDir,3,60,0,0,this());
  if(laser) { //Falls er im Boden steckt
    laser -> SetClrModulation(DoRGBaValue(GetPlrColorDw(GetOwner()), 180, 0));
    laser ->~ Destruction();
  }
  CreateObject(MFLG,0,1,controller)->Set(this);

  bReady=true;
  Sound("BBTP_Alarm.ogg", 0, 0, 50);
}

/* Deaktivierung */

public func ControlUp(object pObjBy)
{
  if(Contained()) return;
  if(pObjBy->~RejectCollect(GetID(), this)) return;
  
  //Punkte bei Belohnungssystem
  if(Hostile(GetOwner(),GetOwner(pObjBy))) 
    DoPlayerPoints(BonusPoints("TechnicalTask"), RWDS_TeamPoints, GetOwner(pObjBy), pObjBy, IC15);

  //Träger = Besitzer
  controller = GetOwner(pObjBy);
  SetController(controller);
  SetOwner(controller);

  bActive=false;
  
  Enter(pObjBy);

  //Eventuell sichern
  if(bReady)
   Defuse();
  //Aufnehmen
  Collect(pObjBy);
  
  return 1;
}

private func Defuse()
{
  SetAction("Defused");
  RemoveObject(laser);
  var flag = FindObject2(Find_ID(MFLG),Find_ActionTarget(this));
  if(flag) RemoveObject(flag);
  SetClrModulation();
  Sound("BBTP_Charge.ogg");
  bReady=false;
  return 1;
}

/* Gegnererkennung */

private func Check()
{
  //Nicht aktiv, nicht suchen
  if(!bReady) return;

  var pVictim;
  if( ObjectCount2(Find_OnLine(0,0,Sin(iDir,80,1),-Cos(iDir,80,1)), 
      Find_Func("CheckEnemy",this,0,true),
      Find_Not(Find_Distance(20)),
      Find_OCF(OCF_Alive),
      Find_NoContainer()) )
    Detonate();
  return 1;
}

/* Zerstörung */

protected func Damage(int iChange) 
{
  //Zündung durch Schaden
  if(GetDamage() < 5) return ;
  Sound("BBTP_Alarm.ogg");
  //Zündung verzögert
  ScheduleCall(this, "Detonate",20);
  //Sofortige Zündung bei hohem Schaden
  if(GetDamage() < 10) return ;
  Detonate();
}

public func Detonate()
{
  //Splitter verschleudern
  var i = 0;
  while(i < 12)
  {
   var ammo = CreateObject(SHRP,0,0,GetOwner());
   ammo->Launch(iDir+RandomX(-15,15),100+Random(80),100+Random(50),3,20,50);
   i++;
  }

  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastParticles("Smoke3",12,10,0,0,100,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter",2,150,0,0,45,30,RGB(40,20,20));
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter",2,100,0,0,30,80);
  Sound("BBTP_Explosion.ogg");

  //Entfernung
  RemoveObject(laser);
  RemoveObject();
}

/* Schaden */

public func OnDmg(int iDmg, int iType)
{
  if(iType == DMG_Bio)	return 100;	//Säure und biologische Schadstoffe
}

/* Sonstiges */

public func RejectEntrance(object pObj)
{
	if(bActive) return true;
  if(GetOCF(pObj) & OCF_Living)
  {
   if(ContentsCount(GetID(),pObj))
    return true;
  }
  return false;
}

protected func Selection()
{
  Sound("BBTP_Charge.ogg");
  return 1;
}

func Hit()
{
  Sound("BBTP_Hit*.ogg");
}

public func Destruction()
{
  //Entfernung
  if(laser)
   RemoveObject(laser);
}
