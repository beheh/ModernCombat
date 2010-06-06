/*-- Effektmanager --*/

#strict 2

/* Infos */

protected func Activate(iByPlayer)
{
	var pClonk = GetCursor(iByPlayer);
	if(iByPlayer == 0) {
		// Menü aufmachen
		CreateMenu(GetID(), pClonk, 0,0,GetName(),0, 1);
		for(var i = 1; i <= 3; i++) {
			AddMenuItem(Format("$EffectLevel$", i), "SetEffectLevel", BOOM, pClonk, 0, i, "$SetEffectLevel$");
		} 
		// Fertig
		AddMenuItem("$Finished$", "End", CHOS, pClonk, 0, pClonk, "$Finished$", 2, 4);
  }
  return(1);
}

protected func SetEffectLevel(id dummy, int iLevel) {
	EFSM_SetEffects(iLevel);
	//Damit keiner in Ligaspielen nervt :p
	if(!GetLeague()) EventInfo4K(0, Format("$NewLevel$", iLevel), BOOM);
}

protected func End(id dummy, object pClonk) {
	CloseMenu(pClonk);
}
