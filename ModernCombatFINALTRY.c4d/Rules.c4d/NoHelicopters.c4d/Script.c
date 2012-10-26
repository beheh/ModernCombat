/*-- Keine Helikopter --*/

#strict 2

global func NoHelicopters()	{return FindObject(NOHC);}
public func IsChooseable()	{return true;}

protected func Activate(iByPlayer)
{
  MessageWindow(GetDesc(), iByPlayer);
  return 1;
}

public func ChooserFinished()
{
  Execute();
}

public func Execute()
{
  //Helikopterfahrzeugspawns suchen und entfernen
  for (var obj in FindObjects(Find_ID(VSPW)))
  {
    var i = GetEffect("IntVehicleSpawn4K", obj);
    if (!i)
      continue;
    var array = EffectVar(0, obj, i), entry;
    for(entry = 0; entry < GetLength(array); entry++)
      if(GetType(array[entry]) == C4V_C4ID && array[entry]->~IsHelicopter())
        DelArrayItem4K(array, entry);
    EffectVar(0, obj, i) = array;
  }
  //Helikopter suchen und entfernen
  for (var obj in FindObjects(Find_Func("IsHelicopter")))
    RemoveObject(obj, true);
}
