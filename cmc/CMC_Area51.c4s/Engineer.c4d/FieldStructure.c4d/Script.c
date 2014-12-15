/*-- Feldstruktur --*/

#strict 2

local pToolkit;

func BuildCost() {return 1;}
func Replaced()	{}

func Construction(object kit)
{
	pToolkit = kit;
	AddEffect("Construction", this, 1, 1, this);
}

protected func FxConstructionTimer(object pTarget, int number, iEffectTime)
{
	var pClonk = FindObject2(Find_ActionTarget(pTarget), Find_Action("Push"));
	if(!pClonk)
	{
		if(pToolkit)
			pToolkit->DoPackPoints(BuildCost());
		Replaced(pTarget);
		return -1;
	}
	else
	{
		DoCon(1, pTarget);
		if(GetOCF(pTarget) & OCF_Fullcon)
		{
			pClonk->SetAction("Walk");
			return -1;
		}
	}
		
}
