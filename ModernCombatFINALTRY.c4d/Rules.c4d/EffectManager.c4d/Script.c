/*-- Effektmanager --*/

#strict 2


/* Infos */

protected func Activate(iByPlayer)
{
  var pClonk = GetCursor(iByPlayer);
  if(iByPlayer == 0)
  {
   //Menü aufmachen
   if(!GetMenu(pClonk)) return;
   CreateMenu(GetID(), pClonk, 0,0,GetName(),0, 1);
   for(var i = 1; i <= 3; i++)
   {AddMenuItem(Format("$EffectLevel$", i), "SetEffectLevel", EFMN, pClonk, 0, i, "$SetEffectLevel$");}
  }
  else
  {
   MessageWindow(GetDesc(),iByPlayer);
  }
  return(1);
}

protected func SetEffectLevel(id dummy, int iLevel)
{
  EFSM_SetEffects(iLevel);
  //Bei Ligaspielen nicht möglich
  if(!GetLeague()) EventInfo4K(0, Format("$NewLevel$", iLevel), EFMN);
}

protected func End(id dummy, object pClonk)
{
  CloseMenu(pClonk);
}
