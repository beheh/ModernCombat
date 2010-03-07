/*-- Schwerverletztenfix --*/

#strict 2
#appendto FKDT

static const FKDT_SuicideTime = 30;
local mediccalls;

protected func Initialize()
{
  Sound("ClonkDie*.ogg");
  mediccalls = 2;
  _inherited();
}

public func Suicide()
{
  for(var icons in FindObjects(Find_ID(_IN3), Find_Owner(GetOwner(clonk))))
    RemoveObject(icons);
  clonk->Kill();
  return(RemoveObject());
}

private func DeathMenu()
{
  var selection = GetMenuSelection(clonk);
  DoEnergy(20, clonk);
  CloseMenu(clonk);
  CreateMenu (FKDT, clonk, this(), 0, Format("%s: Tod in %d", GetName(), suicide), C4MN_Style_Dialog, true);
  AddMenuItem("Suicide", "Suicide", SKUL, clonk, 0, 0, "Begeht Selbstmord.");
  if(mediccalls)
    AddMenuItem("Call for Medic", "MedicCall", _IN3, clonk);
  else{
    AddMenuItem(Format("<c %x>Call for Medic</c>",RGB(128,128,128),suicide), 0, _IN3, clonk);
      }
  SelectMenuItem(selection, clonk); 
  if(suicide <= 0)
    Suicide();

  //SetMenuDecoration(MCDC, pClonk);
  SetMenuTextProgress(1, clonk); 
}

private func MedicCall()
{
  Sound("Medic*.ogg");
  var icon = CreateObject(_IN3, 0, -10, GetOwner(clonk));
  FadeOutIcon(icon, 0);
  return(mediccalls--);
}

private func FadeOutIcon(object pItem, int iRepeat)
{
  if(iRepeat >= 256)
    { RemoveObject(pItem);
      return;              }
  SetClrModulation(RGBa(255,255,255,iRepeat), pItem);
  ScheduleCall(0, "FadeOutIcon", 1, 0, pItem, iRepeat+3);
}