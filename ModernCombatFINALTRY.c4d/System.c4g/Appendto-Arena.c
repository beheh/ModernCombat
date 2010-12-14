/*-- Arena-Regel --*/

//Optionale Objekte können von der Arena-Regel ignoriert werden, indem diese "NoArenaRemove" wiedergeben.

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