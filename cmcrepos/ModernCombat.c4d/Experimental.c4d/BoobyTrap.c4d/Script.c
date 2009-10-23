/*-- Sprengfalle --*/

#strict 2

//Indikatoren

local bActive, iDir, controller, laser;

public func IsEquipment(){return(true);}
public func IsDrawable(){return(true);}
public func IsReloading(){return(false);}
public func CanAim() { return(true); }
public func IsShooting(){return(false);}//Nein, keine Automatikminen. O_o
public func IsRecharging(){return(false);}
public func IsMine(){return(true);}
public func NoArenaRemove(){return(true);}

public func Color(){return(RGB(200,200,200));}

public func HandX()    { return(0); }    // X-Position in der Hand
public func HandY()    { return(0); }    // Y-Position in der Hand
public func HandSize() { return(1000); } // Gr��e in der Hand, Standard: 1000
public func HandBarrel(){return(0); }    // Y-Offset des Laufs
public func BarrelXOffset(){return(0);}
public func BarrelYOffset(){return(0);}


/* Initalisierung */

func Initialize()
{
  SetAction("Defused"); 
  return(1);
}

func Hit()
{
  Sound("BBTP_Hit*.ogg");
}

///Waffe
public func ControlThrow(object caller)
{
  SetUser(caller);
  
  if(!Contained(GetUser()))
  {
    GetUser()->~CheckArmed();//Noch einmal schnell pr�fen.
    if(GetUser()->~ReadyToFire())
    {
      Throw();
      return(true);
    }
  }
  
  return(_inherited(...));
}

public func Throw()
{  
  var user = GetUser();
  iDir = user->AimAngle();
  
  SetController(GetOwner(user));
  Exit(0,0,10);
  Sound("BBTP_Activate.ogg");
  CreateParticle("PSpark",0,0,0,0,60,RGBa(255,0,0,0),this());
  ScheduleCall(0,"FinFuse", 2*36);
  
  //Grafisches Trallala
  SetAction("Fused");
  if(Inside(iDir,-180,-130)) SetPhase(3);
  if(Inside(iDir,-129,-78))  SetPhase(0);
  if(Inside(iDir,-77 ,-27))  SetPhase(1);
  if(Inside(iDir,-26 ,25))   SetPhase(2);
  if(Inside(iDir,26  ,76))   SetPhase(3);
  if(Inside(iDir,77  ,128))  SetPhase(0);
  if(Inside(iDir,129 ,179))  SetPhase(1);
  
  var nextmine = user->~GrabGrenade(GetID());
  user->~ResetShowWeapon(0);
  if(user->~IsAiming())
    if(!nextmine) user->StopAiming();
}

public func Activate(pCaller)
{
  pCaller->~StoreGrenade(this());
  HelpMessage(GetOwner(pCaller),"$Collected$",pCaller,GetID());
}

private func FinFuse()
{
  SetClrModulation(RGBa(255,255,255,80));
  CreateParticle("PSpark",0,0,0,0,60,RGBa(255,0,0,0),this());
  laser = CreateObject(LASR,0,0,GetOwner(controller));
  laser -> Set(iDir,3,60,0,0,this());
  laser -> SetClrModulation(RGBa(255,0,0,230));
  CreateObject(MFLG,0,-2,GetOwner(controller))->Set(this());

  bActive=true;
  Sound("BBTP_Alarm.ogg");
}

/* Deaktivierung */

public func ControlUp(object pObjBy)
{
  if(Contained()) return;
  if(!pObjBy->~RejectCollect(GetID(), this())) Enter(pObjBy);
  if(bActive)
    Defuse();
  Activate(pObjBy);
  return 1;
}

private func Defuse()
{
  SetAction("Defused");
  RemoveObject(laser);
  FindObject2(Find_ID(MFLG),Find_ActionTarget(this()))->RemoveObject();
  SetClrModulation();
  ClearScheduleCall();
  Sound("BBTP_Charge.ogg");
  bActive=false;
  return(1);
}

/* Gegnererkennung */

private func Check()
{
  if(!bActive) return;
  var pVictim;
  if( ObjectCount2(Find_OnLine(0,0,Sin(iDir,60,1),-Cos(iDir,60,1)), 
      Find_Func("CheckEnemy",this,0,true),
      Find_Not(Find_Distance(20)),
      Find_OCF(OCF_Alive),
      Find_NoContainer()) )
    Detonate();
  return 1;
}

public func Detonate()
{
  var i = 0;
  while(i < 7)
   {
    var ammo = CreateObject(SHT1,0,0,GetController());
    ammo->Launch(iDir+RandomX(-15,15),100+Random(80),100+Random(50),3,30,8);
    i++;
   }
  Sound("BBTP_Explosion1.ogg");
  RemoveObject(laser);
  RemoveObject();
}

/* Granaten-HUD */

func GetCharge()
{
  var user = GetUser();
  if(!user) return(0);
  if(!user->~MaxGrenades()) return(0);
  
  return(user->GrenadeCount(GetID())+1);
}

func CustomHUD() {return(true);}

func UpdateHUD(object pHUD)
{
  var user = GetUser();
  if(!user) return;
  if(!user->~MaxGrenades()) return 0;
  
  pHUD->Charge(user->GrenadeCount(GetID()),(user->MaxGrenades() - user->GrenadeCount()) + user->GrenadeCount(GetID()));
  pHUD->Ammo(user->GrenadeCount(GetID()),(user->MaxGrenades() - user->GrenadeCount()) + user->GrenadeCount(GetID()), GetName(), true);
}

/* Besitzererkennung */

public func GetUser()
{
  if(!controller)
    if(Contained())
      if(GetOCF(Contained()) & OCF_Alive)//*grummel* nicht sooo toll
        controller = Contained();
  
  return(controller);
}

public func SetUser(object pUser)
{
  controller = pUser;
  SetController(GetController(pUser));
}

/* Sonstiges */

protected func Selection()
{
  Sound("BBTP_Charge.ogg");
  return(1);
}

public func IsBulletTarget(){if(!Random(6)) return(true);}

protected func Damage(int iChange) 
{
  Detonate();
}
