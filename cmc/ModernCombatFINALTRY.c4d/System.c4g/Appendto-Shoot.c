/*-- Schuss --*/

//Hazardschussfehler �berladen und korrigiert.

#strict 2
#appendto SHT1


public func FxHitCheckTimer(object target, int effect, int time)
{
  var obj;
  var oldx = EffectVar(0, target, effect);
  var oldy = EffectVar(1, target, effect);
  var newx = GetX(target);
  var newy = GetY(target);
  EffectVar(0, target, effect) = GetX(target);
  EffectVar(1, target, effect) = GetY(target);
  var user = EffectVar(2, target, effect);
  
  //Schuss schon Scharf?
  var exclude = EffectVar(2, target, effect);
  //Nicht selber treffen
  if(EffectVar(4, target, effect)) exclude = target;

  //DrawParticleLine("NoGravSpark",newx-oldx, newy-oldy,0,0,1,25,RGB(255,0,0),RGB(0,0,255));
  //Wir suchen nach Objekten entlang der Linie die wir uns seit dem letzten Check
  //bewegt haben. Und sortieren sie nach Distanz (n�here zuerst)
  for(obj in FindObjects(Find_OnLine(oldx,oldy,newx,newy),
  			Find_NoContainer(),
  			Sort_Distance(oldx, oldy)))
  {
    //Excludes
    if(obj == target) continue;
    if(obj == exclude) continue;
    if(obj->~HitExclude(target)) continue;

    //CheckEnemy (user f�r korrekte Entfernung f�r FF-Radius)
    if(!CheckEnemy(obj,user)) continue;
	
    // IsBulletTarget oder Alive
    if(obj->~IsBulletTarget(GetID(target),target,EffectVar(2, target, effect),oldx,oldy) || GetOCF(obj) & OCF_Alive)
    {
      DebugLog("%s IsBulletTarget: %i, %s, %s","HitCheck",GetName(obj),GetID(target),GetName(target),GetName(EffectVar(2, target, effect)));
      return(target-> ~HitObject(obj));
    }
  }

  EffectVar(0, target, effect) = GetX(target);
  EffectVar(1, target, effect) = GetY(target);

  //Der Schuss wird erst "scharf gemacht", d.h. kann den Sch�tzen selbst treffen, wenn
  //der Schuss einmal die Shape des Sch�tzen verlassen hat.

  if(!EffectVar(5,target,effect))
  {
    if(!EffectVar(4, target, effect))
    {
      //Ready gibt an, ob wir schon "scharf" sind. True = Scharf
      var ready = true;
      //Wir suchen alle Objekte mit der ID unseres Sch�tzens an unserer momentanen Stelle
      for(var foo in FindObjects(Find_AtPoint(GetX(target),GetY(target)),Find_ID(EffectVar(3, target, effect))))
      //Und schauen, ob es der Sch�tze ist.
      if(foo == EffectVar(2, target, effect))
        //Wir haben den Sch�tzen gefunden -> Er ist noch an unserer Position
        ready = false;
      //Wir haben den Sch�tzen nicht gefunden
      if(ready)
        //Wir treffen ihn ab jetzt
        EffectVar(4, target, effect) = true;
    }
  }
}