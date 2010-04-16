/*-- Ringmenü/HUD-Update-Fix --*/

#strict
#appendto HZCK

public func UpdateCharge()
{
  // Nur wenn ich Cursor bin
  var pCursor = GetCursor(GetOwner());
  if(pCursor && pCursor != this()) pCursor = pCursor->~GetRealCursor(); 
  if(pCursor != this()) return();

  if(GetOwner() < -1) return();

  // in Gebäuden/Fahrzeugen
  if(Contained())
    if(Contained()->~UpdateCharge(this()))
      return(1);

  // reitet
  if(IsRiding())
    if(GetActionTarget()->~UpdateCharge(this()))
      return(1);

  // ggf. an angefasstes Objekt weiterleiten
  var Content = Contents();
  if(GetAction() S= "Push")
  	if(GetActionTarget()->~IsWeapon())
  		Content = GetActionTarget();

  // HUD
  var hud = GetHUD();
  if(hud) hud->Update(Content, AmmoStoring(),this());

  return(1);
}
