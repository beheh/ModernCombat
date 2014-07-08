/*-- Ausrüstungssteuerung --*/

//Ausrüstungssystem mit verbessertem Kontextmenü und Aufbau.

#strict 2
#appendto L_GE


/* Bei Tod Ausrüstung fallenlassen */

protected func Death()
{
  _inherited();
  if(HasGear()) TakeOffGear();
}

/* Kontextmenü */

protected func ContextUseEquipment(object pCaller)
{
  [$CtxEquipmentDesc$|Image=AMPK|Condition=HasUsableGear]
  if(!HasUsableGear()) return;
  CreateMenu(AMPK,0,0,0,0,0,1);

  //Ausrüstungsfunktionen anzeigen
  for(var i=0; i<GetLength(aGear); ++i)
  {
    var gear = aGear[i];
    if(gear) 
      if(gear->~ConUseable())
        AddMenuItem(gear->ConDesc(), "UseEquipment", GetID(gear), 0, 0, i);

    var gear = aGear[i];
    if(gear)
      AddMenuItem("$CtxUnbuckleEquipment$", Format("TakeOffGear(0,%d)",i), GetID(gear));
  }

  return(1);
}

protected func ContextUnbuckle()
{
  [0|Image=ROCK|Condition=NoContext]
  return;
}