/* Explode, BlastObjects und Sichtfenster-Kamera-Wackel-Effekt */

//Erweitert Explosionen um Objekteverschleuderung und Wackeleffekte für Sichtfenster.

#strict 2


/* Explosion */

global func DoExplosion(int x, int y, int level, object inobj, int cause_plr, id effect_def, string effect_particle, object layer)
{
  //Container nach ContainBlast
  var container = inobj;

  //Äußersten Container finden
  while (container)
    if(GetDefContainBlast(GetID(container)))
      break;
    else
      container = Contained(container);

  //Explosion draußen: Explosionseffekte erzeugen
  if(!container)
  {
    KnockDownObjects(x,y,level*3,layer,cause_plr+1);

    //Material entzünden
      if(!IncinerateLandscape(x,y))
        if(!IncinerateLandscape(x,y-10))
          if(!IncinerateLandscape(x-5,y-5))
            IncinerateLandscape(x+5,y-5);

    //Grafikeffekt
    var blast;
    if(effect_particle)
      effect_def = 0;
    else
      effect_particle = "Blast";

    if(effect_def)
    {
      if(blast = CreateConstruction(effect_def, x,y+level, cause_plr, level*5))
        blast->~Activate();
    }
    else
    {
      var vis;
      if(GBackLiquid(x,y))
      {
        vis = 100;

        var k = 3;

        for(var i = level/k; i > 0; i--)
        {
          var a = Random(360);
          var d = Random(level);
          if(GetEffectData(EFSM_ExplosionEffects) > 1) Bubble (x-Sin(a,d),y+Cos(a,d));
        }
      }
    CreateParticle(effect_particle, x,y, 0,0, level*10, RGBa(255,255,255,vis));
    if(effect_particle == "Blast")
      if(!CastParticles("FSpark", level/5+1, level, x,y, level*5+10,level*10+10, 0x00ef0000,0xffff1010))
        if(blast = CreateConstruction(FXB1, x,y+level, cause_plr, level*5))
          blast->~Activate();
    }
  }
  //Schaden in den Objekten bzw. draußen
  BlastObjects2(x+GetX(),y+GetY(), level, inobj, cause_plr+1, layer);
  if(inobj != container)
   BlastObjects2(x+GetX(),y+GetY(), level, container, cause_plr+1);

  //Landschaft zerstören. Nach BlastObjects, damit neu freigesprengte Materialien nicht betroffen sind
  if(!container && !NoMatExplosions())
  {
   BlastFree(x,y, level, cause_plr+1);
    ShakeFree (x,y,level*3/2);
  }
  return true;
}

global func KnockDownObjects(int x, int y, int level, object layer, int cause_plr_plus_one)
{
  var l_x = x - GetX(), l_y = y - GetY();

  var a = FindObjects(Find_Distance(level, l_x,l_y), Find_NoContainer(), Find_Layer(layer),
          Find_Or(Find_Category(C4D_Object|C4D_Living|C4D_Vehicle), Find_Func("CanBeHitByShockwaves")), Find_Func("BlastObjectsShockwaveCheck",x,y));

  for(var obj in a)
  {
   if(GetCategory(obj) & C4D_Living)
   {
    obj->SetAction("Tumble");
    obj->SetKiller(cause_plr_plus_one-1);
   }
  }
}
  
/* Objekteschädigung und -verschleuderung */

