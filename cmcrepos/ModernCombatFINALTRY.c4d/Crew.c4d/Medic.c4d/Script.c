/*--- Der Sanitäter ---*/

#strict
#include PCMK

static const MDIC_HealMode_Auto       = 0;//Heilmodi-IDs
static const MDIC_HealMode_SingleHeal = 1;
static const MDIC_HealMode_GroupHeal  = 2;
static const MDIC_HealMode_Reanimate  = 3;

static const MDIC_TestDir = COMD_Stop;


public func IsMedic() {return(true);}

public func HealDistance(int iType)
{
  if(!iType) iType = heal_mode;

  if(iType == MDIC_HealMode_SingleHeal)
    return(20);
  if(iType == MDIC_HealMode_GroupHeal)
    return(80);
  if(iType == MDIC_HealMode_Reanimate)
    return(20);
}

public func HealSpeed(int iType)
{
  if(!iType) iType = heal_mode;

  if(iType == MDIC_HealMode_SingleHeal)
    return(3);
  if(iType == MDIC_HealMode_GroupHeal)
    return(1);
}

public func HealTimer(int iType)
{
  if(!iType) iType = heal_mode;

  if(iType == MDIC_HealMode_SingleHeal)
    return(10);
  if(iType == MDIC_HealMode_GroupHeal)
    return(10);
  if(iType == MDIC_HealMode_Reanimate)
    return(10);
}

local heal_mode;//Der Heilmodus.
local aPatients;//Array mit Patienten.

protected func Initialize()
{
  aPatients = CreateArray();
  return(inherited());
}

func AddPatient(object pClonk)
{
  var i = FindPatient(pClonk);
  if(i >= 0)
    return(i);

  i = GetLength(aPatients);
  aPatients[i] = pClonk;
  return(i);
}

func RemovePatient(object pClonk)
{
  var i = FindPatient(pClonk);
  if(i >= 0)
  {
    aPatients[i] = 0;
    CleanArray4K(aPatients);
    return(true);
  }
    
  return(false);
}

func FindPatient(object pClonk)
{
  for(var i = GetLength(aPatients); i >= 0; i--)
  {
    if(aPatients[i] == pClonk)
    {
      return(i);
    }
  }
  return(-1);
}

func ClearPatients()
{
  SetLength(aPatients,0);
}

public func ContextHealMenu(object pCaller)
{
  [$CtxHealMenu$|Image=_IN2|Condition=ReadyToHeal]
  HealMenu(pCaller);
  Sound("FirstAidPackCharge", 1, 0,0, GetOwner()+1);
}

public func ReadyToHeal()
{
  //TODO: Erweitern?
  if(FindContents(FAPK)||FindContents(CDBT))
    return(true);
}

public func HealMenu(object pCaller)
{
  CreateMenu(_IN2,pCaller,this(),0,"$CtxHealMenu$",0,C4MN_Style_Context);
  if(FindContents(FAPK))
  {
    AddMenuItem ("$MenHealSingle$","StartHeal(MDIC_HealMode_SingleHeal)",FAPK,pCaller,0,0,"$DescHealSingle$");
    AddMenuItem ("$MenHealGroup$","StartHeal(MDIC_HealMode_GroupHeal)",FAPK,pCaller,0,0,"$DescHealGroup$");
  }
  if(FindContents(CDBT))
    AddMenuItem ("$MenReanimate$","StartHeal(MDIC_HealMode_Reanimate)",CDBT,pCaller,0,0,"$DescReanimate$");
}

public func ControlThrow()
{
  if(IsHealing()) return(StopHealing());
  return(_inherited());
}

public func ControlDig()
{
  if(IsHealing()) return(StopHealing());
  return(_inherited());
}

