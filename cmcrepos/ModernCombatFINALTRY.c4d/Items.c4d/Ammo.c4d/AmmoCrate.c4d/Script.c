/*-- Munitionskiste --*/

#strict

local idSpawn, iMaxCount, iRespawnTime, iTakeTime;

//Indikator
public func IsSpawnpoint() { return(true); }

protected func Initialize()
{
  SetAction("Closed");
  iRespawnTime = 35*30;
  iTakeTime = 35*3;
  iMaxCount = 3;
  idSpawn = NONE;
  
  return(1);
}

private func PreDef(id idType)
{
  if(!idType)
  {
    iRespawnTime = 35*30;
    iTakeTime = 35*2;
    iMaxCount = 4;
  }
  else if(idType->~IsRifleLaunchedGrenade())
  {
    iRespawnTime = 35*20;
    iTakeTime = 35*1;
    iMaxCount = 4;
  }
  else if(idType->~IsAmmoPacket())
  {
    iRespawnTime = 35*20;
    iTakeTime = 35*2;
    iMaxCount = 3;
  }
  else if(idType->~IsGrenade())
  {
    iRespawnTime = 35*30;
    iTakeTime = 35*2;
    iMaxCount = 3;
  }
  else if(idType == FAPK)
  {
    iRespawnTime = 35*30;
    iTakeTime = 35*3;
    iMaxCount = 2;
  }
  else if(idType == CDBT)
  {
    iRespawnTime = 35*30;
    iTakeTime = 35*3;
    iMaxCount = 2;
  }
  
  iRespawnTime = 35*30;
  iTakeTime = 35*2;
  iMaxCount = 4;
}

public func Set(id idType, int iMax, int iTake, int iRespawn)
{
  PreDef(idType);
  
  if(iRespawn) iRespawnTime = iRespawn;
  if(iTake) iTakeTime = iTake;
  if(iMax) iMaxCount = iMax;

  idSpawn = idType;
  
  RemoveEffect("IntRespawn",this());
  
  if(iRespawnTime && idSpawn)
    AddEffect("IntRespawn", this(), 10, iRespawnTime, this());
   
  if(idSpawn->~IsAmmoPacket())
    SetIcon(idSpawn->AmmoID());//:o
  else
    SetIcon(idSpawn);
  
  return(this());
}

public func SetIcon(id idIcon)
{
  if(idIcon)
  {
    SetGraphics(0,0,idIcon,1,GFXOV_MODE_IngamePicture);
    
    /*if((GetDefWidth(idIcon) > 6)||(GetDefHeight(idIcon) > 6))
    {
      w = 6*1000/GetDefWidth(idIcon);
      h = 6*1000/GetDefHeight(idIcon);
    }*/
    
    var w = 7*1000/GetObjHeight(); 
    var h = 7*1000/GetObjHeight();
    
    SetObjDrawTransform(w,0,+4000,0,h,0,this(),1);
  }
  else
  {
    SetGraphics(0,0,0,1);
  }
}

protected func Open()
{
  if (GetAction() ne "Closed") return();
  SetAction("Opening");
}
  
protected func Close()
{
  if (GetAction() ne "Open") return();  
  SetAction("Closing");	    
}

protected func Grabbed(object pClonk, bool fGrab)
{
  if(!CheckGrab()) return();
  if(idSpawn && pClonk && fGrab && (GetAction() eq "Open")) CheckResupply(pClonk,false);
}

protected func GrabLost(object pClonk)
{
  CheckGrab();
}

private func CheckGrab()
{
  if(FindObject(NOAM)) return(false);

  // Die Truhe soll sich selbst öffnen, wenn (mindestens) ein Clonk sie anfasst...
  if (FindObject(0,0,0,0,0,OCF_CrewMember(),"Push",this()))
  {
    Open();
    return(true);
  }
  else
  {
    Close();
    return(false);
  }
}

protected func Opening() 
{
  Sound("AMCT_Open.ogg"); 
}

protected func Opened() 
{
  if(!CheckGrab() || !idSpawn) return();
  
  //Allen Clonks die die Kiste anfassen und noch keinen Resupply-Effekt haben, einen geben.
  for(var pClonk in FindObjects(Find_Action("Push"),Find_ActionTarget(this())))
  {
    CheckResupply(pClonk,true);
  }
}

protected func CheckResupply(object pClonk, bool bStart)
{
  if(!GetEffect("IntResupply",pClonk))
  {
    AddEffect("IntResupply", pClonk, 10, iTakeTime, 0, GetID(), this(), bStart);
  }
}
  
protected func Closing() 
{
  Sound("AMCT_Close.ogg");
}

public func RejectCollect(id idObj)
{
  if(GetAction() ne "Open") return(true);

  if(idSpawn)
  {
    if(idSpawn != idObj) return(true);
    if(ContentsCount() >= iMaxCount) return(true);
  }
  else
  {
    if(ContentsCount() >= 10) return(true);
  }

  return(false);
}

protected func ControlDig(object pClonk)
{
  if(GetAction() ne "Open") return(1);
  
  if(idSpawn) return(1);

  pClonk->SetCommand(0, "Get", this(), 0,0, 0, 1);
  
  /*if(!idSpawn)
  {
    pClonk->SetCommand(0, "Get", this(), 0,0, 0, 1);
  }
  else
  {
    //...
    Message("%s kann nicht {{%i}} aus der Kiste nehmen.",pClonk,GetName(pClonk),idSpawn);
  }*/
}

