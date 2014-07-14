/*-- Funktionen --*/

#strict 2

global func DecoMessage(string strMessage, int iPlrPlusOne, string szPortrait)
{
  if(!szPortrait)
    szPortrait = "Portrait:NONE::ffffff::1";

	if(iPlrPlusOne)
	{
		iPlrPlusOne--;
	  CustomMessage(strMessage, 0, iPlrPlusOne, 15, 50, 0xFFFFFF, DECO, szPortrait, MSG_Top|MSG_Left|MSG_WidthRel|MSG_XRel, 50);
	
		return true;
	}

	for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
	  CustomMessage(strMessage, 0, i, 15, 50, 0xFFFFFF, DECO, szPortrait, MSG_Top|MSG_Left|MSG_WidthRel|MSG_XRel, 50);
	
	return true;
}