public func ControlThrowDouble(){if(IsHealing()) return(StopHealing());return(_inherited());}
public func ControlDigDouble(){if(IsHealing()) return(StopHealing());return(_inherited());}
public func ControlUpDouble(){if(IsHealing()) return(StopHealing());return(_inherited());}
public func ControlDownDouble(){if(IsHealing()) return(StopHealing());return(_inherited());}
public func ControlLeftDouble(){if(IsHealing()) return(StopHealing());return(_inherited());}
public func ControlRightDouble(){if(IsHealing()) return(StopHealing());return(_inherited());}
public func ControlSpecial(){if(IsHealing()) return(StopHealing());return(_inherited());}

public func ControlUp()
{
  if(IsHealing()) return(StopHealing());
  if(GetPlrDownDouble(GetOwner()) && GetAction() eq "Walk")
  {
    StartHeal(MDIC_HealMode_Auto);
    return(true);
  }
  return(_inherited());
}

/*public func RejectCollect(id idObj, object pObj)
{
  // Spawnpunkt-Hack
  if(idObj == SPNP) return();
  // Munitionspaket?
  if(pObj ->~ IsAmmoPacket())
    // Davon kann man in jeden Fall _eines_ im Inventar haben
    if(!CustomContentsCount("IsAmmoPacket"))
      return(0);
  //Waffe?
  if(pObj ->~ IsWeapon()) {
    //Sonderbehandlung?
    if(!(pObj ->~ OnCollection(this()))) {
      //nein. Standardprozedur:
      //schon so eine Waffe im Inventar? Oder bereits 3 andere Waffen?
      if(ContentsCount(idObj) || CustomContentsCount("IsWeapon") >= WeaponCollectionLimit())
        return(1);  //Ja, nicht aufnehmen
      else
        return(0);
    }
    else
      return(0);
  }
  // Einsammellimit für Ausrüstung
  if(pObj ->~ IsEquipment() && ContentsCount(idObj) > 0)
    return(1);
  //Wieviel haben wir denn schon im inventar?
  if(ContentsCount() - CustomContentsCount("IsWeapon") >= ObjectCollectionLimit())
    return(1);
  for(var gear in aGear)
    if(GetID(gear) == idObj)
      return(1);
  // Ok
  return(0);
}*/

public func WeaponCollectionLimit() { return(2); } // max. Waffen im Inventar
public func ObjectCollectionLimit() { return(3); } // max. Objekte im Inventar

public func HazardGearSupported(object pGear)
{
  if(GetID(pGear) == HARM) return(false);//TODO: Diese Dinge checken.
  return(true);
}

/* Heilsystem */

local HealMark_R;
protected func Heal()
{
  if(heal_mode == MDIC_HealMode_GroupHeal)
  {
    CreateParticle("HealMark",0,0,Sin(HealMark_R,100),Cos(HealMark_R,100),HealDistance()*10,GetPlrColorDw(GetOwner()),this(),true);
    HealMark_R+=20;
    
    GroupHeal();
  }
  
  if(!GetLength(aPatients) || (GetComDir() != MDIC_TestDir))
    StopHealing();
}

public func StartHeal(int iType)
{
  if(!iType || iType == MDIC_HealMode_Auto)
  {
    iType = AutoHealSelect();
    if(iType && (iType != MDIC_HealMode_Auto))
      return(StartHeal(iType));
  }
  
  if(IsHealing())
  {
    StopHealing();
  }
  
  heal_mode = iType;
  
  //Heilmodis
  if(iType == MDIC_HealMode_GroupHeal)
    if(GroupHeal()) return(true);
  
  if(iType == MDIC_HealMode_SingleHeal)//TODO: Menü machen.
    if(SingleHeal()) return(true);

  if(iType == MDIC_HealMode_Reanimate)//TODO: Menü machen.
    if(Reanimate()) return(true);
    
  SetComDir(MDIC_TestDir);
  
  return(false);
}

public func AutoHealSelect()
{
  if(CheckReanimate())
    return(MDIC_HealMode_Reanimate);
    
  if(CheckSingleHeal())
    return(MDIC_HealMode_SingleHeal);
    
  if(CheckGroupHeal())
    return(MDIC_HealMode_GroupHeal);
}