public func GetMaxCount()
{
  return(iMaxCount);
}

public func GetSpawnID()
{
  return(idSpawn);
}

/* Effekte */
public func FxIntRespawnStart(object pTarget, int iEffectNumber, int iTemp)
{
  if(!idSpawn) return(-1);
  while(Contents()) RemoveObject(Contents());//Leeren.
  CreateContents(idSpawn,0,iMaxCount);//Füllen.
}

public func FxIntRespawnTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  if(ContentsCount() < iMaxCount)
  {
    CreateContents(idSpawn);
  }
}

public func FxIntRespawnStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  while(Contents()) RemoveObject(Contents());//Leeren.
}


//für Clonks
public func FxIntResupplyStart(object pTarget, int iEffectNumber, int iTemp, pCrate, bStartTake)
{
  if(!pCrate) return(-1);
  EffectVar(0,pTarget,iEffectNumber) = pCrate; 
  
  if(bStartTake)
  {
    if(FxIntResupplyTimer(pTarget,iEffectNumber,0) == -1)
    {
      FxIntResupplyStop(pTarget,iEffectNumber);//Wird leider nicht aufgerufen. :\
      return(-1);
    }
  }
  else
  {
    var obj = pCrate->Contents(0);
    
    PlayerMessage(GetController(pTarget),"@{{%i}}|%d/%d",pTarget,pCrate->GetSpawnID(),ContentsCount(pCrate->GetSpawnID(),pCrate),pCrate->GetMaxCount());
    
    if(!obj)
    {
      FxIntResupplyStop(pTarget,iEffectNumber);
      return(-1);
    }
  }
}

public func FxIntResupplyTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  var crate = EffectVar(0,pTarget,iEffectNumber);
  if(!crate) return(-1);//Gibts die Kiste auch noch?
  if((GetProcedure(pTarget) ne "PUSH")||(GetActionTarget(0,pTarget) != crate)) return(-1);//Fässt der Clonk die auch noch lieb an?
  
  var obj = crate->Contents(0);
  if(!obj) return(-1);
  var objid = GetID(obj);

  if(obj->~IsAmmoPacket())
    if(!obj->~MayTransfer(pTarget))
      return(-1);
      
  if(obj->~IsGrenade())//Granate?
  {
    if(!pTarget->~StoreGrenade(obj))
      return(-1);
  }
  else
  {
    if(!pTarget->Collect(obj))
      return(-1);//Fail.

    if(obj->~IsWeapon())//Waffe?
      DoAmmo(obj->GetFMData(FM_AmmoID),obj->GetFMData(FM_AmmoLoad),obj);
      
    if(obj->~IsAmmoPacket())//Munition?
      obj->~TransferAmmo(pTarget);
  }
  
  if(!crate->Contents(0))
  {
    EffectVar(1,pTarget,iEffectNumber) = true;
    PlayerMessage(GetController(pTarget),"{{%i}}|%d/%d",pTarget,objid,ContentsCount(objid,crate),crate->GetMaxCount());
    return(-1);
  }
  else
  {
    PlayerMessage(GetController(pTarget),"@{{%i}}|%d/%d",pTarget,objid,ContentsCount(objid,crate),crate->GetMaxCount());
  }
}

public func FxIntResupplyStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  if(!EffectVar(1,pTarget,iEffectNumber))
    PlayerMessage(GetController(pTarget),"",pTarget);

  /*
  var crate = EffectVar(0,pTarget,iEffectNumber);
  if(crate && pTarget->GetAlive())
    if((GetProcedure(pTarget) eq "PUSH")&&(GetActionTarget(0,pTarget) == crate))
      SetCommand(pTarget,"UnGrab");//Loslassen plz. :)
  */
}

/* Bumm */
public func IsCraneGrabable() { return(1); }

/*func Incineration(int iPlr)
{
  ClearScheduleCall(this(), "InstaExplode");
  ScheduleCall(this(), "InstaExplode", 100+Random(80),0,iPlr);
}

func IncinerationEx(int iPlr)
{
  ClearScheduleCall(this(), "InstaExplode");
}

func Damage(int iChange, int iPlr)
{
  if(GetDamage() > 20)
      Incinerate();
      
  if(GetDamage() < 100) return();
  
  InstaExplode(iPlr);
}

func InstaExplode(int iPlr)
{
  //Effektgehasche
  Sound("CrateCrack");
  CastParticles("Splinter", 10, 200, 0,0, 50, 75, RGBa(80,0,0,0), RGBa(30,0,0,0));
  
  //Umliegende Objekte anzünden
  for(var obj in FindObjects(Find_Distance(40+Random(20)),Find_Exclude(this()),Find_Not(Find_Category(C4D_StaticBack))))
  {
    var inc = GetDefCoreVal("ContactIncinerate",0,GetID(obj));
    if(!inc) continue;
    
    if(inc <= 3)
      obj->Incinerate();
    else
      if(!Random(inc-3))
        obj->Incinerate();
  }
  
  while(Contents())
    Contents()->Exit(Contents(),0,0,Random(360),RandomX(-20,+20),RandomX(-20,+20),RandomX(-10,+10)); 
  
  //Explosion
  SetController(iPlr);
  Explode(30);
}*/
