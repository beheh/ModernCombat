/*-- Fahrzeug-Spawn --*/

#strict 2

global func FxIntVehicleSpawn4KStart(object pTarget, int iEffectNumber, int iTemp)
{
  if(iTemp) return;
  EffectVar (0,pTarget,iEffectNumber) = CreateArray();//Spawn-IDs
  EffectVar (1,pTarget,iEffectNumber) = 0;//aktuelles Fahrzeug
  return 1;
}

global func FxIntVehicleSpawn4KTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  var aType    = EffectVar (0,pTarget,iEffectNumber);
  if(!GetLength(aType)) return 0;
  
  var pVehicle = EffectVar (1,pTarget,iEffectNumber);
  if(!pVehicle)
  {
    pVehicle = CreateContents(RandomIndex4K(aType),pTarget);
    if(!pTarget->~Spawn(pVehicle))
      pVehicle->Exit();
  }
  
  EffectVar (1,pTarget,iEffectNumber) = pVehicle;
  
  return 0;
}

global func FxIntVehicleSpawn4KAddType(object pTarget, int iEffectNumber, id idType)
{
  if(FindInArray4K(EffectVar (0,pTarget,iEffectNumber), idType) == -1)
  {
    EffectVar (0,pTarget,iEffectNumber)[GetLength(EffectVar (0,pTarget,iEffectNumber))] = idType;
  }
  return 0;
}

global func FxIntVehicleSpawn4KDelType(object pTarget, int iEffectNumber, id idType)
{
  var i = FindInArray4K(EffectVar (0,pTarget,iEffectNumber), idType);
  if(i != -1)
  {
    EffectVar (0,pTarget,iEffectNumber)[i] = 0;
    CleanArray4K(EffectVar (0,pTarget,iEffectNumber));
  }
  return 0;
}

global func FxIntVehicleSpawn4KStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  return -1;
}

global func SetupVehicleSpawn(array aType, object pTarget, int iFrames)
{
  if(!pTarget) pTarget = this;
  if(!pTarget) return false;
  if(!iFrames) iFrames = 70;
  
  var effect = AddEffect ("IntVehicleSpawn4K",pTarget,50,iFrames,pTarget); 
  
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


/* objektspzifische Funktionen */
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
  pTarget -> SetClrModulation(RGBa(255, 255, 255, 255));//Unsichtbar.
}

public func FxSpawnBeamTimer(pTarget, iEffectNumber, iTime)
{
  var end_mod = EffectVar(0, pTarget, iEffectNumber);
  var iFrames = EffectVar(1, pTarget, iEffectNumber);

  for(var i = 0; i < 2; ++i)
  {
    var cur_X = pTarget->GetX() - GetX();
    var cur_Y = pTarget->GetY() - GetY();
    var x = RandomX( cur_X - pTarget->GetObjWidth() / 2, cur_X + pTarget->GetObjWidth() / 2);
    var y = RandomX( cur_Y - pTarget->GetObjHeight() / 2, cur_Y + pTarget->GetObjHeight() / 2);
    
    CreateParticle("LsrSprk",x,y,0,-2,170,RGBa(0, RandomX(128,255), 255, 50));
  }
  
  var v = 255-(iTime*255/iFrames);
  pTarget->SetClrModulation(RGBa(255,255,255,v),pTarget);
  
  if(iTime >= iFrames)
    return -1;
}

public func FxSpawnBeamStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  pTarget->SetClrModulation(EffectVar(0, pTarget, iEffectNumber), pTarget);
}
