/*-- Sprengfalle --*/

#strict 2

//Indikatoren

local bActive,iDir,controller, laser;

public func IsEquipment(){return(true);}
public func IsDrawable(){return(true);}
public func IsReloading(){return(false);}
public func IsShooting(){return(false);}//Nein, keine Automatikminen. O_o
public func IsRecharging(){return(false);}
public func IsMine(){return(true);}
public func NoArenaRemove(){return(true);}
public func Color(){return(RGB(200,200,200));}
public func HandX()    { return(0); }    // X-Position in der Hand
public func HandY()    { return(0); }    // Y-Position in der Hand
public func HandSize() { return(1000); } // Größe in der Hand, Standard: 1000
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

/* Aktivierung */

public func Activate(object pObjBy)
{
  if(!Contained()) return 0;
  SetUser(pObjBy);
  SetOwner(pObjBy);
  CreateMenu(APMI, pObjBy, this());
  AddMenuItem("$MnuLeft$", "Fuse", LMM2, pObjBy, 0, -90, 0, 2, 0);
  AddMenuItem("$MnuLeftUp$", "Fuse", LMM2, pObjBy, 0, -45, 0, 2, 1);
  AddMenuItem("$MnuUp$", "Fuse", LMM2, pObjBy, 0, 0, 0, 2, 2);
  AddMenuItem("$MnuRightUp$", "Fuse", LMM2, pObjBy, 0, 45, 0, 2, 3);
  AddMenuItem("$MnuRight$", "Fuse", LMM2, pObjBy, 0, 90, 0, 2, 4);
  AddMenuItem("","",LMM2, pObjBy, 0, 0, 0, 2, 8);
  AddMenuItem("$MnuLeftDown$", "Fuse", LMM2, pObjBy, 0, -135, 0, 2, 5);
  AddMenuItem("$Store$", "MnuStore", APMI, pObjBy, 0, pObjBy);
  AddMenuItem("$MnuRightDown$", "Fuse", LMM2, pObjBy, 0, 135, 0, 2, 7);
  AddMenuItem("","",LMM2, pObjBy, 0, 0, 0, 2, 8);
  SelectMenuItem(7, pObjBy); 
  return 1;
}

public func MnuStore(a, pCaller)
{
  pCaller->~StoreGrenade(this());
  HelpMessage(GetOwner(pCaller),"$Collected$",pCaller,GetID());
}

private func Fuse(a, int iAngle)
{
  SetAction("Fused");
  if(iAngle==-135) SetPhase(3);
  if(iAngle==-90) SetPhase(0);
  if(iAngle==-45) SetPhase(1);
  if(iAngle==0) SetPhase(2);
  if(iAngle==45) SetPhase(3);
  if(iAngle==90) SetPhase(0);
  if(iAngle==135) SetPhase(1);
  iDir=iAngle;
  ScheduleCall(0,"FinFuse", 2*36);
  Sound("BBTP_Activate.ogg");
  CreateParticle("PSpark",0,0,0,0,100,RGBa(255,0,0,0),this());
  Exit(0,0,5);
  GetUser()->~GrabGrenade(GetID());
  GetUser()->~ResetShowWeapon(0);
  return(1);
}

private func FinFuse()
{
  SetClrModulation(RGBa(255,255,255,80));
  CreateParticle("PSpark",0,0,0,0,100,RGBa(255,0,0,0),this());
  laser = CreateObject(LASR,0,0,GetOwner());
  laser -> Set(iDir,3,60,0,0,this());
  laser -> SetClrModulation(RGBa(255,0,0,230));
  CreateObject(MFLG,0,-2,GetOwner())->Set(this());

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