//Reanimation
func CheckReanimate(object pClonk)
{
  if(!FindContents(CDBT)) return(false);
  if(!FindContents(CDBT)->Ready()) return(false);
  
  if(!pClonk)
  {
    pClonk = FindObject2(
             Find_OCF(OCF_CrewMember|OCF_Prey),
             Find_Distance(HealDistance(MDIC_HealMode_Reanimate)),
             Find_Exclude(this()),
             Find_Allied(GetOwner()));
  }
  
  if(!CheckHealTarget(pClonk))
    return(false);
    
  if(!IsFakeDeath(pClonk))
    return(false);
  
  return(pClonk);
}

func Reanimate(object pClonk)
{
  pClonk = CheckReanimate(pClonk);
  if(!pClonk) return(false);
  
  //Sound("HealingStart"); 
  SetAction("Reanimation");
  SetDir(GetX(pClonk));
  
  AddEffect("MDICReanimate",this(),150,HealTimer(MDIC_HealMode_Reanimate),this(),0,pClonk);
  
  return(true);
}

//Einzelheilung
func CheckSingleHeal(object pClonk)
{
  if(!FindContents(FAPK)) return(false);
  
  if(!pClonk)
  {
    pClonk = FindObject2(
             Find_OCF(OCF_CrewMember|OCF_Prey),
             Find_Distance(HealDistance(MDIC_HealMode_SingleHeal)),
             Find_Exclude(this()),
             Find_Allied(GetOwner()));
  }
  
  if(!CheckHealTarget(pClonk))
    return(false);
    
  return(pClonk);
}


func SingleHeal(object pClonk)
{
  pClonk = CheckSingleHeal(pClonk);
  if(!pClonk) return(false);
               
  SetAction("MedicHeal");
  Sound("HealingStart"); 
  SetDir(GetX(pClonk));

  AddEffect ("MDICHeal",pClonk,150,HealTimer(MDIC_HealMode_SingleHeal),this(),0,this(),HealSpeed(MDIC_HealMode_SingleHeal),MDIC_HealMode_SingleHeal);
    
  return(true);
}

//GRuppenheilung
func CheckGroupHeal()
{
  if(!FindContents(FAPK)) return(false);
  var b = false;

  var aClonks = FindObjects(
                Find_OCF(OCF_CrewMember|OCF_Prey),
                Find_Distance(HealDistance(MDIC_HealMode_GroupHeal)),
                Find_Exclude(this()),
                Find_Allied(GetOwner()));
  
  for(var pClonk in aClonks)
  {
    if(CheckHealTarget(pClonk))
    {
      b = true;
    }
  }
  
  if(b)
    if(GetLength(aClonks))
      return(aClonks);
    
  return(false);
}

func GroupHeal()
{
  var aClonks = CheckGroupHeal();
  if(!aClonks) return(false);

  for(var pClonk in aClonks)
  {
    if(CheckHealTarget(pClonk))
    {
      SetAction("MedicHeal");
      Sound("HealingStart"); 
  
      AddEffect ("MDICHeal",pClonk,150,HealTimer(MDIC_HealMode_GroupHeal),this(),0,this(),HealSpeed(MDIC_HealMode_GroupHeal),MDIC_HealMode_GroupHeal);
    }
  }
    
  return(false);
}

func CheckHealTarget(object pClonk)
{
  if(pClonk)
  {
    if(GetEnergy(pClonk) < (GetPhysical("Energy",0,pClonk)/1000))
    {
      if(!GetHealingEffect(pClonk))
      {
        return(true);
      }
    }
  }
  return(false);
}

func IsHealing()
{
  if((GetAction() eq "MedicHeal")||(GetAction() eq "Reanimate"))
    return(true);
}

func GetHealingEffect(object pTarget, int iValue)//Nicht fürs reanimieren!
{
  if(!pTarget) pTarget = this();
  return(GetEffect("MDICHeal",pTarget,0,iValue)); 
}

