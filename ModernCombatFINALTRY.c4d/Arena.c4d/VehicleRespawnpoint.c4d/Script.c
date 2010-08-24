/*-- Fahrzeugrespawnpoint --*/

#strict 2

//Indikator
public func IsSpawnpoint() { return true; }

global func FxIntVehicleSpawn4KStart(object pTarget, int iEffectNumber, int iTemp)
{
  if(iTemp) return;
  EffectVar(0, pTarget, iEffectNumber) = CreateArray(); //Spawn-IDs
  EffectVar(1, pTarget, iEffectNumber) = 0; //aktuelles Fahrzeug
  EffectVar(2, pTarget, iEffectNumber) = DIR_Right; //Aktuelle Dir
  EffectVar(3, pTarget, iEffectNumber) = 100; //Sichere Zone
  return 1;
}

global func FxIntVehicleSpawn4KTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  var aType = EffectVar(0,pTarget,iEffectNumber);
  if(!GetLength(aType)) return 0;

  var pVehicle = EffectVar (1,pTarget,iEffectNumber);
  if(!pVehicle)
  {
   pVehicle = CreateContents(RandomIndex4K(aType),pTarget);
   SetDir(EffectVar (2,pTarget,iEffectNumber), pVehicle);
   if(!pTarget->~Spawn(pVehicle))
    Exit(pVehicle);
    SetPosition(GetX(),GetY(), pVehicle);
  }
  
  var iDistance = EffectVar(3, pTarget, iEffectNumber);

  if(iDistance > 0 && Distance(GetX(pVehicle), GetY(pVehicle), GetX(this), GetY(this)) > iDistance) {
    AddEffect("IntVehicleUnused", pVehicle, 51, 10, pVehicle, 0, this, iDistance);
  }
  
  EffectVar(1,pTarget,iEffectNumber) = pVehicle;

  return;
}

global func FxIntVehicleSpawn4KAddType(object pTarget, int iEffectNumber, id idType)
{
  if(FindInArray4K(EffectVar (0,pTarget,iEffectNumber), idType) == -1)
  {
   EffectVar(0,pTarget,iEffectNumber)[GetLength(EffectVar(0,pTarget,iEffectNumber))] = idType;
  }
  return;
}

global func FxIntVehicleSpawn4KDelType(object pTarget, int iEffectNumber, id idType)
{
  var i = FindInArray4K(EffectVar (0,pTarget,iEffectNumber), idType);
  if(i != -1)
  {
   EffectVar(0,pTarget,iEffectNumber)[i] = 0;
   CleanArray4K(EffectVar(0,pTarget,iEffectNumber));
  }
  return 0;
}

global func FxIntVehicleSpawn4KSetDir(object pTarget, int iEffectNumber, int iDir)
{
  EffectVar(2,pTarget,iEffectNumber) = iDir;
  return 0;
}

global func FxIntVehicleSpawn4KSetDist(object pTarget, int iEffectNumber, int iDistance)
{
  EffectVar(3,pTarget,iEffectNumber) = iDistance;
  return;
}


global func FxIntVehicleSpawn4KStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  return -1;
}

global func SetupVehicleSpawn(array aType, int iDir, object pTarget, int iFrames, int iDistance)
{
  if(!pTarget) pTarget = this;
  if(!pTarget) return false;
  if(!iFrames) iFrames = 70;
  if(!iDistance) iDistance = 100;

  var effect = AddEffect("IntVehicleSpawn4K",pTarget,50,iFrames,pTarget); 

  EffectCall(pTarget, effect, "SetDir", iDir);
  EffectCall(pTarget, effect, "SetDist", iDistance);

  if(GetLength(aType))
  {
   for(var i = 0; i < GetLength(aType); i++)
    EffectCall (pTarget,effect,"AddType", aType[i]);
  }
  return true;
}

global func RemoveVehicleSpawn(object pTarget)
{
  if(!pTarget) pTarget = this;
  if(!pTarget) return false;

  RemoveEffect ("IntVehicleSpawn4K", pTarget);
  return true;
}

global func VehicleSpawn_AddType(id idType, object pTarget)
{
  if(!pTarget) pTarget = this;
  if(!pTarget) return false;

  var effect = GetEffect ("IntVehicleSpawn4K", pTarget);
  if(!effect) return false;

  EffectCall (pTarget,effect,"AddType", idType);
  return true;
}

