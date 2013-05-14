/*-- NUR OPEN BETA --*/

#strict 2

#appendto SHTX

public func FxHitCheckTimer(object target, int effect, int time)
{
  var obj;
  // Oh man. :O
  var oldx = EffectVar(0, target, effect);
  var oldy = EffectVar(1, target, effect);
  var newx = GetX(target);
  var newy = GetY(target);
  EffectVar(0, target, effect) = GetX(target);
  EffectVar(1, target, effect) = GetY(target);

  //Schuss schon Scharf?
  var exclude = EffectVar(2, target, effect);
  //Selbst nicht treffbar
  if(EffectVar(4, target, effect)) exclude = target;

  //DrawParticleLine("NoGravSpark",newx-oldx, newy-oldy,0,0,1,25,RGB(255,0,0),RGB(0,0,255));
  // Wir suchen nach Objekten entlang der Linie die wir uns seit dem letzten Check
  // bewegt haben. Und sortieren sie nach Distanz (entfernten zuerst, da wir die zuerst passieren)
  for(obj in FindObjects(Find_OnLine(oldx,oldy,newx,newy),
                         Find_NoContainer(),
                         Sort_Distance(oldx, oldy)))
  {
    if(obj == target) continue;
    if(obj == exclude) continue;
    if(obj->~HitExclude(target)) continue;
    if(!CheckEnemy(obj,target)) continue;
    if(obj->~IsBulletTarget(GetID(target),target,EffectVar(2, target, effect), oldx, oldy) || GetOCF(obj) & OCF_Alive)
    {
      if(!target)
        return -1;
      DebugLog("%s IsBulletTarget: %i, %s, %s","HitCheck",GetName(obj),GetID(target),GetName(target),GetName(EffectVar(2, target, effect)));
      return target-> ~HitObject(obj);
    }
  }

  EffectVar(0, target, effect) = GetX(target);
  EffectVar(1, target, effect) = GetY(target);

  //Der Schuss wird erst "scharf gemacht", d.h. kann den Schützen selbst treffen, wenn der Schuss einmal die Shape des Schützen verlassen hat.

  if(!EffectVar(5,target,effect))
  {
    if(!EffectVar(4, target, effect))
    {
      //Scharf?
      var ready = true;
      //Check nach Objekten mit ID des Schützen an momentaner Stelle
      for(var foo in FindObjects(Find_AtPoint(GetX(target),GetY(target)),Find_ID(EffectVar(3, target, effect))))
      //Gefundenes der Schütze?
      if(foo == EffectVar(2, target, effect))
        ready = false;
      if(ready)
      //Treffbarkeit einstellen
      EffectVar(4, target, effect) = true;
    }
  }
}
