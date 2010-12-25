/*-- Keine Blackhawks --*/

#strict 2

global func NoBlackHawks()      {return FindObject(NOBH);}
public func IsChooseable()      {return true;}


public func ChooserFinished()
{
  Execute();
}

public func Execute()
{
  //Blackhawkfahrzeugspawns suchen und entfernen
  for (var obj in FindObjects(Find_ID(VSPW)))
  {
    var i = GetEffect("IntVehicleSpawn4K", obj);
    if (!i)
      continue;
    var array = EffectVar(0, obj, i), entry;
    while ((entry = GetIndexOf(BKHK, array)) != -1)
      DelArrayItem4K(array, entry);
    EffectVar(0, obj, i) = array;
  }
  //Blackhawks suchen und entfernen
  for (var obj in FindObjects(Find_ID(BKHK)))
    RemoveObject(obj, true);
}