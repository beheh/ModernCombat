/*-- Keine Helikopter --*/

#strict 2

global func NoHelicopters()	{return FindObject(NOBH);}
public func IsChooseable()	{return true;}


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
  	for(entry = 0; entry < array; entry++)
  		if(array[entry] && array[entry]->~IsHelicopter())
      	DelArrayItem4K(array, entry);
    EffectVar(0, obj, i) = array;
  }
  //Helikopter suchen und entfernen
  for (var obj in FindObjects(Find_Func("IsHelicopter")))
    RemoveObject(obj, true);
}
