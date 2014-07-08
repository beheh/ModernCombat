/*-- Effektmanager --*/

#strict 2

public func RWDS_MenuAbort()	{return true;}


/* Auswahlmenü */

protected func Activate(iByPlayer)
{
  var pClonk = GetCursor(iByPlayer);
  if(GetMenu(pClonk))
  {
    if(GetMenu(pClonk)->~RWDS_MenuAbort())
      CloseMenu(pClonk);
    else
      return;
  }
  if(!CreateMenu(GetID(), pClonk, this, 0, 0, 0, C4MN_Style_Dialog)) return;

  //Wenn Spieler 1 und keine Liga
  if(iByPlayer == GetPlayerByIndex(0, C4PT_User) && !GetLeague())
  {
    //Menü öffnen
    CreateMenu(GetID(), pClonk, 0,0,GetName(),0, 1);
    for(var i = 1; i <= 3; i++)
    {
      var szText = Format("$EffectLevel$", i);
      if(i == EFSM_Level) szText = Format("<c ffff33>%s</c>",szText);
      AddMenuItem(szText, "SetEffectLevel", EFMN, pClonk, 0, i, "$SetEffectLevel$");
    }
  }
  else
  {
    //Beschreibung anzeigen
    var szLevel = Format("Level %d", EFSM_Level);
    if(EFSM_Level == 0) szLevel = "$Custom$";
    MessageWindow(Format("$Desc$", szLevel),iByPlayer);
  }
  return 1;
}

protected func SetEffectLevel(id dummy, int iLevel)
{
  //Bei Ligaspielen nicht möglich
  if(GetLeague()) return;
  EFSM_SetEffects(iLevel);
  //Eventnachricht: Effektstufe geändert
  EventInfo4K(0, Format("$NewLevel$", iLevel), EFMN);
}

protected func End(id dummy, object pClonk)
{
  CloseMenu(pClonk);
}