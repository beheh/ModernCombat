/*-- Mastst�ck --*/

#strict 2


/* Initalisierung */

func Initialize()
{
  SetAction("Be");
  AddEffect("DmgObjects", this, 101, 5, this);
  SetOwner(-1);
}

/* Schaden verursachen */

public func FxDmgObjectsTimer(object target, int nr, int time)
{
  if((GetXDir() < 20 && GetYDir() < 20) || Stuck())
  {
    EffectVar(1, target, nr)++;
    //Bei keiner bewegung in 15 Sekunden verschwinden
    if(EffectVar(1, target, nr) > RandomX(90,110))
      FadeOut();

    return false;
  }

  EffectVar(1, target, nr) = 0;

  if(GetType(EffectVar(0, target, nr)) != C4V_Array)
    EffectVar(0, target, nr) = [];

  var wdt = GetObjWidth()+2;
  var hgt = GetObjHeight()+2;
  var objects = FindObjects(Find_AtRect(-wdt/2, -hgt/2, wdt, hgt), Find_NoContainer());
  for(var obj in objects)
  {
    if(!obj)
      continue;

    if(GetIndexOf(obj, EffectVar(0, target, nr)) > -1)
      continue;

    if((GetCategory(obj) & C4D_Structure || GetCategory(obj) & C4D_Vehicle) && obj->~GetPartDamage())
      DoDmg(obj->~GetPartDamage(), DMG_Melee, obj, 10, -1);
    else
      DoDmg(Max(GetXDir(),GetYDir()), DMG_Melee, obj, 10, -1);

    if(GetCategory(obj) & C4D_Living)
      obj->SetAction("Tumble");
  }
  EffectVar(0, target, nr) = objects;
}

/* Aufschlag */

public func Hit(int iXDir, int iYDir)
{
  Sound("StructuralDamage*.ogg");

  var x = GetX(), y = GetY(), xdir, ydir;
  SimFlight(x, y, xdir, ydir, 50, 100);
  var mat = GetMaterial(x, y);

  if(mat == Material("Vehicle")) mat = Material("Wall");
  var clr_offset = Random(3)*3;
  var clr = RGB(Min(GetMaterialVal("Color", "Material", mat, 0 + clr_offset), 255), 
  		Min(GetMaterialVal("Color", "Material", mat, 1 + clr_offset), 255), 
  		Min(GetMaterialVal("Color", "Material", mat, 2 + clr_offset), 255));

  CastParticles("GroundSmoke", RandomX(4,12), 80, AbsX(x), AbsY(y)-1, (iXDir + iYDir) / 4, (iXDir + iYDir) / 2, clr, clr);
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter", RandomX(8,16), 80, AbsX(x), AbsY(y)-1, 20, 50);

  return true;
}