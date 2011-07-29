/*-- Arena-Regel --*/

//Optionale Objekte k�nnen von der Arena-Regel ignoriert werden, indem diese "NoArenaRemove" wiedergeben.

#strict 2

#appendto NODR


protected func Timer()
{
  //Objekte suchen
  for(var pObj in FindObjects(Find_Category(C4D_Object), Find_NoContainer(),
  							Find_Or(Find_Func("IsWeapon"),
  								Find_Func("IsEquipment"),
  								Find_Func("IsUpgrade")),
  							Find_Not(Find_Func("IsMine")),
  							Find_Not(Find_Func("NoArenaRemove"))))
    AddEffect("Arena_Remove", pObj, 1, 400, this);

  //Spezialsuche: Drohnen
  for(pObj in FindObjects(Find_ID(DRSU), Find_NoContainer()))
    if(ActIdle(pObj))
      AddEffect("Arena_Remove", pObj, 1, 530, this);
}

public func FxArena_RemoveTimer(object pTarget)
{
  //Abbruch bei neuer Aktion (Drohne)
  if(!pTarget->ActIdle())
    return(-1);
  //Ansonsten l�schen sofern nicht wieder aufgehoben
  if(!(pTarget->Contained()))
  {
    //Effekte
    pTarget->CastSmoke("Smoke3", 8, 3, 0, 0, 60, 80);
    if(GetEffectData(EFSM_ExplosionEffects) > 1) pTarget->CastParticles("MetalSplinter",4,50,0,0,20,70);
    Sound("Limitation.ogg", 0, pTarget);

    pTarget->RemoveObject();
    return(-1);
  }
}