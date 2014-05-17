/*-- Toastbrot --*/

#strict 2

local toasted, clonk;

public func Activate(object pClonk)
{
  [$TxtEat$]
  
  clonk = pClonk;
  
  if(toasted)
  	Eat(pClonk);
  else
  	ScheduleCall(0, "Eat", 36);
  
  return 1;
}

public func Eat(object pClonk)
{
	if(!pClonk) { pClonk = clonk; }
  pClonk->~Feed(40+(40*toasted));
  RemoveObject();
  return 1;
}

public func SetToasted()
{
	toasted = true;
	SetGraphics("Toasted");
}

/* Produkteigenschaften */

public func IsToasterProduct() { return(1); }
