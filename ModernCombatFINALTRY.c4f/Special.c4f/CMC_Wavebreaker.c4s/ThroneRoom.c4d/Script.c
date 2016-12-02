/*-- Thronraum --*/

#strict 2
#include ATRP


/* Initialisierung */

public func Initialize()
{ 
  SetAction("Be");
}

public func Fall()
{ 
  SetAction("Destroyed");

  //Kategorie wechseln
  SetCategory(C4D_Structure);

  //Effekte
  AddFireEffect(this,150,RGB(50,50,50),true,30);
  //!INFO
  //AddEffect("DmgObjects", this, 101, 5, this);
  AddEffect("DmgObjects2", this, 101, 5, this);
}

/* Aufschlag */

public func Hit3()
{
  if(hit) return;
  hit = true;

  //Aussehen anpassen
  SetGraphics("Ruin");
  SetSolidMask(241,0,241,161);

  //Rauch erzeugen
  CastObjects(SM4K,2,20);

  //Objekte beschädigen
  var wdt = GetObjWidth()+2;
  var hgt = GetObjHeight()+2;
  var objects = FindObjects(Find_AtRect(-wdt/2, -hgt/2, wdt, hgt), Find_NoContainer());
  for(var obj in objects)
  {
    if(!obj)
      continue;

    //Schadenswert errechnen
    DoDmg(100, DMG_Melee, obj, 10, GetOwner()+1);

    if(GetCategory(obj) & C4D_Living)
    {
      obj->SetAction("Tumble");
      Sound("FKDT_FatalHit*.ogg", 0, obj, 40);
    }
  }

  //Kategorie wechseln
  SetCategory(C4D_StaticBack);
}

/* Schaden verursachen */

public func FxDmgObjects2Timer(object target, int nr, int time)
{
  if((GetXDir() < 20 && GetYDir() < 20) || Stuck())
  {
    EffectVar(1, target, nr)++;

    return -1;
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

    //Schadenswert errechnen
    DoDmg(Max(GetXDir(),GetYDir()), DMG_Melee, obj, 10, GetOwner()+1);

    if(GetCategory(obj) & C4D_Living)
    {
      obj->SetAction("Tumble");
      Sound("FKDT_FatalHit*.ogg", 0, obj, 40);
    }
  }
  EffectVar(0, target, nr) = objects;
}