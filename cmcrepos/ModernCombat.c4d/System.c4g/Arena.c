#strict
#appendto NODR

protected func Timer()
{
  // Zeugs suchen
  for(var pObj in FindObjects(Find_Category(C4D_Object), Find_NoContainer(),
                                                         Find_Or(Find_Func("IsWeapon"),
                                                                 Find_Func("IsEquipment"),
                                                                 Find_Func("IsUpgrade")),
                                                         Find_Not(Find_Func("IsMine"),Find_Func("NoArenaRemove"))))
    AddEffect("Arena_Remove", pObj, 1, 170, this());
  // Spezialsuche: Drohnen (muss leider sein)
  for(pObj in FindObjects(Find_ID(DRSU), Find_NoContainer()))
    if(pObj->ActIdle())
      AddEffect("Arena_Remove", pObj, 1, 300, this()); // Die dürfen etwas länger da bleiben
}