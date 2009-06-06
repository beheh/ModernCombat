/*-- Neues Script --*/

#strict
#appendto TIM2

public func Spawn()
{
 for(var obj in FindObjects(Find_Distance(25), Find_Exclude(this()), Find_Func("IsSpawnBlocker")))
 {
 FadeOut4K(35, obj);
 }
 return(_inherited());
}
