/*-- Fahrzeugrespawnpoint --*/

#strict 2

public func IsSpawnpoint()	{return true;}


/* Globale Aufruffunktion */

global func SetupVehicleSpawn(array aType, int iDir, object pTarget, int iFrames, int iDistance)
{
  if(!pTarget) pTarget = this;
  if(!pTarget) return false;
  if(!iFrames) iFrames = 70;
  if(!iDistance) iDistance = 200;

  var effect = AddEffect("IntVehicleSpawn4K",pTarget,50,1,pTarget); 

  EffectCall(pTarget, effect, "SetDir", iDir);
  EffectCall(pTarget, effect, "SetDist", iDistance);
  EffectCall(pTarget, effect, "SetFrames", iFrames);

  if(GetLength(aType))
  {
    for(var i = 0; i < GetLength(aType); i++)
      EffectCall(pTarget,effect,"AddType", aType[i]);
  }
  EffectCall(pTarget, effect, "Spawn");
  return true;
}

global func FxIntVehicleSpawn4KStart(object pTarget, int iEffectNumber, int iTemp)
{
  if(iTemp) return;
  EffectVar(0, pTarget, iEffectNumber) = CreateArray();	//Spawn-IDs
  EffectVar(1, pTarget, iEffectNumber) = 0;		//aktuelles Fahrzeug
  EffectVar(2, pTarget, iEffectNumber) = DIR_Right;	//Aktuelle Dir
  EffectVar(3, pTarget, iEffectNumber) = 100;		//Sichere Zone
  EffectVar(4, pTarget, iEffectNumber) = true;		//Erstes Mal
  EffectVar(5, pTarget, iEffectNumber) = 0;		//Frames
  EffectVar(6, pTarget, iEffectNumber) = 0;		//Getane Frames
  return 1;
}

global func FxIntVehicleSpawn4KSpawn(object pTarget, int iEffectNumber)
{
  var pVehicle = EffectVar(1,pTarget,iEffectNumber);
  var aType = EffectVar(0,pTarget,iEffectNumber);

  //Bei Keine Helikopter-Regel entsprechende IDs entfernen
  if(NoHelicopters())
  {
    for(var i = 0; i < GetLength(aType); i++)
    {
      var array = aType[i];
      if(array[0] && array[0]->~IsHelicopter())
        DelArrayItem4K(aType, i--);
    }
  }

  var data = RandomIndex4K(aType);
  if(!data)
    return;

  var id = data[0];
  var x = data[1], y = data[2], wdt = data[3], hgt = data[4];

  if(FindObject2(Find_AtRect(x-2, y-2, wdt+4, hgt+4), Find_Func("BlockVehicleSpawn")))
    return;

  pVehicle = CreateContents(id, pTarget);
  SetDir(EffectVar(2,pTarget,iEffectNumber), pVehicle);
  if(EffectVar(4, pTarget, iEffectNumber))
  {
    EffectVar(4, pTarget, iEffectNumber) = false;
    Exit(pVehicle);
  }
  else
  {
    SpawnEffect(pVehicle);
  }
  SetPosition(GetX(),GetY(), pVehicle);
  EffectVar(1,pTarget,iEffectNumber) = pVehicle;
  EffectVar(6, pTarget, iEffectNumber) = 0;

  return true;
}

global func FxIntVehicleSpawn4KTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  var aType = EffectVar(0,pTarget,iEffectNumber);
  if(!GetLength(aType)) return 0;
  var pVehicle = EffectVar(1,pTarget,iEffectNumber);
  if(!pVehicle && EffectVar(6, pTarget, iEffectNumber) >= EffectVar(5, pTarget, iEffectNumber))
  {
    EffectCall(pTarget, iEffectNumber, "Spawn");
  }
  else
  {
    if(!pVehicle)
    {
      EffectVar(6, pTarget, iEffectNumber)++;
    }
    else
    {
      if(!GetEffect("IntVehicleUnused", pVehicle))
      {
        AddEffect("IntVehicleUnused", pVehicle, 51, 10, pVehicle, 0, this, EffectVar(3, pTarget, iEffectNumber));
      }
    }
  }
  return;
}

global func FxIntVehicleSpawn4KAddType(object pTarget, int iEffectNumber, id idType)
{
  if(FindInArray4K(EffectVar (0,pTarget,iEffectNumber), idType) == -1)
  {
    EffectVar(0,pTarget,iEffectNumber)[GetLength(EffectVar(0,pTarget,iEffectNumber))] = [idType, GetDefCoreVal("Offset", 0, idType, 0), GetDefCoreVal("Offset", 0, idType, 1), GetDefWidth(idType), GetDefHeight(idType)];
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

global func FxIntVehicleSpawn4KSetFrames(object pTarget, int iEffectNumber, int iFrames)
{
  EffectVar(5,pTarget,iEffectNumber) = iFrames;
  return;
}

global func FxIntVehicleSpawn4KStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  return -1;
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

/* Unbenutze Fahrzeuge beschädigen */

global func FxIntVehicleUnusedStart(object pTarget, int iEffectNumber, id idType, object pSpawner, int iDistance)
{
  EffectVar(0, pTarget, iEffectNumber) = pSpawner;	//Spawner
  EffectVar(1, pTarget, iEffectNumber) = iDistance;	//Sichere Zone
  EffectVar(2, pTarget, iEffectNumber) = 0;		//Timer
  EffectVar(3, pTarget, iEffectNumber) = false;		//Verursacht gerade Schaden
  return 1;
}

global func FxIntVehicleUnusedTimer(object pTarget, int iEffectNumber, int iTime)
{
  var pSpawner = EffectVar(0,pTarget,iEffectNumber);
  //Spawner-Check
  if(!pSpawner)
    return -1;
  //Timer
  EffectVar(2, pTarget, iEffectNumber)++;

  //Damage-Checks
  var iDistance = EffectVar(1,pTarget,iEffectNumber);
  var damage = true;
  if(iDistance < 0)
    damage = false;
  if(Distance(GetX(pTarget), GetY(pTarget), GetX(pSpawner), GetY(pSpawner)) < iDistance)
    damage = false;
  if(FindObject2(Find_Func("IsClonk"), Find_Or(Find_Distance(100, AbsX(GetX(pTarget)), AbsY(GetY(pTarget))), Find_Container(pTarget))))
    damage = false;
  //Schaden
  if(damage)
  {
    if(EffectVar(2, pTarget, iEffectNumber) < 30) return;
    EffectVar(3, pTarget, iEffectNumber) = true;
    DoDmg(Max(1, EffectVar(2, pTarget, iEffectNumber)/10), DMG_Melee, pTarget);
    if(!pTarget) return;
    EffectVar(3, pTarget, iEffectNumber) = false;
  }
  else
  {
    EffectVar(2, pTarget, iEffectNumber) = 0;
  }
  return 1;
}

global func FxIntVehicleUnusedDamaging(object pTarget, int iEffectNumber)
{
  return EffectVar(3, pTarget, iEffectNumber);
}

/* Objektspzifische Funktionen */

global func SpawnEffect(object pObj)
{
  Exit(pObj);
}
