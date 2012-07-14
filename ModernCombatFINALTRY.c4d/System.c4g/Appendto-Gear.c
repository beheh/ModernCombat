/*-- Neues Script --*/

#strict 2

#appendto GEAR

public func GearBuckle(object pClonk)
{
  // verstecken aber nicht l�schen
  Exit();
  SetCategory(1);
  SetPosition(1,1);
  SetVisibility(VIS_None);

  // Callback
  GearEffect(pClonk);
}

public func GearUnbuckle(object pClonk)
{
  // zur�ck zum Hazard
  SetPosition(GetX(pClonk),GetY(pClonk));
  SetCategory(16);
  SetVisibility(VIS_All);

  GearEffectReset(pClonk);
}
