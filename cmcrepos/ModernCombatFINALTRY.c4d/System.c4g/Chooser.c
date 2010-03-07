/*-- Chooser-Append --*/
#strict
#appendto CHOS

protected func Initialize()
{
  _inherited();
  LoadRuleCfg();
}

protected func OpenGoalMenu(id dummy, int iSelection)
{
  if(!pGoal) return(OpenMenu());
  var pClonk = GetCursor();
  if(pGoal->~ConfigMenu(pClonk))
	return(1);//OpenMenu()
  return(_inherited(dummy, iSelection,...));
}

protected func LoadRuleCfg()
{
  var a = GameCall("ChooserRuleConfig");
  if(!GetLength(a)) return();
  
  for(var i=0, idR, def, j, check ; idR = GetDefinition(i, Chooser_Cat) ; i++)
    if(DefinitionCall(idR, "IsChooseable") && !GetLength(FindObjects(Find_ID(idR))))
    {
      if(FindInArray4K(a,idR) > -1)
        aRules[i] = true;
    }
}