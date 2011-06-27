/*-- Limitierte Ausrüstung --*/

#strict 2

public func IsChooseable()	{return true;}


protected func Activate(iPlr)
{
  MessageWindow(GetDesc(), iPlr);
}

global func LimitedEquipment()
{
  return !!ObjectCount2(Find_ID(LIMT));
}

global func CheckLimitation(bool fCheckOnly)
{
  //Auf Limitierung prüfen
  if (!LimitedEquipment())
    return false;

  //Besitzer und maximale Anzahl ermitteln
  var id = GetID(this),
  max = id->~LimitationCount(),
  owner = GetOwner(this);

  //Unbegrenztes oder neutrales ignorieren
  if (!max || owner == NO_OWNER)
    return false;

  //Zugehörige Objekte identifizieren
  var objs = FindObjects(Find_ID(id), Find_Owner(owner), Find_NoContainer(), Find_Func("LimitationActive"), Sort_Func("LimitationTimer")),
  remove = GetLength(objs) >= max;

  if (fCheckOnly)
    return !remove;

  //Alte Objekte entfernen, wenn nötig
  while (GetLength(objs) >= max)
  {
    //Effekte
    var obj = objs[GetLength(objs) - 1],
    size = 20 + 5 * Distance(GetDefWidth(id), GetDefHeight(id));
    obj->CastSmoke("Smoke3", 8, 3, 0, 0, size, size);
    if(GetEffectData(EFSM_ExplosionEffects) > 1) obj->CastParticles("MetalSplinter",4,50,0,0,20,70);
    Sound("Limitation.ogg", 0, obj);

    //Objekt entfernen
    RemoveObject(obj);
    SetLength(objs, GetLength(objs) - 1);
  }

  //Effekt anhängen
  AddEffect("IntLimitation", this, 1);

  return !remove;
}

global func DeactivateLimitation(object pObj)
{
  if (!pObj && !(pObj = this))
    return false;
  if (!pObj->LimitationActive())
    return false;
  while (RemoveEffect("IntLimitation", pObj));
  return true;
}

global func LimitationActive()
{
  return LimitedEquipment() && GetEffect("IntLimitation", this);
}

global func LimitationTimer()
{
  return GetEffect("IntLimitation", this, 0, 6);
}