global func VehicleSpawn_DelType(id idType, object pTarget)
{
  if(!pTarget) pTarget = this;
  if(!pTarget) return false;

  var effect = GetEffect ("IntVehicleSpawn4K", pTarget);
  if(!effect) return false;

  EffectCall (pTarget,effect,"DelType", idType);
  return true;
}

global func VehicleSpawn_SetDir(int iDir, object pTarget)
{
  if(!pTarget) pTarget = this;
  if(!pTarget) return false;

  var effect = GetEffect ("IntVehicleSpawn4K", pTarget);
  if(!effect) return false;

  EffectCall (pTarget,effect,"SetDir", iDir);
  return true;
}

global func VehicleSpawn_SetDist(int iDistance, object pTarget)
{
  if(!pTarget) pTarget = this;
  if(!pTarget) return false;

  var effect = GetEffect ("IntVehicleSpawn4K", pTarget);
  if(!effect) return false;

  EffectCall (pTarget,effect,"SetDistance", iDistance);
  return true;
}

/* Unbenutze Beschädigen */

global func FxIntVehicleUnusedStart(object pTarget, int iEffectNumber, id idType, object pSpawner, int iDistance) {
  EffectVar(0, pTarget, iEffectNumber) = pSpawner; //Spawner
  EffectVar(1, pTarget, iEffectNumber) = iDistance; //Sichere Zone
  return 1;
}

global func FxIntVehicleUnusedTimer(object pTarget, int iEffectNumber, id idType)
{
  var pSpawner = EffectVar(0,pTarget,iEffectNumber);
  var iDistance = EffectVar(1,pTarget,iEffectNumber);
  if(iDistance < 0)
    return -1;
  if(!pTarget || !pSpawner)
    return -1;
  if(GetActionTarget(0, pTarget)) {
    var pCommanders = FindObjects(Find_ActionTarget(GetActionTarget(0, pTarget)), Find_OCF(OCF_Alive));
    var remove = false;
    for(var pCommander in pCommanders) {
      var iDistance = ObjectDistance(pCommander, GetActionTarget(0, pTarget));
      if(iDistance < 50) {
        remove = true;
        break;
      }
    }
    if(remove) return -1;
  }
  if(Distance(GetX(pTarget), GetY(pTarget), GetX(pSpawner), GetY(pSpawner)) < iDistance)
    return -1;
  DoDmg(1, DMG_Melee, pTarget);
  return 1;
}

/* Objektspzifische Funktionen */

public func Spawn(object pObj)
{
  AddEffect("SpawnBeam", pObj, 120, 1, 0, GetID(), 70);
  Exit(pObj);
  //pObj->SetPosition(GetX(),GetY()+GetDefOffset(GetID(pObj),1));
}

public func FxSpawnBeamStart(pTarget, iEffectNumber, temp, iFrames)
{
  EffectVar(0, pTarget, iEffectNumber) = GetClrModulation(pTarget);
  EffectVar(1, pTarget, iEffectNumber) = iFrames;
  SetClrModulation(RGBa(255, 255, 255, 255), pTarget);//Unsichtbar.
}

public func FxSpawnBeamTimer(pTarget, iEffectNumber, iTime)
{
  var end_mod = EffectVar(0, pTarget, iEffectNumber);
  var iFrames = EffectVar(1, pTarget, iEffectNumber);

  for(var i = 0; i < 2; ++i)
  {
   var cur_X = GetX(pTarget) - GetX();
   var cur_Y = GetY(pTarget) - GetY();
   var x = RandomX( cur_X - GetObjWidth(pTarget) / 2, cur_X + GetObjWidth(pTarget) / 2);
   var y = RandomX( cur_Y - GetObjHeight(pTarget) / 2, cur_Y + GetObjHeight(pTarget) / 2);

   CreateParticle("LsrSprk",x,y,0,-2,170,RGBa(0, RandomX(128,255), 255, 50));
  }
  var v = 255-(iTime*255/iFrames);
  SetClrModulation(RGBa(255,255,255,v),pTarget);
  
  if(iTime >= iFrames)
   return -1;
}

public func FxSpawnBeamStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  SetClrModulation(EffectVar(0, pTarget, iEffectNumber), pTarget);
}
