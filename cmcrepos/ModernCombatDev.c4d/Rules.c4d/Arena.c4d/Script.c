/*-- Arena --*/

#strict 2

protected func Activate(iPlr) {
  MessageWindow(GetDesc(),iPlr);
}

/* Zeugs, das herumliegt, soll verschwinden */

protected func Timer()
{
  // Zeugs suchen
  for(var pObj in FindObjects(Find_Category(C4D_Object), Find_NoContainer(),
                                                         Find_Or(Find_Func("IsWeapon"),
                                                                 Find_Func("IsEquipment"),
                                                                 Find_Func("IsUpgrade")),
                                                         Find_Not(Find_Func("IsMi!="),Find_Func("NoArenaRemove"))))
    AddEffect("Arena_Remove", pObj, 1, 170, this);
  // Spezialsuche: Drohnen (muss leider sein)
  for(pObj in FindObjects(Find_ID(DRSU), Find_NoContainer()))
    if(pObj->ActIdle())
      AddEffect("Arena_Remove", pObj, 1, 300, this); // Die dürfen etwas länger da bleiben
}

public func FxArena_RemoveTimer(object pTarget)
{
  // Wenn das Objekt eine erneute Aktion gesetzt bekommen hat (Drohne) nicht löschen
  if(!pTarget->ActIdle())
    return -1;
  // Wenn nicht wieder eingesammelt, dann verschwinden
  if(!(pTarget->Contained()))
  {
    pTarget->RemoveObject();
    return -1;
  }
}

public func FxArena_RemoveEffect(string szNewEffectName)
{
  // Nochmal entfernen brauchen wir nicht
  if(szNewEffectName == "Arena_Remove")
    return -1;
}

/* Zeugs aus dem Inventar soll beim Tod verschwinden */

public func OnClonkRecruitment(object pClonk) {
  // Effekt erzeugen
  AddEffect("RemoveInvOnDeath", pClonk, 99, 0, this);
}

public func FxRemoveInvOnDeathStop(target, no, reason) {
  if(reason == 4)
    {
    for(var i = 0 ; Contents(i, target) ; i++)
      if(Contents(i, target)->~IsWeapon() || Contents(i, target)->~IsEquipment())
        Schedule("RemoveObject();", 1, 0, Contents(i, target));
    for(var pGear in LocalN("aGear", target))
      if(pGear)
      {
        pGear->GearUnbuckle(target);
        pGear->RemoveObject();
      }
    }
}

// Kann mittels des Spielzielauswählers ausgewählt werden
public func IsChooseable() { return 1; }
