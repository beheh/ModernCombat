/*-- Ausrüstung --*/

//Deaktiviert Hilfenachrichten des Hazard-Ausrüstungssystems.

#strict 2
#appendto GEAR


public func GearBuckle(object pClonk)
{
  //Verstecken aber nicht löschen
  Exit();
  SetCategory(1);
  SetPosition(1,1);
  SetVisibility(VIS_None);

  //Callback
  GearEffect(pClonk);
}

public func GearUnbuckle(object pClonk)
{
  //Zurück zum Clonk
  SetPosition(GetX(pClonk),GetY(pClonk));
  SetCategory(16);
  SetVisibility(VIS_All);

  GearEffectReset(pClonk);
}