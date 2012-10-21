/*-- Neues Script --*/

#strict 2

#appendto HZCK

static const SR4K_LayerBlueCast = 8;

/* Einstellungen */

protected func ContextSettings(object pCaller)
{
  [$Settings$|Image=CSTR]
  if(GetMenu(pCaller) == CSTR)
    var iSel = GetMenuSelection(pCaller);

  //Einstellungsmenü erstellen
  CreateMenu(CSTR, pCaller, pCaller, 0, "$Settings$", 0, C4MN_Style_Context, true);

  //QuickInventar
  if(pCaller->QuickInventoryOff())
    AddMenuItem("$CtxQuickInventoryOff$", Format("SetQuickInventoryOn(Object(%d))", ObjectNumber(pCaller)), SM06, pCaller, 0, 0, "$CtxQuickInventoryDesc$");
  else
    AddMenuItem("$CtxQuickInventoryOn$", Format("SetQuickInventoryOff(Object(%d))", ObjectNumber(pCaller)), SM05, pCaller, 0, 0, "$CtxQuickInventoryDesc$");

  //Hazard-Hilfsnachrichten
  if(pCaller->HelpMessagesOff())
    AddMenuItem("$CtxHelpMessagesOff$", Format("ContextHelpMessagesOn(Object(%d))", ObjectNumber(pCaller)), SM06, pCaller, 0, 0, "$CtxHelpMessagesDesc$");
  else
    AddMenuItem("$CtxHelpMessagesOn$", Format("ContextHelpMessagesOff(Object(%d))", ObjectNumber(pCaller)), CXIN, pCaller, 0, 0, "$CtxHelpMessagesDesc$");

  //Inventarsperre
  if(pCaller->GetInvLockMode())
    AddMenuItem("$CtxInvLockOn$", Format("SwitchInventoryLockMode(Object(%d))", ObjectNumber(pCaller)), WPN2, pCaller, 0, 0, "$CtxInvLockDesc$");
  else
    AddMenuItem("$CtxInvLockOff$", Format("SwitchInventoryLockMode(Object(%d))", ObjectNumber(pCaller)), SM06, pCaller, 0, 0, "$CtxInvLockDesc$");

  //Kompakt-Death-Menü
  if(pCaller->ShorterDeathMenu())
    AddMenuItem("$CtxShorterDMOn$", Format("SwitchDeathMenuMode(Object(%d))", ObjectNumber(pCaller)), FKDT, pCaller, 0, 0, "$CtxShorterDMDesc$");
  else
    AddMenuItem("$CtxShorterDMOff$", Format("SwitchDeathMenuMode(Object(%d))", ObjectNumber(pCaller)), SM06, pCaller, 0, 0, "$CtxShorterDMDesc$");

  //Radiomusik
  if(pCaller->RadioMusicAct())
    AddMenuItem("$CtxRadioMusicOn$", Format("SwitchRadioMusicMode(Object(%d))", ObjectNumber(pCaller)), RDIO, pCaller, 0, 0, "$CtxRadioMusicDesc$");
  else
    AddMenuItem("$CtxRadioMusicOff$", Format("SwitchRadioMusicMode(Object(%d))", ObjectNumber(pCaller)), SM06, pCaller, 0, 0, "$CtxRadioMusicDesc$");
    
  //Kompass
  if(!CMPS_fNoCompass)
  {
  	if(!CompassForbidden(GetOwner(pCaller)))
  		AddMenuItem("$CtxCompassOn$", Format("SwitchCompassMode(Object(%d))", ObjectNumber(pCaller)), CMPM, pCaller, 0, 0, 0, 2, 2);
  	else
  		AddMenuItem("$CtxCompassOff$", Format("SwitchCompassMode(Object(%d))", ObjectNumber(pCaller)), CMPM, pCaller, 0, 0, 0, 2, 3);
  }
  
  //ol-stich
  if(GetPlayerRank(GetOwner(pCaller)) >= 15)
  {
  	var rgb;
  	if((rgb = GetScreenRGB(GetOwner(pCaller), SR4K_LayerBlueCast)) && rgb->GetAlpha() < 250)
  		AddMenuItem("$CtxBlueCastOff$", Format("SwitchBlueCast(Object(%d))", ObjectNumber(pCaller)), SM06, pCaller, 0, 0, 0, 2, 3);
  	else
  		AddMenuItem("$CtxBlueCastOn$", Format("SwitchBlueCast(Object(%d), true)", ObjectNumber(pCaller)), EFLN, pCaller, 0, 0, 0, 2, 3);
  }

  //Achievements zurücksetzen
  AddMenuItem("$CtxResetAch$", "ContextResetAch", RWDS, pCaller, 0, 0, "$CtxResetAchDesc$");

  SelectMenuItem(iSel, pCaller);

  return true;
}

public func SwitchBlueCast(object pCaller, bool fSwitchOn)
{
	var rgb = ScreenRGB(pCaller, RGB(126,200,234), 0, 0, false, SR4K_LayerBlueCast);
	var a = 255 - 42 * fSwitchOn;
	rgb->SetAlpha(a);
	
	return ContextSettings(pCaller);
}

public func SwitchCompassMode(object pCaller)
{
	SetPlrExtraData(GetOwner(), "CMC_ForbidCompass", !CompassForbidden(GetOwner()));
	Sound("Click", 1, 0,0, GetOwner()+1);
	var compass;
	if(CompassForbidden(GetOwner(pCaller)))
	{
		if(compass = FindObject2(Find_ID(CMPS), Find_Owner(GetOwner())))
			RemoveObject(compass);
	}
	else
		FindObject2(Find_ID(1HUD), Find_Owner(GetOwner()))->CreateCompass();
		
	return ContextSettings(pCaller);
}