global func FxMDICHealStart(object pTarget, int iEffectNumber, int iTemp, pSani,iHealSpeed,iType)
{
/*
  Effektvariablen:
  0 = Sanitäter
  1 = HeilTyp
  2 = Heilgeschwindigkeit
  3 = HUD
*/
  EffectVar(0,pTarget,iEffectNumber) = pSani;
  EffectVar(1,pTarget,iEffectNumber) = iHealSpeed;
  EffectVar(2,pTarget,iEffectNumber) = iType;
  
  pSani->AddPatient(pTarget);
  
  var pHUD = CreateObject(1HHD,0,0,GetOwner(pSani));
  pHUD->Set(pTarget,_IN2,RGB(255,0,0),pSani);
  EffectVar(3,pTarget,iEffectNumber) = pHUD;
  
  var charge = GetEnergy(pTarget)*100/(GetPhysical("Energy",0,pTarget)/1000);
  pHUD->Update(charge,100);//Anzeige aktualisieren...
  
  Sound("HealingStart");
  
  RemoveEffect("*Poison",pTarget);//O,o ... na von mir aus.
}

global func FxMDICHealTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  var pSani = EffectVar(0,pTarget,iEffectNumber);
  if(!pSani) return(-1);
    
  if(ObjectDistance(pTarget,pSani)>=pSani->HealDistance(EffectVar(2,pTarget,iEffectNumber)))
    return(-1);
    
  if(GetEnergy(pTarget) >= GetPhysical("Energy",0,pTarget)/1000)
    return(-1);
    
  if(GetAction(pSani) ne "MedicHeal")
    return(-1);
   
  DoEnergy(EffectVar(1,pTarget,iEffectNumber),pTarget);//Nur wegen DIESER Codezeile gibts einen neuen Clonk! Lolz! >.<
  
  var charge = GetEnergy(pTarget)*100/(GetPhysical("Energy",0,pTarget)/1000);
  EffectVar(3,pTarget,iEffectNumber)->Update(charge,100);//Anzeige aktualisieren...

  //Effektgehasche!!11elf
  pTarget->CreateParticle("ShockWave",0,0,0,0,5*GetObjHeight(pTarget)+25+Sin(iEffectTime*2,25),RGB(210,20,20),pTarget);

  pSani->Sound("Healing*");
  if(!Random(5))
    pSani->CastParticles("Paper",1,8,2,6,10,12,RGBa(255,160,160),RGBa(255,255,255));
}

global func FxMDICHealStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  if(EffectVar(3,pTarget,iEffectNumber))
    EffectVar(3,pTarget,iEffectNumber)->Remove();//Toll ausfaden. ;)
  
  if(EffectVar(0,pTarget,iEffectNumber))
  {
    Sound("HealingEnd");
    EffectVar(0,pTarget,iEffectNumber)->Healed(pTarget);
  }
    
  EffectVar(0,pTarget,iEffectNumber)->RemovePatient(pTarget);
}

func Healed(object pClonk)
{
  //...
}

public func StopHealing()
{
  if((heal_mode == MDIC_HealMode_SingleHeal)
   ||(heal_mode == MDIC_HealMode_GroupHeal))
  {
    Sound("HealingEnd");
  }
  
  if(heal_mode == MDIC_HealMode_Reanimate)
  {
    //...
  }
  
  SetAction("Walk");
  SetComDir(COMD_Stop);
  heal_mode = 0;
    
  return(true);
}

/* Reanimiereffekt */
func IsReanimating()
{
  if((GetAction() eq "Reanimate") && GetEffect("MDICReanimate"))
    return(true);
}