global func BlastObjects2(int x, int y, int level, object container, int cause_plr_plus_one, object layer)
{
  var range = level/2*3;

  var obj;

  //Koordinaten sind immer global angegeben. In lokale Koordinaten umrechnen
  var l_x = x - GetX(), l_y = y - GetY();

  //Im Container?
  if(container)
  {
   if(GetObjectLayer(container) == layer)
   {
    BlastObject(level, container, cause_plr_plus_one);
    for (obj in FindObjects(Find_Container(container), Find_Layer(layer)))
     if(obj) {
       if(GetCategory(obj) & C4D_Living) BlastObject(level, obj, cause_plr_plus_one); //DoDmg(level, DMG_Explosion, obj, 0, cause_plr_plus_one); //Muss um eins erhöht sein!
     }
   }
  }
  else
  {
   //Objekt ist draußen
   //Objekte am Explosionspunkt beschädigen
   for (var obj in FindObjects(Find_AtRect(l_x-5, l_y-5, 10,10), Find_NoContainer(), Find_Layer(layer)))
      if(obj && (!obj->~UseOwnHitbox() || obj->~ExplosionHitbox(x, y, level)))
        BlastObject(level, obj, cause_plr_plus_one);
   //Objekte im Explosionsradius schleudern
   var shockwave_objs = FindObjects(Find_Distance(range, l_x,l_y), Find_NoContainer(), Find_Layer(layer),
       Find_Or(Find_Category(C4D_Object|C4D_Living|C4D_Vehicle), Find_Func("CanBeHitByShockwaves")), Find_Func("BlastObjectsShockwaveCheck",x,y));
   var cnt = GetLength(shockwave_objs);
    if(cnt)
    {
     //Die Schleuderenergie teilt sich bei vielen Objekten auf
     //Log("Shockwave objs %v (%d)", shockwave_objs, cnt);
     var shock_speed = Sqrt(level * level / BoundBy(cnt, 2, 12));
     for (var obj in shockwave_objs) if(obj) //obj noch prüfen, weil OnShockwaveHit Objekte aus dem Array löschen könnte
     {
      //Objekt hat benutzerdefinierte Reaktion auf die Schockwelle?
      if(obj->~OnShockwaveHit(level, x,y)) continue;
      //Lebewesen leiden besonders
      var cat = GetCategory(obj);
      if(cat & C4D_Living)
      {
       BlastObject(level/2, obj, cause_plr_plus_one);
      }
      //Killverfolgung bei Projektilen
      if(cat & C4D_Object) SetController(cause_plr_plus_one-1, obj);
      //Schockwelle
      var mass_fact = 20, mass_mul = 100; if(cat & C4D_Living) { mass_fact = 8; mass_mul = 80; }
      mass_fact = BoundBy(GetMass(obj)*mass_mul/1000, 4, mass_fact);
      var dx = 100*(GetX(obj)-x)+Random(51)-25;
      var dy = 100*(GetY(obj)-y)+Random(51)-25;
      var vx, vy;
      if(dx)
      {
       vx = Abs(dx)/dx * (100*level-Abs(dx)) * shock_speed / level / mass_fact;
      }
      vy = (Abs(dy) - 100*level) * shock_speed / level / mass_fact;
      if(cat & C4D_Object)
      {
       //Objekte nicht zu schnell werden lassen
       var ovx = GetXDir(obj, 100), ovy = GetYDir(obj, 100);
       if(ovx*vx > 0) vx = (Sqrt(vx*vx + ovx*ovx) - Abs(vx)) * Abs(vx)/vx;
       if(ovy*vy > 0) vy = (Sqrt(vy*vy + ovy*ovy) - Abs(vy)) * Abs(vy)/vy;
      }
      //Log("%v v(%v %v)   d(%v %v)  m=%v  l=%v  s=%v", obj, vx,vy, dx,dy, mass_fact, level, shock_speed);
      Fling(obj, vx,vy, 100, true);
     }
    }
  }
  return true;
}

global func BlastObject(int level, object obj, int cause_plr_plus_one)
{
  var icon;
  if(this) icon = this->~GetKillIcon();
  if(!icon) icon = GetID();
  DoDmg(level, DMG_Explosion, obj, 0, cause_plr_plus_one, icon);
  var blastinc = GetDefCoreVal("BlastIncinerate", "DefCore", GetID(obj));
  if(blastinc && GetDamage(obj) > blastinc) Incinerate(obj);
  return true;
}

global func DamageObjects(int iDistance, int iDamage, object pObject, int iX, int iY, int iAttachment)
{
  if(!pObject) pObject = this;
  var x = GetX(pObject)+iX;
  var y = GetY(pObject)+iY;
  var dealer = -1;
  dealer = GetController(pObject);
  if(dealer == -1) dealer = GetOwner(pObject);
  var icon = pObject->~GetKillIcon();
  if(!icon) icon = GetID(pObject);
  for(var obj in FindObjects(pObject->Find_Distance(iDistance,iX,iY),
  				pObject->Find_Exclude(this),
  				Find_NoContainer()))
  {
    DoDmg(iDamage - (Distance(GetX(obj),GetY(obj),x,y)*iDamage/iDistance), DMG_Explosion, obj, 0, dealer+1, icon, iAttachment);
  }
  return true;
}

/* Fake-Explosion */

global func FakeExplode(int iLevel, int iControllerPlusOne, object obj)
{
  //Existenz bestätigen
  if(!obj) obj = this;
  if(!obj) return false;

  //Dummy erstellen und explodieren
  var dummy = CreateObject(GetID(obj), 0, 0, iControllerPlusOne-1);
  SetPosition(GetX(), GetY(), dummy);
  SetController(iControllerPlusOne-1, dummy);
  Explode(iLevel, dummy);
  if(dummy) RemoveObject(dummy);

  return true;
}

/* Dekorativer Explosion */

global func DecoExplode(int iLevel, object pObj, int iNoRemove)
{
  //Existenz bestätigen
  if(!pObj && !(pObj = this))
    return;

  //Explosion
  Explode(iLevel, pObj, 0, 0, true);

  //Effekte
  var x = GetX(pObj)-GetX(), y = GetY(pObj)-GetY();
  CreateParticle("Blast", x, y, 0, 0, 10*iLevel);
  CastParticles("FSpark", iLevel/5+1, iLevel, x, y, iLevel*5+10, iLevel*10+10, RGBa(255), RGBa(255,20,20,255));

  //Sound 
  Sound(Format("Blast%d", BoundBy(iLevel/10-1, 1, 3)), 0, pObj);

  //Objekt entfernen
  if(!iNoRemove)
    RemoveObject(pObj);
}