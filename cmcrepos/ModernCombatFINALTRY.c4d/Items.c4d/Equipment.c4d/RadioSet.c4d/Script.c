/*-- Funkger‰t --*/

#strict 2

protected func Departure(object pContainer) {
	ScheduleCall(this, "Reset", 1, 0, pContainer);
	return true;
}

public func Reset(object pContainer) {
	if(!pContainer) return RemoveObject();
	Enter(pContainer);
	for(var i = 1; i < ContentsCount(0, pContainer); i++) {
		ShiftContents(pContainer, 0, 0, i == ContentsCount(0, pContainer)-1);
	}
}

public func ControlThrow() {
	return true;
}

protected func Activate(object pCaller)
{
	/*var ring = CreateSpeedMenu(this, pCaller);
	ring->AddThrowItem("$TacticalAid$", "TacticalAid", pCaller,FUNK);
	ring->AddUpItem("$GoHere$", "SettingUp", [pCaller, GetOwner(pCaller), TMGH],TMGH);
	ring->AddDownItem("$DefendHere$", "SettingUp", [pCaller, GetOwner(pCaller), TMDH], TMDH);
	ring->AddLeftItem("$Retreat$", "SettingUp2", [pCaller, GetOwner(pCaller), TMRT], TMRT);*/
}

/*public func SettingUp2(aarray)
{
	var pObject = aarray[0];
	var iPlayer = aarray[1];
	var idTM = aarray[2];
 
	var pMovement = CreateObject(idTM, -100, -100, iPlayer);
	SetVisibility(VIS_Owner | VIS_Allies, pMovement);
	pMovement->SetAction("Follow", pObject);
}

public func SettingUp(aarray)
{   
 var pObject = aarray[0];
 var iPlayer = aarray[1];
 var idTM = aarray[2];
  
 var pCursor = CreateObject(CRSR, 0, -15, iPlayer);
 SetVisibility(VIS_Owner, pCursor);
 //pCursor -> SetClonk(pObject);
 //pCursor -> SetAid(idTM);
 SetComDir(COMD_None, pObject);
 MakeCrewMember(pCursor, iPlayer);
 UnselectCrew(iPlayer);
 SetCrewEnabled(0, pObject);
 SelectCrew(iPlayer, pCursor, 1);
 SetCursor(iPlayer, pCursor);
}

protected func TacticalAid(object pCaller)
{
 var ring = CreateSpeedMenu(FindObject(TACA), pCaller);
  //CreateMenu(FUNK, pCaller, pCaller, 0, "Dieser Text sollte nie erscheinen!", 0, C4MN_Style_Normal);
  ring->AddUpItem("$NonDestructive$", "InitTacticalAidMenu", [pCaller, 1], TAND);   //Hier musste ich InitTacticalAidMenu umschreiben, damit es ein Array akzeptiert,
  ring->AddLeftItem("$Selective$", "InitTacticalAidMenu", [pCaller, 2], TASA);      //weil AddFooItem nur einen Parameter unterst¸tzt. Dazu bleibt nur zu sagen:
  ring->AddRightItem("$Indiscriminate$", "InitTacticalAidMenu", [pCaller, 3], TAIA);//"wer hat die scheiﬂe geschrieben? :L" -- Ace
}*/