public func FxMDICReanimateStart(object pTarget, int iEffectNumber, int iTemp, pClonk)
{
/*
  Effektvariablen:
  0 = Opfer *g*
  1 = HUD
  2 = Status (In Prozent!)
*/
  if(EffectCall(pTarget,iEffectNumber,"Check") == -1)
    return(-1);

  EffectVar(0,pTarget,iEffectNumber) = pClonk;
  pTarget->AddPatient(pClonk);
  
  var pHUD = CreateObject(1HHD,0,0,GetOwner(pTarget));
  pHUD->Set(pClonk,_IN3,RGB(0,128,255),pTarget);
  pHUD->Update(0,100);//Nötig?
  EffectVar(1,pTarget,iEffectNumber) = pHUD;
  
  PauseFakeDeath(true, pClonk);//Das währ sonst fies.
  
  Sound("MsgUsing",0,this());
}

public func FxMDICReanimateTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  if(EffectCall(pTarget,iEffectNumber,"Check") == -1)
    return(-1);
   
  EffectVar(2,pTarget,iEffectNumber) += GetEnergy(EffectVar(0,pTarget,iEffectNumber))/12;
  
  EffectVar(1,pTarget,iEffectNumber)->Update(EffectVar(2,pTarget,iEffectNumber),100);
  
  if(EffectVar(2,pTarget,iEffectNumber) >= 100)
  {
    if(EffectCall(pTarget,iEffectNumber,"Do") == -1)
      return(-1);
  }
  
  Sound("Heartbeat");//,EffectVar(0,pTarget,iEffectNumber));
  //DoEnergy(1,EffectVar(0,pTarget,iEffectNumber));//Sinn?
}

public func FxMDICReanimateCheck(object pTarget, int iEffectNumber)
{
  var pCDBT = FindContents(CDBT,pTarget);
  
  /*if(GetAction(EffectVar(0,pTarget,iEffectNumber)) ne "Dead")
    return(-1);*/
    
  if(GetAction(pTarget) ne "Reanimation")
    return(-1);
  
  if(!pCDBT)
    return(-1);
    
  if(!pCDBT->Ready())
    return(-1);
    
  if(ObjectDistance(pTarget,EffectVar(0,pTarget,iEffectNumber)) >= pTarget->HealDistance(MDIC_HealMode_Reanimate))
    return(-1);
}

public func FxMDICReanimateDo(object pTarget, int iEffectNumber)
{
  var pClonk = EffectVar(0,pTarget,iEffectNumber);
  var pCDBT = FindContents(CDBT,pTarget);
  
  pClonk->DoEnergy(10+Random(4)-1);//TODO: Doof.
  
  //Effektgehasche
  Sound("MsgYourOk*",0,pTarget); 
  Message("{{_IN1}} $MsgYourOk$",pTarget);
  
  SetComDir(COMD_Stop,pTarget);
  pTarget->SetAction("KneelUp");
  
  return(-1);
}

public func FxMDICReanimateStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  if(EffectVar(1,pTarget,iEffectNumber))
    EffectVar(1,pTarget,iEffectNumber)->Remove();
    
  var pClonk = EffectVar(0,pTarget,iEffectNumber);
    
  if(EffectVar(2,pTarget,iEffectNumber) < 100)
  {
    FakeDeath(pClonk);//zur Sicherheit
    PauseFakeDeath(false,pClonk);
  }
  else
  {
    StopFakeDeath(pClonk);
    pTarget->Reanimated(pClonk);
  }
    
  pTarget->RemovePatient(pClonk);
}

func Reanimated(object pClonk)
{
  //Tolle Nachrichten Ausgeben.
  var msg;
  
  if(GetController(pClonk) == GetController())//Gleicher Spieler?
  {
    msg = $MsgReanimationCrew$;
    msg = msg[Random(GetLength(msg))];
    Log(msg, GetTaggedPlayerName(GetController()));
  }
  else
  {
    msg = $MsgReanimation$;
    msg = msg[Random(GetLength(msg))];
    Log(msg, GetTaggedPlayerName(GetController()),GetTaggedPlayerName(GetController(pClonk)));
  }
}