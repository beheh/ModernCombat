/*-- Terminal --*/

#strict 2

local szText, arSymbols;

/* Initalisierung */

protected func Initialize()
{
  szText = "";
  arSymbols = [];

	return true;
}

protected func ControlUp(object caller)
{
  MessageWindow(Format("$TxtMsg$", GetName(caller), szText), GetOwner(caller), GetID(), GetName());
	
	if(arSymbols[GetOwner(caller)])
		RemoveObject(arSymbols[GetOwner(caller)]);
	
	return true;
}

public func SetNewInfo(int iPlr, string sText)
{
	if(!arSymbols[iPlr])
  	arSymbols[iPlr] = CreateObject(SMT0, 0, 0, iPlr);

	arSymbols[iPlr]->Set(this);
	szText = sText;

	return true;
}
