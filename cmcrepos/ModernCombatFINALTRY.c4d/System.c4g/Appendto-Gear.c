/*-- Ausr�stung --*/

//Deaktiviert Hilfenachrichten des Hazard-Ausr�stungssystems.

#strict 2
#appendto GEAR


public func GearBuckle(object pClonk)
{
  //Verstecken aber nicht l�schen
  Exit();
  SetCategory(1);
  SetPosition(1,1);
  SetVisibility(VIS_None);

  //Callback
  GearEffect(pClonk);
}

public func GearUnbuckle(object pClonk)
{
  //Zur�ck zum Clonk
  SetPosition(GetX(pClonk),GetY(pClonk));
  SetCategory(16);
  SetVisibility(VIS_All);

  GearEffectReset(pClonk);
}