#strict

protected func Activate(int iPlr)
{
  CreateScriptPlayer("Bot",0,0,0,GetID());
}

protected func InitializePlayer(int iPlr)
{
  if(GetPlayerType(iPlr) != C4PT_Script) return();
  var iCrew, pCrew;
  while(pCrew = GetCrew(iPlr, iCrew++))
    SetupBot4K(Random(4)+1,pCrew,10);
}

protected func RelaunchPlayer(int iPlr, object pCrew)
{
  if(GetPlayerType(iPlr) != C4PT_Script) return();
  SetupBot4K(Random(4)+1,pCrew,10);
}